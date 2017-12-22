/*
################################################################################
#
#  egs_brachy spec_scoring.h
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

/*! \file spec_scoring.h
 *
 * \brief Definition of spectrum scoring classes.
 * */

#include <sstream>
#include "egs_advanced_application.h"
#include "egs_functions.h"
#include "egs_input.h"
#include "egs_interface2.h"
#include "egs_scoring.h"
#include "ginfo.h"
#include "pubsub.h"


/*! \brief a class to use for scoring information about total energy initialzed,
 * escaping sources etc
 * */
class EnergyScoringStats : public Subscriber {

    EGS_Float total_energy_initialized; /*!< \brief total energy of particles initialized so far */
    EGS_Float energy_escaping_sources;  /*!< \brief total energy of particles escaping the source geometry
                                        note: doesn't currently exclude particles reentering
                                        the source geometry and then escaping again */

    EGS_Float energy_escaping_geom;  /*!< \brief total energy of particles escaping the simulation geometry */

    EGS_Float getParticleEnergy(const EGS_Particle *p, bool subtractRM=true) {
        EGS_Float rest_mass = 0;
        if (subtractRM && p->q != 0){
            rest_mass = the_useful->prm;
        }
        return (p->E - rest_mass)*p->wt;
    }

    /*! \brief add energy from initial particle to total */
    void scoreParticleInitialized(EGS_Particle *p) {
        /* When particle is first initialized by getNextParticle it does not
         * include rest mass so we tell getParticleEnergy not to subtract the
         * rest mass here. */
        total_energy_initialized += getParticleEnergy(p, false);
    }

    /*! \brief add energy from a particle escaping a source to the total */
    void scoreParticleEscapingSource(EGS_Particle *p) {
        energy_escaping_sources += getParticleEnergy(p);
    }

    /*! \brief add energy from a escaping simulation geometry to the total */
    void scoreParticleEscapingGeom(EGS_Particle *p) {
        energy_escaping_geom += getParticleEnergy(p);
    }

public:

    EnergyScoringStats(Publisher *publisher) :
        total_energy_initialized(0),
        energy_escaping_sources(0),
        energy_escaping_geom(0) {

        publisher->subscribe(this, PARTICLE_INITIALIZED);
        publisher->subscribe(this, PARTICLE_ESCAPING_SOURCE);
        publisher->subscribe(this, PARTICLE_ESCAPING_GEOM);

    };


    /*! \brief Needed for loading data from egsdat file */
    void scoreEnergyInitialized(EGS_Float E) {
        total_energy_initialized += E;
    }
    /*! \brief return the ratio of energy escaping the source to total
        * energy initialized */
    EGS_Float escapingSourcesRatio() {
        if (total_energy_initialized > 0) {
            return energy_escaping_sources/total_energy_initialized;
        }
        return 0;
    };


    /*! \brief return the ratio of energy escaping the simulation geometry
        * to total energy initialized */
    EGS_Float escapingGeomRatio() {
        if (total_energy_initialized > 0) {
            return energy_escaping_geom/total_energy_initialized;
        }

        return 0;
    }

    /*! \brief returns the total energy initialized */
    EGS_Float totalEnergyInitialized() {
        return total_energy_initialized;
    }

    /*! \brief returns the total paticle energy escaping source geometry */
    EGS_Float energyEscapingSources() {
        return energy_escaping_sources;
    }

    /*! \brief returns the total energy of particles escaping simulation geometry */
    EGS_Float energyEscapingGeom() {
        return energy_escaping_geom;
    }


    /*! \brief delegate messages to appropriate scoring functions */
    void update(EB_Message message, void *particle) {

        if (message == PARTICLE_INITIALIZED) {
            EGS_Particle *p = static_cast<EGS_Particle *>(particle);
            scoreParticleInitialized(p);
        } else if (message == PARTICLE_ESCAPING_SOURCE) {
            EGS_Particle *p = static_cast<EGS_Particle *>(particle);
            scoreParticleEscapingSource(p);
        } else if (message == PARTICLE_ESCAPING_GEOM) {

            EGS_Particle *p = static_cast<EGS_Particle *>(particle);
            scoreParticleEscapingGeom(p);
        }

    }


    /*! \brief write results to console */
    void outputResults() {

        string sep(80, '=');

        egsInformation("\n\nEnergy Initialized and Escaping\n%s\n", sep.c_str());
        egsInformation("Total Energy Initialized    : %.5G MeV", totalEnergyInitialized());
        egsInformation(
            "\nTotal Energy Escaping Sources : %.5G MeV (%.3f%%)",
            energyEscapingSources(),
            escapingSourcesRatio()*100
        );

        egsInformation(
            "\nTotal Energy Escaping Geometry: %.5G MeV (%.3f%%)\n",
            energyEscapingGeom(),
            escapingGeomRatio()*100
        );

    }

    int outputData(ostream *ofile) {
        (*ofile) << total_energy_initialized << " ";
        (*ofile) << energy_escaping_sources << " ";
        (*ofile) << energy_escaping_geom << "\n";
        return 0;
    };

    int readData(istream *ifile) {
        (*ifile) >> total_energy_initialized;
        (*ifile) >> energy_escaping_sources;
        (*ifile) >> energy_escaping_geom;
        return 0;
    }

    void resetCounter() {
        total_energy_initialized = 0;
        energy_escaping_sources = 0;
        energy_escaping_geom = 0;
    }

    int addState(istream &data) {
        EGS_Float total_energy_initialized_tmp;
        EGS_Float energy_escaping_sources_tmp;
        EGS_Float energy_escaping_geom_tmp;

        data >> total_energy_initialized_tmp;
        data >> energy_escaping_sources_tmp;
        data >> energy_escaping_geom_tmp;

        total_energy_initialized += total_energy_initialized_tmp;
        energy_escaping_sources += energy_escaping_sources_tmp;
        energy_escaping_geom += energy_escaping_geom_tmp;

        return 0;
    };

};


/*! \brief abstract base class for scoring spectrum information
 *
 * Inputs:
 *
 *      type               - determines which derived class to initialize ('surface count',
 *                           'energy weighted surface', 'energy fluence in region')
 *      particle type      - which kind of particle to score
 *      minimum energy     - don't score any particles below this energy (defaults to 0.001 MeV)
 *      maximum energy     - don't score any particles above this energy (defaults to source->getEmax())
 *      number of bins     - number of scoring bins to use (bin width =  (E_max - E_min) / nbins
 *      output format      - xmgr, csv, or egsnrc (defaults to xmgr)
 *      egsnrc format mode - if esgnrc output format is chosen, which mode to use (see egs++ docs)
 *
 *      geometry           - for 'energy fluence in region' mode  used in conjunction with 'scoring region'
 *                           input for determining which geometry region to score the spectrum in
 *      scoring region     - for 'energy fluence in region' mode  used in conjunction with 'geometry'
 *                           input for determining which geometry region to score the spectrum in
 *      file extension - if provided the output will be written to
 *                       sim_input_file.{file extension}.{output format} otherwise the output
 *                       file will be given a name like sim_input_file.surfcount.agr
 *
 * Sample input:
 * \verbatim

 :start spectrum scoring:
     type = surface count # surface count, energy weighted surface, energy fluence in region
     particle type = photon # photon, electron, positron
     minimum energy = 0.001
     maximum energy = 1.00
     number of bins = 1000
     output format = xmgr # xmgr (default), csv, egsnrc
     file extension = my_spectrum # (optional)
 :stop spectrum scoring:
 \endverbatim

 * */
class BaseSpectrumScorer : public Subscriber {

protected:

    string format; /*!< format to output results in */
    int egsnrc_mode; /*!< egsnrc spectrum output format mode */
    int particle_type; /*!< What kind of particle are we interested in */
    EGS_I64 cur_history; /*!< keep track of the current history we are on  */
    EGS_I64 eff_history; /*!< effective histories (for normalization) */

    EGS_Float bin_width;
    EGS_Float total_scored;

    EGS_BaseSource *source; /*!< simulation source, used for limits on energy, normalization etc */

    bool valid;

    string fextension; /*!< file extension to use */


    /*! \brief output results to csv file */
    string outputCSV(string filename);

    /*! \brief output results to xmgrace file */
    string outputXMGR(string filename);

    /*! \brief output results to EGSnrc tabulated spectrum format */
    string outputEGSnrc(string filename);

    /*! \brief get index of bin that that E falls in */
    int getBin(EGS_Float E) const;

    virtual string getTitle() const {
        return "EGS_Brachy: spectrum";
    }

    virtual string getSubTitle() const {
        return "";
    }

    virtual string getYAxisLabel() const {
        return "";
    }

    virtual string getXAxisLabel() const {
        return "E / MeV";
    }

    string getFileName(string root) const {
        return root + "." + getFileExtension();
    }

    virtual string getFileExtension() const {
        return "spec";
    };

    /*! \brief set r & dr to result/uncertainty for given bin. Normalization
     * can be done in this routine */
    virtual void getResult(int bin, EGS_Float &r, EGS_Float &dr);

    /*! \brief Return name of particle we are scoring */
    string getParticleName() const {

        if (particle_type == 0) {
            return "photon";
        } else if (particle_type == -1) {
            return "electron";
        } else if (particle_type == 1) {
            return "positron";
        }

        return "";
    }

    virtual void outputTotal() {
        egsInformation("Total quantity scored: %.3g\n", total_scored);
    };


public:

    static const int DEFAULT_NBINS = 100;


    int nbins; /*!< \brief number of bins to score spectrum in */
    EGS_ScoringArray *bins; /*!< \brief spectrum scoring array */
    EGS_Float e_min; /*!< \brief minimum spectrum energy to score */
    EGS_Float e_max; /*!< \brief maximum spectrum energy to score */

    BaseSpectrumScorer(EGS_Input *input, EGS_BaseSource *src, GeomInfo *ginfo, Publisher *publisher);

    /*! \brief factory method for creating one of the derived spectrum scoring classes */
    static BaseSpectrumScorer *getSpectrumScorer(EGS_Input *inp, EGS_BaseSource *source, GeomInfo *ginfo,
            Publisher *publisher);

    virtual ~BaseSpectrumScorer();

    EGS_Float getBinWidth() const;

    double getParticleEnergy(const EGS_Particle *p) const;

    /*! \brief Receives messages from application. This method handles
     * NEW_HISTORY messages but delegates all other messages to the `score`
     * methods of the derived classes.
     */
    virtual void update(EB_Message message, void *data=0);

    /*! override in derived classes to do scoring */
    virtual void score(EB_Message message, void *data=0) = 0;

    void setEffectiveHistories(EGS_Float effective_histories);

    /*! \brief, write the scored spectrum to file with filename. Returns 0
     * on success, all other values indicate a failure */
    int outputResults(string root_name);

    virtual string getInfo() const {
        return getTitle();
    }

    /*! returns true if scorer was initialized correctly, otherwise false */
    bool isValid() const {
        return valid;
    }


    int outputData(ostream *ofile);

    int readData(istream *ifile);

    void resetCounter();

    int addState(istream &data);


};

/*! \brief A class for scoring a histogram of the number of particles escaping a
 * source geometry
 *
 * Sample input:
 * \verbatim

 :start spectrum scoring:
     type = surface count
     particle type = photon
     minimum energy = 0.001
     maximum energy = 1.00
     number of bins = 1000
     output format = xmgr
 :stop spectrum scoring:

 \endverbatim
 * */
class SurfaceCountSpectrum : public BaseSpectrumScorer {


    void getResult(int bin, EGS_Float &r, EGS_Float &dr);

    string getTitle() const {
        return "EGS_Brachy: " + getParticleName() + " counts on surface of source";
    }

    string getSubTitle() const {
        return "";
    }

    string getYAxisLabel() const {
        return "counts / total counts / MeV\\S-1";
    }

    void outputTotal();

    string getFileExtension() const {
        return fextension != "" ? fextension : "surfcount";
    }

public:


    SurfaceCountSpectrum(EGS_Input *input, EGS_BaseSource *src, GeomInfo *ginfo, Publisher *publisher):
        BaseSpectrumScorer(input, src, ginfo, publisher) {

        publisher->subscribe(this, PARTICLE_ESCAPING_SOURCE);
    };

    virtual void score(EB_Message message, void *data=0);


};


/*! \brief A class for scoring the energy weighted spectrum (normalized to
 * total radiant energy) of particles on the surface of a source geometry.
 *
 * See  Taylor & Rogers, Med. Phys., 35 , 4933 -- 4944, 2008
 *
 * Sample input: \verbatim

 :start spectrum scoring:
     type = energy weighted surface
     particle type = photon
     minimum energy = 0.001
     maximum energy = 1.00
     number of bins = 1000
     output format = xmgr
 :stop spectrum scoring:

 \endverbatim

*/
class EnergyWeightedSurfaceSpectrum : public BaseSpectrumScorer {

    void getResult(int bin, EGS_Float &r, EGS_Float &dr);

    string getTitle() const {
        return "EGS_Brachy: energy weighted surface " + getParticleName() + " spectrum";
    }

    string getSubTitle() const {
        return "";
    }

    string getYAxisLabel() const {
        return "(dR/dE)/R / MeV\\S-1";
    }

    void outputTotal();

    string getFileExtension() const {
        return fextension != "" ? fextension : "ewsurf";
    }

public:

    EnergyWeightedSurfaceSpectrum(EGS_Input *input, EGS_BaseSource *src, GeomInfo *ginfo, Publisher *publisher):
        BaseSpectrumScorer(input, src, ginfo, publisher) {

        publisher->subscribe(this, PARTICLE_ESCAPING_SOURCE);
    };

    virtual void score(EB_Message message, void *data=0);


};

/*! \brief A class for scoring the energy weighted spectrum (normalized to
 * total radiant energy) of particles on the surface of a source geometry.
 *
 * Note: this currently only works if there are no other geometries overlapping
 * the scoring region. If there is, you will get incorrect results!
 *
 * Sample input: \verbatim

 :start spectrum scoring:
     type = energy weighted surface
     particle type = photon
     minimum energy = 0.001
     maximum energy = 1.00
     number of bins = 1000
     output format = xmgr
     geometry = your_phantom_geom_name
     scoring region =  1 # which region of `your_phantom_geom` to score in (defaults to 0)
 :stop spectrum scoring:

 \endverbatim

*/
class EnergyFluenceSpectrumInVoxel : public BaseSpectrumScorer {

    EGS_BaseGeometry *geometry;
    EGS_Float region_volume;
    int local_scoring_region;
    int scoring_region;


    void getResult(int bin, EGS_Float &r, EGS_Float &dr);

    string getTitle() const {
        stringstream ss;
        ss << "EGS_Brachy: " << getParticleName() << " energy fluence spectrum in region ";
        ss << local_scoring_region << " of '"<<geometry->getName()<<"'";
        return ss.str();
    }

    string getYAxisLabel() const {
        return "fluence / MeV / cm\\S2";
    }

    void outputTotal();

    string getFileExtension() const {
        return fextension != "" ? fextension : "voxelflu";
    }

public:


    EnergyFluenceSpectrumInVoxel(EGS_Input *input, EGS_BaseSource *src, GeomInfo *ginfo, Publisher *publisher):
        BaseSpectrumScorer(input, src, ginfo, publisher) {

        if (particle_type != 0) {
            egsFatal(
                "Energy fluence spectra may only be scored for photons. See:\n\t"
                "https://github.com/nrc-cnrc/EGSnrc/issues/109/\n"
                "for details."
            );
        }

        string gname;
        int err = input->getInput("geometry",gname);
        if (err) {
            egsWarning("EnergyFluenceSpectrumInVoxel: missing or invalid `geometry` input\n");
            valid =  false;
            return;
        }

        geometry = EGS_BaseGeometry::getGeometry(gname);
        if (!geometry) {
            egsWarning("EnergyFluenceSpectrumInVoxel: unable to find geometry named `%s` \n", gname.c_str());
            valid =  false;
            return;
        }

        err = input->getInput("scoring region", local_scoring_region);
        if (err) {
            egsWarning("EnergyFluenceSpectrumInVoxel: no `scoring region` defined. Assuming region 0\n");
            local_scoring_region = 0;
        }

        if (scoring_region < 0 || local_scoring_region > geometry->regions()) {
            egsWarning("EnergyFluenceSpectrumInVoxel: invalid region `scoring region` defined. Assuming region 0\n");
            local_scoring_region = 0;
        }

        GeomRegT geomreg(geometry, local_scoring_region);
        scoring_region = ginfo->localToGlobal(geomreg);

        region_volume = geometry->getMass(local_scoring_region)/geometry->getRelativeRho(local_scoring_region);

        publisher->subscribe(this, PARTICLE_TAKING_STEP);

    };


    virtual void score(EB_Message message, void *data=0);


};
