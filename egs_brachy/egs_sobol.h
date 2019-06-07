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

/*! \file egs_sobol.h
 *
 * \brief Header file for EGS_Sobol (EGS_RandomGenerator wrapper for Sobol QRNG)
 * \author Randle Taylor
 *
 */
#ifndef EGS_SOBOL_
#define EGS_SOBOL_

#ifdef WIN32

    #ifdef BUILD_SOBOL_DLL
        #define EGS_SOBOL_EXPORT __declspec(dllexport)
    #else
        #define EGS_SOBOL_EXPORT __declspec(dllimport)
    #endif
    #define EGS_SOBOL_LOCAL

#else

    #ifdef HAVE_VISIBILITY
        #define EGS_SOBOL_EXPORT __attribute__ ((visibility ("default")))
        #define EGS_SOBOL_LOCAL  __attribute__ ((visibility ("hidden")))
    #else
        #define EGS_SOBOL_EXPORT
        #define EGS_SOBOL_LOCAL
    #endif

#endif

#include "sobol.h"

#include "egs_rndm.h"
#include "egs_input.h"
#include "egs_functions.h"


/*! \brief EGS_RandomGenerator wrapper for Sobol library
 */
class EGS_SOBOL_LOCAL EGS_Sobol : public EGS_RandomGenerator {

public:

    EGS_Sobol(EGS_Input *inp): copy(0) {

        int dimension;
        int err = inp->getInput("dimension", dimension);
        if (err || dimension < 0 || dimension > 3) {
            dimension = 3;
        }

        init_seed=-1;
        err = inp->getInput("initial seed", init_seed);
        if (err || init_seed < 1) {
            init_seed = sobol::tau_sobol(dimension);
        }

        np = dimension;

        setState(dimension, init_seed);
    };

    EGS_Sobol(int dimension=3, EGS_I64 iseed=-1) :
        EGS_RandomGenerator(dimension), copy(0) {

        np = dimension;

        if (iseed < 0) {
            iseed=sobol::tau_sobol(dimension);
        }

        init_seed = iseed;

        setState(dimension, iseed);
    };

    EGS_Sobol(const EGS_Sobol &r): copy(0) {
        np =  r.dim;
        init_seed = r.init_seed;
        setState(r.dim, r.init_seed);
    };

    ~EGS_Sobol() {
        /*if (copy){
            delete copy;
        }
        */
    };

    /*! \brief Fill the array pointed to by \a array with random numbers
     */
    void fillArray(int n, EGS_Float *array);

    /*! \brief Output information about this RNG using egsInformation() */
    void describeRNG() const;

    EGS_RandomGenerator *getCopy();

    void setState(EGS_RandomGenerator *r);

    void saveState();

    void resetState();

    int  rngSize() const {
        return baseSize() + sizeof(int) + 2*sizeof(EGS_I64);
    };

protected:

    /*! Stores the pointers ix and jx, the carry c, the initial seeds that
     * were used to initialize the generator and the 97 element array u.
     */
    bool storePrivateState(ostream &data);

    /*! Reads the same data stored by storePrivateState() */
    bool setPrivateState(istream &data);


private:

    void setState(int dimension, EGS_I64 init_seed);

    int dim;
    EGS_I64 init_seed, /*! \brief initial seed for random number generator */
            cur_seed; /*! \brief current seed being used for random number generator (required to reinitialize */

    EGS_Sobol *copy;
};
#endif
