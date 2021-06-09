#            seed name: I125-Best2301
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

# First, let's define the source in the centre of the capsule
    :start geometry:
        library = egs_planes
        name = source_planes
        type = EGS_Zplanes
        positions = -0.1975 -0.175 0.175 0.1975
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = source_end_cap_1
        midpoint = 0 0 -0.175
        type = EGS_cSpheres
        radii = 0.0125 0.0225

        :start media input:
            media = W PolySty
            set medium = 0 0
            set medium = 1 1
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = source_end_cap_2
        midpoint = 0 0 0.175
        type = EGS_cSpheres
        radii = 0.0125 0.0225

        :start media input:
            media = W PolySty
            set medium = 0 0
            set medium = 1 1
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_cylinders
        name = source_cyl
        midpoint = 0 0 0
        type = EGS_ZCylinders
        radii = 0.0125 0.0225

        :start media input:
            media = W PolySty
            set medium = 0 0
            set medium = 1 1
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_cdgeometry
        name = source
        base geometry = source_planes
        set geometry = 0 source_end_cap_1
        set geometry = 1 source_cyl
        set geometry = 2 source_end_cap_2
    :stop geometry:

# Now let's do the cylindrical cladding
    :start geometry:
        library = egs_cylinders
        name = cladding
        midpoint = 0 0 0
        type = EGS_ZCylinders
        radii = 0.032 0.04

        :start media input:
            media = AIR_TG43 Ti
            set medium = 0 0
            set medium = 1 1
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_genvelope
        name = cladding_and_source
        base geometry = cladding
        inscribed geometries = source
    :stop geometry:

# Now let's take care of the ends of the capsule and put everything together
    :start geometry:
        library = egs_planes
        name = planes
        type = EGS_Zplanes
        positions = -0.25 -0.21 0.21 0.25
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = sph_end_cap_1
        midpoint = 0 0  -0.21
        type = EGS_cSpheres
        radii = 0.032 0.040

        :start media input:
            media = AIR_TG43 Ti
            set medium = 0 0
            set medium = 1 1
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = sph_end_cap_2
        midpoint = 0 0  0.21
        type = EGS_cSpheres
        radii = 0.032 0.040

        :start media input:
            media = AIR_TG43 Ti
            set medium = 0 0
            set medium = 1 1
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_cdgeometry
        name = seed
        base geometry = planes
        set geometry = 0 sph_end_cap_1
        set geometry = 1 cladding_and_source
        set geometry = 2 sph_end_cap_2
    :stop geometry:

    simulation geometry = seed
:stop geometry definition:
