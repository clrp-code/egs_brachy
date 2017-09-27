/*
################################################################################
#
#  egs_brachy eb_volcor.cpp
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

/*! \file eb_volcor.cpp
 * \author Randle Taylor (randle.taylor@gmail.com)
 * \brief Main implementation of volume correction routines
 *
 */

#include <eb_volcor.h>
#include <iostream>
#include <fstream>
#include "gzstream.h"

namespace ebvolcor {

/*! looks at first two bytes of a stream and checks if they match
 * the file type specifiers for gzip files */
bool isGZip(istream &vfile) {
    return (vfile.get() == 0x1f && vfile.get() == 0x8b);
}

/*! getShapeVolume takes an EGS_Input for a shape and
* returns the volume of the shape.  Currently the volume will
* be calculated automatically for cylinders, spheres and
* box shapes. Other shapes must specify a volume using
* the *shape volume* input key. For example:
\verbatim

:start shape:
    type = my_new_shape
    input key 1 = 1234
    input key 2= 5678
    shape volume = 123456
:stop shape:

\endverbatim
*
* If *shape volume* is present for a cylinder, sphere or box shape that value
* will be used and the automatic calculation will be ignored
* */
EGS_Float getShapeVolume(EGS_Input *shape_inp) {

    if (!shape_inp) {
        return -1;
    }

    EGS_Float volume;
    int err = shape_inp->getInput("shape volume", volume);
    if (!err) {
        return volume;
    }

    string shape_type;
    shape_inp->getInput("type", shape_type);

    if (shape_type == "cylinder") {
        EGS_Float radius, height;
        shape_inp->getInput("radius", radius);
        shape_inp->getInput("height", height);
        return M_PI*radius*radius*height;
    } else if (shape_type == "sphere") {
        EGS_Float radius, height;
        shape_inp->getInput("radius", radius);
        shape_inp->getInput("height", height);
        return 4./3.*M_PI*radius*radius*radius;
    } else if (shape_type == "spherical shell" || shape_type == "egsSphericalShell") {
        EGS_Float ri, ro;
        int hemi;
        int err = shape_inp->getInput("inner radius", ri);
        if (err) {
            egsFatal("getShapeVolume :: missing 'inner radius' input for shape\n");
        }

        err = shape_inp->getInput("outer radius", ro);
        if (err) {
            egsFatal("getShapeVolume :: missing 'outer radius' input for shape\n");
        }

        err = shape_inp->getInput("hemisphere", hemi);
        if (err) {
            hemi = 0;
        }
        EGS_Float vol = 4./3.*M_PI*(ro*ro*ro - ri*ri*ri);
        if (hemi !=0) {
            return vol/2;
        }

        return vol;

    } else if (shape_type == "box") {
        vector<EGS_Float> box_size;
        shape_inp->getInput("box size", box_size);
        if (box_size.size() == 3) {
            return box_size[0]*box_size[1]*box_size[2];
        } else {
            return box_size[0]*box_size[0]*box_size[0];
        }
    }
    if (shape_type == "") {
        egsWarning("Either include a `type` or `shape volume` input key.");
    } else {
        egsWarning(
            "The volume (in cm^3) for shape type '%s' must be specified using a `shape volume` input key.",
            shape_type.c_str()
        );
    }

    return -1;

}

/*! \brief read mode from input */
void Options::setMode() {
    vector<string> choices;
    choices.push_back("none");
    choices.push_back("zero dose");
    choices.push_back("correct");

    mode = (VolCorMode)input->getInput("correction type",choices, (int)NO_CORRECTION);
}

/*! \brief create bounding shape from the shape input and calculate its volume */
int Options::setBoundsShape() {

    EGS_Input *shape_inp = input->takeInputItem("shape");
    if (!shape_inp) {
        egsWarning("VolCor::Options::setBoundsShape - no `shape` input found.\n");
        return 1;
    }

    bounds_volume = getShapeVolume(shape_inp);
    if (bounds_volume < 0) {
        egsWarning("VolCor::Options::setBoundsShape - unable to calculate volume.\n");
        delete shape_inp;
        return 1;
    }

    bounds = EGS_BaseShape::createShape(shape_inp);
    if (!bounds) {
        egsWarning("VolCor::Options::setBoundsShape - `shape` input not valid.\n");
        delete shape_inp;
        return 1;
    }

    sobolAllowed = bounds->getObjectType() == "box";

    delete shape_inp;
    return 0;

}

/*! set user requested density or default to DEFAULT_RAND_POINT_DENSITY */
void Options::setDensity() {

    int err = input->getInput("density of random points (cm^-3)", density);
    if (err) {
        egsWarning("The volume correction 'density of random points (cm^-3)' input was not found\n");
        density =  (EGS_Float)DEFAULT_RAND_POINT_DENSITY;
    }

    npoints = floor(max(1., density*bounds_volume));
}

/*! set user requested RNG or default to EGS_RandomGenerator::defaultRNG */
void Options::setRNG() {

    EGS_Input *rng_input = input->getInputItem("rng definition");

    if (rng_input) {
        string type;
        int err = rng_input->getInput("type", type);
        if (!err && rng_input->compare(type, "sobol")) {
            if (!sobolAllowed) {
                egsWarning(
                    "Sobol QRNG are not allowed for non rectilinear shapes. "
                    "Using default Ranmar instead.\n"
                );
            } else {
                rng = new EGS_Sobol(rng_input);
            }
        } else {
            rng = EGS_RandomGenerator::createRNG(rng_input);
        }
    }

    if (!rng) {
        if (sobolAllowed) {
            rng = new EGS_Sobol();
        } else {
            rng = EGS_RandomGenerator::defaultRNG();
        }
    }

}

EGS_Vector Options::getRandomPoint() {
    return bounds->getRandomPoint(rng);
}

void VolumeCorrector::setupOptions() {

    EGS_Input *source_cor_inp = input->takeInputItem("source volume correction");
    source_opts = new Options(source_cor_inp);
    if (source_cor_inp) {
        delete source_cor_inp;
    }


    EGS_Input *gen_cor_inp = input->takeInputItem("extra volume correction");
    gen_opts = new Options(gen_cor_inp);
    if (gen_cor_inp) {
        delete gen_cor_inp;
    }

    EGS_Input *file_cor_inp = input->takeInputItem("volume correction from file");

    if (file_cor_inp) {
        EGS_Input *ij;
        int input_num =1;
        while ((ij = file_cor_inp->takeInputItem("phantom file"))) {

            string phantom, filename;
            vector<string> phant_file;
            ij->getInput("phantom file", phant_file);
            if (phant_file.size() != 2) {
                egsWarning("VolumeCorrector::setupOptions ignoring 'phantom file' input number %d\n", input_num);
            } else {
                phantom_files[phant_file[0]] = phant_file[1];
            }

            input_num += 1;
            delete ij;

        }

        delete file_cor_inp;
    }

}

double VolumeCorrector::correctGeneralVolumes() {

    HitCounterT hit_counter;

    for (EGS_I64 i=0; i < gen_opts->npoints; i++) {

        EGS_Vector point = gen_opts->getRandomPoint();
        int global_reg = base_geom->isWhere(point);

        for (size_t pidx=0; pidx < phantoms.size(); pidx++) {
            bool phant_is_covered = !phantoms[pidx]->globalRegIsInPhant(global_reg);
            if (!phant_is_covered) {
                continue;
            }

            int phant_reg = phantoms[pidx]->geometry->isWhere(point);

            PhantRegT phant_and_reg(pidx, phant_reg);
            hit_counter[phant_and_reg] += 1;
        }

    }

    applyVolumeCorrections(gen_opts, hit_counter);

    return 0;

}
// source specific MC volume calculation
//
// Works by generating random points within the sources boundary shape and
// then, if the point is within the source, transforming each point to
// all the sources locations and checking which phantom region the point lands in.
// The volume of a region occupied by a source then is the bounding shape volume
// times the fraction of points landing in the source. The volume of the region
// then is the uncorrected volume minus the volume occupied by source
//     Vcor(reg) = V(reg) - V_source(reg)
//               = V(reg) - V_b * n_points_in_source(reg)/n_total
//
// where V_b is volume of bounding shape
double VolumeCorrector::correctPhantomVolumesForSources() {

    EGS_Vector point;

    HitCounterT hit_counter;
    EGS_I64 n_in_source = 0;


    for (EGS_I64 i=0; i < source_opts->npoints; i++) {

        point = source_opts->getRandomPoint();
        base_transform->transform(point);

        int ir = base_geom->isWhere(point);
        if (!ginfo->isSource(ir)) {
            continue;
        }
        n_in_source += 1;
        base_transform_inv->transform(point);


        for (size_t sidx = 0; sidx < transforms.size();  sidx++) {

            EGS_Vector transformed(point);
            transforms[sidx]->transform(transformed);


            for (size_t phant= 0; phant<phantoms.size(); phant++) {
                int reg = phantoms[phant]->geometry->isWhere(transformed);
                bool in_phant = reg >= 0;

                if (in_phant) {
                    PhantRegT phant_reg(phant, reg);
                    hit_counter[phant_reg] += 1;
                }

            }
        }
    }


    applyVolumeCorrections(source_opts, hit_counter);

    return source_opts->bounds_volume*(double)n_in_source/(int)source_opts->npoints;

}

void VolumeCorrector::applyVolumeCorrections(Options *opts, HitCounterT hit_counter) {

    bool zero_dose = opts->mode == ZERO_DOSE;
    double vol = opts->bounds_volume;
    double npoints =  opts->npoints;

    for (HitCounterT::iterator hi = hit_counter.begin(); hi != hit_counter.end(); hi++) {

        int phant_idx = hi->first.first;
        int phant_reg = hi->first.second;
        if (phant_reg < 0) {
            continue;
        }
        int hits = hi->second;
        double reg_vol = phantoms[phant_idx]->getUncorrectedVolume(phant_reg);
        double corrected_vol = zero_dose ? 0 : max(reg_vol - vol*double(hits)/npoints, 0.);
        phantoms[phant_idx]->setCorrectedVolume(phant_reg, corrected_vol);
    }

}

void readVolumes(istream &vfile, vector<RegVolume> &reg_volumes) {
    size_t nrecords;
    vfile >> nrecords;
    for (size_t rec = 0; rec < nrecords; rec++) {
        int reg;
        EGS_Float vol;
        vfile >> reg >> vol;
        reg_volumes.push_back(RegVolume(reg, vol));
    }
}

int loadVolumes(string fname, vector<RegVolume> &reg_volumes) {
    ifstream vfile(fname.c_str());
    if (!vfile.is_open()) {
        return 1;
    }

    if (isGZip(vfile)) {
        igzstream gzf(fname.c_str());
        readVolumes(gzf, reg_volumes);
        gzf.close();
    } else {
        readVolumes(vfile, reg_volumes);
    }
    vfile.close();

    return 0;

}

map<string, int> VolumeCorrector::loadFileVolumeCorrections() {

    map<string, int> nreg_read_for_phant;

    for (size_t phant_idx=0; phant_idx < phantoms.size(); phant_idx++) {

        string phant_name =  phantoms[phant_idx]->geometry->getName();

        if (phantom_files.count(phant_name)) {

            string fname = phantom_files[phant_name];

            vector<RegVolume> volumes;
            int error = loadVolumes(fname, volumes);
            if (error) {
                egsFatal(
                    "VolumeCorrector::loadFileVolumeCorrections: failed to read "
                    "volumes for '%s' from file '%s'\n", phant_name.c_str(), fname.c_str()
                );
            }

            nreg_read_for_phant[phant_name] = volumes.size();

            for (size_t rv=0; rv < volumes.size(); rv++) {
                RegVolume reg_vol = volumes[rv];
                phantoms[phant_idx]->setCorrectedVolume(reg_vol.first,reg_vol.second);
            }
        }

    }
    return nreg_read_for_phant;
}
}

