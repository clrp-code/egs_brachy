#               seed name: Pd103-BrachySeed_Pd-1
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
        midpoint = 0 0  -0.1786
        type = EGS_cSpheres
        radii = 0.0349

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
        name     = cylinders
        library  = egs_rz
        radii    = 0.019 0.0298 0.0349 0.04
        z-planes = -0.18 -0.17835 -0.155 -0.05945 0.05945 0.155 0.17835 0.18

        :start media input:
            media = Ti, AIR_TG43, Pt90Ir10
            set medium = 0 1 1
            set medium = 2 4 2
            set medium = 5 6 1
            set medium = 7 9 1
            set medium = 11 13 1
            set medium = 14 1
            set medium = 20 1
        :stop media input:

    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = source_1
        midpoint = 0 0  -0.18425
        type = EGS_cSpheres
        radii = 0.0275

        :start media input:
            media = AlSilicate
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = source_2
        midpoint = 0 0  0.18425
        type = EGS_cSpheres
        radii = 0.0275

        :start media input:
            media = AlSilicate
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_gunion
        name = seed
        geometries = source_1 source_2 cylinders sph_end_cap_1 sph_end_cap_2

    :stop geometry:

    simulation geometry = seed

:stop geometry definition:
