/*
################################################################################
#
#  egs_brachy phantom.h
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

/*! \file phantom.h
 * \author Randle Taylor (randle.taylor@gmail.com)
 * \brief Header file for phantom objects
 *
 */
#ifndef EB_PHANTOM_
#define EB_PHANTOM_

#include <map>
#include <set>
#include "egs_scoring.h"
#include "egs_application.h"
#include "egs_interface2.h"
#include "pubsub.h"

/*! \brief Used internally for keeping track of results for regions, allowing
 * us to sort by dose etc */
struct RegionResult;

/*! \brief A class to represent a single phantom for scoring dose in
 * egs_brachy.
 *
 * A simulation may have an arbitrary number of these phantoms.  This class
 * handles scoring both tracklength and interaction scoring, outputing the top
 * N doses to the console and output to 3ddose files.
 *
 * Region numbers are all 'local' region numbers.  That is the phantom does not
 * know anything about its global region number. egs_brachy should convert to
 * local region number before calling any method taking a region number as
 * input.
 *
 * */
class EB_Phantom : public Subscriber {

private:

    const static string allowed_phantom_geom_types[];

    EGS_Application *app;  ///< Parent application instance. Required for constructing filenames
    EGS_ScoringArray *tlen_score; ///< Tracklength dose scoring array
    EGS_ScoringArray *edep_score; ///< Interaction scored dose scoring array
    EGS_ScoringArray *prim_score; ///< Tracklenth scored dose from primary particles
    EGS_ScoringArray *sscat_score; ///< Tracklenth scored dose from single scattered particles

    EGS_ScoringArray *mscat_score; ///< Tracklenth scored dose from multiple scattered particles

    int nsources;
    EGS_Float dose_scale;

    EGS_Float total_radiant_e;
    EGS_I64 cur_history;
    EGS_Float effective_histories;

    Publisher *publisher;

    std::map<int, double> corrected_volumes; ///< Corrected volume in a given region

    /*! \brief write some stats about dose arrays*/
    void outputDoseStats(EGS_ScoringArray *score, string type);

    /*! \brief write the top_n doses to console */
    void outputTopDoses(int top_n, vector<RegionResult> region_results);

    /*! \brief write the phantom boundaries, doses and uncertainties to 3ddose file*/
    void output3ddoseResults(string);

    /*! \brief write input scoring array to 3ddose file*/
    void output3DDoses(ostream &out, EGS_ScoringArray *score, string type);

    /*! \brief write the phantom bounds to 3ddose file*/
    void output3DBounds(ostream &out);

    /*! \brief initialize  and write an egsphant file for this phantom */
    void outputEGSPhant(string);

    /*! \brief write actual egsphant data to file for this phantom */
    void writeEGSPhant(ostream &);


    /*! \brief create a vector of RegionResult structs which can then be
     * sorted by dose value.  used for output routines */
    vector<RegionResult> getRegionResults();

    /*! \brief get result for region from scoring array and normalize based
     * on the type requested. */
    void getResult(EGS_ScoringArray *, int ireg, string type, EGS_Float &r, EGS_Float &dr);

    /*! \brief get all active scoring arrays, their types and descriptions */
    void getScoringArrays(vector<EGS_ScoringArray *> &scores, vector<string> &types, vector<string> &descriptions);

    /*! \brief get all active scoring arrays for writing to egsdat file*/
    void getEGSdatScoringArrays(vector<EGS_ScoringArray *> &scores);


public:

    enum GeomDirections {
        XDIR, ///< XDIR=0 x dir for rectilinear, r dir for spherical, z dir for cylindrical
        YDIR, ///< YDIR=1 y dir for rectilinear, r dir for cylindrical
        ZDIR  ///< ZDIR=2 z dir for rectilinear
    };

    /*! \brief EB_Phantom constructor */
    EB_Phantom(EGS_Application *, EGS_BaseGeometry *, set<int> global_regions, int nsource, Publisher *publisher);

