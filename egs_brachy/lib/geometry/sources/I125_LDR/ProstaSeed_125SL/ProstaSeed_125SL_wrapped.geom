# #            seed name: I125-ProstaSeed_125SL
# #            air kerma: 4.35671E-14
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
        name = end_sphere_1
        type = EGS_cSpheres
        radii = 0.04
        midpoint = 0 0 -0.185

        :start media input:
            media = Ti
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = end_sphere_2
        type = EGS_cSpheres
        radii = 0.04
        midpoint = 0 0 0.185

        :start media input:
            media = Ti
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = end_inner_sphere_1
        type = EGS_cSpheres
        radii = 0.035
        midpoint = 0 0 -0.16
        :start media input:
            media = AIR_TG43
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = end_inner_sphere_2
        type = EGS_cSpheres
        radii = 0.035
        midpoint = 0 0 0.16

        :start media input:
            media = AIR_TG43
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = source_sphere_mid
        type = EGS_cSpheres
        radii = 0.025

        :start media input:
            media = Ag
        :stop media input:

    :stop geometry:

    :start geometry:
        name = source_sphere_1
        library = egs_gtransformed
        my geometry = source_sphere_mid
        :start transformation:
            translation = 0, 0, -0.12
        :stop transformation:
    :stop geometry:

    :start geometry:
        name = source_sphere_2
        library = egs_gtransformed
        my geometry = source_sphere_mid
        :start transformation:
            translation = 0, 0, -0.06
        :stop transformation:
    :stop geometry:

    :start geometry:
        name = source_sphere_4
        library = egs_gtransformed
        my geometry = source_sphere_mid
        :start transformation:
            translation = 0, 0, 0.06
        :stop transformation:
    :stop geometry:

    :start geometry:
        name = source_sphere_5
        library = egs_gtransformed
        my geometry = source_sphere_mid
        :start transformation:
            translation = 0, 0, 0.12
        :stop transformation:
    :stop geometry:

    :start geometry:
        library = egs_rz
        name = cylinders
        radii = 0.035 0.04
        z-planes = -0.185 -0.16 0.16 0.185

        :start media input:
            media = Ti, AIR_TG43
            set medium = 1 1
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_gunion
        name = seed_unwrapped
        geometries = source_sphere_1 source_sphere_2 source_sphere_mid source_sphere_4 source_sphere_5 end_inner_sphere_1 end_inner_sphere_2 cylinders end_sphere_1 end_sphere_2
    :stop geometry:
	
    :start geometry:
		library = egs_rz
		name = wrapper
		radii = 0.0401
		z-planes = -0.1851 0.1851
		:start media input:
			media = WATER_0.998
		:stop media input:
    :stop geometry:

    :start geometry:
        library = egs_genvelope
        name = seed
        base geometry = wrapper
        inscribed geometries = seed_unwrapped
    :stop geometry:

    simulation geometry = seed
	
:stop geometry definition:
