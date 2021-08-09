# #            seed name: I125-SelectSeed_130.002
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
        name = sph_end_cap_1 
        library = egs_spheres
        type = EGS_cSpheres
        midpoint = 0 0  -0.185
        radii = 0.04

        :start media input:
            media = Ti
            set medium = 0 0
        :stop media input:
    :stop geometry:

    :start geometry:
        name    = source_and_cladding
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -0.185   0 0 1

        :start layer:
            thickness    = 0.0147
            top radii    = 0.035 0.04
            bottom radii = 0.035 0.04
            media        = AIR_TG43 Ti
        :stop layer:

        :start layer:
            thickness    = 0.0003
            top radii    = 0.0258 0.035 0.04
            bottom radii = 0.0258 0.035 0.04
            media        = AgClAgI AIR_TG43 Ti
        :stop layer:

        :start layer:
           thickness    = 0.34
           top radii    = 0.0255 0.0258 0.035 0.04
           bottom radii = 0.0255 0.0258 0.035 0.04
           media        = Ag AgClAgI AIR_TG43 Ti
        :stop layer:

        :start layer:
            thickness    = 0.0003
            top radii    = 0.0258 0.035 0.04
            bottom radii = 0.0258 0.035 0.04
            media        = AgClAgI AIR_TG43 Ti
        :stop layer:

        :start layer:
            thickness    = 0.0147
            top radii    = 0.035 0.04
            bottom radii = 0.035 0.04
            media        = AIR_TG43 Ti
        :stop layer:

    :stop geometry:

    :start geometry:
        name = sph_end_cap_2
        library = egs_spheres
        type = EGS_cSpheres
        midpoint = 0 0 0.185
        radii = 0.04

        :start media input:
            media = Ti
            set medium = 0 0
        :stop media input:
    :stop geometry:

    :start geometry:
        name = the_seed
        library = egs_gunion
        geometries = source_and_cladding sph_end_cap_1 sph_end_cap_2
    :stop geometry:

    simulation geometry = the_seed
:stop geometry definition:
