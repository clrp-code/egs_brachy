/*
################################################################################
#
#  egs_brachy phsp.cpp
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

/*! \file phsp.cpp
 *
 * \brief Implementation of the PHSPControl object.
 * */

#include "phsp.h"
#include <sys/types.h>
#include <sys/stat.h>
#include "egs_interface2.h"
#include "iaea_header.h"

/*! \brief return true if input path is an existing directory */
bool dirExists(string path) {

    struct stat info;
    int res = stat(path.c_str(), &info);
    return res == 0 && (info.st_mode & S_IFDIR);
}


PHSPControl::PHSPControl(EGS_Input *inp, EGS_AffineTransform *trans, EGS_AdvancedApplication *app, Publisher *pub):
    num_written(0), transform(trans), boundary_step(1E-4) {

    id = 1;

    string dir;
    int err = inp->getInput("phsp output directory", dir);
    if (err) {
        dir = egsJoinPath(app->getAppDir(), app->getWorkDir());
    }

    if (!dirExists(dir)) {
        egsFatal("PHSPControl:: unable to access directory %s\n", dir.c_str());
    }

    fname = egsJoinPath(dir, app->getOutputFile())+".phsp";

    vector<string> yn_choices;
    yn_choices.push_back("no");
    yn_choices.push_back("yes");
    kill_after_scoring = (bool)inp->getInput("kill after scoring", yn_choices, 0);
    print_header = (bool)inp->getInput("print header", yn_choices, 0);

    err = inp->getInput("boundary step", boundary_step);


    vector<string> access_choices;
    access_choices.push_back("append");
    access_choices.push_back("write");
    int imode = inp->getInput("access mode", access_choices, 0);
    if (imode == 0) {
        mode = APPEND;
    } else if (imode == 1) {
        mode = WRITE;
    } else {
        egsFatal("PHSPControl:: unknown access mode\n");
    }

    initSource();

    pub->subscribe(this, PARTICLE_ESCAPED_SOURCE);
}


short PHSPControl::getIAEAParticleType(const EGS_Particle *p) {

    if (p->q == 0) {
        return PHOTON;
    } else if (p->q == -1) {
        return ELECTRON;
    } else if (p->q == 1) {
        return POSITRON;
    }

    egsFatal("PHSPControl::Tried to write unknown particle type\n");
    return -1;
}

void PHSPControl::writeParticle(EGS_Particle *p) {


    // first move back to non transformed location
    EGS_Vector new_loc(p->x);
    EGS_Vector new_dir(p->u);
    EGS_Vector new_dir_norm(p->u);

    new_dir_norm.normalize();
    transform->transform(new_loc);
    transform->rotate(new_dir);

    // take very small step so we are ensured to be outside source geom
    new_loc += boundary_step*new_dir_norm;


    // if particles haven't been split by brem splitting they
    // should be statistically independent when leaving source
    IAEA_I32 n_stat = p->wt < 1. ? 0 : 1;

    const IAEA_I32 type = getIAEAParticleType(p);
    const IAEA_Float E = (IAEA_Float)p->E,
                     wt = (IAEA_Float)p->wt,
                     x = (IAEA_Float)new_loc.x,
                     y = (IAEA_Float)new_loc.y,
                     z = (IAEA_Float)new_loc.z,
                     u = (IAEA_Float)new_dir.x,
                     v = (IAEA_Float)new_dir.y,
                     w = (IAEA_Float)new_dir.z;

    const IAEA_Float extra_float = 0.;
    const IAEA_I32 extra_int = 0;

    iaea_write_particle(&id, &n_stat, &type, &E, &wt, &x, &y, &z, &u, &v, &w, &extra_float, &extra_int);

    num_written++;

    if (kill_after_scoring) {
        p->wt = 0.;
        int np = the_stack->np - 1;
        the_stack->wt[np] = 0;
    }

}


void PHSPControl::initSource() {

    char *hname = new char[fname.size()+1];
    copy(fname.begin(), fname.end(), hname);
    hname[fname.size()] =  '\0';

    IAEA_I32 iaea_idx;
    iaea_new_source(&id, hname, &mode, &iaea_idx, fname.size());

    // we're not currently writing any extra parameters
    IAEA_I32 n_extra_float=0, n_extra_int=0;
    iaea_set_extra_numbers(&id, &n_extra_float, &n_extra_int);

    if (iaea_idx < 0) {
        egsFatal("PHSPControl:: failed to initialize source with IAEA error %d\n", iaea_idx);
    }

    delete[] hname;

}


void PHSPControl::update(EB_Message message, void *particle) {
    if (message == PARTICLE_ESCAPED_SOURCE) {
        EGS_Particle *p = static_cast<EGS_Particle *>(particle);
        writeParticle(p);
    }
}


void PHSPControl::finish(EGS_I64 n_orig_particles) {
    IAEA_I64 n_orig = n_orig_particles;
    iaea_set_total_original_particles(&id, &n_orig);
}


void PHSPControl::outputResults() {

    string sep(80, '=');

    IAEA_I64 n_particles;
    IAEA_I32 particle_type = ALL_TYPES;

    IAEA_I32 result;

    iaea_get_max_particles(&id, &particle_type, &n_particles);

    egsInformation("\n\nPHSP File Scoring\n%s\n", sep.c_str());
    egsInformation("\nOutput file             : %s", (fname+".*").c_str());
    egsInformation("\nNew Particles written   : %d", num_written);
    egsInformation("\nTotal Particles         : %d", n_particles);
    egsInformation("\nBoundary Step (cm)      : %.2G", boundary_step);
    if (print_header) {
        iaea_print_header(&id, &result);
    }

}

void PHSPControl::destroySource() {
    IAEA_I32 result;
    iaea_destroy_source(&id, &result);
}

