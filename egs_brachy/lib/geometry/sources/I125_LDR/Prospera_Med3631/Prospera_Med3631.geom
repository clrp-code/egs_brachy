# #            seed name: I125-Prospera_Med3631
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
:start geometry definition:

    :start geometry:
        name = cladding_cyl
        library = egs_rz
        radii =  0.0355,0.0405
        z-planes = -0.1945, 0.1945
        :start media input:
            media = AIR_TG43, Ti
            set medium = 1 1
        :stop media input:
    :stop geometry: 

    :start geometry:
        library = egs_spheres
        name = base_end_sphere_outer
        type = EGS_cSpheres
        radii = 0.0405

        :start media input:
           media = WATER_0.998 Ti   
           set medium = 1 1 
		 # media =  Ti   
         # set medium = 1 
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = base_end_sphere_inner
        type = EGS_cSpheres
        radii = 0.0355

        :start media input:
            media = AIR_TG43
        :stop media input:
    :stop geometry:

    :start geometry:
        name = outer_end_1
        library = egs_gtransformed
        my geometry = base_end_sphere_outer
        :start transformation:
            translation = 0, 0, -0.1945
        :stop transformation:
    :stop geometry:

    :start geometry:
        name = outer_end_2
        library = egs_gtransformed
        my geometry = base_end_sphere_outer
        :start transformation:
            translation = 0, 0, 0.1945
        :stop transformation:
    :stop geometry:

    :start geometry:
        name = inner_end_1
        library = egs_gtransformed
        my geometry = base_end_sphere_inner
        :start transformation:
            translation = 0, 0, -0.1895
        :stop transformation:
    :stop geometry:

    :start geometry:
        name = inner_end_2
        library = egs_gtransformed
        my geometry = base_end_sphere_inner
        :start transformation:
            translation = 0, 0, 0.1895
        :stop transformation:
    :stop geometry:


    :start geometry:
        library = egs_spheres
        name = base_source
        type = EGS_cSpheres
        radii = 0.028

        :start media input:
            media = SdVBC24H24
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = base_marker
        type = EGS_cSpheres
        radii = 0.028

        :start media input:
            media = Au80Cu20
        :stop media input:
    :stop geometry:

    :start geometry:
        name = source_1
        library = egs_gtransformed
        my geometry = base_source
        :start transformation:
            translation = 0, 0, -0.1807
        :stop transformation:
    :stop geometry:

    :start geometry:
        name = source_2
        library = egs_gtransformed
        my geometry = base_source
        :start transformation:
            translation = 0, 0, -0.1084
        :stop transformation:
    :stop geometry:

    :start geometry:
        name = marker_1
        library = egs_gtransformed
        my geometry = base_marker
        :start transformation:
            translation = 0, 0, -0.0361
        :stop transformation:
    :stop geometry:

    :start geometry:
        name = marker_2
        library = egs_gtransformed
        my geometry = base_marker
        :start transformation:
            translation = 0, 0, 0.0361
        :stop transformation:
    :stop geometry:

    :start geometry:
        name = source_3
        library = egs_gtransformed
        my geometry = base_source
        :start transformation:
            translation = 0, 0, 0.1084
        :stop transformation:
    :stop geometry:

    :start geometry:
        name = source_4
        library = egs_gtransformed
        my geometry = base_source
        :start transformation:
            translation = 0, 0, 0.1807
        :stop transformation:
    :stop geometry:

    :start geometry:
        library = egs_gunion
        name = seed
        geometries = source_1 source_2 marker_1 marker_2 source_3 source_4 inner_end_1 inner_end_2 cladding_cyl outer_end_1 outer_end_2
    :stop geometry:

    simulation geometry = seed
	
	
:stop geometry definition:
