/*
################################################################################
#
#  egs_brachy latch.cpp
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

/*! \file latch.cpp
 * \brief methods for setting/checking latch bits for egs_brachy
 *
 * */

#include "latch.h"

void Latch::setFlag(Flag flag, EGS_Particle *p) {
    setFlag(flag, p->latch);
}

void Latch::setFlag(Flag flag, int &latch) {
    latch |= (1 << flag);
}

void Latch::unsetFlag(Flag flag, EGS_Particle *p) {
    unsetFlag(flag, p->latch);
}

void Latch::unsetFlag(Flag flag, int &latch) {
    latch &= ~(1 << flag);
}

bool Latch::checkFlag(Flag flag, EGS_Particle *p) {
    return checkFlag(flag, p->latch);
}

bool Latch::checkFlag(Flag flag, int latch) {
    return latch & (1 << flag);
}

void Latch::update(EB_Message message, void *particle) {

    if (message == PARTICLE_ESCAPED_SOURCE) {
        EGS_Particle *p = static_cast<EGS_Particle *>(particle);
        setFlag(ESCAPED_SOURCE, p);
        setPrimary(p);
    }

}

void Latch::addScatter(EGS_Particle *p) {
    addScatter(p->latch);
}

void Latch::setPrimary(EGS_Particle *p) {
    setPrimary(p->latch);
}

void Latch::setPrimary(int &latch) {
    setFlag(PRIM_PARTICLE, latch);
}

void Latch::addScatter(int &latch) {
    if (checkFlag(PRIM_PARTICLE, latch)) {
        unsetFlag(PRIM_PARTICLE, latch);
        setFlag(SSCAT_PARTICLE, latch);
    } else if (checkFlag(SSCAT_PARTICLE, latch)) {
        unsetFlag(SSCAT_PARTICLE, latch);
        setFlag(MSCAT_PARTICLE, latch);
    } else {
        setFlag(SSCAT_PARTICLE, latch);
    }
}


bool Latch::isPrimary(int latch) {
    return checkFlag(PRIM_PARTICLE, latch);
}

bool Latch::isPrimary(EGS_Particle *p) {
    return isPrimary(p->latch);
}

bool Latch::isSingleScat(int latch) {
    return checkFlag(SSCAT_PARTICLE, latch) && !checkFlag(MSCAT_PARTICLE, latch);
}

bool Latch::isSingleScat(EGS_Particle *p) {
    return isSingleScat(p->latch);
}

bool Latch::isMultScat(int latch) {
    return checkFlag(MSCAT_PARTICLE, latch);
}

bool Latch::isMultScat(EGS_Particle *p) {
    return isMultScat(p->latch);
}

/*bool Latch::inSource(EGS_Particle *p){
    return checkFlag(IN_SOURCE, p);
}
*/

bool Latch::hasEscaped(EGS_Particle *p) {
    return checkFlag(ESCAPED_SOURCE, p);
}

