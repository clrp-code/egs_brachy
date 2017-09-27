/*
################################################################################
#
#  egs_brachy spec_scoring.cpp
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

/*! \file spec_scoring.cpp
 * \brief implementation of spectrum scoring classes.
 */

#include "spec_scoring.h"
#include <fstream>
#include <stdarg.h>  // For va_start, etc.

std::string string_format(const std::string fmt, ...) {
    int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
    std::string str;
    va_list ap;
    while (1) {     // Maximum two passes on a POSIX system...
        str.resize(size);
        va_start(ap, fmt);
        int n = vsnprintf((char *)str.data(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size) {  // Everything worked
            str.resize(n);
            return str;
        }
        if (n > -1) { // Needed size returned
            size = n + 1;    // For null char
        } else {
            size *= 2;    // Guess at a larger size (OS specific)
        }
    }
    return str;
}

string getFileNameFromPath(const string &s) {

    char sep = '/';

#ifdef _WIN32
    sep = '\\';
#endif

    size_t i = s.rfind(sep, s.length());
    if (i != string::npos) {
        return (s.substr(i+1, s.length() - i));
    }

    return ("");
}

BaseSpectrumScorer *BaseSpectrumScorer::getSpectrumScorer(EGS_Input *inp, EGS_BaseSource *source, GeomInfo *ginfo,
        Publisher *publisher) {

    vector<string> types;
    types.push_back("surface count");
    types.push_back("energy weighted surface");
    types.push_back("energy fluence in region");

    string type = types[inp->getInput("type", types, 0)];

    BaseSpectrumScorer *scorer = 0;
    if (type == "surface count") {
        scorer = new SurfaceCountSpectrum(inp, source, ginfo, publisher);
    } else if (type == "energy weighted surface") {
        scorer = new EnergyWeightedSurfaceSpectrum(inp, source, ginfo, publisher);
    } else if (type == "energy fluence in region") {
        scorer = new EnergyFluenceSpectrumInVoxel(inp, source, ginfo, publisher);
    }

    if (!scorer || !scorer->isValid()) {
        return 0;
    }

    return scorer;

}

BaseSpectrumScorer::BaseSpectrumScorer(EGS_Input *input, EGS_BaseSource *src, GeomInfo *ginfo, Publisher *publisher) {

    valid = true;
    source = src;


    cur_history = 0;
    eff_history = 0;
    total_scored = 0;
    egsnrc_mode = 0;

    int err = input->getInput("minimum energy", e_min);
    if (err) {
        e_min = 0.001;
    }

    err = input->getInput("maximum energy", e_max);
    if (err) {
        e_max = source->getEmax();
    }

    err = input->getInput("number of bins", nbins);
    if (err || nbins < 1) {
        nbins = DEFAULT_NBINS;
    }

    vector<string> particle_choices;
    particle_choices.push_back("photon");
    particle_choices.push_back("electron");
    particle_choices.push_back("positron");
    particle_type = input->getInput("particle type", particle_choices, 0);

    //convert particle type to particle charge
    if (particle_type == 1) {
        particle_type = -1;
    } else if (particle_type == 2) {
        particle_type = 1;
    }


    err = input->getInput("file extension", fextension);
    if (err) {
        fextension = "";
    }

    vector<string> choices;
    choices.push_back("xmgr");
    choices.push_back("csv");
    choices.push_back("egsnrc");
    int choice = input->getInput("output format", choices, 0);
    format = choices[choice];

    if (format == "egsnrc") {
        err = input->getInput("egsnrc format mode", egsnrc_mode);
        if (err) {
            egsnrc_mode = 0;
        }
        if (egsnrc_mode < 0 || egsnrc_mode > 2) {
            egsWarning("Unsupported egsnrc spectrum output format mode. Defaulting to mode 0\n");
        }

    }



    bins = new EGS_ScoringArray(nbins);

    bin_width = (e_max - e_min)/nbins;

    publisher->subscribe(this, NEW_HISTORY);
}


BaseSpectrumScorer::~BaseSpectrumScorer() {
    if (bins) {
        delete bins;
    }
}

void BaseSpectrumScorer::setEffectiveHistories(EGS_Float effh) {
    eff_history = (EGS_I64)effh;
}

int BaseSpectrumScorer::getBin(EGS_Float E) const {
    return min(nbins-1, (int)((E - e_min)/bin_width));
}


EGS_Float BaseSpectrumScorer::getBinWidth() const {
    return bin_width;
}

double BaseSpectrumScorer::getParticleEnergy(const EGS_Particle *p) const {
    double rest_mass = p->q == 0 ? 0 : the_useful->prm;
    return (p->E - rest_mass);
}

int BaseSpectrumScorer::outputResults(string rootname) {

    string fname;
    if (format == "csv") {
        fname = outputCSV(rootname);
    } else if (format == "egsnrc") {
        fname = outputEGSnrc(rootname);
    } else if (format == "xmgr") {
        fname = outputXMGR(rootname);
    } else {
        return 1;
    }

    string sep(80,'-');
    egsInformation("\n%s\n",sep.c_str());
    egsInformation("%s\n", getTitle().c_str());
    egsInformation("Output written to : %s\n", getFileNameFromPath(fname).c_str());
    egsInformation("Particle type     : %s\n", getParticleName().c_str());
    egsInformation(
        "Energy range      : %.3FMeV <= E <= %.3FMeV (%d bins of width %.3GMeV)\n",
        e_min, e_max, nbins, bin_width
    );
    outputTotal();
    egsInformation("%s\n",sep.c_str());

    return 0;
}

string BaseSpectrumScorer::outputCSV(string rootname) {
    ofstream out;
    string fname = getFileName(rootname)+".csv";
    out.open(fname.c_str());
    out << "E,F,dF\n";
    double mid, r, dr;
    for (int i=0; i < nbins; i++) {
        mid = e_min + (i+0.5)*bin_width;
        getResult(i, r, dr);
        out << string_format("%.5E,%.5E,%.5E\n", mid, r, dr);
    }

    out.close();
    return fname;
}

string BaseSpectrumScorer::outputEGSnrc(string rootname) {

    ofstream out;
    string fname = getFileName(rootname)+".spectrum";
    out.open(fname.c_str());
    out << "egs_brachy scored spectrum"<< endl;


    if (egsnrc_mode == 0 || egsnrc_mode == 1) {
        // egsnrc modes
        out << nbins <<","<<e_min<<","<< egsnrc_mode << endl;

        double top, prob, r, dr;
        for (int i=0; i < nbins; i++) {

            getResult(i, r, dr);

            top = e_min + (i + 1)*bin_width;
            prob =  egsnrc_mode == 0 ? r : r/bin_width;

            out << string_format("%.5E,%.5E\n", top, prob);
        }

    } else if (egsnrc_mode == 2) {
        // egs++ line spectrum

        double r, dr, norm=0;
        vector<double> mids;
        vector<double> probabilities;

        // get non zero bins
        for (int i=0; i < nbins; i++) {
            getResult(i, r, dr);

            if (r > 0) {
                mids.push_back(e_min + (i+0.5)*bin_width);
                probabilities.push_back(r);
                norm += r;
            }
        }

        for (size_t i = 0; i < mids.size(); i++) {
            if (i == 0) {
                out << string_format("%d,%.5E,%d\n", mids.size(), mids[i], egsnrc_mode);
            }
            out << string_format("%.5E,%.5E\n", mids[i], probabilities[i]/norm);
        }
    }
    out.close();

    return fname;
}

string BaseSpectrumScorer::outputXMGR(string rootname) {

    ofstream out;
    string fname = getFileName(rootname)+".agr";
    out.open(fname.c_str());
    out << "# EGS_Brachy spectrum\n";
    out << "# \n";
    out << "@    legend 0.2, 0.8\n";
    out << "@    legend box linestyle 0\n";
    out << "@    legend font 4\n";
    out << "@    xaxis  label \""+getXAxisLabel()+"\"\n";
    out << "@    xaxis  label char size 1.3\n";
    out << "@    xaxis  label font 4\n";
    out << "@    xaxis  ticklabel font 4\n";
    out << "@    yaxis  label \""+getYAxisLabel()+"\"\n";
    out << "@    yaxis  label char size 1.3\n";
    out << "@    yaxis  label font 4\n";
    out << "@    yaxis  ticklabel font 4\n";
    out << "@    title \"" << getTitle() << "\"\n";
    out << "@    title font 4\n";
    out << "@    title size 1.500000\n";
    out << "@    subtitle \"" << getSubTitle() << "\"\n";
    out << "@    subtitle font 4\n";
    out << "@    subtitle size 1.000000\n";

    out<<"@    s0 errorbar linestyle 0\n";
    out<<"@    s0 legend \"Spectrum\"\n";
    out<<"@target G0.S0\n";
    out<<"@type xydy\n";

    double mid, bot, top, r, dr;
    for (int i=0; i < nbins; i++) {

        bot = e_min + i*bin_width;
        mid = e_min + (i+0.5)*bin_width;
        top = e_min + (i+ 1)*bin_width;

        getResult(i, r, dr);

        if (i == 0) {
            out << string_format("%.5E %.5E %.5E\n", bot, 0., 0.);
        }


        out << string_format("%.5E %.5E %.5E\n", bot, r, 0.);
        out << string_format("%.5E %.5E %.5E\n", mid, r, dr);
        out << string_format("%.5E %.5E %.5E\n", top, r, 0.);

        if (i == nbins-1) {
            out << string_format("%.5E %.5E %.5E\n", top, 0., 0.);
        }
    }

    out.close();
    return fname;
}

void BaseSpectrumScorer::update(EB_Message message, void *data) {

    if (message == NEW_HISTORY) {
        cur_history = *(EGS_I64 *)(data);
        bins->setHistory(cur_history);
    } else {
        score(message, data);
    }

}

void BaseSpectrumScorer::getResult(int i, EGS_Float &r, EGS_Float &dr) {
    bins->currentResult(i, r, dr);
}

int BaseSpectrumScorer::outputData(ostream *ofile) {

    (*ofile) << cur_history << " " << eff_history << endl;
    (*ofile) << total_scored << endl;
    bool ok = bins->storeState(*ofile);
    if (!ok) {
        return 1;
    }

    return 0;

}

int BaseSpectrumScorer::readData(istream *ifile) {

    (*ifile) >> cur_history >> eff_history;
    (*ifile) >> total_scored;
    bool ok = bins->setState(*ifile);
    if (!ok) {
        return 1;
    }

    return 0;

}

void BaseSpectrumScorer::resetCounter() {

    cur_history = 0;
    setEffectiveHistories(0);
    total_scored = 0;
    bins->reset();

}

int BaseSpectrumScorer::addState(istream &data) {

    EGS_I64 cur_history_tmp, eff_history_tmp;
    EGS_Float total_scored_tmp;
    EGS_ScoringArray tmp_bins(bins->regions());

    data >> cur_history_tmp >> eff_history_tmp;
    data >> total_scored_tmp;
    bool ok = tmp_bins.setState(data);
    if (!ok) {
        return 1;
    }

    cur_history += cur_history_tmp;
    eff_history += eff_history_tmp;
    total_scored += total_scored_tmp;
    (*bins) += tmp_bins;

    return 0;
}


/*********************************************************************************
 * Surface Count Spectrum */

void SurfaceCountSpectrum::score(EB_Message message, void *data) {

    if (message == PARTICLE_ESCAPING_SOURCE) {
        EGS_Particle *p= static_cast<EGS_Particle *>(data);

        double energy = getParticleEnergy(p);

        if (e_min <= energy && energy <= e_max && p->q == particle_type) {
            bins->score(getBin(energy), 1.);
            total_scored += 1.;
        }
    }

}

void SurfaceCountSpectrum::getResult(int i, EGS_Float &r, EGS_Float &dr) {

    bins->currentResult(i, r, dr);
    EGS_Float norm = bin_width * total_scored / cur_history;
    r /= norm;
    dr /= norm;

}

void SurfaceCountSpectrum::outputTotal() {


    egsInformation("Scoring metric    : absolute photon counts on surface of source\n") ;
    egsInformation("Total counts      : %.5G \n", total_scored);
}


/*********************************************************************************
 * Energy weighted surface spectrum */

void EnergyWeightedSurfaceSpectrum::score(EB_Message message, void *data) {

    if (message == PARTICLE_ESCAPING_SOURCE) {
        EGS_Particle *p= static_cast<EGS_Particle *>(data);

        double energy = getParticleEnergy(p);

        if ((e_min <= energy) && (energy <= e_max) && (p->q == particle_type)) {
            bins->score(getBin(energy), energy*p->wt);
            total_scored += energy*p->wt;
        }
    }

}

void EnergyWeightedSurfaceSpectrum::getResult(int i, EGS_Float &r, EGS_Float &dr) {

    bins->currentResult(i, r, dr);
    EGS_Float norm = bin_width * total_scored / cur_history;
    r /= norm;
    dr /= norm;

}

void EnergyWeightedSurfaceSpectrum::outputTotal() {

    egsInformation("Scoring metric    : Energy weighted spectrum on surface of source\n") ;
    egsInformation("Total Radiant E   : %.5G MeV\n", total_scored);

}

/*********************************************************************************
 * Energy fluence spectrum in Voxel */

/* Note: this only works if there are no other geometries overlapping
 * the scoring region! */
void EnergyFluenceSpectrumInVoxel::score(EB_Message message, void *data) {

    if (message == PARTICLE_TAKING_STEP) {
        EGS_Particle *p= static_cast<EGS_Particle *>(data);

        bool in_scoring_region = p->ir == scoring_region;
        bool in_energy_scoring_range = (e_min <= p->E) && (p->E <= e_max);
        bool correct_particle =  p->q == particle_type;

        if (in_scoring_region && in_energy_scoring_range && correct_particle) {
            EGS_Float score = the_epcont->tvstep*p->wt;
            bins->score(getBin(p->E), score);
            total_scored += score;
        }
    }

}

void EnergyFluenceSpectrumInVoxel::getResult(int i, EGS_Float &r, EGS_Float &dr) {

    double eff_history_norm = eff_history / cur_history;
    bins->currentResult(i, r, dr);
    EGS_Float norm = bin_width * region_volume * eff_history_norm;
    r /= norm;
    dr /= norm;

}

void EnergyFluenceSpectrumInVoxel::outputTotal() {

    egsInformation("Scoring metric            : Energy fluence in region\n") ;
    egsInformation("Scoring region (global)   : %d \n", scoring_region) ;
    egsInformation("Scoring region (local)    : %d of %s\n", local_scoring_region, geometry->getName().c_str());
    egsInformation("Region Volume     : %.3G cm^3\n", region_volume) ;
    egsInformation("Total E fluence   : %.5G MeV/cm^2\n", total_scored);

}
