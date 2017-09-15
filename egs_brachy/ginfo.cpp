/*
################################################################################
#
#  egs_brachy ginfo.cpp
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

/*! \file ginfo.cpp
 *
 * \brief implementation of Geometry Info class.
 */
#include "ginfo.h"
#include <limits>
#include <algorithm>
#include <sstream>

typedef pair <string, vector<int> > CDGeomRegType;

// comarison function for sorting CD 'set geometry' region definitions
bool pairCompare(const CDGeomRegType &firstElem, const CDGeomRegType &secondElem) {
    return firstElem.second[0] < secondElem.second[0];
}

// simple string join routine
string join(const vector<string> &v, string delim) {

    string s="";

    for (vector<string>::const_iterator p = v.begin(); p != v.end(); ++p) {
        s += *p;
        if (p != v.end() - 1) {
            s += delim;
        }
    }
    return s;
}


// build up our geometry tree based from composite geometries
Node GeomInfo::build_tree(string root, vector<Node> &children) {

    for (size_t i =0; i < gmap[root].children.size(); i++) {
        string child = gmap[root].children[i];
        vector<Node> empty;
        children.push_back(build_tree(child, empty));
    }

    return Node(root, children);
}

// parse the input geometry definition and figures out which geometries are inscribed in others
// and then builds up a tree of nodes that can be used for figuring out the egs++ region
// numbers assigned to each geometry
int GeomInfo::initializeFromInput(EGS_Input *input) {

    int err = input->getInput("simulation geometry", sim_geom_name);
    if (err) {
        egsWarning("GeomInfo::initializeFromInput: missing keyword 'simulation geometry'\n");
        return 1;
    }

    input->getInput("source envelope geometry", source_envelope_name);

    err = input->getInput("phantom geometries", phantom_names);
    if (err) {
        egsWarning("GeomInfo::initializeFromInput: missing keyword 'phantom geometries'\n");
        return 1;
    }
    sort(phantom_names.begin(), phantom_names.end());

    err = input->getInput("source geometries", source_names);
    if (err) {
        egsWarning("GeomInfo::initializeFromInput: missing keyword 'source geometries'\n");
        return 1;
    }

    // keep track of the the items we're taking so we can add them back to the input
    // for the actual geometry creation routinte to use
    vector<EGS_Input *> items;

    EGS_Input *ij;
    while ((ij = input->takeInputItem("geometry")) != 0) {

        items.push_back(ij);

        string name("");
        int err = ij->getInput("name", name);
        if (err) {
            egsWarning("GeomInfo::initializeFromInput: Found geometry with no name.  All geometries must be named.\n");
            return 1;
        }

        gmap[name].children = getChildren(name, ij);

    }

    // now add them back to the geometry input
    for (vector<EGS_Input *>::iterator item = items.begin(); item != items.end(); item++) {
        input->addInputItem(**item);
        delete *item;
    }


    // we've got all geometries in the input. Now we need build up a tree of geometries with
    // the simulation geometry as the root node. This allows the setGeometryIndexes method
    // to figure out the order of geometries and starting/ending regions in egs++
    // by traversing the tree
    vector<Node> empty; // need a default empty vector
    geom_tree = new Node(build_tree(sim_geom_name, empty));

    return 0;

}


/* counts the number of transformation blocks for an auto envelope geometry
 * and hence counts the number of inscribed geometries in an envelope
 */
size_t countAutoEnvelopeInscribed(EGS_Input *input) {

    EGS_Input *inscribedInp = input->getInputItem("inscribed geometry");
    if (!inscribedInp) {
        return 0;
    }

    EGS_Input *transformInp = inscribedInp->getInputItem("transformations");
    if (!transformInp) {
        return 1;
    }

    vector<EGS_Input *> items;
    EGS_Input *ij;
    size_t count = 0;

    while ((ij = transformInp->takeInputItem("transformation")) != 0) {
        count++;
        items.push_back(ij);
    }

    // now add them back to the geometry input
    for (vector<EGS_Input *>::iterator item = items.begin(); item != items.end(); item++) {
        transformInp->addInputItem(**item);
        delete *item;
    }

    return count;
}


// return base geometrty name for egs_genvelope or egs_autoenvelope geometry
string getGeomBaseName(EGS_Input *input) {
    string base;
    input->getInput("base geometry", base);
    return base;
}


// return a vector of all the names of the inscribedd geometries in an EGS_AutoEnvelope input
vector<string> getAutoEnvelopeChildren(EGS_Input *input) {

    vector<string> children;
    children.push_back(getGeomBaseName(input));

    EGS_Input *inscribed_inp = input->getInputItem("inscribed geometry");
    if (!inscribed_inp) {
        return children;
    }

    string inscribed_name;
    inscribed_inp->getInput("inscribed geometry name", inscribed_name);

    size_t ninscribed = countAutoEnvelopeInscribed(input);
    for (size_t i=0; i < ninscribed; i++) {
        children.push_back(inscribed_name);
    }

    return children;
}


// return a vector of all the names of the inscribedd geometries in an egs_genvelope input
vector<string> getGEnvelopeChildren(EGS_Input *input) {
    vector<string> children;
    input->getInput("inscribed geometries", children);
    children.insert(children.begin(), getGeomBaseName(input));
    return children;
}


// return a vector of all the names of the inscribed geometries in an egs_cdgeometry input
vector<string> getCDChildren(EGS_Input *input) {
    vector<string> children;
    string name;
    input->getInput("name", name);

    //children.push_back(getGeomBaseName(input));

    vector<EGS_Input *> items;
    EGS_Input *ij;

    vector<int> start_regs, stop_regs;
    vector<string> names;
    vector<CDGeomRegType> regs;

    while ((ij = input->takeInputItem("set geometry")) != 0) {
        items.push_back(ij);

        vector<string> aux;
        ij->getInput("set geometry", aux);

        int start_reg, stop_reg;
        string name;

        if (aux.size() == 2) {
            start_reg = atoi(aux[0].c_str());
            stop_reg = start_reg;
            name = aux[1];
        } else if (aux.size() == 3) {
            start_reg = atoi(aux[0].c_str());
            stop_reg = atoi(aux[1].c_str());
            name = aux[2];
        }

        vector<int> start_stop_regs;
        start_stop_regs.push_back(start_reg);
        start_stop_regs.push_back(stop_reg);
        regs.push_back(CDGeomRegType(name, start_stop_regs));

    }


    // now add them back to the geometry input
    for (vector<EGS_Input *>::iterator item = items.begin(); item != items.end(); item++) {
        input->addInputItem(**item);
        delete *item;
    }

    sort(regs.begin(), regs.end(), pairCompare);
    for (size_t i=0; i< regs.size(); i++) {
        for (int g = regs[i].second[0]; g <= regs[i].second[1]; g++) {
            children.push_back(regs[i].first);
        }
    }

    return children;
}


// return a vector of all the names of the inscribedd geometries in an egs_genvelope input
vector<string> getNDChildren(EGS_Input *input) {

    vector<string> children;

    string type;
    input->getInput("type", type);

    if (type == "EGS_XYZGeometry") {
        return children;
    }

    input->getInput("dimensions", children);
    return children;
}


// return a vector of all the names of the inscribedd geometries in an egs_genvelope input
vector<string> getUnionChildren(EGS_Input *input) {
    vector<string> children;
    input->getInput("geometries", children);
    return children;
}


// return a vector of all the names of the inscribedd geometries in an egs_genvelope input
vector<string> getGStackChildren(EGS_Input *input) {
    vector<string> children;
    input->getInput("geometries", children);
    return children;
}


// return a vector of the names of all the children for a given geometry input
// currently only works for egs_autoenvelope and egs_genevelope geometries
// but could be expaned to work for unions etc
vector<string> GeomInfo::getChildren(string name, EGS_Input *input) {

    string library;
    int err = input->getInput("library", library);

    vector<string> children;
    if (err) {
        return children;
    }

    if (library == "egs_autoenvelope") {
        children = getAutoEnvelopeChildren(input);
    } else if (library == "egs_genvelope") {
        children = getGEnvelopeChildren(input);
    } else if (library == "egs_cdgeometry") {
        children = getCDChildren(input);
    } else if (library == "egs_ndgeometry") {
        children = getNDChildren(input);
    } else if (library == "egs_gunion") {
        children = getUnionChildren(input);
    } else if (library == "egs_gstack") {
        children = getGStackChildren(input);
    }

    return children;
}


// return the index of the geometry name in the the given input vector or -1 if not found
int findGeomInVec(EGS_BaseGeometry *geom, vector<string> geoms) {
    for (size_t p=0; p < geoms.size(); p++) {
        if (geoms[p] == geom->getName()) {
            return p;
        }
    }

    return -1;
}


// return the maximum number of regions for the input geometry names
int maxNRegOfGeoms(vector<string> gnames, int start) {
    int max = 0;
    for (size_t i = start; i < gnames.size(); i++) {
        int nreg = EGS_BaseGeometry::getGeometry(gnames[i])->regions();
        max =  nreg > max ? nreg : max;
    }
    return max;
}


// return the number of regions in the subdivision at index idx of a composite geometry
int nregForSubDiv(GeomRegionInfo gri, int idx) {

    EGS_BaseGeometry *geom = EGS_BaseGeometry::getGeometry(gri.name);

    if (gri.type == "EGS_AEnvelope" ||
            gri.type == "EGS_ASwitchedEnvelope" ||
            gri.type == "EGS_EnvelopeGeometry" ||
            gri.type == "EGS_FastEnvelope") {

        if (idx == 0) {
            EGS_BaseGeometry *base = EGS_BaseGeometry::getGeometry(gri.children[idx]);
            return base->regions();
        }

        return maxNRegOfGeoms(gri.children, 1);

    } else if (gri.type == "EGS_CDGeometry") {
        return maxNRegOfGeoms(gri.children, 0);
    } else if (gri.type == "EGS_NDGeometry") {
        return EGS_BaseGeometry::getGeometry(gri.children[idx])->regions();
    } else if (gri.type == "EGS_UnionGeometry") {
        return maxNRegOfGeoms(gri.children, 0);
    } else if (gri.type == "EGS_StackGeometry") {
        return maxNRegOfGeoms(gri.children, 0);
    }

    return geom->regions();
}


// traverse tree and set geometry start/end regions  for a given geometry
void GeomInfo::getGeomRegs(Node root, vector<GeomRegionInfo> &ordered, int start) {

    EGS_BaseGeometry *g = EGS_BaseGeometry::getGeometry(root.name);
    int nreg = g->regions();
    gmap[root.name].name = root.name;
    gmap[root.name].type = g->getType();
    gmap[root.name].start = start;
    gmap[root.name].end = start + nreg - 1;
    gmap[root.name].nreg = nreg;

    ordered.push_back(gmap[root.name]);
    for (size_t i=0; i < gmap[root.name].children.size(); i++) {
        getGeomRegs(root.children[i], ordered, start);
        start += nregForSubDiv(gmap[root.name], i);
    }

}


// set up all arrays required to decide which geometry/phantom a region
// is in and whether or not we are scoring dose in it.
// To do this we loop through all geometries, decide whether it is a phantom
// or not, then loop through each region in the geometry and set the local
// region number phantom index etc
void GeomInfo::setGeometryIndexes(EGS_BaseGeometry *geom) {

    getGeomRegs(*geom_tree, ordered_geom_data, 0);

    nreg_total = EGS_BaseGeometry::getGeometry(sim_geom_name)->regions();
    ngeom = ordered_geom_data.size();

    for (int rr = 0; rr < nreg_total; rr++) {
        global_ir_to_geom.push_back(NULL); // region number to parent geometry
        global_ir_to_phant.push_back(-1); // IF is phantom THEN phantom index else -1
        global_ir_to_source.push_back(-1); // IF is phantom THEN phantom index else -1
        global_ir_to_local_ir.push_back(-1); // global region to local region number
    }

    for (int gg=0; gg < ngeom; gg++) {

        GeomRegionInfo ginfo = ordered_geom_data[gg];
        EGS_BaseGeometry *geom = EGS_BaseGeometry::getGeometry(ginfo.name);

        geom_to_regioninfo[geom] = ginfo;

        int phant_idx =  findGeomInVec(geom, phantom_names);
        int source_idx =  findGeomInVec(geom, source_names);

        for (int rr = ginfo.start; rr <= ginfo.end; rr++) {
            global_ir_to_geom[rr] = geom; // region number to parent geometry
            if (global_ir_to_phant[rr] == -1 || phant_idx > -1) {
                global_ir_to_phant[rr] = phant_idx; // IF is phantom THEN phantom index else -1
            }
            if (global_ir_to_source[rr] == -1 || source_idx > -1) {
                global_ir_to_source[rr] = source_idx; // IF is source THEN source index else -1
            }
            global_ir_to_local_ir[rr] = rr - ginfo.start; // global region to local region number
        }
    }
}


GeomRegT GeomInfo::globalToLocal(int ir) {
    if (ir < 0) {
        return GeomRegT(0,0);
    }
    return GeomRegT(global_ir_to_geom[ir], global_ir_to_local_ir[ir]);
}


int GeomInfo::globalToLocalReg(int ir) {
    return ir >= 0 && ir < nreg_total ? global_ir_to_local_ir[ir] : -1;
}


int GeomInfo::localToGlobal(GeomRegT local) {
    GeomRegionInfo gri = geom_to_regioninfo[local.first];
    return gri.start + local.second;
}


bool GeomInfo::isPhantom(int ir) {
    return ir >= 0 && ir < nreg_total && global_ir_to_phant[ir] >= 0;
}


bool GeomInfo::isSource(int ir) {
    return ir >= 0 && ir < nreg_total && global_ir_to_source[ir] >= 0;
}


int GeomInfo::phantomFromRegion(int ir) {
    return ir >= 0 && ir < nreg_total ? global_ir_to_phant[ir] : -1;
}


void GeomInfo::printInfo() {

    egsInformation("Total number of geometries               = %d\n", ngeom);
    egsInformation("Total number of regions                  = %d\n", nreg_total);
    egsInformation("Simulation geometry                      = %s\n", sim_geom_name.c_str());
    egsInformation("Envelope containing sources              = %s\n", source_envelope_name.c_str());
    string sgnames = join(source_names, ", ");
    egsInformation("Source geometries                        = %s\n", sgnames.c_str());

    egsInformation("\nName                           | First Reg Idx |  Last Reg Idx | Nreg \n%s\n",string(80,
                   '-').c_str());
    for (int gg=0; gg < ngeom; gg++) {
        GeomRegionInfo gr = ordered_geom_data[gg];
        egsInformation("%-30s | %13d | %13d | %12d \n", gr.name.c_str(), gr.start, gr.end, gr.nreg);
    }
    egsInformation("%s\n",string(80, '-').c_str());
}
