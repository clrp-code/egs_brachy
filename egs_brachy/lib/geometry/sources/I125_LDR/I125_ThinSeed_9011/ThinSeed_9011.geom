# #            seed name: I125-ThinSeed_9011
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
        name = sph_end_cap_1
        midpoint = 0 0  -0.202
        type = EGS_cSpheres
        radii = 0.0255

        :start media input:
            media = Ti
            set medium = 0 0
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = sph_end_cap_2
        midpoint = 0 0  0.202
        type = EGS_cSpheres
        radii = 0.0255

        :start media input:
            media = Ti
            set medium = 0 0
        :stop media input:
    :stop geometry:

    :start geometry:

        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -0.202   0 0 1
        name    = cones

        # end cap cylinder
        :start layer: 
		     thickness    = 0.0020    # From Kennedy et al, Med. Phys., 37 , 1681 - 1688, 2010 
            top radii    = 0.0255
            bottom radii = 0.0255
            media        = Ti
        :stop layer:

        # air gap
        :start layer:         
			thickness    = 0.059825       # From Kennedy et al, Med. Phys., 37 , 1681 - 1688, 2010 
            top radii    = 0.0198 0.0255
            bottom radii = 0.0198 0.0255
            media        = Ar Ti
        :stop layer:

        # source end cylinder
        :start layer:
            thickness    = 0.000175
            top radii    = 0.01025 0.0198 0.0255
            bottom radii = 0.010425 0.0198 0.0255
            media        = AgBrAgI_6.20 Ar Ti
        :stop layer:

        # source end cone
        :start layer:
           thickness    = 0.005                             # From Kennedy et al, Med. Phys., 37 , 1681 - 1688, 2010 
           top radii    = 0.01025 0.010425 0.0198 0.0255
           bottom radii = 0.01525 0.015425 0.0198 0.0255
           media        = Ag AgBrAgI_6.20 Ar Ti
        :stop layer:

        # mid source
        :start layer:
            thickness    = 0.27
            top radii    = 0.01525 0.015425 0.0198 0.0255
            bottom radii = 0.01525 0.015425 0.0198 0.0255
            media        =  Ag AgBrAgI_6.20 Ar Ti
        :stop layer:

        # source end cone
        :start layer:
           thickness    = 0.005                          # From Kennedy et al, Med. Phys., 37 , 1681 - 1688, 2010 
           top radii    = 0.01525 0.015425 0.0198 0.0255
           bottom radii = 0.01025 0.010425 0.0198 0.0255
           media        = Ag AgBrAgI_6.20 Ar Ti
        :stop layer:

        # source end cylinder
        :start layer:
            thickness    = 0.000175
            top radii    = 0.010425 0.0198 0.0255
            bottom radii = 0.01025 0.0198 0.0255
            media        = AgBrAgI_6.20 Ar Ti
        :stop layer:

        # air gap
        :start layer:
           # thickness    = 0.061325
		    thickness    = 0.059825
            top radii    = 0.0198 0.0255
            bottom radii = 0.0198 0.0255
            media        = Ar Ti
        :stop layer:

        # end cap cylinder
        :start layer:
		 #  thickness    = 0.000175
            thickness    = 0.0020
            top radii    = 0.0255
            bottom radii = 0.0255
            media        = Ti
        :stop layer:

    :stop geometry:

    :start geometry:
        library = egs_gunion
        name = seed
        geometries = cones sph_end_cap_1 sph_end_cap_2
    :stop geometry:

    simulation geometry = seed

:stop geometry definition:
