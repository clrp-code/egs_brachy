#               seed name: Pd103- PharmaSeed_BT-103-3
#
#  Copyright (C) 2020, Rowan M Thomson and D.W.O. Rogers
#
# This file is part of the egs_brachy package to be used with EGSnrc
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
# If you use this file, please cite
#       M Chamberland, R E P Taylor, D W O Rogers and R M Thomson
#       egs_brachy: a versatile and fast Monte Carlo code for brachytherapy
#       Phys Med Biol 61(2016) 8214 -- 8231
# and
#       Habib Safigholi, Marc J.P. Chamberland, Randle E.P. Taylor,
#       Christian H. Allen, Martin P. Martinov, D. W. O. Rogers and Rowan
#       M. Thomson,  Update of the CLRP TG-43 parameter database for
#       low-energy brachytherapy sources, Med. Phys. 47 (2020) 4656-4669
#
#
:start geometry definition:

    :start geometry:
        library = egs_spheres
        name = end_sphere_1
        type = EGS_cSpheres
        radii = 0.04
        midpoint = 0 0 -0.185

        :start media input:
            media = Ti
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = end_sphere_2
        type = EGS_cSpheres
        radii = 0.04
        midpoint = 0 0 0.185

        :start media input:
            media = Ti
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = end_inner_sphere_1
        type = EGS_cSpheres
        radii = 0.035
        midpoint = 0 0 -0.166
        :start media input:
            media = AIR_TG43
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = end_inner_sphere_2
        type = EGS_cSpheres
        radii = 0.035
        midpoint = 0 0 0.166

        :start media input:
            media = AIR_TG43
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = source_1
        type = EGS_cSpheres
        radii = 0.0275 0.02755
        midpoint = 0 0 -0.16

        :start media input:
            media = PolySty Pd
            set medium = 1 1
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = source_2
        type = EGS_cSpheres
        radii = 0.0275 0.02755
        midpoint = 0 0 -0.095

        :start media input:
            media = PolySty Pd
            set medium = 1 1
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = source_3
        type = EGS_cSpheres
        radii = 0.0275 0.02755
        midpoint = 0 0 0.095

        :start media input:
            media = PolySty Pd
            set medium = 1 1
        :stop media input:
    :stop geometry:


    :start geometry:
        library = egs_spheres
        name = source_4
        type = EGS_cSpheres
        radii = 0.0275 0.02755
        midpoint = 0 0 0.16

        :start media input:
            media = PolySty Pd
            set medium = 1 1
        :stop media input:
    :stop geometry:
  
    :start geometry:
        library = egs_rz
        name = cylinders
        radii = 0.025 0.035 0.040
        z-planes = -0.185 -0.166 -0.055 0.055 0.166 0.185

        :start media input:
            media = Ti AIR_TG43 Au
            set medium = 0 1 1
            set medium = 2 2
            set medium = 3 4 1
            set medium = 5 9 1
        :stop media input:
    :stop geometry:

    :start geometry:
        name = end_cyl
        library = egs_rz
        radii = 0.04
        z-planes = -0.185, 0.185
        :start media input:
           media = Ti
        :stop media input:
    :stop geometry:

    :start geometry:
        name = outer_end1
        library = egs_gunion
        geometries = source_1 end_inner_sphere_1 end_cyl end_sphere_1
    :stop geometry:

    :start geometry:
        name = inner_end1
        library = egs_gunion
        geometries = source_1 end_inner_sphere_1 end_cyl
    :stop geometry:

    :start geometry:
        name = centre
        library = egs_gunion
        geometries = source_1 source_2 source_3 source_4 cylinders
    :stop geometry:
    
    :start geometry:
        name = inner_end2
        library = egs_gunion
        geometries = source_4 end_inner_sphere_2 end_cyl
    :stop geometry:
   
    :start geometry:
        name = outer_end2
        library = egs_gunion
        geometries = source_4 end_inner_sphere_2 end_cyl end_sphere_2
    :stop geometry:

    :start geometry:
        name = the_planes
        library = egs_planes
        type = EGS_Zplanes
        positions = -0.225001 -0.185 -0.166 0.166 0.185 0.225001
    :stop geometry:

	:start geometry:
		name = the_seed
		library = egs_cdgeometry
		base geometry = the_planes
		set geometry = 0 outer_end1
		set geometry = 1 inner_end1
		set geometry = 2 centre
		set geometry = 3 inner_end2
		set geometry = 4 outer_end2		
	:stop geometry:
	
    simulation geometry = the_seed

:stop geometry definition:

:start view control:
    xmin = -0.5
    xmax = 0.5
    ymin = -0.5
    ymax = 0.5
    zmin = -1
    zmax = 1
:stop view control:
