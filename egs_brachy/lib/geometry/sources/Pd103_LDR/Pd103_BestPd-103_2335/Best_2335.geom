#               seed name: Pd103-Best_2335
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

# Let's assemble the end caps of the seed first
    :start geometry:
        library = egs_spheres
        name = base_end_cap_sphere
        midpoint = 0 0 0
        type = EGS_cSpheres
        radii = 0.028 0.032 0.040

        :start media input:
            media = Poly_Best2335_0.5gpcm AIR_TG43 Ti
            set medium = 0 0
            set medium = 1 1
            set medium = 2 2
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_gtransformed
        name = end_cap_sph_1
        my geometry = base_end_cap_sphere
        :start transformation:
            translation = 0 0 -0.21
        :stop transformation:
    :stop geometry:

    :start geometry:
        library = egs_gtransformed
        name = end_cap_sph_2
        my geometry = base_end_cap_sphere
        :start transformation:
            translation = 0 0 0.21
        :stop transformation:
    :stop geometry:

# Let's make the Tungsten marker
    :start geometry:
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -0.06   0 0 1
        name    = marker

        :start layer:
            thickness    = 0.12
            top radii    = 0.025
            bottom radii = 0.025
            media        = W
        :stop layer:
    :stop geometry:

# Let's make the sources
    :start geometry:
        library = egs_spheres
        name = base_source_sphere
        midpoint = 0 0 0
        type = EGS_cSpheres
        radii = 0.028

        :start media input:
            media = Poly_Best2335_0.5gpcm
            set medium = 0 0
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_gtransformed
        name = source_1
        my geometry = base_source_sphere
        :start transformation:
            translation = 0 0 -0.21
        :stop transformation:
    :stop geometry:

    :start geometry:
        library = egs_gtransformed
        name = source_2
        my geometry = base_source_sphere
        :start transformation:
            translation = 0 0 -0.15
        :stop transformation:
    :stop geometry:

    :start geometry:
        library = egs_gtransformed
        name = source_3
        my geometry = base_source_sphere
        :start transformation:
            translation = 0 0 -0.09
        :stop transformation:
    :stop geometry:

    :start geometry:
        library = egs_gtransformed
        name = source_4
        my geometry = base_source_sphere
        :start transformation:
            translation = 0 0 0.09
        :stop transformation:
    :stop geometry:

    :start geometry:
        library = egs_gtransformed
        name = source_5
        my geometry = base_source_sphere
        :start transformation:
            translation = 0 0 0.15
        :stop transformation:
    :stop geometry:

    :start geometry:
        library = egs_gtransformed
        name = source_6
        my geometry = base_source_sphere
        :start transformation:
            translation = 0 0 0.21
        :stop transformation:
    :stop geometry:

# Now let's do the cylindrical cladding and envelope with the sources and marker
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
        library = egs_gunion
        name = cladding_sources_and_marker
        geometries = source_1 source_2 source_3 source_4 source_5 source_6 marker cladding
    :stop geometry:

# Let's define our planes for the CDGeometry and put it all together
    :start geometry:
        library = egs_planes
        name = planes
        type = EGS_Zplanes
        positions = -0.25 -0.21 0.21 0.25
    :stop geometry:

    :start geometry:
        library = egs_cdgeometry
        name = seed
        base geometry = planes
        set geometry = 0 end_cap_sph_1
        set geometry = 1 cladding_sources_and_marker
        set geometry = 2 end_cap_sph_2
    :stop geometry:

    simulation geometry = seed
:stop geometry definition:
