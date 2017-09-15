/*
################################################################################
#
#  egs_brachy phsp.h
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

/*! \file phsp.h
 *
 * \brief Definition of the PHSPControl object.
 * */

#include "egs_functions.h"
#include "egs_input.h"
#include "egs_advanced_application.h"
#include "egs_transformations.h"
#include "pubsub.h"
#include "iaea_phsp.h"

class PHSPControl : public Subscriber {

    enum ACCESS {
        READ = 1, ///< IAEA Read mode
        WRITE = 2, ///< IAEA Write mode
        APPEND = 3 ///< IAEA Append mode
    };

    enum PARTICLE_TYPE {
        ALL_TYPES = -1,
        PHOTON = 1,
        ELECTRON = 2,
        POSITRON = 3,
        NEUTRON = 4,
        PROTON = 5
    };

    IAEA_I32 mode;  ///< Access mode
    string fname;   ///< root name of phsp header
    IAEA_I32 id;    ///< IAEA Source ID (just set to 1  currently)

    IAEA_I64 num_written; ///< Number of particles written to phsp file


    EGS_AffineTransform *transform; /*! transform used to return particle to
                                     * origin before writing to phsp */

    EGS_Float boundary_step; /*! Distance to push particle past source boundary
                              *  before  writing to phase space.
                              *  Defaults to 1E-4 cm*/

    bool kill_after_scoring; ///< Set wt = 0 for particle after scoring if true

    bool print_header; ///< User has requested the phsp header gets printed after run

    /*! \brief create/open new source and set extra numbers  */
    void initSource();

    /*! \brief write a single particle to the phsp */
    void writeParticle(EGS_Particle *p);


    /*! \brief convert a particle to its IAEA Particle type */
    short getIAEAParticleType(const EGS_Particle *p);



public:

    /*! \brief PHSP Control constructor */
    PHSPControl(EGS_Input *inp, EGS_AffineTransform *trans, EGS_AdvancedApplication *app, Publisher *pub);

    /*! \brief set final number of particles written and destroy source */
    void finish(EGS_I64 n_orig_particles);

    /*! \brief destroy the source */
    void destroySource();


    /*! \brief output file name and number of particles written */
    void outputResults();

    /*! \brief receive PARTICLE_ESCAPED_SOURCE message */
    void update(EB_Message message, void *particle);

};

