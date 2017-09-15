/*
################################################################################
#
#  egs_brachy recycle.cpp
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

/*! \file recycle.cpp
 * \brief class imlementations for recycling
 */

#include "recycle.h"

RecycleOpts::RecycleOpts(EGS_Input *inp) {

    if (!inp) {
        return;
    }

    int err = inp->getInput("times to reuse recycled particles", nrecycle);
    if (err || nrecycle < 1) {
        nrecycle = 1;
    }

    if (nrecycle == 1) {
        vector<string> yn;
        yn.push_back("no");
        yn.push_back("yes");
        rotate = (bool)inp->getInput("rotate recycled particles", yn, 0);
    } else {
        rotate = true;
    }


}

void RecycleOpts::printInfo() {

    egsInformation("    Number of times to recycle particles = %d\n", nrecycle);
    egsInformation("    Rotate recycled particles            = %s\n", rotate ? "yes" : "no");

}

