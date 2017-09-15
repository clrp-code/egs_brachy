/*
################################################################################
#
#  egs_brachy egs_brachy.h
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

/*! \file egs_brachy.h
 * \author Randle Taylor (randle.taylor@gmail.com)
 * \brief the main egs_brachy application header file
 */

#ifndef EGS_BRACHY_
#define EGS_BRACHY_

// stdlib dependencies
#include <map>
#include <set>
#include <stack>
#include <cstdlib>

// egs++ dependencies
#include "egs_advanced_application.h"
#include "egs_functions.h"
#include "egs_input.h"
#include "egs_interface2.h"
#include "egs_interpolator.h"
#include "egs_alias_table.h"
#include "egs_rndm.h"
#include "egs_scoring.h"
#include "egs_transformations.h"
#include "egs_autoenvelope/egs_autoenvelope.h" // required for Superposition mode
#include "egs_rz/egs_rz.h"
#include "egs_spheres/egs_spheres.h"

// local dependencies
#include "pubsub.h"
#include "ginfo.h"
#include "muen.h"
#include "phantom.h"
#include "eb_volcor.h"
#include "spec_scoring.h"
#include "recycle.h"
#include "latch.h"
#include "phsp.h"
#include "timing.h"


#define PRINT_PARTICLE(P) cout << "\nParticle in reg "<<P.ir << " at "<<P.x.x << " "<<P.x.y<<" "<<P.x.z << " wt "<<P.wt<< " E "<<P.E << " q "<<P.q << " latch "<<P.latch<<endl;
#define PRINT_PARTICLE_WITH_DIR(P) cout << "\nParticle in reg "<<P.ir << " at "<<P.x.x << " "<<P.x.y<<" "<<P.x.z << " dir "<<P.u.x << " "<<P.u.y<<" "<<P.u.z << " wt "<<P.wt<< " E "<<P.E << " q "<<P.q << " latch "<<P.latch<<endl;
#define NUM_STUCK_STEPS 1000
#define SAME_POSITION_TOLERANCE 1E-10
#define EB_EPSILON 1E-10



/*! \brief The main egs_brachy application class. See the [Main Page](\ref egs_brachy_main)
 * for full documentation. */
class APP_EXPORT EB_Application : public EGS_AdvancedApplication {

    enum RunMode {
        RM_NORMAL, ///< Standard running mode
        RM_SUPERPOSITION, ///< Superposition mode for intersource effects
        RM_VC_ONLY  ///< Run volume correction routines then quit
    };


    RunMode run_mode; ///< Which run mode are we using (RM_NORMAL, RM_SUPERPOSITION or RM_VC_ONLY */
    string run_mode_name;

    EnergyScoringStats *escoring; ///< Energy related scoring/stats
    vector<BaseSpectrumScorer *> spectrum_scorers;
    //SpectrumScoringControl *spec_scoring;

    RecycleOpts *recycle_opts;
    bool single_generator;
    bool is_phsp_source;

    vector<EGS_Float> source_weights;
    int active_source;

    PHSPControl *phsp;

    bool score_tlen;   ///< true when tracklength estimator is enabled
    bool score_edep;   ///< true when energy deposition is enabled
    bool score_scat;   ///< true when scatter scoring is enabled

    string output_egsdat_format; ///< text or gzip

    bool output_3ddose_files; ///< false if run mode is 'volume correction only'
    string output_dose_format; ///< text or gzip
    bool output_egsphant; ///< true if user requests egsphant ouput
    string output_egsphant_format; ///< text or gzip

    bool output_voxinfo; ///< true if user requests voxel info file
    string output_voxinfo_format; ///< text or gzip

    vector<string> output_volcor_phantoms; ///< vector of phantom names to output volume correctino files for
    string output_volcor_format; ///< text or gzip

    int record_n_init; ///< if > 0 write initial pos of record_n_init particles to {input_file}.pinit
    vector<EGS_Vector> p_init_locs;

    EGS_Vector last_position;
    EGS_Float last_R;
    EGS_Float cur_R;
    int steps_at_same_loc;
    EGS_I64 n_stuck;

    EGS_BaseGeometry *source_envelope_geom;  ///< \brief geometry that the sources are embedded in
    EGS_ASwitchedEnvelope *superpos_geom;  ///< \brief an ASwitchedEnv cast of simulation geometry.

    vector<EB_Phantom *> phantom_geoms; ///< pointers to all of the phantom objects

    vector<EGS_AffineTransform *> source_transforms; ///< transforms to locations of all sources
    EGS_AffineTransform *base_transform; ///< same as source_transforms[0]
    EGS_AffineTransform *base_transform_inv; ///< same as source_transforms[0].inverse()

    map<int, EGS_Interpolator *> media_muen; ///< Map from medium index to muen interpolator for that medium
    map<string, string> media_muen_names;


    bool do_brem_split;
    int nbr_split; ///< Number of times to split bremstrahlung phtons

    bool do_bcse;
    int bcse_med_num;
    EGS_Float bcse_factor;
    static const EGS_Float DEFAULT_BCSE_FACTOR;

    EGS_Float flu_cutoff; ///< fluorescent photon cutoff energy
    EGS_Float source_ecut;  ///< ecut for source objects
    EGS_Float source_pcut;  ///< pcut for source objects
    EGS_Float global_ecut;  ///< ecut for source objects
    EGS_Float global_pcut;  ///< pcut for source objects

    bool global_i_do_rr;  ///< enable range rejection outside of sources
    EGS_Float global_e_max_rr;  ///< max range rejection energy globally

    bool source_i_do_rr;  ///< enable range rejection in sources
    EGS_Float source_e_max_rr;  ///< max range rejection energy for source objects

    GeomInfo ginfo; ///< meta data about the geometries

    EB_Phantom *gcr_phantom;  ///< phantom object to use in getCurrentResult (defaults to 1st phantom)
    int gcr_phantom_reg; ///< region of phantom to use for getCurrentResult (default to 0)

    ebvolcor::Results  source_vc_results;  ///< results from source volume correctio box phantom
    ebvolcor::Results  gen_vc_results; ///< results from general volume correction
    ebvolcor::FileResults  file_vc_results; ///< results from precomputed volume correction

    Publisher pevent_pub; ///< Particle event publisher

    EB_TimingTree timing_blocks; ///< Track CPU times of various functions

    map<int, EGS_I64> steps_in_sources;
    map<int, EGS_I64> steps_in_phantoms;
    map<int, EGS_I64> steps_in_other;

    ogzstream *gz_data_out;  ///< GZip file for outputing egsdat
    igzstream *gz_data_in;  ///< GZip file for outputing egsdat

    static string revision;    ///< the usercode revision number


    /*! \brief set up Phantom objects for any geometries that user has
     * requested scoring for */
    int createPhantoms();


    /*! \brief run the volume correction routines */
    int correctVolumes();

    /*! \brief track length scoring initialization  */
    void initTrackLengthScoring(EGS_Input *);

    /*! \brief load muen data for requested media */
    void initMuenData(EGS_Input *);

    /*! \brief set up whether to output extra info files*/
    void initOutputFiles(EGS_Input *);

    /*! \brief set up phsp scoring*/
    void initPHSPScoring(EGS_Input *);

    /*! \brief energy deposition scoring initialization  */
    void initEDepScoring(EGS_Input *);

    /*! \brief energy deposition scoring initialization  */
    void initScatScoring(EGS_Input *);

    /*! \brief disable all ausgab calls*/
    void clearAusgabCalls();

    /*! \brief enable an array of ausgab calls */
    void enableAusgabCalls(int ncalls, AusgabCall calls[]);


    /*! \brief setup which phantom/region will be used for getCurrentResult */
    void initGCRScoring(EGS_Input *);

    /*! \brief setup any required ausgab calls
     *
     * For efficiency, only those ausgab calls which are
     * strictly neccessary should be enabled */
    void initAusgabCalls();

    /*! \brief Initialize dose scaling factor if requested */
    void initDoseScaling(EGS_Input *);

    /*! \brief Initialize cross section scaling if requested */
    void initXCCScaling(EGS_Input *);

    /*! \brief Initialize all spectrum scoring objects */
    void initSpectrumScoring(EGS_Input *);

    int initCrossSections();

    void discardTopParticle(int idisc=1);

    void calcEffectiveHistories();

    bool isStuck();

public:


    /*! \brief egs_brachy constructor */
    EB_Application(int argc, char **argv) :
        EGS_AdvancedApplication(argc,argv),
        run_mode(RM_NORMAL),
        escoring(),
        recycle_opts(0),
        active_source(0),
        phsp(0),
        score_edep(0),
        score_scat(0),
        output_3ddose_files(true),
        record_n_init(0),
        n_stuck(0),
        superpos_geom(0),
        phantom_geoms(),
        do_brem_split(false),
        nbr_split(1),
        do_bcse(false),
        bcse_factor(1),
        flu_cutoff(0),
        ginfo(),
        gcr_phantom(0), gcr_phantom_reg(0),
        gz_data_out(0),
        gz_data_in(0),
        nsources(0),
        effective_histories(0) {

        for (int i=-1; i < 2; i++) {
            steps_in_sources[i] = 0;
            steps_in_phantoms[i] = 0;
            steps_in_other[i] = 0;
        }

        pevent_pub.subscribe(&latch_control, PARTICLE_ESCAPING_SOURCE);
        pevent_pub.subscribe(&latch_control, PARTICLE_ESCAPED_SOURCE);
    }

    /*! \brief egs_brachy destructor */
    ~EB_Application() {

        for (vector<EB_Phantom>::size_type pp=0; pp < phantom_geoms.size(); pp++) {
            delete phantom_geoms[pp];
        }
        phantom_geoms.clear();

        map<int, EGS_Interpolator *>::iterator mit = media_muen.begin();
        for (; mit != media_muen.end(); mit++) {
            delete mit->second;
        }
        media_muen.clear();


        for (vector<EGS_AffineTransform *>::size_type st=0; st < source_transforms.size(); st++) {
            delete source_transforms[st];
        }
        source_transforms.clear();

        if (phsp) {
            delete phsp;
        }

        if (escoring) {
            delete escoring;
        }

        vector<BaseSpectrumScorer *>::iterator it = spectrum_scorers.begin();
        for (; it != spectrum_scorers.end(); it++) {
            delete *it;
        }
        spectrum_scorers.clear();

        if (recycle_opts) {
            delete recycle_opts;
        }

        if (base_transform_inv) {
            delete base_transform_inv;
        }

        if (gz_data_in) {
            delete gz_data_in;
        }

        if (gz_data_out) {
            delete gz_data_out;
        }

    };

    int nsources; ///< total number of particle sources in current simulation

    EGS_Float effective_histories;

    Latch latch_control;

    /*! \brief Print information about the egs_brachy user code */
    void describeUserCode() const;

    /*! \brief Describe the simulation

      Add extra information to egs_applications describeSimulation
     */
    void describeSimulation();

    void printIncludedFiles();

    /*! \brief set the run mode and then call EGS_AdvancedApplication::initSimulation */
    int initSimulation();

    /*! \brief egs_brachy specific run control initialization */
    int initRunControl();

    /*! \brief Get run mode from the input file */
    int initRunMode();


    /*! \brief initialze all scoring and variance reduction parameters */
    int initScoring();

    /*! \brief initialze all variance reduction parameters */
    int initVarianceReduction();

    /*! \brief Initialize Russian roulette variance reduction if requested */
    int initRussianRoulette(EGS_Input *);

    /*! \brief Initialize BCSE variance reduction if requested */
    int initBCSE(EGS_Input *);

    /*! \brief user scoring of dose, spectra etc */
    int ausgab(int iarg);

    /*! \brief Output intermediate results.
     * The egs_brachy version outputs the standard egs++ data
     * along with egs_brachy specific information such as
     * phantom and spectrum scoring information.  egs_brachy
     * also allows you to output data in gzip format.
    */
    int outputData();

    /*! \brief helper function for outputData */
    int outputDataHelper(ostream *);

    /*! \brief helper function for outputData */
    int egsApplicationOutputData(ostream *);

    /*! \brief helper function for outputData */
    int egsAdvApplicationOutputData(ostream *);

    /*! \brief helper function for outputData */
    int egsBrachyOutputData(ostream *);

    /*! \brief Read data required for restarting simulations.
     *
     * */
    int readData();

    /*! \brief helper function for outputData */
    int readDataHelper(istream *);

    /*! \brief helper function for outputData */
    int egsApplicationReadData(istream *);

    /*! \brief helper function for outputData */
    int egsAdvApplicationReadData(istream *);

    /*! \brief helper function for outputData */
    int egsBrachyReadData(istream *);

    int combineResults();

    /*! \brief Reset the application to a 'pristine' state.
     * Adapted from egs_application.cpp to allow combining
     * in text or gzip format.
     */
    void resetCounter();

    /*! \brief Add data from a parallel job.
     * Add standard egs++ data as well as egs_brachy specific scoring
     * information.
    */
    int addState(istream &data);


    /* \brief output any results to console */
    void outputResults();

    /*! \brief Reports the current results for this batch of the simulation */
    void getCurrentResult(double &sum, double &sum2, double &norm,
                          double &count);

    /*! \brief Set source ecut/pcut if different from global ecut/pcut */
    virtual void startNewParticle();
    virtual void enterNewRegion();

    virtual int runSimulation();

    string getOutputVolcorFormat() {
        return output_volcor_format;
    }

protected:

    void addRecycledParticlesToStack(EGS_Particle *p, bool new_hist=false);
    void copyParticleToSourceLoc(EGS_Particle *p, int source, bool kill_orig, bool rotate, EGS_Float new_wt);

    void doPhotonSplitting(int);


    /* \brief initialize a single shower history.
     *
     * Particle is initiated from source and then transformed to location
     * of next source */
    int simulateSingleShower();
    int startNewShower();

    /*! \brief override default initGeometry so we can manually create our own geometry.
     *
     * This allows us to track region numbers for each geometry object individually
     */
    int initGeometry();


    /* \brief override default initSource to allow us to get source position weighting */
    int initSource();


    /*! \brief read in the location of all particle sources */
    int initSourceTransforms();

    /* \brief take an input containging multiple `transformation` input blocks
     * and return a vector of pointers to the generated transforms.
     */
    vector<EGS_AffineTransform *> createTransforms(EGS_Input *input);

};
#endif
