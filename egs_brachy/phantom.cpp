/*
################################################################################
#
#  egs_brachy phantom.cpp
#  Copyright (C) 2016 Rowan Thomson, Dave Rogers, Randle Taylor, and Marc
#  Chamberland
#
#  This file is part of egs_brachy
#
#  egs_brachy is free software: you can redistribute it and/or modify it
#  under the terms of the GNU Affero General Public License as published
#  by the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  egs_brachy is distributed in the hope that it will be useful, but
#  WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Affero General Public License for more details:
#  <http://www.gnu.org/licenses/>.
#
################################################################################
#
#  When egs_brachy is used for publications, please cite our paper:
#  M. J. P. Chamberland, R. E. P. Taylor, D. W. O. Rogers, and R. M. Thomson,
#  egs brachy: a versatile and fast Monte Carlo code for brachytherapy,
#  Phys. Med. Biol. 61, 8214-8231 (2016).
#
################################################################################
#
#  Author:        Randle Taylor, 2016
#
#  Contributors:  Marc Chamberland
#                 Dave Rogers
#                 Rowan Thomson
#
################################################################################
*/

/*! \file phantom.cpp
 * \author Randle Taylor (randle.taylor@gmail.com)
 * \brief Implementation of phantom objects
 *
 */
#include <algorithm>
#include <vector>
#include <fstream>
#include "phantom.h"
#include <iomanip>
#include "gzstream.h"
#include "latch.h"
#include "egs_run_control.h"



/* Used internally for keeping track of results for regions, allowing us
 * to sort by dose etc */
struct RegionResult {

    int reg;
    double volume;

    double tlen;
    double tlen_err;

    double edep;
    double edep_err;

};


/* comparison function useful for sorting vectors of RegionResults */
bool tlenRegTopResultCompare(const RegionResult &firstElem, const RegionResult &secondElem) {
    return firstElem.tlen > secondElem.tlen;
}

bool edepRegTopResultCompare(const RegionResult &firstElem, const RegionResult &secondElem) {
    return firstElem.edep > secondElem.edep;
}



/* utility function for replacing spaces with underscores in a string */
string space2underscore(std::string text) {
    for (std::string::iterator it = text.begin(); it != text.end(); ++it) {
        if (*it == ' ') {
            *it = '_';
        }
    }
    return text;
}

/* only geometries that support getting regional volume/mass can be used as phantoms */
const string EB_Phantom::allowed_phantom_geom_types[] = {"EGS_cSpheres", "EGS_cSphericalShell", "EGS_XYZGeometry", "EGS_RZ"};


/* initializer for phantom class */
EB_Phantom::EB_Phantom(EGS_Application *parent, EGS_BaseGeometry *geom, set<int> global_regions, int nsource,
                       Publisher *pub) :
    app(parent), tlen_score(0), edep_score(0),
    prim_score(0), sscat_score(0), mscat_score(0),
    nsources(nsource),
    dose_scale(1.),
    total_radiant_e(0),
    cur_history(0),
    publisher(pub),
    geometry(geom), global_regions(global_regions) {

    global_reg_start = *global_regions.begin();
    global_reg_stop = *global_regions.rbegin();

    publisher->subscribe(this, NEW_HISTORY);

}

void EB_Phantom::update(EB_Message message, void *data) {
    if (message == NEW_HISTORY) {
        setHistory(*((EGS_I64 *)data));
    } else if (message == PARTICLE_ESCAPING_SOURCE) {
        EGS_Particle *p = static_cast<EGS_Particle *>(data);
        total_radiant_e += p->E*p->wt;
    }
}

void EB_Phantom::scoreTlen(int ir, EGS_Float dose, EGS_Particle *p) {
    if (ir < 0 || ir >= geometry->regions()) {
        egsFatal("bad %d \n", ir);
    }

    if (tlen_score) {
        tlen_score->score(ir, dose);
    }

    if (prim_score) {

        if (Latch::isPrimary(p)) {
            prim_score->score(ir, dose);
        } else if (Latch::isSingleScat(p)) {
            sscat_score->score(ir, dose);
        } else if (Latch::isMultScat(p)) {
            mscat_score->score(ir, dose);
        }
    }
}

/*! \brief add energy deposition dose to region ir */
void EB_Phantom::scoreEdep(int ir, EGS_Float dose) {
    if (edep_score) {
        edep_score->score(ir, dose);
    }
}

/*! \brief add energy deposition dose to region ir */
void EB_Phantom::setDoseScale(EGS_Float scale) {
    dose_scale = scale;
}

bool EB_Phantom::allowedPhantomGeom(const string &geom_type) {
    // Check if the input geometry is one that egs_brachy can handle

    int end = (int)(sizeof(allowed_phantom_geom_types)/sizeof(string));

    for (int i=0; i<end; i++) {
        if (allowed_phantom_geom_types[i] == geom_type) {
            return true;
        }
    }

    return false;
}

vector<int> EB_Phantom::getRegionsWithCorrections() {
    vector<int> regs;
    for (std::map<int, double>::iterator it=corrected_volumes.begin(); it != corrected_volumes.end();  ++it) {
        regs.push_back(it->first);
    }
    return regs;
}

void EB_Phantom::setCorrectedVolume(int ir, double corrected_vol) {
    corrected_volumes[ir] = corrected_vol;
}

/* turn on interaction scoring */
void EB_Phantom::enableTLenScoring() {
    if (!tlen_score) {
        tlen_score = new EGS_ScoringArray(geometry->regions());
    }
}

/* turn on interaction scoring */
void EB_Phantom::enableInteractionScoring() {
    if (!edep_score) {
        edep_score = new EGS_ScoringArray(geometry->regions());
    }
}

/* turn on interaction scoring */
void EB_Phantom::enableScatterScoring() {
    if (!prim_score) {
        prim_score = new EGS_ScoringArray(geometry->regions());
        sscat_score= new EGS_ScoringArray(geometry->regions());
        mscat_score= new EGS_ScoringArray(geometry->regions());

        publisher->subscribe(this, PARTICLE_ESCAPING_SOURCE);
    }
}

/* needs to be called at start of every history for every phantom for
 * proper scoring of statistics */
void EB_Phantom::setHistory(EGS_I64 current_case) {

    cur_history = current_case;

    if (tlen_score) {
        tlen_score->setHistory(current_case);
    }

    if (edep_score) {
        edep_score->setHistory(current_case);
    }

    if (prim_score) {
        prim_score->setHistory(current_case);
        sscat_score->setHistory(current_case);
        mscat_score->setHistory(current_case);
    }
}

void EB_Phantom::setEffectiveHistories(EGS_Float neff) {

    effective_histories = neff;
}

/* call to tell phantom to output its results */
void EB_Phantom::outputResults(int top_n, string output_3ddose, string output_egsphant, string output_voxinfo,
                               string output_volcor) {

    if (output_voxinfo == "text" || output_voxinfo == "gzip") {
        outputVoxelInfo(output_voxinfo);
    }

    if (output_volcor == "text" || output_volcor == "gzip") {
        outputVolumeCorrection(output_volcor);
    }

    vector<RegionResult> region_results = getRegionResults();

    if (top_n) {
        outputTopDoses(top_n, region_results);
    }

    if (output_3ddose == "text" || output_3ddose == "gzip") {
        output3ddoseResults(output_3ddose);
    }

    if (output_egsphant== "text" || output_egsphant == "gzip") {
        outputEGSPhant(output_egsphant);
    }

}
EGS_Float EB_Phantom::getRealRho(int ireg) {
    int med = geometry->medium(ireg);
    return geometry->getRelativeRho(ireg)*the_media->rho[med];
}


EGS_Float EB_Phantom::getRealMass(int ireg) {
    return getCorrectedVolume(ireg)*getRealRho(ireg);
}

EGS_Float EB_Phantom::getUncorrectedMass(int ireg) {
    return getUncorrectedVolume(ireg)*getRealRho(ireg);
}

EGS_Float EB_Phantom::getUncorrectedVolume(int ireg) {
    // EGS_BaseGeometry::getMass is volume*relativeRho()
    EGS_Float volume = geometry->getMass(ireg)/geometry->getRelativeRho(ireg);
    return volume;
}

EGS_Float EB_Phantom::getCorrectedVolume(int ireg) {
    bool has_correction = corrected_volumes.find(ireg) != corrected_volumes.end();
    return has_correction ? corrected_volumes[ireg] : getUncorrectedVolume(ireg);
}


void EB_Phantom::getCurrentScore(int ireg, double &sum, double &sum2) {
    if (tlen_score) {
        tlen_score->currentScore(ireg, sum, sum2);
    } else if (edep_score) {
        edep_score->currentScore(ireg, sum, sum2);
    } else {
        sum = 0;
        sum2 = 0;
    }

}

double EB_Phantom::getTlenNorm(int ireg) {
    double eff_hist_norm = (double)effective_histories/cur_history;
    return 1.602E-10*nsources*dose_scale/eff_hist_norm;
}

void EB_Phantom::getResult(EGS_ScoringArray *score, int ireg, string type, EGS_Float &r, EGS_Float &dr) {

    score->currentResult(ireg, r, dr);
    dr = r > 0 ? dr/r: 1;

    // scoring arrays are already normalized to number of statistically
    // independent histories

    // correct norm from per history to per effective history
    double eff_hist_norm = (double)effective_histories/cur_history;

    //egsInformation("Normalization: effective_histories %d, cur_history %d, eff_hist_norm %f\n",effective_histories, cur_history, eff_hist_norm);


    if (type == "tlen") {
        r *= 1.602E-10*nsources*dose_scale/eff_hist_norm;
    } else if (type == "edep") {
        r *= 1.602E-10*nsources*dose_scale/eff_hist_norm;
    } else { // scatter norm
        r *= (cur_history/total_radiant_e)*nsources*dose_scale/eff_hist_norm;
    }


}

// Note: this could be made more memory efficient by using an ordered set;
// For every iteration, insert new region result and then if the
// set has more than TOP_N results, reduce the size to TOP_N
vector<RegionResult> EB_Phantom::getRegionResults() {

    int nreg = geometry->regions();
    vector<RegionResult> region_results;

    // retrieve results and add to region_results to allow for sorting by top doses
    for (int ireg = 0; ireg < nreg; ireg++) {

        double result=0, dr=0;
        if (tlen_score) {
            getResult(tlen_score, ireg, "tlen", result, dr);
        }

        double result_edep=0, result_edep_dr=0;
        if (edep_score) {
            getResult(edep_score, ireg, "edep", result_edep, result_edep_dr);
        }

        EGS_Float volume = getCorrectedVolume(ireg);

        RegionResult regr = { ireg, volume, result, dr, result_edep, result_edep_dr };
        region_results.push_back(regr);

    }

    return region_results;

}

/* output statistics about dose arrays */
void EB_Phantom::outputDoseStats(EGS_ScoringArray *score, string type) {

    EGS_Float maxr=0;
    int nreg = score->regions();

    for (int i=0; i < nreg; i++) {
        EGS_Float r, dr;
        getResult(score, i, type, r, dr);

        maxr = max(maxr, r);

    }

    EGS_Float avg=0, avg_err=0, avg_gt_20=0, avg_gt_20_err=0;
    int n_gt_20 = 0;

    for (int i=0; i < nreg; i++) {
        EGS_Float r, dr;
        getResult(score, i, type, r, dr);
        avg += r;
        avg_err += dr;

        if (r >= maxr*0.20) {
            n_gt_20++;
            avg_gt_20 += r;
            avg_gt_20_err += dr;
        }
    }

    string units = "Gy/hist";
    if (type == "pr" || type == "ss" || type == "ms" || type == "to") {
        units = "Gy/R";
    }

    egsInformation("    Dose Scaling                     =  %.3G\n", dose_scale);
    egsInformation("    Average dose                     =  %.3G %s\n", avg/nreg, units.c_str());
    egsInformation("    Average error on dose            =  %.3G %\n", 100*avg_err/nreg);
    egsInformation("    Average error on dose > 0.2*Dmax =  %.3G %\n", 100*avg_gt_20_err/n_gt_20);
    egsInformation("    # of voxels with dose > 0.2*Dmax =  %d (%.3G%)\n", n_gt_20, 100.*double(n_gt_20)/nreg);

}

/* output Top N doses for tracklength and interaction scoring */
void EB_Phantom::outputTopDoses(int top_n, vector<RegionResult> region_results) {
    top_n = min(top_n, geometry->regions());


    string fmt = "\n\nTop %d dose results for geometry named '%s'";
    if (fabs(dose_scale -1) > 1E-10) {
        fmt += " (scaled)";
    }

    egsInformation((fmt+"\n").c_str(), top_n, (geometry->getName()).c_str());

    string title = " Region  Med #  Volume(Nominal) / cm^3  Tracklength Dose / Gy/hist  Interaction Dose / Gy/hist\n";
    egsInformation("%s\n", string(title.size()-1, '=').c_str());
    egsInformation(title.c_str());

    string sep(title.size()-1, '-');
    egsInformation((sep+"\n").c_str());

    // display top n results
    vector<RegionResult> top_results(top_n);
    if (!tlen_score && edep_score) {
        partial_sort_copy(region_results.begin(), region_results.end(), top_results.begin(), top_results.end(),
                          edepRegTopResultCompare);
    } else {
        partial_sort_copy(region_results.begin(), region_results.end(), top_results.begin(), top_results.end(),
                          tlenRegTopResultCompare);
    }

    for (vector<RegionResult>::iterator rrit=top_results.begin(); rrit!=top_results.end(); rrit++) {
        RegionResult rr = *rrit;
        double vol = getCorrectedVolume(rr.reg);
        double uncorvol = getUncorrectedVolume(rr.reg);

        int med = geometry->medium(rr.reg);
        if (vol > 0 && fabs(vol - uncorvol)/vol > 1E-6) {
            egsInformation(
                "%7d  %5d %11.3E(%8.3E)    %13.3E +/- %5.2f%%    %13.3E +/- %5.2f%%\n",
                rr.reg, med, vol, uncorvol, rr.tlen, 100*rr.tlen_err, rr.edep, 100*rr.edep_err
            );
        } else {
            egsInformation(
                "%7d  %5d %11.3E               %13.3E +/- %5.2f%%    %13.3E +/- %5.2f%%\n",
                rr.reg, med, vol, rr.tlen, 100*rr.tlen_err, rr.edep, 100*rr.edep_err
            );

        }
    }

}

void EB_Phantom::getScoringArrays(vector<EGS_ScoringArray *> &scores, vector<string> &types,
                                  vector<string> &descriptions) {

    if (tlen_score) {
        scores.push_back(tlen_score);
        types.push_back("tlen");
        descriptions.push_back("tracklength");
    }

    if (edep_score) {
        scores.push_back(edep_score);
        types.push_back("edep");
        descriptions.push_back("energy deposition");
    }

    if (prim_score) {
        scores.push_back(tlen_score);
        scores.push_back(prim_score);
        scores.push_back(sscat_score);
        scores.push_back(mscat_score);
        types.push_back("to");
        types.push_back("pr");
        types.push_back("ss");
        types.push_back("ms");
        descriptions.push_back("total primary + scattered");
        descriptions.push_back("primary");
        descriptions.push_back("single scattered");
        descriptions.push_back("multiple scattered");
    }

}

void EB_Phantom::getEGSdatScoringArrays(vector<EGS_ScoringArray *> &scores) {

    if (tlen_score) {
        scores.push_back(tlen_score);
    }

    if (edep_score) {
        scores.push_back(edep_score);
    }

    if (prim_score) {
        scores.push_back(prim_score);
        scores.push_back(sscat_score);
        scores.push_back(mscat_score);
    }

}


/* create 3ddose file and write bounds/doses to it */
void EB_Phantom::output3ddoseResults(string format) {

    vector<EGS_ScoringArray *> scores;
    vector<string> types;
    vector<string> descriptions;
    getScoringArrays(scores, types, descriptions);

    egsInformation("\nDose Output Files for %s\n%s\n", geometry->getName().c_str(), string(80,'=').c_str());
    for (size_t s=0; s < scores.size(); s++) {

        string type = types[s];

        string sep = type == "tlen" ? "" : "."+type;
        string extension = space2underscore("."+geometry->getName()) + sep + ".3ddose";
        extension += (format == "gzip") ? ".gz" : "";
        string fname = app->constructIOFileName(extension.c_str(), true);
        egsInformation(
            "Writing %s dose to %s%s\n",
            descriptions[s].c_str(),
            app->getOutputFile().c_str(),
            extension.c_str()
        );

        outputDoseStats(scores[s], type);
        if (format == "gzip") {
            ogzstream out;
            out.open(fname.c_str());
            output3DBounds(out);
            output3DDoses(out, scores[s], type);
            out.close();

        } else {
            ofstream out;
            out.open(fname.c_str());
            output3DBounds(out);
            output3DDoses(out, scores[s], type);
            out.close();
        }

    }

}


/* write the boundaries section for a 3dose files. This will work for arbitrary EGS_BaseGeometry
 * classes, but will of course only be accurate for geometries which have implemented getNRegDir
 * and getBound. */
void EB_Phantom::output3DBounds(ostream &out) {

    int nx = max(1, geometry->getNRegDir(XDIR));
    int ny = max(1, geometry->getNRegDir(YDIR));
    int nz = max(1, geometry->getNRegDir(ZDIR));

    out << nx << "  " << ny << "  " << nz << "\n";

    for (int dir=XDIR; dir <= ZDIR; dir++) {

        int nbounds = geometry->getNRegDir(dir) + 1;

        if (nbounds <= 1) {
            out << -999 << "\t" << 999 << "\n";
        } else {
            for (int idx=0; idx < nbounds; idx++) {
                string delim = (idx == nbounds - 1) ? "\n" : "\t";
                out<< geometry->getBound(dir, idx) << delim ;
            }
        }
    }
}

void EB_Phantom::output3DDoses(ostream &out, EGS_ScoringArray *score, string type) {


    // write doses on first loop, uncertainty on second
    for (int loop = 0; loop < 2; loop++) {
        for (int ireg=0; ireg < score->regions(); ireg++) {

            double result, dr;
            getResult(score, ireg, type, result, dr);

            if (loop == 0) {
                out << result << "\t";
            } else {
                out << dr << "\t";
            }

        }
        out << endl;
    }

}

void EB_Phantom::writeEGSPhant(ostream &out) {

    int nmed = EGS_BaseGeometry::nMedia();

    out << nmed << endl;
    for (int i=0; i < nmed; i++) {
        out << EGS_BaseGeometry::getMediumName(i) << endl;
    }

    for (int i=0; i < nmed; i++) {
        out << the_etcontrol->estepe << "\t";
    }
    out << endl;

    output3DBounds(out);

    int nx = max(1, geometry->getNRegDir(XDIR));
    int ny = max(1, geometry->getNRegDir(YDIR));
    int nz = max(1, geometry->getNRegDir(ZDIR));

    for (int loop = 0; loop < 2; loop++) {
        for (int kk=0; kk < nz; kk++) {
            for (int jj=0; jj < ny; jj++) {
                for (int ii=0; ii < nx; ii++) {
                    int ireg = ii + jj*nx + kk*nx*ny;

                    if (loop == 0) {
                        out << geometry->medium(ireg)+1;
                    } else {
                        out << getRealRho(ireg) << "\t";
                    }
                }
                out << endl;
            }
            out << endl;
        }
    }

}

void EB_Phantom::outputEGSPhant(string format) {

    egsInformation("\nEGSPhant Output File for %s\n%s\n", geometry->getName().c_str(), string(80,'=').c_str());

    string extension = space2underscore("."+geometry->getName()) + ".egsphant";
    extension += (format == "gzip") ? ".gz" : "";
    string out_name = app->getOutputFile()+extension;
    string fname = app->constructIOFileName(extension.c_str(), true);

    egsInformation("Writing egsphant file to %s\n", (out_name).c_str());

    if (format == "gzip") {
        ogzstream ogout(fname.c_str());
        writeEGSPhant(ogout);
        ogout.close();
    } else {
        ofstream out(fname.c_str());
        writeEGSPhant(out);
        out.close();
    }
}

EGS_Float EB_Phantom::avgVoxelVol() {

    EGS_Float total = 0;
    int nreg;
    for (nreg = 0; nreg < geometry->regions(); nreg++) {
        total += getCorrectedVolume(nreg);
    }
    return nreg > 0 ? total / nreg : 0;
}

void EB_Phantom::writeVoxelInfo(ostream &out) {

    out << std::setprecision(6) << std::scientific;
    out << "Region, Volume, Uncorrected Volume, Mass, Density, Med, Dose, Unc"<< endl;

    for (int i=0; i < geometry->regions(); i++) {
        EGS_Float r=0, dr=0;
        if (tlen_score) {
            getResult(tlen_score, i, "tlen", r, dr);
        } else if (edep_score) {
            getResult(edep_score, i, "edep", r, dr);
        }
        out << i << ", " << getCorrectedVolume(i) << ", " << getUncorrectedVolume(i) << ", ";
        out << getRealMass(i) << ", " << getRealRho(i) << ", "<<EGS_BaseGeometry::getMediumName(geometry->medium(i))<<", ";
        out << r << ", "<<dr<<endl;
    }
}

void EB_Phantom::outputVoxelInfo(string format) {

    egsInformation("\nVoxel Info File Output File for %s\n%s\n", geometry->getName().c_str(), string(80,'=').c_str());
    string extension = space2underscore("."+geometry->getName()) + ".voxels";
    extension += (format == "gzip") ? ".gz" : "";
    string fname = app->constructIOFileName(extension.c_str(), true);

    egsInformation(
        "Writing voxel info file to %s\n",
        (app->getOutputFile()+"."+geometry->getName() + extension).c_str()
    );

    if (format == "gzip") {
        ogzstream out;
        out.open(fname.c_str());
        writeVoxelInfo(out);
        out.close();
    } else {
        ofstream out;
        out.open(fname.c_str());
        writeVoxelInfo(out);
        out.close();
    }
}

void EB_Phantom::writeVolumeCorrection(ostream &out) {
    size_t nrecords = corrected_volumes.size();

    out << nrecords << endl;

    for (int i=0; i < geometry->regions(); i++) {
        bool has_correction = corrected_volumes.find(i) != corrected_volumes.end();
        if (has_correction) {
            out << i << " " << getCorrectedVolume(i) << endl;
        }
    }
}

void EB_Phantom::outputVolumeCorrection(string format) {

    egsInformation("\nVolume Correction Output File for %s\n%s\n", geometry->getName().c_str(), string(80,'=').c_str());
    string extension = space2underscore("."+geometry->getName()) + ".volcor";
    extension += (format == "gzip") ? ".gz" : "";
    string fname = app->constructIOFileName(extension.c_str(), true);
    string gname = geometry->getName();

    egsInformation(
        "Writing volume correction file for %s to %s file %s\n",
        gname.c_str(), format.c_str(), (app->getOutputFile()+"."+gname+".volcor").c_str());

    if (format == "gzip") {
        ogzstream out(fname.c_str());
        writeVolumeCorrection(out);
        out.close();
    } else {
        ofstream out;
        out.open(fname.c_str());
        writeVolumeCorrection(out);
        out.close();
    }
}

int EB_Phantom::outputData(ostream *ofile) {

    (*ofile) << total_radiant_e << endl;

    vector<EGS_ScoringArray *> scores;
    getEGSdatScoringArrays(scores);

    vector<EGS_ScoringArray *>::iterator it = scores.begin();
    for (; it != scores.end(); ++it) {
        bool ok = (*it)->storeState(*ofile);
        if (!ok) {
            return 1;
        }
    }

    return 0;

}

int EB_Phantom::readData(istream *ifile) {


    (*ifile) >> total_radiant_e;

    vector<EGS_ScoringArray *> scores;
    getEGSdatScoringArrays(scores);

    vector<EGS_ScoringArray *>::iterator it = scores.begin();
    for (; it != scores.end(); ++it) {
        bool ok = (*it)->setState(*ifile);
        if (!ok) {
            return 1;
        }
    }
    return 0;

}

int EB_Phantom::addState(istream &data) {

    EGS_Float total_radiant_e_tmp;
    data >> total_radiant_e_tmp;
    total_radiant_e += total_radiant_e_tmp;

    vector<EGS_ScoringArray *> scores, tmp_scores;
    getEGSdatScoringArrays(scores);

    vector<EGS_ScoringArray *>::iterator it = scores.begin();
    for (; it != scores.end(); ++it) {

        EGS_ScoringArray tmp((*it)->regions());

        bool ok = tmp.setState(data);
        if (!ok) {
            return 1;
        }

        (**it) += tmp;
    }

    return 0;

}

void EB_Phantom::resetCounter() {
    total_radiant_e = 0;

    vector<EGS_ScoringArray *> scores;
    getEGSdatScoringArrays(scores);

    vector<EGS_ScoringArray *>::iterator it = scores.begin();
    for (; it != scores.end(); ++it) {
        (*it)->reset();
    }


}
