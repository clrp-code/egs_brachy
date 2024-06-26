#               seed name: Pd103-Advantage_IAPd-103A
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
        positions = -0.225 -0.1908442888 -0.1828290902 0.1828290902 0.1908442888 0.225
    :stop geometry:

    :start geometry:
        library = egs_cylinders
        name = tube
        type = EGS_ZCylinders
        radii = 0.035 0.04

        :start media input:
            media = AIR_TG43, Ti
            set medium= 1 1
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_cylinders
        name = Ti_tube
        type = EGS_ZCylinders
        radii = 0.04

        :start media input:
            media = Ti
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = end_sphere1
        type = EGS_cSpheres
        radii = 0.0405
        midpoint = 0 0  -0.1845

        :start media input:
            media = Ti
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = end_sphere2
        type = EGS_cSpheres
        radii = 0.0405
        midpoint = 0 0  0.1845

        :start media input:
            media = Ti
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = end_sphere_inner
        type = EGS_cSpheres
        radii = 0.089
        midpoint = 0 0  -0.101

        :start media input:
            media = AIR_TG43
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_gunion
        name = overlap1
        geometries = end_sphere_inner Ti_tube
    :stop geometry:

    :start geometry:
        name = overlap2
        library = egs_gtransformed
        my geometry = overlap1
        :start transformation:
            rotation = 0 3.14159265
        :stop transformation:
    :stop geometry:

    :start geometry:
        library = egs_cdgeometry
        name = cladding
        base geometry = planes
        set geometry = 0 end_sphere1
        set geometry = 1 overlap1
        set geometry = 2 tube
        set geometry = 3 overlap2
        set geometry = 4 end_sphere2
    :stop geometry:

    :start geometry:
        name = marker
        library = egs_rz
        radii =  0.025
        z-planes = -0.0625 0.0625
        :start media input:
            media = Ag
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = source1
        midpoint = 0 0  -0.1559
        type = EGS_cSpheres
        radii = 0.025

        :start media input:
            media = ADV_PD_POLY
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = source2
        midpoint = 0 0 -0.0968
        type = EGS_cSpheres
        radii = 0.025

        :start media input:
            media = ADV_PD_POLY
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = source3
        midpoint = 0 0 0.0968
        type = EGS_cSpheres
        radii = 0.025

        :start media input:
            media = ADV_PD_POLY
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = source4
        midpoint = 0 0 0.1559
        type = EGS_cSpheres
        radii = 0.025

        :start media input:
            media = ADV_PD_POLY
        :stop media input:
    :stop geometry:


    :start geometry:
        library = egs_genvelope
        name = seed
        base geometry = cladding
        inscribed geometries = marker source1 source2 source3 source4
    :stop geometry:


    simulation geometry = seed

:stop geometry definition:

:start view control:
    xmin = -0.5
    xmax = 0.5
    ymin = -0.5
    ymax = 0.5
    zmin = -1
    zmax = 1
:stop view control:
