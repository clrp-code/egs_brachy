/*
################################################################################
#
#  egs_brachy eb_iaeaphsp_source.cpp
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

/*! \file eb_iaeaphsp_source.h
 * \brief a minimal IAEA phase space source for egs_brachy
 * */
#include <sstream>
#include "eb_iaeaphsp_source.h"
#include "egs_application.h"
#include "egs_input.h"
#include "egs_functions.h"



IAEA_I32 EB_IAEASource::next_source_id = 0;
const string EB_IAEASource::iaea_header_ext =  ".IAEAheader";

EB_IAEASource::EB_IAEASource(EGS_Input *input, EGS_ObjectFactory *f) :
    EGS_BaseSource(input,f) {

    otype = "EB_IAEAPHSPSource";

    int err = input->getInput("header file", phsp_file_name);
    if (err) {
        egsWarning("EB_IAEASource: no 'header file' input\n");
        return;
    }

    size_t e_sz = iaea_header_ext.size();
    size_t p_sz = phsp_file_name.size();
    if ((p_sz > e_sz) && (phsp_file_name.substr(p_sz - e_sz) == iaea_header_ext)) {
        phsp_file_name = phsp_file_name.substr(0,p_sz - e_sz);
    }

    p_source_id = &source_id;
    source_id = next_source_id++;

    EGS_Application *app = EGS_Application::activeApplication();

    i_parallel = app->getIparallel();
    n_parallel =  app->getNparallel();

    Nread = 0;
    count = 0;
    Npos = 0;
    Nlast = 0;

    openPHSPFile();

    if (!isValid()) {
        egsWarning(
            "EB_IAEASource: errors while opening the phase space file %s\n",
            phsp_file_name.c_str()
        );
        return;
    }

    initSourceParams();

    count = 0;

    stringstream ss;
    ss << "Phase space source from " + phsp_file_name << endl;
    ss << "    Number of original particles  = " << Nincident << endl;
    ss << "    Number of particles in phsp   = " << Nparticle << endl;
    ss << "    Max energy of particles       = " << Emax << " MeV" << endl;
    description = ss.str();

}


EB_IAEASource::~EB_IAEASource() {
    if (isValid()) {
        IAEA_I32 result;
        iaea_destroy_source(p_source_id, &result);
    }

}


void EB_IAEASource::openPHSPFile() {

    IAEA_I32 access = 1; // read only
    IAEA_I32 result;
    char *hname = new char[phsp_file_name.size()+1];
    copy(phsp_file_name.begin(), phsp_file_name.end(), hname);
    hname[phsp_file_name.size()] =  '\0';

    iaea_new_source(p_source_id, hname, &access, &result, phsp_file_name.size());

    is_valid = result > 0;


}

void EB_IAEASource::initSourceParams() {

    IAEA_Float ftmp;
    IAEA_I64 i64tmp;

    Emin = 0;

    iaea_get_maximum_energy(p_source_id, &ftmp);
    Emax = (EGS_Float)ftmp;

    iaea_get_total_original_particles(p_source_id, &i64tmp);
    Nincident = (IAEA_I64)i64tmp;

    IAEA_I32 type = -1;
    iaea_get_max_particles(p_source_id, &type, &i64tmp);
    Nparticle = (IAEA_I64)i64tmp;

    iaea_get_used_original_particles(p_source_id, &i64tmp);
    Nused = (IAEA_I64)i64tmp;

    Npos = 0;
    Nlast = Nparticle;
    Nfirst = 1;

}

IAEA_I64 EB_IAEASource::getNextParticle(EGS_RandomGenerator *rndm, int &q,
                                        int &latch, EGS_Float &E, EGS_Float &wt, EGS_Vector &x, EGS_Vector &u) {

    IAEA_I32 n_stat, type, extra_ints;
    IAEA_Float Ei, wti, xi, yi, zi, ui, vi, wi, extra_floats;

    iaea_get_particle(
        p_source_id, &n_stat,
        &type, &Ei, &wti,
        &xi, &yi, &zi,
        &ui, &vi, &wi,
        &extra_floats, &extra_ints
    );
    Nread++;

    if (n_stat < 0) {
        egsInformation(
            "\nEB_IAEASource:: Error reading particle from phsp file (n_stat=%d).\n"
            "Rewinding to first record (may cause statistical biases)\n",
            n_stat
        );
        IAEA_I32 result;
        iaea_set_record(p_source_id, &Nfirst, &result);
        return getNextParticle(rndm, q, latch, E, wt, x, u);
    }


    if (type == 1) {
        q = 0;
    } else if (type == 2) {
        q = -1;
    } else if (type == 3) {
        q = 1;
    }


    latch = 0;
    E = Ei;
    wt = wti;
    x.x = xi;
    x.y = yi;
    x.z = zi;
    u.x = ui;
    u.y = vi;
    u.z = wi;
    count++;

    return count;
}


void EB_IAEASource::setSimulationChunk(IAEA_I64 nstart, IAEA_I64 nrun) {

    if (nstart < 0 || nrun < 1 || nstart + nrun > Nparticle) {
        egsWarning(
            "EB_IAEASource::setSimulationChunk(): illegal attempt "
            "to set the simulation chunk between %lld and %lld ignored\n",
            nstart, nstart + nrun - 1);
        return;
    }

    Nfirst = nstart + 1;
    Nlast = Nfirst + nrun;
    Npos = Nfirst;

    IAEA_I32 res;
    iaea_set_record(p_source_id, &Npos, &res);

    if (res !=0) {
        egsFatal("EB_IAEASource:: failed to set record to %d", Npos);
    }

}


EGS_Float EB_IAEASource::getEmax() const {
    return Emax;
};

bool EB_IAEASource::isValid() const {
    return is_valid;
}

EGS_Float EB_IAEASource::getFluence() const {
    double aux = ((double) count)/((double) Nparticle);
    return Nincident*aux;
}

bool EB_IAEASource::storeState(ostream &data) const {

    data << endl;

    bool res = egsStoreI64(data, Nread);
    if (!res) {
        return res;
    }
    data << "  ";

    res = egsStoreI64(data, Nfirst);
    if (!res) {
        return res;
    }
    data << "  ";

    res = egsStoreI64(data, Nlast);
    if (!res) {
        return res;
    }
    data << "  ";

    res = egsStoreI64(data, Npos);
    if (!res) {
        return res;
    }
    data << "  ";

    res = egsStoreI64(data, count);
    if (!res) {
        return res;
    }
    data << "  ";

    return true;
}

bool EB_IAEASource::setState(istream &data) {


    bool res = egsGetI64(data,Nread);
    if (!res) {
        return res;
    }

    res = egsGetI64(data,Nfirst);
    if (!res) {
        return res;
    }

    res = egsGetI64(data,Nlast);
    if (!res) {
        return res;
    }

    res = egsGetI64(data,Npos);
    if (!res) {
        return res;
    }


    IAEA_I32 ires;
    iaea_set_record(p_source_id, &Npos, &ires);
    if (ires !=0) {
        egsInformation("Failed to find record %d\n", ires);
        return false;
    }

    res = egsGetI64(data, count);

    return res;

};

bool EB_IAEASource::addState(istream &data) {

    IAEA_I64 tmp_Nread = Nread;
    IAEA_I64 tmp_count = count;
    bool res = setState(data);
    Nread += tmp_Nread;
    count += tmp_count;

    return res;
};

void EB_IAEASource::resetCounter() {
    Nread = 0;
    count = 0;
}

extern "C" {

    EB_IAEA_SOURCE_EXPORT EGS_BaseSource *createSource(EGS_Input *input, EGS_ObjectFactory *f) {
        return createSourceTemplate<EB_IAEASource>(input,f,"iaea phsp source");
    }

}
