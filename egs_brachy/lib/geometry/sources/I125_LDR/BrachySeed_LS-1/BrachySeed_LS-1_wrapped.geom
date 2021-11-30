#            seed name: I125-BrachySeed_LS-1
# #          air kerma: 5.02309E-14
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
        library = egs_planes
        name = planes
        type = EGS_Zplanes
        positions = -0.22 -0.18 0.18 0.22
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = sph_end_cap_out
        midpoint = 0 0  -0.18
        type = EGS_cSpheres
        radii = 0.04

        :start media input:
            media = Ti
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = sph_end_cap_in
        midpoint = 0 0  -0.1785
        type = EGS_cSpheres
        radii = 0.035

        :start media input:
            media = AIR_TG43
        :stop media input:
    :stop geometry:


    :start geometry:
        library = egs_gunion
        name = sph_end_cap_1
        geometries = sph_end_cap_in sph_end_cap_out
    :stop geometry:

    :start geometry:
        name = sph_end_cap_2
        library = egs_gtransformed
        my geometry = sph_end_cap_1
        :start transformation:
            rotation = 0 3.14159265
        :stop transformation:
    :stop geometry:

    :start geometry:
        name    = cylinders
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -0.18   0 0 1

        :start layer:
            thickness    = 0.03
            top radii    = 0.03 0.04
            bottom radii = 0.03 0.04
            media        = AIR_TG43 Ti
        :stop layer:

   
		 :start layer:
            thickness    = 0.09
            top radii    = 0.019 0.03 0.04
            bottom radii = 0.019 0.03 0.04
            media        = Pt90Ir10 AIR_TG43 Ti
         :stop layer:

 
	    :start layer:
            thickness    = 0.12
            top radii    = 0.0190 0.0195 0.04
            bottom radii = 0.0190 0.0195 0.04
            media        = Pt90Ir10 AIR_TG43 Ti
        :stop layer:

        
	    :start layer:
            thickness    = 0.09
            top radii    = 0.019 0.03 0.04
            bottom radii = 0.019 0.03 0.04
            media        = Pt90Ir10 AIR_TG43 Ti
        :stop layer:

        :start layer:
            thickness    = 0.03
            top radii    = 0.03 0.04
            bottom radii = 0.03 0.04
            media        = AIR_TG43 Ti
        :stop layer:

    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = source_1
        midpoint = 0 0  -0.18
        type = EGS_cSpheres
        radii = 0.025

        :start media input:
            media = AlSilicate
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = source_2
        midpoint = 0 0  0.18
        type = EGS_cSpheres
        radii = 0.025

        :start media input:
            media = AlSilicate
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_gunion
        name = seed
        geometries = source_1 source_2 cylinders sph_end_cap_1 sph_end_cap_2
    :stop geometry:
	
    :start geometry:
		library = egs_rz
		name = wrapper
		radii = 0.0401
		z-planes = -0.2201 0.2201
		:start media input:
			media = WATER_0.998
		:stop media input:
    :stop geometry:

    :start geometry:
        library = egs_genvelope
        name = seed
        base geometry = wrapper
        inscribed geometries = seed_unwrapped
    :stop geometry:

    simulation geometry = seed

:stop geometry definition:
