/*
################################################################################
#
#  egs_brachy eb_iaeaphsp_source.h
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
 *
 * A minimal IAEA phsp source for use with egs_brachy.
 *
 * See the EB_IAEASource page for input details.
 *
 * */
#ifndef EB_IAEA_SOURCE_
#define EB_IAEA_SOURCE_

#include "egs_config1.h"
#include "egs_vector.h"
#include "egs_base_source.h"
#include "egs_rndm.h"
#include "egs_alias_table.h"
#include "../iaea_phsp/iaea_phsp.h"

#include <fstream>
using namespace std;

#ifdef WIN32

    #ifdef BUILD_EB_IAEA_PHSP_SOURCE_DLL
        #define EB_IAEA_SOURCE_EXPORT __declspec(dllexport)
    #else
        #define EB_IAEA_SOURCE_EXPORT __declspec(dllimport)
    #endif
    #define EB_IAEA_SOURCE_LOCAL

#else

    #ifdef HAVE_VISIBILITY
        #define EB_IAEA_SOURCE_EXPORT __attribute__ ((visibility ("default")))
        #define EB_IAEA_SOURCE_LOCAL  __attribute__ ((visibility ("hidden")))
    #else
        #define EB_IAEA_SOURCE_EXPORT
        #define EB_IAEA_SOURCE_LOCAL
    #endif

#endif

/*! \brief A phase space file source for egs_brachy

  \ingroup Sources

A phase space file source reads and delivers particles from a
an IAEA phase space file. Note this source is an incomplete
implementation and may only be suitable for use with egs_brachy.

A phase space file source is defined as follows:
\verbatim
:start source:
    library = eb_iaeaphsp_source
    name = some_name
    header file = path to the phase space header file
    particle type = one of photons, electrons, positrons, all, or charged
:stop source:

\endverbatim

*/
class EB_IAEA_SOURCE_EXPORT EB_IAEASource : public EGS_BaseSource {

public:

    /*! \brief Constructor

    Construct a phase space file source from the information pointed to by
    \a inp. */
    EB_IAEASource(EGS_Input *, EGS_ObjectFactory *f=0);
    ~EB_IAEASource();

    EGS_I64 getNextParticle(EGS_RandomGenerator *rndm,
                            int &q, int &latch, EGS_Float &E, EGS_Float &wt,
                            EGS_Vector &x, EGS_Vector &u);

    EGS_Float getEmax() const;

    EGS_Float getFluence() const;

    void setSimulationChunk(EGS_I64 nstart, EGS_I64 nrun);
    bool storeState(ostream &data) const;
    bool setState(istream &data);
    bool addState(istream &data);
    void resetCounter();

    bool isValid() const;


protected:

    bool        is_valid;
    string      phsp_file_name; //!< The phase space file name
    ifstream    phsp_file;      //!< Phase space data stream

    IAEA_I32 source_id;
    IAEA_I32 *p_source_id;

    EGS_Float   Emax,    //!< Maximum energy (obtained from the phsp file)
                Emin,    //!< Minimum energy (obtained from the phsp file)
                Nincident;    //!< Number of incident particles that created the file

    IAEA_I64     Nparticle, //!< Number of particles in the file
                 Nphoton,   //!< Number of photons in the file
                 Nused,   //!< Number of particles used so far
                 Npos,      //!< Next record to be read
                 Nfirst,    //!< first record this source can use
                 Nlast,     //!< Last record this source can use
                 Nread,     //!< Number of particles read from file so far
                 count;     //!< Particles delivered so far

    IAEA_I32 n_parallel,
             i_parallel;

    void openPHSPFile();
    void initSourceParams();

    static IAEA_I32 next_source_id;

    static const string iaea_header_ext;


};

#endif
