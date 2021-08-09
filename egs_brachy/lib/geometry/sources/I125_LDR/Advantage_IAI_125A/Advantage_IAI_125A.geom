#           I125-Advantage_IAI_125A
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
	     positions = -0.225 -0.185 -0.17 0.17 0.185 0.225
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
        radii = 0.040
        midpoint = 0 0  -0.185

        :start media input:
            media = Ti
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = end_sphere_inner
        type = EGS_cSpheres
		 radii = 0.035
		 midpoint = 0 0  -0.1650

        :start media input:
            media = AIR_TG43, Ti
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = end_sphere_inner2
        type = EGS_cSpheres
		 radii = 0.035
		 midpoint = 0 0  -0.1650

        :start media input:
            media = AIR_TG43, Ti
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_gunion
        name = overlap_sph
        geometries = end_sphere_inner end_sphere1
    :stop geometry:

    :start geometry:
        library = egs_gunion
        name = overlap_tube
        geometries = end_sphere_inner2 Ti_tube
    :stop geometry:

    :start geometry:
        name = overlap_sph2
        library = egs_gtransformed
        my geometry = overlap_sph
        :start transformation:
            rotation = 0 3.14159265
        :stop transformation:
    :stop geometry:

    :start geometry:
        name = overlap_tube2
        library = egs_gtransformed
        my geometry = overlap_tube
        :start transformation:
            rotation = 0 3.14159265
        :stop transformation:
    :stop geometry:

    :start geometry:
        library = egs_cdgeometry
        name = cladding
        base geometry = planes
        set geometry = 0 overlap_sph
        set geometry = 1 overlap_tube
        set geometry = 2 tube
        set geometry = 3 overlap_tube2
        set geometry = 4 overlap_sph2
    :stop geometry:

    :start geometry:
        library = egs_rz
        name = source
        radii = 0.025 0.0251
        z-planes = -0.1501 -0.15 0.15 0.1501

        :start media input:
            media = AgI_6.003 Ag
            set medium = 1 1
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_genvelope
        name = seed
        base geometry = cladding
        inscribed geometries = source
    :stop geometry:

    simulation geometry = seed

:stop geometry definition:
