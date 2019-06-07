/*
###############################################################################
#
#  EGSnrc egs++ envelope geometry headers
#  Copyright (C) 2015 National Research Council Canada
#
#  This file is part of EGSnrc.
#
#  EGSnrc is free software: you can redistribute it and/or modify it under
#  the terms of the GNU Affero General Public License as published by the
#  Free Software Foundation, either version 3 of the License, or (at your
#  option) any later version.
#
#  EGSnrc is distributed in the hope that it will be useful, but WITHOUT ANY
#  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
#  FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for
#  more details.
#
#  You should have received a copy of the GNU Affero General Public License
#  along with EGSnrc. If not, see <http://www.gnu.org/licenses/>.
#
###############################################################################
#
#  Author:          Randle Taylor (randle.taylor@gmail.com) on behalf of
#                   the Carleton Laboratory for Radiotherapy Physics
#
#  Contributors:
#
###############################################################################
*/

/*! \file egs_sobol.cpp
 *
 * \brief Implementation for EGS_Sobol (EGS_RandomGenerator wrapper for Sobol QRNG)
 * \author Randle Taylor
 *
 */
#include "egs_sobol.h"

void EGS_Sobol::saveState() {
    if (copy) {
        copy->setState(this);
    }
    else {
        copy = new EGS_Sobol(*this);
    }
    copy->copy = 0;
}

void EGS_Sobol::resetState() {
    if (copy) {
        setState(copy);
    }
}

EGS_RandomGenerator *EGS_Sobol::getCopy() {
    EGS_Sobol *c = new EGS_Sobol(dim, init_seed);
    c->setState(dim, cur_seed);
    return c;
}

void EGS_Sobol::setState(EGS_RandomGenerator *r) {

    EGS_Sobol *r1 = dynamic_cast<EGS_Sobol *>(r);
    if (!r1) {
        egsFatal("EGS_Sobol::setState: attampt to set my state by a non EGS_Sobol RNG!\n");
    }

    cur_seed = r1->cur_seed;
    dim = r1->dim;
    np = dim;
}

bool EGS_Sobol::storePrivateState(ostream &data) {

    if (!egsStoreI64(data, count)) {
        return false;
    }

    if (!egsStoreI64(data, cur_seed)) {
        return false;
    }

    data << dim << endl;

    return data.good();
}

bool EGS_Sobol::setPrivateState(istream &data) {

    if (!egsGetI64(data, count)) {
        return false;
    }

    if (!egsGetI64(data, cur_seed)) {
        return false;
    }

    int dimension;
    data >> dimension;

    setState(dimension, cur_seed);

    return data.good();
}


void EGS_Sobol::describeRNG() const {
    egsInformation("Random number generator:\n"
                   "============================================\n");
    egsInformation("  type                = Sobol\n");
    egsInformation("  dimensions          = %d\n", dim);
    egsInformation("  initial seed        = %lld\n", init_seed);
    egsInformation("  current seed        = %lld\n", cur_seed);
    egsInformation("  numbers used so far = %lld\n", count);
}

void EGS_Sobol::setState(int dimension, EGS_I64 seed) {

    cur_seed = seed;

    if (dimension < 0 || dimension > 3) {
        dim = 3;
    }
    else {
        dim = dimension;
    }
}

void EGS_Sobol::fillArray(int n, EGS_Float *array) {
    // note cur_seed gets updated each call to i8_sobol
    sobol::i8_sobol(dim, &cur_seed, array);
    count += dim; // we generate dim number every call
}

