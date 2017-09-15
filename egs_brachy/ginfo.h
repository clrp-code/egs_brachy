/*
################################################################################
#
#  egs_brachy ginfo.h
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

/*! \file ginfo.h
 *
 * \brief ginfo contains classes for organizing information about the geometries
 * present in an egs_brachy simulation.
 */
#ifndef EB_GINFO_
#define EB_GINFO_

#include <map>
#include <set>
#include <cstdlib>
#include <algorithm>

#include "egs_functions.h"
#include "egs_input.h"
#include "phantom.h"


// Tree node used for organizing/sorting geometries and their children
class Node {
public:
    string name;
    vector<Node> children;

    Node(string n, vector<Node> children) {
        name = n;
        for (size_t i=0; i < children.size(); i++) {
            addNode(children[i]);
        }

    };

    void addNode(Node node) {
        children.push_back(node);
    }

};


/*! \brief pair of geometry and local region number */
typedef pair<EGS_BaseGeometry *, int> GeomRegT;


/*! \brief a struct to contain elementary information aboud a geometry */
struct GeomRegionInfo {

    string name; /*!< the name given to the geometry by the user */
    string type; /*!< egs++ geometry type */
    vector<string> children; /*!< list of children contained by this geometry (if any) */
    int nreg; /*!< total number of regions in this geometry */
    int start; /*!< global start region number */
    int end; /*!< global end region number */

};

/*! \brief a mapping from geometry name to information about that geometry */
typedef  map<string, GeomRegionInfo> GeomRegionInfoMapT;


/*! \brief a container for organizing meta data about the geometries */
class GeomInfo {

    vector<int> global_ir_to_phant; ///< \brief array of size nreg mapping global region number to
    /// the phantoms geometry index or -1 if not a phant
    //
    vector<int> global_ir_to_source; ///< \brief array of size nreg mapping global region number to
    /// the source geometry index or -1 if not a source


    vector<EGS_BaseGeometry *> global_ir_to_geom; ///< \brief array of size nreg mapping global region number to
    /// pointer for the geometry it belongs to

    vector<int> global_ir_to_local_ir; ///< \brief array of size nreg mapping global region number to
    /// the local region number of the geometry it belongs to.
    /// e.g. it would look like:
    /// global_ir_to_local_ir = [0, 1, 2, 3, 0, 1, 2, 3, 5]
    /// for a geometry consisting of two objects, the first with 4 regions
    /// and the second with 5 regions
    //
    GeomRegionInfoMapT gmap;

    vector<string> getChildren(string name, EGS_Input *inp);

    void getGeomRegs(Node, vector<GeomRegionInfo> &, int);
    void setGeomMap();

    Node build_tree(string root, vector<Node> &children);

public:


    string sim_geom_name;         /*!< name of the simulation geometry */
    string source_envelope_name;         /*!< name of the geometry that embeds the sources (required for superpostion mode) */
    vector<string> phantom_names; /*!< A list of all geometries to create scoring
                                    phantom objects for (arbitrary ordering */
    vector<string> source_names;  /*!< Names of geometries the user has specified as being source geoms */

    Node *geom_tree; /*!< a tree representing the parent/child relationships of the geometry */

    int ngeom;     ///< total number of geometries created
    int nreg_total; ///<  total number of regions in the simulation geometry


    GeomInfo() {};
    ~GeomInfo() {
        if (geom_tree) {
            delete geom_tree;
        }
    }

    vector<GeomRegionInfo> ordered_geom_data; /*!< All geometries in their constructed order */

    map<EGS_BaseGeometry *, GeomRegionInfo> geom_to_regioninfo; /*!< mapping from geometry to info about that geometry */

    vector<EB_Phantom *> phantom_geoms; ///< pointers to all of the phantom objects

    /*! \brief Initialize the GeomInfo structure (ginfo)
     * This function reads the geometry input block, ensures all
     * the required keys are present and pulls out information like
     * which geometries are phantoms and sources.
     *
     * Note since we can't do a deep copy of the input we have to take all
     * the geometry input items and then re-write them to the ginput
     */
    int initializeFromInput(EGS_Input *input);

    /*! \brief Setup all arrays required to decide which geometry/phantom a
     * region is in and whether or not we are scoring dose in it.

     * To setup the arrays we loop through all geometries, decide whether it is
     * a phantom or not, then loop through each region in the geometry and set
     * the local region number phantom index etc */
    void setGeometryIndexes(EGS_BaseGeometry *sim_geom);

    /*! \brief return the phantom index for this region number or -1 if region
     * is not in a phantom */
    int phantomFromRegion(int ir);

    /*! \brief convert global region number to (EGS_BaseGeometry, local_reg_num) pair */
    GeomRegT globalToLocal(int ir);

    /*! \brief convert global region number to local_reg_num */
    int globalToLocalReg(int ir);

    /* \brief convert an (EGS_BaseGeometry, local_reg_num) to global region number */
    int localToGlobal(GeomRegT);


    /* \brief return true if global reg is inside a phantom object */
    bool isPhantom(int ir);

    /* \brief return true if global reg is inside the first source (a.ka.
     * generator) object */
    bool isSource(int ir);

    /* \brief print information about geometries and their region number*/
    void printInfo();

};

#endif
