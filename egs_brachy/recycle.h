/*
################################################################################
#
#  egs_brachy recycle.h
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

/*! \file recycle.h
 * \brief class definitions for recycling
 */

#ifndef EGS_BRACHY_RECYCLE
#define EGS_BRACHY_RECYCLE

// stdlib dependencies
#include <map>
#include <set>
#include <stack>
#include <cstdlib>

// egs++ dependencies
#include "egs_functions.h"
#include "egs_input.h"
#include "egs_rndm.h"
#include "egs_transformations.h"

// local dependencies
#include "pubsub.h"
#include "ginfo.h"
#include "latch.h"


class RecycleOpts {


public:

    RecycleOpts(EGS_Input *inp);

    int nrecycle;
    bool rotate;

    void printInfo();

};

#endif
