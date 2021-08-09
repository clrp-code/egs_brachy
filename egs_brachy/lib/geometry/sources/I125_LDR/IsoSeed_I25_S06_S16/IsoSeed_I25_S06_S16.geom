# #            seed name: I125-IsoSeed_I25_S06_S16
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
        library = egs_spheres
        name = end_cap_1
        midpoint = 0 0  -0.185
        type = EGS_cSpheres
        radii = 0.04

        :start media input:
            media = Ti
            set medium = 0 0
        :stop media input:
    :stop geometry:
	
	:start geometry:

        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 0.185 0 0 -1
        name    = capsule_and_source
		
        :start layer:
			thickness    = 0.01
            top radii    = 0.035 0.04
            bottom radii = 0.035 0.04
            media        = AIR_TG43 Ti
        :stop layer:
		
		:start layer:
            thickness    = 0.35
            top radii    = 0.0085 0.011 0.03 0.035 0.04
            bottom radii = 0.0085 0.011 0.03 0.035 0.04
 
	         media        = Au AIR_TG43 Al2O3 AIR_TG43 Ti
        :stop layer:
		
		:start layer:
			thickness    = 0.01
            top radii    = 0.035 0.04
            bottom radii = 0.035 0.04
            media        = AIR_TG43 Ti
        :stop layer:
		
	 :stop geometry:	
	 
	 :start geometry:
        library = egs_spheres
        name = end_cap_2
        midpoint = 0 0  0.185
        type = EGS_cSpheres
        radii = 0.04
		
		:start media input:
            media = Ti
            set medium = 0 0
        :stop media input:
    :stop geometry:
	
	:start geometry:
        library = egs_gunion
        name = seed
        geometries = capsule_and_source end_cap_1 end_cap_2
    :stop geometry:

    simulation geometry = seed

:stop geometry definition:
	
	
	
		
		