/*
################################################################################
#
#  egs_brachy egs_brachy.cpp
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

/*! \file egs_brachy.cpp
 * \author Randle Taylor (randle.taylor@gmail.com)
 * \brief the main egs_brachy application implementation file
 */

#include <algorithm>
#include <fstream>
#include <string>
#include <iomanip>
#include <assert.h>
#include "gzstream.h"
#include "zlib.h"

#include "egs_brachy.h"
#include "egs_ausgab_object.h"
#include "egs_rndm.h"
#include "egs_run_control.h"


using namespace std;

/* taken from egs_advance_application.cpp for outputData functions */
#define egsGetRNGPointers F77_OBJ_(egs_get_rng_pointers,EGS_GET_RNG_POINTERS)
extern __extc__ void egsGetRNGPointers(EGS_I32 *, EGS_I32 *);
#define egsGetRNGArray F77_OBJ_(egs_get_rng_array,EGS_GET_RNG_ARRAY)
extern __extc__ void egsGetRNGArray(EGS_Float *);
#define egsSetRNGState F77_OBJ_(egs_set_rng_state,EGS_SET_RNG_STATE)
extern __extc__ void egsSetRNGState(const EGS_I32 *, const EGS_Float *);
#define egsGetSteps F77_OBJ_(egs_get_steps,EGS_GET_STEPS)
extern __extc__ void egsGetSteps(double *, double *);
#define egsSetSteps F77_OBJ_(egs_set_steps,EGS_SET_STEPS)
extern __extc__ void egsSetSteps(const double *, const double *);
#define egsOpenUnits F77_OBJ_(egs_open_units,EGS_OPEN_UNITS)
extern __extc__ void egsOpenUnits(const EGS_I32 *);
#define egsGetElectronData F77_OBJ_(egs_get_electron_data,EGS_GET_ELECTRON_DATA)
extern __extc__ void egsGetElectronData(void (*func)(EGS_I32 *,EGS_Float *,
                                        EGS_Float *,EGS_Float *,EGS_Float *),const EGS_I32 *,const EGS_I32 *);
#define egsGetPhotonData F77_OBJ_(egs_get_photon_data,EGS_GET_PHOTON_DATA)
extern __extc__ void egsGetPhotonData(void (*func)(EGS_I32 *,EGS_Float *,
                                      EGS_Float *,EGS_Float *,EGS_Float *),const EGS_I32 *,const EGS_I32 *);
/* end taken from egs_advance_application.cpp for outputData functions */

extern "C" void F77_OBJ_(egs_scale_xcc,EGS_SCALE_XCC)(const int *,const EGS_Float *);
extern "C" void F77_OBJ_(egs_scale_bc,EGS_SCALE_BC)(const int *,const EGS_Float *);
extern "C" void F77_OBJ_(egs_bcse,EGS_BCSE)(const int *,const EGS_Float *);
extern "C" void F77_OBJ_(egs_uniform_photons,EGS_UNIFORM_PHOTONS)(const int *,const EGS_Float *);

void printParticleWithSpherical(EGS_Particle p) {
    EGS_Float x,y,z, r, R, theta, phi;

    x = p.x.x;
    y = p.x.y;
    z = p.x.z;

    r = sqrt(x*x + y*y);
    R = sqrt(x*x + y*y + z*z);
    theta = acos(z/R)*180/M_PI;
    phi = atan2(y, x)*180/M_PI;

    PRINT_PARTICLE_WITH_DIR(p);
    cout << " R           = " << R << endl;
    cout << " r           = " << r << endl;
    cout << " z           = " << z << endl;
    cout << " theta (deg) = " << theta << endl;
    cout << " phi (deg)   = " << phi << endl;

}


string EB_Application::revision = "$Revision: 0.9.1 $";

const EGS_Float EB_Application::DEFAULT_BCSE_FACTOR = 100;


void EB_Application::describeUserCode() const {
    egsInformation(
        "\n               ***************************************************"
        "\n               *                                                 *"
        "\n               *                  egs_brachy                     *"
        "\n               *                                                 *"
        "\n               ***************************************************"
        "\n\n");
    egsInformation("This is EB_Application %s based on\n"
                   "      EGS_AdvancedApplication %s\n\n",
                   egsSimplifyCVSKey(revision).c_str(),
                   egsSimplifyCVSKey(base_revision).c_str());
}

void EB_Application::describeSimulation() {

    if (!geometry && !source) {
        return;
    }

    egsInformation("\n\negs_brachy Run Mode Information\n%s\n\n",string(80,'=').c_str());
    egsInformation("Run mode                                   =  %s\n", run_mode_name.c_str());
    egsInformation("Single generator  (may be overriden later) =  %s\n", single_generator ? "yes" :"no");
    if (gcr_phantom) {
        egsInformation(
            "getCurrentResult phantom/reg               =  %s %d",
            gcr_phantom->geometry->getName().c_str(),
            gcr_phantom_reg
        );
    }

    egsInformation("\n\negs_brachy Geometry Information\n%s\n\n", string(80,'=').c_str());

    ginfo.printInfo();

    egsInformation("\n\negs_brachy Phantom Details\n%s\n\n", string(80,'=').c_str());
    egsInformation("                              |       |           |              |  Sph (Rmin, Rmax)    |                      |   \n");
    egsInformation("                              |       |           |  Avg Vox Vol |   RZ (Rmin, Rmax)    |   RZ (Zmin, Zmax)    |   \n");
    egsInformation("Name                          | Type  |   Nreg    |    / cm^3    |  XYZ (Xmin, Xmax)    |  XYZ (Ymin, Ymax)    | XYZ (Zmin, Zmax)\n");
    egsInformation("%s\n", string(120, '-').c_str());

    for (size_t p=0; p < phantom_geoms.size(); p++) {

        EB_Phantom *phant = phantom_geoms[p];
        EGS_BaseGeometry *geom = phant->geometry;

        string type = geom->getType();

        if (type == "EGS_XYZGeometry") {
            type = "XYZ";
            int nx, ny, nz;
            nx = geom->getNRegDir(EB_Phantom::XDIR);
            ny = geom->getNRegDir(EB_Phantom::YDIR);
            nz = geom->getNRegDir(EB_Phantom::ZDIR);

            EGS_Float xmin, xmax, ymin, ymax, zmin, zmax;
            xmin = geom->getBound(EB_Phantom::XDIR, 0);
            xmax = geom->getBound(EB_Phantom::XDIR, nx-1);
            ymin = geom->getBound(EB_Phantom::YDIR, 0);
            ymax = geom->getBound(EB_Phantom::YDIR, ny-1);
            zmin = geom->getBound(EB_Phantom::ZDIR, 0);
            zmax = geom->getBound(EB_Phantom::ZDIR, nz-1);

            egsInformation(
                "%-30s| %5s | %9d | %12.5G | (%8.3F, %8.3F) | (%8.3F, %8.3F) | (%8.3F, %8.3F)\n",
                geom->getName().c_str(), type.c_str(), geom->regions(), phant->avgVoxelVol(),
                xmin, xmax, ymin, ymax, zmin, zmax
            );

        } else if (type == "EGS_RZ") {
            type = "RZ";
            int nr, nz;
            nr = geom->getNRegDir(EGS_RZGeometry::RDIR);
            nz = geom->getNRegDir(EGS_RZGeometry::ZDIR);

            EGS_Float rmin, rmax, zmin, zmax;
            rmin = geom->getBound(EGS_RZGeometry::RDIR, 0);
            rmax = geom->getBound(EGS_RZGeometry::RDIR, nr-1);
            zmin = geom->getBound(EGS_RZGeometry::ZDIR, 0);
            zmax = geom->getBound(EGS_RZGeometry::ZDIR, nz-1);

            egsInformation(
                "%-30s| %5s | %9d | %12.5G | (%8.3F, %8.3F) | (%8.3F, %8.3F) |\n",
                geom->getName().c_str(), type.c_str(), geom->regions(), phant->avgVoxelVol(),
                rmin, rmax, zmin, zmax
            );
        } else if (type == "EGS_cSphericalShell" || type == "EGS_cSpheres") {
            type = (type == "EGS_cSphericalShell" ? "SphSh" : "Sph");

            int nr = geom->getNRegDir(0);

            EGS_Float rmin, rmax;
            rmin = geom->getBound(EGS_cSpheres::RDIR, 0);
            rmax = geom->getBound(EGS_cSpheres::RDIR, nr);

            egsInformation(
                "%-30s| %5s | %9d | %12.5G | (%8.3F, %8.3F) |                      | \n",
                geom->getName().c_str(), type.c_str(), geom->regions(), phant->avgVoxelVol(),
                rmin, rmax
            );
        }

    }

    egsInformation("\n\negs_brachy Volume correction details\n%s\n", string(80, '-').c_str());

    egsInformation("\nSource specific volume correction details\n%s\n", string(80, '-').c_str());
    if (!source_vc_results.success) {
        egsInformation("Source correction not requested or failed\n");
    } else {
        source_vc_results.outputResults("Source");
    }

    egsInformation("\nExtra volume correction details\n%s\n", string(80, '-').c_str());
    if (!gen_vc_results.success) {
        egsInformation("Extra correction not requested or failed\n");
    } else {
        gen_vc_results.outputResults();
    }

    egsInformation("\nFile volume correction details\n%s\n", string(80, '-').c_str());
    if (!file_vc_results.success) {
        egsInformation("File correction not requested or failed\n");
    } else {
        file_vc_results.outputResults();
    }

    egsInformation("\n\n");
    if (geometry) {
        geometry->printInfo();
    }

    if (source) {
        egsInformation("\n\negs_brachy Source Information\n%s\n", string(80,'=').c_str());
        egsInformation("%s\n\n", source->getSourceDescription());
        egsInformation("Number of source locations = %d\n", source_transforms.size());
        egsInformation("Single source generator    = %s\n", (single_generator ? "yes" : "no"));
        egsInformation("Source | Location (x cm, y cm, z cm)  | Rotated | Weight\n%s\n", string(80,'-').c_str());
        for (size_t tt =0; tt < source_transforms.size(); tt++) {
            EGS_Vector trans = source_transforms[tt]->getTranslation();
            egsInformation(
                "%6d | ( % 7.3F, % 7.3F, % 7.3F) |  %5s  | % 7.2G\n",
                tt+1, trans.x, trans.y, trans.z,
                source_transforms[tt]->hasRotation() ? "true":"false",
                source_weights[tt]
            );
        }
    }

    if (rndm) {
        egsInformation("\n\n");
        rndm->describeRNG();
    }

    if (a_objects_list.size() > 0) {
        egsInformation("The following ausgab objects are included in the simulation\n");
        egsInformation("===========================================================\n\n");
        for (size_t j=0; j<a_objects_list.size(); ++j) {
            egsInformation("%s",a_objects_list[j]->getObjectDescription());
        }
        egsInformation("\n\n");
    }

    printIncludedFiles();

    if (final_job) {
        helpInit(0,false);
    }
}

bool containsInclude(string str) {
    string::iterator end_pos = std::remove(str.begin(), str.end(), ' ');
    str.erase(end_pos, str.end());
    size_t found = str.find("includefile");
    return found != string::npos;
}

void EB_Application::printIncludedFiles() {


    string fname = constructIOFileName(".egsinp", false);
    egsInformation("\n\nIncluded Files\n%s\n", string(80,'=').c_str());
    egsInformation("Input file:\n\t%s\n", fname.c_str());
    egsInformation("The following files were included in this simulation:\n");
    ifstream inf(fname.c_str());

    string line;
    while (inf) {
        getline(inf, line);
        if (containsInclude(line)) {
            egsInformation("\t%s\n", muen::trim(line).c_str());
        }
    };

}


int EB_Application::initGeometry() {
    // override default initGeometry so we can manually create our own geometry
    // which will allow us to track region numbers for each geometry object individually

    timing_blocks.addTimer("egs_brachy::initGeometry");

    EGS_Input *ginput = input->getInputItem("geometry definition");

    if (!ginput) {
        egsWarning("EB_Application::createGeometry: no geometry specification in this input\n");
        egsFatal("Simulation stopped\n");
    }

    int err = ginfo.initializeFromInput(ginput);
    if (err) {
        egsWarning("EB_Application::createGeometry: incomplete or incorrect geometry specification\n");
        delete ginput;
        egsFatal("Simulation stopped\n");
    }

    err = EGS_AdvancedApplication::initGeometry();

    if (err) {
        egsWarning("Failed to create a geometry from input file\n");
        delete ginput;
        egsFatal("Simulation stopped\n");
    }


    geometry->ref();

    ginfo.setGeometryIndexes(geometry);



    if (run_mode == RM_SUPERPOSITION) {
        if (ginfo.source_envelope_name == "") {
            egsFatal("Missing 'source envelope geometry' input required for superposition mode\n");
        }

        source_envelope_geom = EGS_BaseGeometry::getGeometry(ginfo.source_envelope_name);
        if (!source_envelope_geom) {
            egsFatal("source envelope geometry '%s' can not be found\n", ginfo.source_envelope_name.c_str());
        }

        if (source_envelope_geom->getType() != "EGS_ASwitchedEnvelope") {
            egsFatal("You must use an %s source envelope geometry type for superposition mode\n", "EGS_ASwitchedEnvelope");
        }
        superpos_geom = static_cast<EGS_ASwitchedEnvelope *>(source_envelope_geom);
    }else{
        for (int gg=0; gg < ginfo.ngeom; gg++) {
            GeomRegionInfo gr = ginfo.ordered_geom_data[gg];
            if (gr.type == "EGS_ASwitchedEnvelope"){
                egsFatal("EGS_ASwitchedEnvelope should not be used when not using 'run mode = superposition\n");
            }
        }
    }


    err = initSourceTransforms();
    if (err) {
        egsFatal("Failed to initiate source locations\n");
    }

    delete ginput;

    err = createPhantoms();
    if (err) {
        egsWarning("Failed to create phantom objects\n");
        egsFatal("Simulation stopped\n");
    }

    err = correctVolumes();


    timing_blocks.stopTimer();
    return 0;

}

vector<EGS_AffineTransform *> EB_Application::createTransforms(EGS_Input *input) {

    vector<EGS_AffineTransform *> transforms;
    if (input) {
        EGS_Input *trans_inp;

        while ((trans_inp = input->takeInputItem("transformation"))) {
            EGS_AffineTransform *transform = EGS_AffineTransform::getTransformation(trans_inp);
            if (!transform) {
                egsWarning("Invalid transform input given\n");

            }
            transforms.push_back(transform);
            delete trans_inp;

        }
    }

    return transforms;

}

int EB_Application::initSourceTransforms() {

    if (!input) {
        return -1;
    }

    EGS_Input *source_inp = input->getInputItem("source definition");

    if (!source_inp) {
        return -1;
    }

    EGS_Input *source_loc_inp = source_inp->takeInputItem("transformations");

    if (source_loc_inp) {
        source_transforms = createTransforms(source_loc_inp);
        delete source_loc_inp;
    }

    if (source_transforms.size() == 0) {
        EGS_AffineTransform *unity_trans = new EGS_AffineTransform();
        source_transforms.push_back(unity_trans);
        egsWarning("EB_Application:: missing or invalid source `transformations` input item. Assuming single source at origin");
    }
    nsources = (int)source_transforms.size();
    base_transform = source_transforms[0];
    base_transform_inv = new EGS_AffineTransform(base_transform->inverse());

    return 0;

}

int EB_Application::correctVolumes() {


    timing_blocks.addTimer("egs_brachy::correctVolumes");
    EGS_Input *vol_cor_inp = input->takeInputItem("volume correction");
    if (!vol_cor_inp) {
        egsWarning("Input item `volume correction` was not found\n");
        return 1;
    }

    if (run_mode == RM_SUPERPOSITION) {
        for (int i=0; i < nsources; i++) {
            superpos_geom->activateByIndex(i);
        }
    }
    ebvolcor::VolumeCorrector vc(vol_cor_inp, phantom_geoms, geometry, &ginfo, source_transforms);
    delete vol_cor_inp;

    source_vc_results = vc.runSourceCorrection(timing_blocks);

    gen_vc_results = vc.runGeneralCorrection(timing_blocks);

    file_vc_results = vc.runFileCorrection(timing_blocks);

    timing_blocks.stopTimer();

    return 0;

}


// set up Phantom objects for any geometries that user has requested scoring for
int EB_Application::createPhantoms() {

    for (size_t idx=0; idx < ginfo.phantom_names.size(); idx++) {
        string name = ginfo.phantom_names[idx];

        EGS_BaseGeometry *phant_geom = EGS_BaseGeometry::getGeometry(name);
        if (!phant_geom) {
            egsInformation("\n\nUnable to find phantom geometry `%s`.\n\n    This is a fatal error\n\n", name.c_str());
            return 1;
        }

        string phant_type = phant_geom->getType();

        if (!EB_Phantom::allowedPhantomGeom(phant_type)) {
            egsInformation("\n\nPhantoms of geometry type `%s` are not currently supported.\n\n    This is a fatal error\n\n",
                           phant_type.c_str());
            return 1;
        }

        set<int> global_regions;

        for (int reg=0; reg < ginfo.nreg_total; reg++) {
            if (ginfo.phantomFromRegion(reg) == (int)idx) {
                global_regions.insert(reg);
            }
        }

        if (global_regions.size()==0) {
            egsFatal(
                "EB_Application::createPhantoms - No phantom regions detected.\n"
                "Are you sure your phantom was included in the final simulation geometry?\n"
            );
        }
        EB_Phantom *phantom = new EB_Phantom(this, phant_geom, global_regions, nsources, &pevent_pub);
        phantom_geoms.push_back(phantom);

    }

    return 0;

}


int EB_Application::initSource() {


    EGS_Input *source_inp = input->getInputItem("source definition");

    if (!source_inp) {
        return -1;
    }


    /* check if user has defined source weighting */
    source_inp->getInput("source weights", source_weights);

    // fill up source_weights so it always has length==nsources
    while ((int)source_weights.size() < nsources) {
        source_weights.push_back(1);
    }

    // normalize weights
    double max_wt = *max_element(source_weights.begin(), source_weights.end());
    vector<EGS_Float>::iterator it =source_weights.begin();
    for (; it != source_weights.end(); ++it) {
        *it /= max_wt;
    }

    int err = EGS_AdvancedApplication::initSource();
    if (err) {
        egsFatal("Failed to initialize source\n");
    }

    string stype = source->getObjectType();
    is_phsp_source = stype == "EGS_PhspSource" || stype == "EB_IAEAPHSPSource";

    if (is_phsp_source && !single_generator) {
        single_generator = true;
        egsInformation("Phase space being used. Overriding requested 'single generator = no' parameter. Setting to 'yes'\n");
    }


    return err;

}

int EB_Application::initSimulation() {

    timing_blocks.addTimer("egs_brachy::initSimulation");
    initRunMode();

    int res = EGS_AdvancedApplication::initSimulation();

    if (run_mode == RM_VC_ONLY) {
        output_3ddose_files = false;
        describeSimulation();
        finishSimulation();
        timing_blocks.stopTimer();
        return 1;
    }

    timing_blocks.stopTimer();
    return res;

}

int EB_Application::initRunControl() {

    vector<string> format_choices;
    format_choices.push_back("text");
    format_choices.push_back("gzip");
    EGS_Input *run_control = input->getInputItem("run control");
    int format = run_control->getInput("egsdat file format", format_choices, 0);
    output_egsdat_format = format == 0 ? "text" : "gzip";

    return EGS_AdvancedApplication::initRunControl();

}

int EB_Application::initRunMode() {
    timing_blocks.addTimer("egs_brachy::initRunMode");

    string run_mode_inp;
    EGS_Input *rm = input->takeInputItem("run mode");
    if (!rm) {
        timing_blocks.stopTimer();
        return 1;
    }

    vector<string> choices;
    choices.push_back("normal"); // RM_NORMAL
    choices.push_back("superposition"); //RM_SUPERPOSITION
    choices.push_back("volume correction only"); //RM_VC_ONLY

    run_mode = (RunMode)rm->getInput("run mode", choices, (int)RM_NORMAL);
    run_mode_name = choices[run_mode];


    vector<string> yn_choices;
    yn_choices.push_back("no");
    yn_choices.push_back("yes");

    single_generator = (bool)rm->getInput("single generator", yn_choices, 1);

    delete rm;

    timing_blocks.stopTimer();

    return 0;

}

int EB_Application::initCrossSections() {
    timing_blocks.addTimer("egs_brachy::initCrossSections");
    EGS_Input *transportp = input->getInputItem("MC transport parameter");
    if (!transportp) {
        transportp = input->getInputItem("transport parameter");
    }

    if (!transportp) {
        egsFatal("Missing `MC transport parameter` input item\n");
        return 1;
    }

    int err = transportp->getInput("fluorescent photon cutoff", flu_cutoff);
    if (err) {
        flu_cutoff = 0.001;
    }

    int ret = EGS_AdvancedApplication::initCrossSections();

    global_ecut = the_bounds->ecut;
    global_pcut = the_bounds->pcut;

    err = transportp->getInput("source ecut", source_ecut);
    if (err) {
        source_ecut = global_ecut;
    }

    err = transportp->getInput("source pcut", source_pcut);
    if (err) {
        source_pcut = global_pcut;
    }

    egsInformation("\negs_brachy transport parameter options:\n%s\n",string(80,'-').c_str());
    egsInformation("Fluorescent photon cutoff                         %.3G\n", flu_cutoff);
    egsInformation("Source PCUT                                       %.3G\n", source_pcut);
    egsInformation("Source ECUT                                       %.3G\n", source_ecut);

    timing_blocks.stopTimer();
    return ret;
}


int EB_Application::initScoring() {

    timing_blocks.addTimer("egs_brachy::initScoring");

    EGS_Input *options = input->takeInputItem("scoring options");

    if (options) {

        initGCRScoring(options);
        initAusgabCalls();
        initTrackLengthScoring(options);
        initEDepScoring(options);
        initScatScoring(options);
        initXCCScaling(options);
        initDoseScaling(options);
        initSpectrumScoring(options);

        escoring = new EnergyScoringStats(&pevent_pub);

        initPHSPScoring(options);
        initOutputFiles(options);


        delete options;
    } else {
        egsFatal("\n\nMissing input section 'scoring options'\n\n");
    }

    initVarianceReduction();

    timing_blocks.stopTimer();
    return 0;
}

void EB_Application::initGCRScoring(EGS_Input *inp) {

    gcr_phantom = 0;
    gcr_phantom_reg = 0;

    vector<string> gcr_inp;
    int err = inp->getInput("current result phantom region", gcr_inp);
    if (err || gcr_inp.size()==0) {
        gcr_phantom = phantom_geoms[0];
        gcr_phantom_reg = 0;
        return;
    }


    // user requested a phantom, find it
    if (gcr_inp.size() >= 1) {
        for (size_t idx=0; idx < ginfo.phantom_names.size(); idx++) {
            if (ginfo.phantom_names[idx] == gcr_inp[0]) {
                gcr_phantom = phantom_geoms[idx];
            }
        }
    }

    // couldn't find requested phantom
    if (!gcr_phantom) {
        egsWarning(
            "Did not find '%s' phantom for getCurrentResult. Using phantom %s\n",
            gcr_inp[0].c_str(), ginfo.phantom_names[0].c_str()
        );
        gcr_phantom = phantom_geoms[0];
        gcr_phantom_reg = 0;
        return;
    }

    // user has also requested a region
    if (gcr_inp.size() > 1) {
        int ireg = atol(gcr_inp[1].c_str());
        if (ireg > 0 && ireg < gcr_phantom->geometry->regions()) {
            gcr_phantom_reg = ireg;
        }
    }

}

void EB_Application::initOutputFiles(EGS_Input *inp) {

    vector<string> yn_choices;
    yn_choices.push_back("no");
    yn_choices.push_back("yes");
    output_egsphant = (bool)inp->getInput("output egsphant files", yn_choices, 0);
    output_voxinfo = (bool)inp->getInput("output voxel info files", yn_choices, 0);
    inp->getInput("output volume correction files for phantoms", output_volcor_phantoms);

    vector<string> format_choices;
    format_choices.push_back("text");
    format_choices.push_back("gzip");

    int format = inp->getInput("dose file format", format_choices, 0);
    output_dose_format = format == 0 ? "text" : "gzip";

    format = inp->getInput("egsphant file format", format_choices, 0);
    output_egsphant_format = format == 0 ? "text" : "gzip";

    format = inp->getInput("voxel info file format", format_choices, 0);
    output_voxinfo_format = format == 0 ? "text" : "gzip";

    format = inp->getInput("volume correction file format", format_choices, 0);
    output_volcor_format = format == 0 ? "text" : "gzip";

    inp->getInput("record initial particle positions", record_n_init);

}

void EB_Application::initPHSPScoring(EGS_Input *inp) {

    EGS_Input *phsp_inp = inp->takeInputItem("phsp scoring");

    if (phsp_inp) {
        phsp = new PHSPControl(phsp_inp, base_transform_inv, this, &pevent_pub);
    }

}


int EB_Application::initVarianceReduction() {

    timing_blocks.addTimer("egs_brachy::initVarianceReduction");

    egsInformation("\n\negs_brachy Variance Reduction Information\n%s\n\n",string(80,'=').c_str());

    EGS_Input *vr = input->takeInputItem("variance reduction");

    /* Range rejection on by default in non source objects
     * and off in any source objects */
    global_i_do_rr = true;
    global_e_max_rr = 2.511;

    source_i_do_rr = false;
    source_e_max_rr = 0.512;

    the_egsvr->i_do_rr = (int)source_i_do_rr;
    the_egsvr->e_max_rr = source_e_max_rr;

    if (!vr) {
        egsInformation("Global Range Rejection                           = %s\n",  global_i_do_rr ? "Yes" : "No");
        egsInformation("Global Range Rejection Maximum Energy            = %.3G MeV\n",  global_e_max_rr);
        egsInformation("Source Range Rejection                           = %s\n",  source_i_do_rr ? "Yes" : "No");
        egsInformation("Source Range Rejection Maximum Energy            = %.3G MeV\n",  source_e_max_rr);
        timing_blocks.stopTimer();
        return 1;
    }


    EGS_Input *ri = vr->takeInputItem("particle recycling");
    if (ri) {

        egsInformation("Particle Recycling\n");
        recycle_opts = new RecycleOpts(ri);
        recycle_opts->printInfo();
        if (!single_generator) {
            single_generator = true;
            egsInformation("    Overriding requested 'single generator = no' parameter. Setting to 'yes'\n");
        }
        egsInformation("\n");

        delete ri;
    }


    /* Range rejection */
    vector<string> yn_choices;
    yn_choices.push_back("no");
    yn_choices.push_back("yes");

    string old_rr_setting;
    int err = vr->getInput("range rejection", old_rr_setting);
    if (!err) {
        egsFatal(
            "'range rejection' is not a valid setting for egs_brachy.\n"
            "Please use 'global range rejection' and 'source range rejection' settings instead.\n"
        );
    }

    EGS_Float old_rr_max_e;
    err = vr->getInput("range rejection max energy", old_rr_max_e);
    if (!err) {
        egsFatal(
            "'range rejection max energy' is not a valid setting for egs_brachy.\n"
            "Please use 'global range rejection max energy' and 'source range rejection max energy' settings instead.\n"
        );
    }


    global_i_do_rr = vr->getInput("global range rejection", yn_choices, 1);
    egsInformation("Global Range Rejection                           = %s\n",  global_i_do_rr ? "Yes" : "No");
    if (global_i_do_rr) {
        int err = vr->getInput("global range rejection max energy", global_e_max_rr);
        if (err) {
            global_e_max_rr = 2.511;
        }
        if (global_e_max_rr < 0.512) {
            egsFatal("Global Range Rejection max energy must be at least 0.512MeV\n");
        } else {
            egsInformation("Global Range Rejection Maximum Energy            = %.3G MeV\n",  global_e_max_rr);
        }
    }

    source_i_do_rr = vr->getInput("source range rejection", yn_choices, 0);
    egsInformation("Source Range Rejection                           = %s\n",  source_i_do_rr ? "Yes" : "No");
    if (source_i_do_rr) {
        int err = vr->getInput("source range rejection max energy", source_e_max_rr);
        if (err) {
            source_e_max_rr = 2.511;
        }
        if (source_e_max_rr < 0.512) {
            egsFatal("Source Range Rejection max energy must be at least 0.512MeV\n");
        } else {
            egsInformation("Source Range Rejection Maximum Energy            = %.3G MeV\n",  source_e_max_rr);
        }
    }

    the_egsvr->i_do_rr = source_i_do_rr;
    the_egsvr->e_max_rr = source_e_max_rr;


    /* Brem Cross Section Enhancement*/
    err = initBCSE(vr);
    egsInformation("BCSE                                      = %s\n",  do_bcse ? "Yes" : "No");
    if (!err) {
        egsInformation(
            "    Brem Cross Section Enhancement medium = %s (%d)\n",
            EGS_BaseGeometry::getMediumName(bcse_med_num), bcse_med_num
        );
        egsInformation("    Brem Cross Section Enhancement factor = %.3G\n", bcse_factor);
    }


    /* Brem Splitting */
    err = vr->getInput("split brem photons", nbr_split);
    egsInformation("Brem Splitting                            = %s\n",  nbr_split > 1? "Yes" : "No");
    if (!err && nbr_split > 1) {
        do_brem_split = true;
        egsInformation("    Splitting brem photons N times        = %d\n", nbr_split);
        the_egsvr->nbr_split = nbr_split;
    }


    /* Charged Particle Russian Roulette*/
    err =  initRussianRoulette(vr);
    egsInformation("Charged Particle Russian Roulette         = %s\n", the_egsvr->i_play_RR ? "Yes" : "No");
    if (!err || the_egsvr->i_play_RR) {

        if (bcse_factor > 1) {
            the_egsvr->prob_RR = 1./(nbr_split*bcse_factor);
            egsInformation(
                "    Survival probability (1/nbrsplt*bcse) = 1/(%d*%.3G) (%.3G%%)\n",
                nbr_split, bcse_factor,  the_egsvr->prob_RR*100
            );
        } else {
            the_egsvr->prob_RR = 1./nbr_split;
            egsInformation(
                "    Survival probability  (1/nbrsplt)     = 1/%d (%.3G%%)\n",
                nbr_split, the_egsvr->prob_RR*100
            );
        }
    }


    delete vr;
    timing_blocks.stopTimer();
    return 0;

}

int EB_Application::initRussianRoulette(EGS_Input *scoring_options) {

    vector<string> yn_choices;
    yn_choices.push_back("no");
    yn_choices.push_back("yes");

    int play_rr = scoring_options->getInput("russian roulette", yn_choices, 0);
    if (play_rr == 0) {
        return 1;
    }

    the_egsvr->i_play_RR = 1;

    AusgabCall rr_calls[] = {
        BeforeBrems, AfterBrems,
        BeforeAnnihFlight, AfterAnnihFlight,
        BeforeAnnihRest, AfterAnnihRest,
        FluorescentEvent
    };

    int ncalls = sizeof(rr_calls)/sizeof(rr_calls[0]);
    enableAusgabCalls(ncalls, rr_calls);

    return 0;
}

int EB_Application::initBCSE(EGS_Input *inp) {

    vector<string> bcse_inp;
    int err = inp->getInput("bcse medium", bcse_inp);
    if (err || bcse_inp.size()==0) {
        return 1;
    }

    bcse_med_num = EGS_BaseGeometry::getMediumIndex(bcse_inp[0]);
    if (bcse_med_num < 0) {
        egsWarning("Requested an unknown medium `%s` for BCSE\n",bcse_inp[0].c_str());
        return 1;
    }


    if (bcse_inp.size() == 1) {
        bcse_factor = DEFAULT_BCSE_FACTOR;
    } else {
        bcse_factor = atof(bcse_inp[1].c_str());
    }

    do_bcse = bcse_factor > 1;
    if (!do_bcse) {
        return 1;
    }

    the_egsvr->i_play_RR = 1;

    AusgabCall bcse_calls[] = {BeforeBrems, AfterBrems, FluorescentEvent};
    enableAusgabCalls(3, bcse_calls);

    int egs_med_num = bcse_med_num+1;
    F77_OBJ_(egs_bcse, EGS_BCSE)(&egs_med_num, &bcse_factor);

    return 0;
}

void EB_Application::initSpectrumScoring(EGS_Input *scoring_input) {

    bool header_printed = false;

    while (EGS_Input *spec_inp=scoring_input->takeInputItem("spectrum scoring")) {

        if (!header_printed) {
            egsInformation("\n\negs_brachy Spectrum Scoring Information\n%s\n\n",string(80,'=').c_str());
            header_printed = true;
        }

        BaseSpectrumScorer *scorer = BaseSpectrumScorer::getSpectrumScorer(spec_inp, source, &ginfo, &pevent_pub);

        if (scorer) {
            spectrum_scorers.push_back(scorer);
            egsInformation("Added scorer with title: %s\n", scorer->getInfo().c_str());
        }
        delete spec_inp;
    }
}

void EB_Application::clearAusgabCalls() {
    for (size_t call=BeforeTransport; call <= UnknownCall; call++) {
        setAusgabCall((AusgabCall)call, false);
    }

}

void EB_Application::initAusgabCalls() {


    clearAusgabCalls();

    AusgabCall always_call[] = {BeforeTransport, AfterTransport, FluorescentEvent};
    enableAusgabCalls(3, always_call);

}



void EB_Application::initTrackLengthScoring(EGS_Input *scoring_options) {

    vector<string> choices;
    choices.push_back("no");
    choices.push_back("yes");

    score_tlen = scoring_options->getInput("score tracklength dose", choices, 1);

    if (score_tlen) {
        for (size_t p=0; p < phantom_geoms.size(); p++) {
            phantom_geoms[p]->enableTLenScoring();
        }
        initMuenData(scoring_options);
    }

}

map<string, string> getMuenForMedia(EGS_Input *scoring_options) {

    map<string, string> muen_for_med;

    vector<string> muen_meds;
    scoring_options->getInput("muen for media", muen_meds);

    for (size_t i=0; i < muen_meds.size(); i++) {
        string med_name = muen_meds[i];
        muen_for_med[med_name] = med_name;
    };

    EGS_Input *muen_inp;
    while ((muen_inp = scoring_options->takeInputItem("muen for medium"))) {
        string scoring, transport;
        int err = muen_inp->getInput("transport medium", transport);
        if (err) {
            egsFatal("Missing 'transport medium' key in 'muen for medium' block\n");
            delete muen_inp;
        }
        err = muen_inp->getInput("scoring medium", scoring);
        if (err) {
            egsFatal("Missing 'scoring medium' key in 'muen for medium' block\n");
            delete muen_inp;
        }

        muen_for_med[transport] = scoring;

        delete muen_inp;
    }

    return muen_for_med;

}

void EB_Application::initMuenData(EGS_Input *scoring_options) {

    egsInformation("\n\negs_brachy Muen Data Information\n%s\n\n",string(80,'=').c_str());

    string muen_file;
    bool missing_muen = scoring_options->getInput("muen file", muen_file) != 0;
    if (missing_muen) {
        egsFatal("\n\nMissing input scoring options: muen file\n\n    This is a fatal error\n\n");
    }

    // transport med name -> scoring med name
    media_muen_names = getMuenForMedia(scoring_options);

    if (media_muen_names.size()==0) {
        egsWarning(
            "EB_Application:: missing or invalid 'muen for media' input item or 'muen substitution' block(s).\n"
            "   No muen data has been read.\n"
        );
    }

    // read muen data file and set media_muen for all media requested
    muen::MuenDataParser dp;
    int err = dp.setMuenFile(muen_file);
    if (err) {
        egsFatal("Failed to read muen data from %s\n",muen_file.c_str());
    }

    egsInformation("Read muen data from %s\n", muen_file.c_str());

    map<string, string>::iterator mit = media_muen_names.begin();
    for (; mit != media_muen_names.end(); mit++) {

        string transport_med = mit->first;
        string scoring_med = mit->second;
        EGS_Interpolator *tmp_muen = dp.getMuenInterpolator(scoring_med);
        if (!tmp_muen) {
            egsFatal("EB_Application::Failed to get muen interpolator for %s\n", scoring_med.c_str());
        }

        int med_idx = EGS_BaseGeometry::getMediumIndex(transport_med);
        if (med_idx < 0) {
            egsFatal(
                "EB_Application:: You requested scoring medium %s for transport medium %s but %s"
                " is not present in the geometry\n", scoring_med.c_str(), transport_med.c_str(), transport_med.c_str()
            );
        }

        media_muen[med_idx] = tmp_muen;
        egsInformation(
            "Set medium %d (%s) to use muen data: %s\n",
            med_idx, transport_med.c_str(), scoring_med.c_str()
        );
    }


}

void EB_Application::enableAusgabCalls(int ncalls, AusgabCall calls[]) {
    for (int i= 0; i <  ncalls; i++) {
        setAusgabCall(calls[i], true);
    }

}

void EB_Application::initEDepScoring(EGS_Input *scoring_options) {
    // Set up energy deposition scoring if requested by user

    vector<string> choices;
    choices.push_back("no");
    choices.push_back("yes");

    score_edep = scoring_options->getInput("score energy deposition", choices, 0);

    if (score_edep) {
        for (size_t p=0; p < phantom_geoms.size(); p++) {
            phantom_geoms[p]->enableInteractionScoring();
        }

        AusgabCall edep_calls[] = {ExtraEnergy, UserDiscard, PegsCut, EgsCut};
        enableAusgabCalls(4, edep_calls);
    }
}

void EB_Application::initScatScoring(EGS_Input *scoring_options) {
    // Set up energy deposition scoring if requested by user

    vector<string> choices;
    choices.push_back("no");
    choices.push_back("yes");

    score_scat = scoring_options->getInput("score scatter dose", choices, 0);
    if (score_scat && !score_tlen) {
        egsFatal("Tracklength scoring must be enabled to score scatter dose\n");
    }

    if (score_scat) {

        for (size_t p=0; p < phantom_geoms.size(); p++) {
            phantom_geoms[p]->enableScatterScoring();
        }

        AusgabCall photon_scat_calls[] = {
            AfterAnnihFlight, AfterAnnihRest,
            AfterPair, AfterCompton, AfterPhoto, AfterRayleigh,
            FluorescentEvent
        };

        int ncalls = sizeof(photon_scat_calls)/sizeof(photon_scat_calls[0]);
        enableAusgabCalls(ncalls, photon_scat_calls);
    }
}

void EB_Application::initDoseScaling(EGS_Input *scoring_options) {

    EGS_Float scale = 1.;

    int err = scoring_options->getInput("dose scaling factor", scale);

    if (!err && scale > 0) {
        for (size_t p=0; p < phantom_geoms.size(); p++) {
            phantom_geoms[p]->setDoseScale(scale);
        }
    }
}

void EB_Application::initXCCScaling(EGS_Input *scoring_options) {

    EGS_Input *scale;

    while ((scale = scoring_options->takeInputItem("scale xcc"))) {
        vector<string> tmp;
        int err = scale->getInput("scale xcc",tmp);
        //egsInformation("Found 'scale xcc', err=%d tmp.size()=%d\n",err,tmp.size());
        if (!err && tmp.size() == 2) {
            int imed = EGS_BaseGeometry::getMediumIndex(tmp[0]) + 1;
            if (imed > 0) {
                EGS_Float fac = atof(tmp[1].c_str());
                egsInformation("\n ***** Scaling xcc of medium %d with %G\n",imed,fac);
                F77_OBJ_(egs_scale_xcc,EGS_SCALE_XCC)(&imed,&fac);
            }
        }
        delete scale;
    }

    while ((scale = scoring_options->takeInputItem("scale bc"))) {
        vector<string> tmp;
        int err = scale->getInput("scale bc",tmp);
        //egsInformation("Found 'scale xcc', err=%d tmp.size()=%d\n",err,tmp.size());
        if (!err && tmp.size() == 2) {
            int imed = EGS_BaseGeometry::getMediumIndex(tmp[0]) + 1;
            if (imed > 0) {
                EGS_Float fac = atof(tmp[1].c_str());
                egsInformation("\n ***** Scaling bc of medium %d with %G\n",imed,fac);
                F77_OBJ_(egs_scale_bc,EGS_SCALE_BC)(&imed,&fac);
            }
        }
        delete scale;
    }
}


void EB_Application::copyParticleToSourceLoc(EGS_Particle *p, int source, bool kill_orig, bool rotate,
        EGS_Float new_wt) {

    // copy current location of particle
    EGS_Vector new_loc(p->x);
    EGS_Vector new_dir(p->u);

    // first move back to non transformed location
    base_transform_inv->transform(new_loc);
    base_transform_inv->rotate(new_dir);

    if (rotate) {
        double angle = 2*M_PI*rndm->getUniform();
        EGS_RotationMatrix rot = EGS_RotationMatrix::rotZ(angle);
        new_loc *= rot;
        new_dir *= rot;
    }

    // now transform particle to position of requested source
    source_transforms[source]->transform(new_loc);
    source_transforms[source]->rotate(new_dir);

    // where is new particle
    int new_reg = geometry->isWhere(new_loc);
    int new_med = geometry->medium(new_reg);

    // add a new particle to top of stack

    int cur_np = the_stack->np;
    int new_np = the_stack->np+1;
    int np_idx = new_np-1;

    the_stack->ir[np_idx] = new_reg + 2;
    the_stack->latch[np_idx] = p->latch;
    the_stack->E[np_idx] = p->E;
    the_stack->wt[np_idx] = new_wt;;
    the_stack->iq[np_idx] = p->q;
    the_stack->x[np_idx] = new_loc.x;
    the_stack->y[np_idx] = new_loc.y;
    the_stack->z[np_idx] = new_loc.z;
    the_stack->u[np_idx] = new_dir.x;
    the_stack->v[np_idx] = new_dir.y;
    the_stack->w[np_idx] = new_dir.z;
    the_stack->dnear[np_idx] = 0; // set to 0 because this move happens at boundary

    the_stack->np += 1;
    the_stack->npold = cur_np;

    the_epcont->irnew = new_reg+2;
    the_useful->medium_new = new_med+1;

    enterNewRegion();

    if (kill_orig) {
        the_stack->wt[np_idx-1] = 0;
        p->wt = 0;
    }
}

void EB_Application::startNewParticle() {

    EGS_AdvancedApplication::startNewParticle();

    int ireg = the_stack->ir[the_stack->np-1] - 2;
    if (ginfo.isSource(ireg)) {
        the_bounds->ecut = source_ecut;
        the_bounds->pcut = source_pcut;
    } else {
        the_bounds->ecut = global_ecut;
        the_bounds->pcut = global_pcut;
    }

}

void EB_Application::enterNewRegion() {

    EGS_AdvancedApplication::enterNewRegion();

    int ireg = the_epcont->irnew-2;
    if (ginfo.isSource(ireg)) {
        the_bounds->ecut_new = source_ecut;
        the_bounds->pcut_new = source_pcut;
        the_egsvr->i_do_rr = source_i_do_rr;
        the_egsvr->e_max_rr = source_e_max_rr;
    } else {
        the_bounds->ecut_new = global_ecut;
        the_bounds->pcut_new = global_pcut;
        the_egsvr->i_do_rr = global_i_do_rr;
        the_egsvr->e_max_rr = global_e_max_rr;
    }

}

void EB_Application::discardTopParticle(int idisc) {

    top_p.wt = 0;
    the_stack->wt[the_stack->np-1] = 0;
    the_epcont->idisc = idisc;

}

void EB_Application::addRecycledParticlesToStack(EGS_Particle *p, bool new_hist) {

    if (!recycle_opts) {
        return;
    }

    EGS_Float wt = p->wt/source_weights[active_source];

    for (int recycle_idx=0; recycle_idx < recycle_opts->nrecycle; recycle_idx++) {

        int start_loc, stop_loc;

        if (run_mode == RM_SUPERPOSITION) {
            start_loc = active_source;
            stop_loc = start_loc + 1;
        } else {
            start_loc = 0;
            stop_loc = nsources;
        }

        for (int source_loc = start_loc; source_loc < stop_loc; source_loc++) {
            // we only want to kill the original particle if this is not
            // new history from phsp
            bool kill =  source_loc == start_loc && recycle_idx == 0 && !new_hist;
            EGS_Float new_wt = wt*source_weights[source_loc];
            copyParticleToSourceLoc(p, source_loc, kill, recycle_opts->rotate, new_wt);
        }
    }


}

bool EB_Application::isStuck() {

    /* if particle number has changed, reset the stuck particle counter so that
     * particles generated from splitting don't trigger same position error */
    bool same_particle = the_stack->npold == the_stack->np;
    if (!same_particle){
        steps_at_same_loc = 0;
        return false;
    }

    cur_R = top_p.x.length2();
    bool in_same_pos = fabs(cur_R - last_R) < SAME_POSITION_TOLERANCE;
    if (in_same_pos) {
        steps_at_same_loc++;
    }
    last_R = cur_R;

    if (steps_at_same_loc > NUM_STUCK_STEPS) {
        //PRINT_PARTICLE(top_p);
        egsWarning("Particle hasn't moved in %d steps. Discarding particle\n", NUM_STUCK_STEPS);
        printParticleWithSpherical(top_p);
        n_stuck++;
        discardTopParticle();

        return true;
    }
    return false;

}

int EB_Application::ausgab(int iarg) {


    bool is_photon = top_p.q == 0;
    bool is_before_transport = iarg == BeforeTransport;
    bool is_after_transport = iarg == AfterTransport;

    if (is_after_transport && isStuck()) {
        return 1;
    }

    int global_ir = top_p.ir;
    int irnew = the_epcont->irnew-2;
    int irold = the_epcont->irold-2;

    bool in_phantom = ginfo.isPhantom(global_ir);
    bool in_source = ginfo.isSource(global_ir);

    /* track number of steps */
    if (is_before_transport) {
        if (in_source) {
            steps_in_sources[top_p.q]++;
        } else if (in_phantom) {
            steps_in_phantoms[top_p.q]++;
        } else {
            steps_in_other[top_p.q]++;
        }
    }

    /* Fluorescent photon cutoff */
    bool discard_fluorescent = iarg == FluorescentEvent && top_p.E <= flu_cutoff;
    if (discard_fluorescent) {
        discardTopParticle();
        return 0;
    }

    /* track scattering with latch bits if required */
    if (score_scat && (iarg > AfterTransport) && !in_source) {
        latch_control.addScatter(&top_p);
        for (int ip=the_stack->npold-1; ip < the_stack->np; ip++) {
            latch_control.addScatter(the_stack->latch[ip]);
        }
    }


    bool next_is_source = ginfo.isSource(irnew);
    bool last_was_source = ginfo.isSource(irold);

    /* note a particle leaving source could be a particle that has re-entered a
     * source. escaping_source will only be true on first time leaving source*/
    bool leaving_source = is_before_transport && in_source && !next_is_source;
    bool escaping_source = leaving_source && !latch_control.hasEscaped(&top_p);

    /* same as leaving/escaping but for after transport rather than before*/
    bool left_source = is_after_transport && !in_source && last_was_source;
    bool escaped_source = left_source && !latch_control.hasEscaped(&top_p);

    bool escaping_geom = is_before_transport && irnew  < 0;
    bool escaped_geom = global_ir < 0;


    /* these notifications may be used by subscribers like spectrum
     * scoring objects, latch control etc */
    SendMessage send_messages[] = {
        SendMessage(is_before_transport, PARTICLE_TAKING_STEP),
        SendMessage(is_after_transport, PARTICLE_TOOK_STEP),
        SendMessage(escaping_source, PARTICLE_ESCAPING_SOURCE),
        SendMessage(escaped_source, PARTICLE_ESCAPED_SOURCE),
        SendMessage(escaping_geom, PARTICLE_ESCAPING_GEOM),
        SendMessage(escaped_geom, PARTICLE_ESCAPED_GEOM)
    };
    int nmesg = sizeof(send_messages)/sizeof(send_messages[0]);
    for (int m=0; m < nmesg; m++) {
        if (send_messages[m].first) {
            EB_Message msg = send_messages[m].second;
            pevent_pub.notify(send_messages[m].second, &top_p);
            if (msg == PARTICLE_ESCAPED_SOURCE) {
                latch_control.setPrimary(the_stack->latch[the_stack->np-1]);
            }
        }
    }


    if (escaped_geom) {
        return 0;
    }

    /* move particles to new locations if single generator or recycling */
    if (escaped_source && is_after_transport) {

        EGS_Float wt = top_p.wt;

        if (run_mode == RM_SUPERPOSITION) {
            superpos_geom->setActiveByIndex(active_source);
        }

        if (recycle_opts) {
            addRecycledParticlesToStack(&top_p);
        } else if (single_generator && active_source > 0) {
            copyParticleToSourceLoc(&top_p, active_source, true, false, wt);
        }

        return 0;

    }

    /* handle bcse and splitting */
    doPhotonSplitting(iarg);


    /* And finally below here we handle dose scoring */
    bool in_vaccuum = the_useful->medium <= 0;
    bool score_tracklength = score_tlen && is_photon && is_before_transport;
    bool score_interaction = score_edep && iarg <= ExtraEnergy;
    bool dose_scoring_not_needed = !in_phantom || in_vaccuum || !(score_tracklength || score_interaction);

    if (dose_scoring_not_needed) {
        return 0;
    }


    // which phantom/region are we in
    int phant_idx = ginfo.phantomFromRegion(global_ir);
    int phant_ir = ginfo.globalToLocalReg(global_ir);

    EB_Phantom *phant = phantom_geoms[phant_idx];

    bool needs_vol_cor = run_mode == RM_SUPERPOSITION && superpos_geom->hasInactiveGeom(global_ir);

    if (score_tracklength) {

        EGS_Interpolator *interp = media_muen[the_useful->medium-1];
        EGS_Float muen_val = interp ? interp->interpolateFast(the_epcont->gle) : 0;
        EGS_Float tracklength_edep = the_epcont->tvstep*top_p.E*muen_val*top_p.wt;

        EGS_Float vol;
        if (needs_vol_cor) {
            vol = phant->getUncorrectedVolume(phant_ir);
        } else {
            vol = phant->getCorrectedVolume(phant_ir);
        }

        if (vol > 0) {
            tracklength_edep /= vol;
            phant->scoreTlen(phant_ir, tracklength_edep, &top_p);
        }
    }

    if (score_interaction) {
        EGS_Float mass;
        if (needs_vol_cor) {
            mass = phant->getUncorrectedMass(phant_ir);
        } else {
            mass = phant->getRealMass(phant_ir);
        }
        if (mass > 0) {
            EGS_Float edep = getEdep()*top_p.wt/mass;
            phant->scoreEdep(phant_ir, edep);
        }
    }

    return 0;
}

void EB_Application::doPhotonSplitting(int iarg) {

    bool is_phat = fabs(top_p.wt - 1.) < EB_EPSILON;

    if (do_bcse) {

        bool in_bcse_med = the_useful->medium - 1 == bcse_med_num;

        if (iarg == BeforeBrems) {
            the_egsvr->nbr_split = do_brem_split ? nbr_split : 1;
            if (!in_bcse_med) {
                /* standard UBS for brem photons OUTSIDE MED_BCSE */
                the_egsvr->nbr_split *= (int)bcse_factor;
            }
        } else if (iarg == AfterBrems) {
            if (in_bcse_med) {
                /* Just came back from a brem event in MED_BCSE, reduce photon weight
                and play Russian Roulette to whether or not reduce electron energy */
                for (int ip=the_stack->npold; ip < the_stack->np; ip++) {
                    the_stack->wt[ip] /= bcse_factor;
                }
                EGS_Float rnd = rndm->getUniform();
                if (rnd > 1./bcse_factor) {
                    the_stack->E[the_stack->npold-1] += top_p.E;
                }
            } else {
                /* Turn off UBS after splitting that one fat brem photons outside MED_BCSE */
                the_egsvr->nbr_split = 1;
            }

        } else if (iarg == FluorescentEvent && is_phat) {

            /* Uniformly splits fat relaxation photons that are descendents of either:
                1. eii in MED_BCSE (usually MED_BCSE is the target) or, 2.
                back-scattered fat electrons that escaped target and interacted
                elsewhere (eii and photo) This is done using UNIFORM_PHOTONS  */

            int nsplit = do_brem_split ? (int)(bcse_factor*nbr_split) : (int)(bcse_factor);
            the_stack->npold = the_stack->np;
            F77_OBJ_(egs_uniform_photons, EGS_UNIFORM_PHOTONS)(&nsplit, &top_p.E);

        }

    } else if (do_brem_split && iarg == FluorescentEvent && is_phat) {

        /* a fluorescent photon has just been put on the stack - split it */
        the_stack->npold = the_stack->np;
        F77_OBJ_(egs_uniform_photons, EGS_UNIFORM_PHOTONS)(&nbr_split, &top_p.E);
    }

}

void EB_Application::calcEffectiveHistories() {

    if (is_phsp_source) {
        effective_histories = source->getFluence();
    } else {
        effective_histories = last_case;
    }

    if (recycle_opts) {
        if (run_mode == RM_SUPERPOSITION) {
            // only recycle at 1 source location in superposition mode
            effective_histories *= recycle_opts->nrecycle;
        } else {
            effective_histories *= recycle_opts->nrecycle*nsources;
        }
    }
}


void EB_Application::outputResults() {
    timing_blocks.addTimer("egs_brachy::outputResults");

    calcEffectiveHistories();

    string sep(80, '=');
    egsInformation(
        "\n\nResults for egs_brachy run\n%s\n",
        string(80, '-').c_str()
    );


    egsInformation("\n\nHistory Information\n%s\n",sep.c_str());
    egsInformation("Last case            = %llu\n", last_case);
    egsInformation("current case         = %llu\n", current_case);
    egsInformation("run->getNcase()      = %llu\n", run->getNcase());
    egsInformation("source->getFluence() = %f\n", source->getFluence());
    if (recycle_opts) {
        egsInformation("Particles recycled  = %d\n", recycle_opts->nrecycle);
    }
    egsInformation("Effective histories  = %.5G\n", effective_histories);


    egsInformation("\n\nGeometry Errors\n%s\n", sep.c_str());
    egsInformation("Number of geometry errors (/max allowed) = %d / %d\n", run->geomErrorCount, run->geomErrorMax);
    egsInformation("Number of 'stuck' particles discarded    = %llu\n", n_stuck);


    // tell scoring objects how many histories were actually run
    // (needed for parallel run normalization)
    EGS_I64 hist_norm = run->getNdone();
    pevent_pub.notify(NEW_HISTORY, &hist_norm);

    if (!single_generator) {
        egsInformation(
            "Warning: Stats about energy escaping source may not be accurate when "
            "not using a single source generator\n"
        );
    }

    if (phsp) {
        phsp->finish(current_case);
        phsp->outputResults();
        phsp->destroySource();
    }

    if (escoring) {
        escoring->outputResults();
    }

    if (spectrum_scorers.size() > 0) {
        egsInformation("\nSpectrum scoring results\n%s\n",sep.c_str());
        string root = constructIOFileName("", true);
        vector<BaseSpectrumScorer *>::iterator it = spectrum_scorers.begin();
        for (; it != spectrum_scorers.end(); it++) {
            (*it)->setEffectiveHistories(effective_histories);
            (*it)->outputResults(root);
        }
    }

    if (record_n_init > 0) {

        string fname = constructIOFileName("", true)+".pinit";
        ofstream out;
        out.open(fname.c_str());
        out << std::fixed << std::showpoint << std::setprecision(6);
        for (vector<EGS_Vector>::iterator it = p_init_locs.begin(); it != p_init_locs.end(); ++it) {
            out << (*it).x << "\t" << (*it).y << "\t" << (*it).z << endl;
        }
        out.close();
        egsInformation("\nInitial Particle Positions\n%s\n",sep.c_str());
        egsInformation("%d Initial particle positions written to .pinit file\n", p_init_locs.size());
    }

    timing_blocks.addTimer("egs_brachy::outputPhantomResults");
    for (vector<EB_Phantom *>::iterator p = phantom_geoms.begin(); p != phantom_geoms.end(); p++) {
        (*p)->setEffectiveHistories(effective_histories);
        bool output_vc = find(output_volcor_phantoms.begin(), output_volcor_phantoms.end(),
                              (*p)->geometry->getName()) != output_volcor_phantoms.end();
        string dd_format = output_3ddose_files ? output_dose_format : "";
        string vc_format = output_vc ? output_volcor_format : "";
        string ep_format = output_egsphant ? output_egsphant_format : "";
        string vi_format = output_voxinfo ? output_voxinfo_format : "";
        (*p)->outputResults(20, dd_format, ep_format, vi_format, vc_format);
    }
    timing_blocks.stopTimer();

    egsInformation("\nStep Counts\n%s\n", sep.c_str());
    EGS_Float total_steps = 0;

    for (int i=-1; i < 2; i++) {
        total_steps += steps_in_sources[i];
        total_steps += steps_in_phantoms[i];
        total_steps += steps_in_other[i];
    }

    egsInformation("Total particle steps               : %20llu\n", (EGS_I64)total_steps);
    for (int i=-1; i < 2; i++) {
        if (steps_in_sources[i]  > 0) {
            egsInformation("q=%2d Steps taken in sources        : %20llu (%.2f%%)\n", i, steps_in_sources[i],
                           100*steps_in_sources[i]/total_steps);
        }
        if (steps_in_phantoms[i]  > 0) {
            egsInformation("q=%2d Steps taken in phantoms       : %20llu (%.2f%%)\n", i, steps_in_phantoms[i],
                           100*steps_in_phantoms[i]/total_steps);
        }
        if (steps_in_other[i]  > 0) {
            egsInformation("q=%2d Steps taken in other objects  : %20llu (%.2f%%)\n", i, steps_in_other[i],
                           100*steps_in_other[i]/total_steps);
        }
    }

    timing_blocks.stopTimer();
    timing_blocks.outputInfo();


}


void EB_Application::getCurrentResult(double &sum, double &sum2, double &norm, double &count) {

    count = current_case;
    calcEffectiveHistories();
    gcr_phantom->setEffectiveHistories(effective_histories);
    gcr_phantom->getCurrentScore(gcr_phantom_reg, sum, sum2);
    norm=gcr_phantom->getTlenNorm(gcr_phantom_reg);

}

int EB_Application::runSimulation() {
    timing_blocks.addTimer("egs_brachy::runSimulation");
    int result = EGS_Application::runSimulation();
    timing_blocks.stopTimer();
    return result;
}

/* \brief initialize a particle from source and then transform to next source */
int EB_Application::simulateSingleShower() {

    last_case = current_case;

    current_case = source->getNextParticle(rndm, p.q, p.latch, p.E, p.wt, p.x, p.u);


    int initial_source;
    if ((is_phsp_source && !recycle_opts) || !single_generator) {
        initial_source = active_source;
    } else {
        initial_source = 0;
    }

    p.wt = source_weights[active_source];

    if (run_mode == RM_SUPERPOSITION) {
        superpos_geom->setActiveByIndex(initial_source);
    }

    // transform particle to position of next source
    source_transforms[initial_source]->transform(p.x);
    source_transforms[initial_source]->rotate(p.u);

    p.ir = geometry->isWhere(p.x);

    if (p.ir < 0) {
        egsFatal("EB_Application::simulateSingleShower() - Particle initiated outside "
                 "the geometry! Please check your source locations.\n");
    }

    if (is_phsp_source) {
        // we assume phase space particles are outside the source
        pevent_pub.notify(PARTICLE_ESCAPED_SOURCE, &p);

        // set up stack with particles if recyling and phsp source
        if (recycle_opts) {
            addRecycledParticlesToStack(&p, true);
        }
    } else if (!ginfo.isSource(p.ir)) {

        printParticleWithSpherical(p);
        egsFatal("Particle started in region %d which is outside a source geometry. Please check your geometry\n", p.ir);

    }

    if (record_n_init > 0 && (int)p_init_locs.size() < record_n_init) {
        p_init_locs.push_back(p.x);
    }

    cur_R = p.x.length2();
    last_R = cur_R;
    steps_at_same_loc = 0;

    int err = startNewShower();
    if (err) {
        return err;
    }

    // shower() just resets the stack before calling egsShower() but when using
    // a phsp source with recycling, we've already set up stack so go straight
    // to egsShower() in that case;
    if (is_phsp_source && recycle_opts) {
        err = 0;
        egsShower();
    } else {
        err = shower();
    }

    active_source++;
    if (active_source == nsources) {
        active_source =0;
    }

    return err || finishShower();
}


int EB_Application::startNewShower() {


    // notify subscribers a valid particle has been initialized
    pevent_pub.notify(PARTICLE_INITIALIZED, &p);

    //  sets current case in ausgab objects
    int err = EGS_Application::startNewShower();

    if (err) {
        return err;
    }

    // now set current case in all phantom objects
    if (current_case != last_case) {

        pevent_pub.notify(NEW_HISTORY, &current_case);

        last_case = current_case;
    }

    return 0;
}


/* start of stop/restart functionality  ****************************************/
int EB_Application::egsApplicationOutputData(ostream *out) {
    /* adapted from egs_application.cpp::outputData */
    if (!run->storeState(*out)) {
        return 2;
    }
    if (!egsStoreI64(*out,current_case)) {
        return 3;
    }
    (*out) << endl;
    if (!rndm->storeState(*out)) {
        return 4;
    }
    if (!source->storeState(*out)) {
        return 5;
    }
    for (size_t j=0; j<a_objects_list.size(); ++j) {
        if (!a_objects_list[j]->storeState(*out)) {
            return 6;
        }
    }

    return 0;
}

int EB_Application::egsAdvApplicationOutputData(ostream *out) {
    /* adapted from egs_advanced_application.cpp::outputData */
    EGS_I32 np, ip;
    egsGetRNGPointers(&np,&ip);
    if (np < 1) {
        return 11;
    }
    if (np > 10000000) {
        egsWarning("EGS_AdvancedApplication::outputData(): egsGetRNGPointers"
                   " returns a huge number? (%d)\n",np);
        return 12;
    }
    EGS_Float *array = new EGS_Float [np];
    egsGetRNGArray(array);
    (*out) << "  " << np << "  " << ip << endl;
    for (int j=0; j<np; j++) {
        (*out) << array[j] << " ";
    }
    (*out) << endl;
    double ch_steps, all_steps;
    egsGetSteps(&ch_steps,&all_steps);
    (*out) << ch_steps << "  " << all_steps << endl;
    delete [] array;
    return out->good() ? 0 : 13;

}

int EB_Application::egsBrachyOutputData(ostream *out) {

    int err;

    (*out) << std::setprecision(9);

    if (escoring) {
        err= escoring->outputData(out);
        if (err) {
            return err;
        }
    }

    vector<BaseSpectrumScorer *>::iterator spec_it = spectrum_scorers.begin();
    for (; spec_it != spectrum_scorers.end(); ++spec_it) {
        err = (*spec_it)->outputData(out);
        if (err) {
            return err;
        }
    }

    vector<EB_Phantom *>::iterator phant_it = phantom_geoms.begin();
    for (; phant_it != phantom_geoms.end(); phant_it++) {
        err = (*phant_it)->outputData(out);
        if (err) {
            return err;
        }
    }

    return 0;

}

int EB_Application::outputDataHelper(ostream *out) {

    int err = egsApplicationOutputData(out);
    if (err) {
        return err;
    }
    err = egsAdvApplicationOutputData(out);
    if (err) {
        return err;
    }

    return egsBrachyOutputData(out);
}

int EB_Application::outputData() {

    timing_blocks.addTimer("egs_brachy::outputData");

    bool use_gz = output_egsdat_format == "gzip";
    string extension(".egsdat");
    extension += (use_gz ? ".gz" : "");
    string ofile = constructIOFileName(extension.c_str(), true);

    int err;
    if (use_gz) {
        if (gz_data_out) {
            delete gz_data_out;
        }
        gz_data_out = new ogzstream(ofile.c_str());

        if (!(gz_data_out)) {
            egsWarning("EGS_Application::outputData: failed to open %s for writing\n",ofile.c_str());
            timing_blocks.stopTimer();
            return 1;
        }
        err = outputDataHelper(gz_data_out);
        gz_data_out->close();
    } else {

        if (data_out) {
            delete data_out;
        }
        data_out = new ofstream(ofile.c_str());
        if (!(*data_out)) {
            egsWarning("EGS_Application::outputData: failed to open %s for writing\n",ofile.c_str());
            timing_blocks.stopTimer();
            return 1;
        }
        err = outputDataHelper(data_out);
    }

    timing_blocks.stopTimer();
    return err;

}

int EB_Application::egsApplicationReadData(istream *in) {
    /* adapted from egs_application.cpp::readData */
    if (!run->setState(*in)) {
        return 2;
    }
    if (!egsGetI64(*in,current_case)) {
        return 3;
    }
    last_case = current_case;
    if (!rndm->setState(*in)) {
        return 4;
    }
    if (!source->setState(*in)) {
        return 5;
    }
    for (int j=0; j<a_objects_list.size(); ++j) {
        if (!a_objects_list[j]->setState(*in)) {
            return 6;
        }
    }
    return 0;
}

int EB_Application::egsAdvApplicationReadData(istream *in) {
    /* adapted from egs_advanced_application.cpp::readData */
    int np, ip;
    (*in) >> np >> ip;
    if (np < 1) {
        return 11;
    }
    if (np > 10000000) {
        egsWarning("EB_Application::egsAdvApplicationReadData(): got huge size "
                   "for the mortran random array? (%d)\n",np);
        return 12;
    }
    EGS_Float *array = new EGS_Float [np];
    for (int j=0; j<np; j++) {
        (*in) >> array[j];
    }
    if (!in->good()) {
        return 13;
    }
    egsSetRNGState(&ip,array);
    delete [] array;
    double ch_steps, all_steps;
    (*in) >> ch_steps >> all_steps;
    egsSetSteps(&ch_steps,&all_steps);
    return in->good() ? 0 : 13;

}

int EB_Application::egsBrachyReadData(istream *in) {
    int err;

    if (escoring) {
        err= escoring->readData(in);
        if (err) {
            return err;
        }
    }

    vector<BaseSpectrumScorer *>::iterator spec_it = spectrum_scorers.begin();
    for (; spec_it != spectrum_scorers.end(); ++spec_it) {
        err = (*spec_it)->readData(in);
        if (err) {
            return err;
        }
    }

    vector<EB_Phantom *>::iterator phant_it = phantom_geoms.begin();
    for (; phant_it != phantom_geoms.end(); phant_it++) {
        err = (*phant_it)->readData(in);
        if (err) {
            return err;
        }
    }
    return 0;

}

int EB_Application::readDataHelper(istream *in) {

    int err = egsApplicationReadData(in);
    if (err) {
        return err;
    }
    err = egsAdvApplicationReadData(in);
    if (err) {
        return err;
    }

    return egsBrachyReadData(in);
}

/* start of stop/restart functionality  ****************************************/
int EB_Application::readData() {

    timing_blocks.addTimer("egs_brachy::readData");

    bool use_gz = output_egsdat_format == "gzip";
    string extension(".egsdat");
    extension += (use_gz ? ".gz" : "");
    string ifile = constructIOFileName(extension.c_str(), true);

    int err;
    if (use_gz) {
        if (gz_data_in) {
            delete gz_data_in;
        }
        gz_data_in = new igzstream(ifile.c_str());

        if (!(gz_data_in)) {
            egsWarning("EGS_Application::readData: failed to open %s for reading\n", ifile.c_str());
            timing_blocks.stopTimer();
            return 1;
        }
        err = readDataHelper(gz_data_in);
    } else {

        if (data_in) {
            delete data_in;
        }
        data_in = new ifstream(ifile.c_str());
        if (!(*data_in)) {
            egsWarning("EGS_Application::readData: failed to open %s for writing\n",ifile.c_str());
            timing_blocks.stopTimer();
            return 1;
        }
        err = readDataHelper(data_in);
    }

    timing_blocks.stopTimer();
    return err;

}


int EB_Application::combineResults() {
    /* Adapted from egs_application.cpp::combineResults */
    egsInformation(
        "\n                      Suming the following .egsdat files:\n"
        "=======================================================================\n");
    char buf[512];
    resetCounter();
    EGS_Float last_cpu = 0;
    EGS_I64 last_ncase = 0;
    int ndat = 0;
    bool ok = true;
    for (int j=1; j<500; j++) {

        bool use_gz = output_egsdat_format == "gzip";
        string name("%s_w%d.egsdat");
        name += (use_gz ? ".gz" : "");

        sprintf(buf, name.c_str(), output_file.c_str(), j);
        string dfile = egsJoinPath(app_dir,buf);
        istream *data;
        ifstream in;
        igzstream gzin;
        bool opened = false;
        if (use_gz) {
            gzin.open(dfile.c_str());
            opened = gzin.rdbuf()->is_open();
            data = &gzin;
        } else {
            in.open(dfile.c_str());
            opened = in.is_open();
            data = &in;
        }

        if (opened) {
            int err = addState(*data);
            ++ndat;
            if (!err) {
                EGS_I64 ncase = run->getNdone();
                EGS_Float cpu = run->getCPUTime();
                egsInformation("%2d %-30s ncase=%-14lld cpu=%-11.2f\n",
                               ndat,buf,ncase-last_ncase,cpu-last_cpu);
                last_ncase = ncase;
                last_cpu = cpu;
            } else {
                ok = false;
                egsWarning("%2d %-30s error %d\n",ndat,buf,err);
            }
        }
    }
    if (ndat > 0) {
        egsInformation(
            "=======================================================================\n");
        egsInformation("%40s%-14lld cpu=%-11.2f\n\n","Total ncase=",last_ncase,
                       last_cpu);
    }
    if (ndat > 0) {
        return ok ? 0 : -1;
    } else {
        return 1;
    }
}

void EB_Application::resetCounter() {
    // Reset everything in the base class
    EGS_AdvancedApplication::resetCounter();
    // Reset our own data to zero.

    if (escoring) {
        escoring->resetCounter();
    }

    vector<BaseSpectrumScorer *>::iterator spec_it = spectrum_scorers.begin();
    for (; spec_it != spectrum_scorers.end(); ++spec_it) {
        (*spec_it)->resetCounter();
    }

    vector<EB_Phantom *>::iterator phant_it = phantom_geoms.begin();
    for (; phant_it != phantom_geoms.end(); phant_it++) {
        (*phant_it)->resetCounter();
    }

}

int EB_Application::addState(istream &data) {
    // Call first the base class addState() function to read and add
    // all data related to source, RNG, CPU time, etc.
    int err = EGS_AdvancedApplication::addState(data);
    if (err) {
        return err;
    }

    if (escoring) {
        err = escoring->addState(data);
        if (err) {
            return err;
        }
    }

    vector<BaseSpectrumScorer *>::iterator spec_it = spectrum_scorers.begin();
    for (; spec_it != spectrum_scorers.end(); ++spec_it) {
        err = (*spec_it)->addState(data);
        if (err) {
            return err;
        }
    }

    vector<EB_Phantom *>::iterator phant_it = phantom_geoms.begin();
    for (; phant_it != phantom_geoms.end(); phant_it++) {
        err = (*phant_it)->addState(data);
        if (err) {
            return err;
        }
    }

    return 0;

}
/* end of stop/restart functionality  ****************************************/

#ifdef BUILD_APP_LIB
APP_LIB(EB_Application);
#else
APP_MAIN(EB_Application);
#endif
