/*
################################################################################
#
#  egs_brachy eb_volcor.h
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

/*! \file eb_volcor.h
 * \brief Volume correction routines for egs_brachy
 * \author Randle Taylor (randle.taylor@gmail.com)
 * \version 0.1
 *
 *
 * The source specific volume correction here is identical to the one used by
 * the egs_autoenvelope geometry.  There is also an additional general purpose
 * volume correction routine to allow for correcting volumes of multiple
 * overlapping phantoms.
 *
 * egs_brachy uses its own volume correction routines to allow for correction
 * of arbitrary phantom types (rather than just auto_envelopes).  In practice
 * this may not be required and the autoenvelope volume corrections and the
 * egs_brachy volume correction routines(eb_volcor) can be factored out and
 * combined into a single general purpose MC volume correction library.
 */


#ifndef EB_VOLCOR_
#define EB_VOLCOR_

#include <map>
#include <set>
#include <cstdlib>

#include "egs_functions.h"
#include "egs_input.h"
#include "egs_rndm.h"
#include "egs_shapes.h"

#include "phantom.h"
#include "ginfo.h"
#include "timing.h"

#include "egs_autoenvelope/egs_sobol.h" // required for Superposition mode

namespace ebvolcor {

enum VolCorMode {NO_CORRECTION, ZERO_DOSE, CORRECT_VOLUME};

/*! \brief PhantRegT is a pair of the form (PhantomNumber, PhantomRegion) e.g. a pair
 * of (2, 12) would represent region 12 (i.e. the 13th region) of phantom 2
 * (i.e. the 3rd phantom) */
typedef pair<int, int> PhantRegT;

/*! \brief RegVolumeT is a pair of the form (RegionNumber, Volume) */
typedef pair<int, EGS_Float> RegVolume;

/*! \brief HitCounterT is used for counting how many random points land in a
 * given phantoms region */
typedef std::map<PhantRegT, EGS_I64> HitCounterT;


/*! \brief Volume correction initialization helper class
 *
 * Options is a small helper class for parsing a volume correction input item A
 * sample input including both a source sspecific correction and a general
 * purpose correction would look like this: \verbatim

 :start volume correction:


     :start source volume correction:
         correction type = correct # correct, none, zero dose
         density of random points (cm^-3) = 1E8

         :start shape:

             type = sphere
             radius = 0.11
             midpoint = 0 0 0

         :stop shape:

     :stop source volume correction:

     :start extra volume correction:

         correction type = correct # correct, none, zero dose
         density of random points (cm^-3) = 1E5

         :start shape:

             type = box
             box size = 8
             midpoint = 0 0 0

         :stop shape:

     :stop extra volume correction:

 :stop volume correction:
\endverbatim

 * An EGS_RNG input can also optionally be included in either of the volume
 * correction inputs to use something other than the EGSnrc default RNG.
 *
 * */
class Options {

    const static unsigned long DEFAULT_RAND_POINT_DENSITY = 100000000;

    EGS_Input *input;


    EGS_BaseShape *bounds;
    bool sobolAllowed;


    EGS_RandomGenerator *rng;

    void setMode();
    int setBoundsShape();
    void setDensity();
    void setRNG();

public:
    Options(EGS_Input *inp):

        input(inp), bounds(NULL), sobolAllowed(false), rng(NULL) {

        valid=true;

        if (!inp) {
            valid = false;
            return;
        }

        setMode();

        int err = setBoundsShape();
        if (err) {
            valid = false;
            return;
        }

        setDensity();
        setRNG();

    }

    ~Options() {
        if (rng) {
            delete rng;
        }
        if (bounds) {
            delete bounds;
        }
    }

    bool valid;

    EGS_Float bounds_volume;
    EGS_Float density;
    EGS_Float npoints;
    VolCorMode mode;

    EGS_Vector getRandomPoint();
};


/*! \brief Struct used to collect and output results about a volume correction run.*/
struct Results {

    bool success;  /*!< did the volume correction succeeed? */
    EGS_Float time; /*!< how long (s) did the volume correction take */
    double density; /*!< what was the density of points used for the VC */
    double npoints; /*!< what was the total number of points used for the VC */
    EGS_Float bounds_volume; /*!< what was the volume of the bounding shape */
    EGS_Float other_volume; /*!< what was the estimated volume of the inscribed geometry */

    map<int, vector<int> > regions_corrected;

    Results():
        success(false),
        time(0),
        density(0),
        npoints(0),
        bounds_volume(0),
        other_volume(0) {};

    Results(Options *opts):
        success(false),
        time(0),
        other_volume(0) {
        density = opts->density;
        npoints =  opts->npoints;
        bounds_volume = opts->bounds_volume;
    }

    void outputResults(string extra="") {

        egsInformation("Time taken                   = %.2G s\n", time);
        egsInformation("Density of points used       = %.0E points/cm^-3\n", density);
        egsInformation("Number of source points used = %G\n", npoints);
        egsInformation("Bounding shape volume        = %.4E cm^3\n", bounds_volume);
        if (extra != "") {
            egsInformation("Volume of %-10s         = %.4E cm^3\n", extra.c_str(), other_volume);
        }

    }

};

struct FileResults {

    bool success;  /*!< did the volume correction succeeed? */
    EGS_Float time; /*!< how long (s) did the volume correction take */
    map<string, string> phantom_files;
    map<string, int> nreg;

    FileResults():
        success(false),
        time(0) {};

    FileResults(map<string, string> phant_files):
        success(false),
        time(0),
        phantom_files(phant_files) {};

    void outputResults() {
        egsInformation("Time taken                   = %.2G s\n", time);
        for (map<string, int>::iterator it=nreg.begin(); it !=nreg.end(); it++) {
            string phant_name = it->first;
            int npoints = it->second;
            string file_name = phantom_files[phant_name];

            egsInformation("Read %d voxel volumes for '%s' from %s\n", npoints, phant_name.c_str(), file_name.c_str());

        }
    }

};


/*! \brief An object for controlling the volume correction routine.
 *
 * See EB_Application::correctVolumes for usage.
 *
 */
class VolumeCorrector {

    EGS_Input *input;
    Options *source_opts;
    Options *gen_opts;
    map<string, string> phantom_files;
    vector<EB_Phantom *> phantoms;
    EGS_BaseGeometry *base_geom;
    //EGS_BaseGeometry *source_geom;
    GeomInfo *ginfo;
    vector<EGS_AffineTransform *> transforms;
    EGS_AffineTransform *base_transform;
    EGS_AffineTransform *base_transform_inv;

    void setupOptions();

    double correctPhantomVolumesForSources();
    double correctGeneralVolumes();
    void applyVolumeCorrections(Options *options, HitCounterT hit_counter);
    map<string, int> loadFileVolumeCorrections();

public:
    VolumeCorrector(EGS_Input *volcor_input,
                    vector<EB_Phantom *> phantoms, EGS_BaseGeometry *base_geom,
                    GeomInfo *geom_info, vector<EGS_AffineTransform *> transforms = vector<EGS_AffineTransform *>()):
        input(volcor_input),
        phantoms(phantoms),
        base_geom(base_geom),
        ginfo(geom_info),

        transforms(transforms) {

        setupOptions();

        if (transforms.size()>0) {
            base_transform = transforms[0];
            base_transform_inv = new EGS_AffineTransform(transforms[0]->inverse());
        }


    };

    ~VolumeCorrector() {
        if (source_opts) {
            delete source_opts;
        }

        if (gen_opts) {
            delete gen_opts;
        }
        if (base_transform_inv) {
            delete base_transform_inv;
        }
    }

    Results runSourceCorrection(EB_TimingTree &timer) {
        timer.addTimer("VolumeCorrector::runSourceCorrection");
        Results results(source_opts);

        if (!source_opts->valid || source_opts->mode == NO_CORRECTION) {
            timer.stopTimer();
            return results;

        }

        clock_t start_time = clock();
        results.other_volume = correctPhantomVolumesForSources();
        clock_t end_time = clock();
        results.time = (end_time-start_time)/(double)CLOCKS_PER_SEC;

        results.success = true;
        timer.stopTimer();

        return results;
    }

    Results runGeneralCorrection(EB_TimingTree &timer) {
        timer.addTimer("VolumeCorrector::runGeneralCorrection");
        Results results(gen_opts);

        if (!gen_opts->valid ||  gen_opts->mode == NO_CORRECTION) {
            timer.stopTimer();
            return results;
        }

        clock_t start_time = clock();
        results.other_volume = correctGeneralVolumes();
        clock_t end_time = clock();

        results.time = (end_time-start_time)/(double)CLOCKS_PER_SEC;
        results.success = true;

        timer.stopTimer();
        return results;
    }

    FileResults runFileCorrection(EB_TimingTree &timer) {
        timer.addTimer("VolumeCorrector::runFileCorrection");
        vector<int> nreg_corrected;
        FileResults results(phantom_files);

        if (phantom_files.size()==0) {
            timer.stopTimer();
            return results;
        }

        clock_t start_time = clock();
        results.nreg =  loadFileVolumeCorrections();
        clock_t end_time = clock();
        results.time = (end_time-start_time)/(double)CLOCKS_PER_SEC;

        results.success = true;
        timer.stopTimer();

        return results;
    };

};

}

#endif