    /*! \brief EB_Phantom destructor */
    ~EB_Phantom() {

        if (tlen_score) {
            delete tlen_score;
        }
        if (edep_score) {
            delete edep_score;
        }
        if (prim_score) {
            delete prim_score;
        }
        if (sscat_score) {
            delete sscat_score;
        }
        if (mscat_score) {
            delete mscat_score;
        }

    };

    /*! \brief function for checking whether a given geometry type
     * is allowed to be used as a phantom */
    static bool allowedPhantomGeom(const string &geom_type);

    /*! \brief add tracklength dose to region ir */
    void scoreTlen(int ir, EGS_Float dose, EGS_Particle *p);

    /*! \brief add energy deposition dose to region ir */
    void scoreEdep(int ir, EGS_Float dose);

    /*! \brief get current tlen score for region*/
    void getCurrentScore(int ireg, double &sum, double &sum2);

    double getTlenNorm(int ireg);

    /*! \brief set dose scaling factor for output */
    void setDoseScale(EGS_Float);

    void update(EB_Message message, void *data);

    /*! \brief get uncorrected volume for a given region */
    EGS_Float getCorrectedVolume(int ireg);

    /*! \brief get corrected volume for a given region */
    EGS_Float getUncorrectedVolume(int ireg);


    /*! \brief return a vector of all regions which were corrected */
    vector<int> getRegionsWithCorrections();

    /*! \brief Allow user to tell phantom what the actual volume of a region is */
    void setCorrectedVolume(int ir, double fraction);

    /*! \brief enableTLenScoring must be called before simulation
     * begins if you want to score dose with tracklength estimator in
     * addition to tracklength scoring */
    void enableTLenScoring();

    /*! \brief enableInteractionScoring must be called before simulation
     * begins if you want to score dose with interaction scoring in
     * addition to tracklength scoring */
    void enableInteractionScoring();

    /*! \brief enableInteractionScoring must be called before simulation
     * begins if you want to score dose with interaction scoring in
     * addition to tracklength scoring */
    void enableScatterScoring();


    /*! \brief set current history on scoring arrays for proper statistics */
    void setHistory(EGS_I64 current_case);

    /*! \brief set number of effective histories for normalizing scoring arrays */
    void setEffectiveHistories(EGS_Float current_case);

    /*! \brief return average voxel volume */
    EGS_Float avgVoxelVol();

    /*! \brief write voxel volumes, mass, desnity etc */
    void outputVoxelInfo(string format);

    /*! \brief write voxel info file */
    void writeVoxelInfo(ostream &);

    /*! \brief intialize and output write voxel volumes to file */
    void outputVolumeCorrection(string format);

    /*! \brief write voxel volumes to file */
    void writeVolumeCorrection(ostream &);

    /*! \brief tell phantom to output its results. */
    void outputResults(int top_n=20, string output_3ddose="text", string output_egsphant="text",
                       string output_voxinfo="text", string output_volcor="text");

    /*! \brief check whether a global region falls within this phantom */
    bool globalRegIsInPhant(int global_reg) {
        return (global_reg_start <= global_reg) && (global_reg <= global_reg_stop);
    }

    /*! \brief convert global region to local phantom region */
    int globalToLocal(int global_reg) {
        if ((global_reg_start <= global_reg) && (global_reg <= global_reg_stop)) {
            return global_reg - global_reg_start;
        }

        return -1;
    }



    int outputData(ostream *ofile);
    int readData(istream *ifile);
    void resetCounter();
    int addState(istream &ifile);

    /*! \brief return actual density for region */
    EGS_Float getRealRho(int ireg);

    /*! \brief return (corrected) real mass for region */
    EGS_Float getRealMass(int ireg);

    /*! \brief return uncorrected for region */
    EGS_Float getUncorrectedMass(int ireg);


    /*! \brief the phantom geometry object */
    EGS_BaseGeometry *geometry;

    /*! \brief the set of all global regions contained in this phantom
     *
     * \todo There's no reason to store the global regions. The
     * global_reg_start and global_reg_stop values should just be passed
     * into the constructur instead to save on memory.  */
    set<int> global_regions;

    int global_reg_start; ///< starting global region index for this phantom
    int global_reg_stop; ///< ending global region index for this phantom


};

#endif
