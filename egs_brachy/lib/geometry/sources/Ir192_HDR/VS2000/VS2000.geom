#              Ir192, Varian, VariSource, VS2000
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
#       low-energy brachytherapy sources, Med. Phys. 47 (2020)
#
#
:start geometry definition:

    :start geometry:
        library = egs_planes
        name = planes
        type = EGS_Zplanes
        positions = -5. 0.3205 0.35
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = end_cap
        midpoint = 0 0 0.3205
        type = EGS_cSpheres
        radii = 0.0295

        :start media input:
            media = Ti44.4Ni55.6
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_cylinders
        name = cyl_cable
        midpoint = 0 0 0
        type = EGS_ZCylinders
        radii = 0.0295

        :start media input:
            media = Ti44.4Ni55.6
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_cdgeometry
        name = cable
        base geometry = planes
        set geometry = 0 cyl_cable
        set geometry = 1 end_cap
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = source_end_1
        midpoint = 0 0 0.108
        type = EGS_cSpheres
        radii = 0.017

        :start media input:
            media = Ir
        :stop media input:
    :stop geometry:

    :start geometry:
        name = source_cyl
        library = egs_rz
        radii =  0.017
        z-planes = -0.108 0.108
        :start media input:
            media = Ir
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = source_end_2
        midpoint = 0 0 -0.108
        type = EGS_cSpheres
        radii = 0.017

        :start media input:
            media = Ir
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_gunion
        name = base_source
        geometries = source_cyl source_end_1 source_end_2
    :stop geometry:

    :start geometry:
        name = source_1
        library = egs_gtransformed
        my geometry = base_source
        :start transformation:
            translation = 0 0 0.125
        :stop transformation:
    :stop geometry:

    :start geometry:
        name = source_2
        library = egs_gtransformed
        my geometry = base_source
        :start transformation:
            translation = 0 0 -0.125
        :stop transformation:
    :stop geometry:

    :start geometry:
        library =  egs_genvelope
        name = source
        base geometry = cable
        inscribed geometries = source_1 source_2
    :stop geometry:

    simulation geometry = source

:stop geometry definition:
