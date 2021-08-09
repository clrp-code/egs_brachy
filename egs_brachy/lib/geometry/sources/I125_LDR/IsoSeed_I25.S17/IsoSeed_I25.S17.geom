# #            seed name: I125-IsoSeed_I25.S17
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
        name = global_planes
        type = EGS_Zplanes
        positions = -0.225, -0.185, -0.1730, -0.1728, -0.1703, -0.17, 0.17, 0.1703, 0.1728, 0.1730, 0.185, 0.225
    :stop geometry:

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
        library = egs_cylinders
        name = cladding_only
        midpoint = 0 0 0
        type = EGS_ZCylinders

        radii = 0.035, 0.04

        :start media input:
            media = AIR_TG43, Ti
            set medium = 0 0
            set medium = 1 1
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_cylinders
        name = source_end_AgI_and_cladding
        midpoint = 0 0 0
        type = EGS_ZCylinders

        radii = 0.0280, 0.035, 0.04

        :start media input:
            media = AgI, AIR_TG43, Ti
            set medium = 0 0
            set medium = 1 1
            set medium = 2 2
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_cylinders
        name = source_end_Ag_AgI_and_cladding
        midpoint = 0 0 0
        type = EGS_ZCylinders

        radii = 0.0278, 0.0280, 0.035, 0.04

        :start media input:
            media = Ag, AgI, AIR_TG43, Ti
            set medium = 0 0
            set medium = 1 1
            set medium = 2 2
            set medium = 3 3
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_cylinders
        name = source_end_Ni_Ag_AgI_and_cladding
        midpoint = 0 0 0
        type = EGS_ZCylinders

        radii = 0.0253, 0.0278, 0.0280, 0.035, 0.04

        :start media input:
            media = Ni, Ag, AgI, AIR_TG43, Ti
            set medium = 0 0
            set medium = 1 1
            set medium = 2 2
            set medium = 3 3
            set medium = 4 4
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_cylinders
        name = source_center_Mo_Ni_Ag_AgI_and_cladding
        midpoint = 0 0 0
        type = EGS_ZCylinders

        radii = 0.025, 0.0253, 0.0278, 0.0280, 0.035, 0.04

        :start media input:
            media = Mo, Ni, Ag, AgI, AIR_TG43, Ti
            set medium = 0 0
            set medium = 1 1
            set medium = 2 2
            set medium = 3 3
            set medium = 4 4
            set medium = 5 5
        :stop media input:
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
        library = egs_cdgeometry
        name = seed
        base geometry = global_planes
        set geometry = 0 end_cap_1
        set geometry = 1 cladding_only
        set geometry = 2 source_end_AgI_and_cladding
        set geometry = 3 source_end_Ag_AgI_and_cladding
        set geometry = 4 source_end_Ni_Ag_AgI_and_cladding
        set geometry = 5 source_center_Mo_Ni_Ag_AgI_and_cladding
        set geometry = 6 source_end_Ni_Ag_AgI_and_cladding
        set geometry = 7 source_end_Ag_AgI_and_cladding
        set geometry = 8 source_end_AgI_and_cladding
        set geometry = 9 cladding_only
        set geometry = 10 end_cap_2
    :stop geometry:

    simulation geometry = seed

:stop geometry definition:
