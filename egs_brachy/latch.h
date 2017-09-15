/*
################################################################################
#
#  egs_brachy latch.h
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

/*! \file latch.h
 * \brief handle particles latch bits for egs_brachy
 * */

#ifndef EGS_BRACHY_LATCH
#define EGS_BRACHY_LATCH
#include "egs_advanced_application.h"
#include "pubsub.h"

/*! \brief A class for handling latch bits relevant to egs_brachy.  The Latch
 * class listens for particle events and sets/unsets latch bits on the particle
 * based on the event type. */
class Latch : public Subscriber {

    enum Flag {
        IN_SOURCE, /*!< a particle is in the source (either hasn't
                     escaped yet, or has re-entered) */
        ESCAPED_SOURCE, /*!< an initial particle has escaped the source
                          (this bit will still be set on particles which
                          ave re-entered a source geometry)*/

        PRIM_PARTICLE,
        SSCAT_PARTICLE,
        MSCAT_PARTICLE

    };




public:

    /*! \brief listen to events and delegate to appropriate handler */
    void update(EB_Message message, void *particle);

    /*! \brief True if particle is currently in a source */
    //bool inSource(EGS_Particle *p);

    /*! \brief True if particle has escaped a source */
    static bool hasEscaped(EGS_Particle *p);

    /*! \brief Set flag on input particle p */
    static void setFlag(Flag flag, EGS_Particle *p);

    /*! \brief Set flag on input latch */
    static void setFlag(Flag flag, int &latch);

    /*! \brief Unset flag on input particle p */
    static void unsetFlag(Flag flag, EGS_Particle *p);

    /*! \brief Unset flag on input latch p */
    static void unsetFlag(Flag flag, int &latch);

    /*! \brief Check whether flag is set on particle p*/
    static bool checkFlag(Flag flag, EGS_Particle *p);

    /*! \brief Check whether flag is set on latch*/
    static bool checkFlag(Flag flag, int latch);

    /* \brief add scattering event to particle latch */
    static void addScatter(EGS_Particle *p);

    /* \brief add scattering event to latch */
    static void addScatter(int &latch);

    /* \brief set primary flag for latch */
    static void setPrimary(int &latch);

    /* \brief set primary flag for particle*/
    static void setPrimary(EGS_Particle *p);

    /* \brief check if unscattered */
    static bool isPrimary(int latch);
    static bool isPrimary(EGS_Particle *p);

    /* \brief check if single scattered*/
    static bool isSingleScat(int latch);
    static bool isSingleScat(EGS_Particle *p);

    /* \brief check if multiple scattered*/
    static bool isMultScat(int latch);
    static bool isMultScat(EGS_Particle *p);

};

#endif
