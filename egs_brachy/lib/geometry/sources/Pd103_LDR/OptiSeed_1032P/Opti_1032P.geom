#               seed name: Pd103-Opti_1032P
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
 # It assumed the cups in the end cups are filled with air

:start geometry definition:

    :start geometry:
        library = egs_planes
        name = planes
        type = EGS_Zplanes
        positions = -0.25 0.25
    :stop geometry:

    :start geometry:
        library = egs_rz 
        name = cylinders
        radii = 0.02   0.0225 0.040 
		      # 0.0225 0.025  0.0405
        z-planes = -0.25 -0.19 -0.12 -0.1 0.1 0.12 0.19 0.25

        :start media input:
            media = C85.7H14.3 PolySty AIR_TG43 Au
            set medium = 1 1
            set medium = 2 2
            set medium = 3 3
            set medium = 4 2
            set medium = 5 1
            set medium = 10 3
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = end_sphere_1
        type = EGS_cSpheres
        radii = 0.02
        midpoint = 0 0  -0.235

        :start media input:
            media = AIR_TG43   # AIR IN THE ENDCAP HOLE FOR AIR KERMA STRENGHT AND DOSE RATE CONSTANT CACULATIONS
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = end_sphere_2
        type = EGS_cSpheres
        radii = 0.02
        midpoint = 0 0  0.235

        :start media input:
            media = AIR_TG43   # AIR IN THE ENDCAP HOLE FOR AIR KERMA STRENGHT AND DOSE RATE CONSTANT CACULATIONS
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_gunion
        name = seed0
        geometries = end_sphere_1 end_sphere_2 cylinders
    :stop geometry:

    :start geometry:
        library = egs_cdgeometry
        name = Final
		# seed_trimmed
        base geometry = planes
        set geometry = 0 seed0
    :stop geometry:

    simulation geometry = Final
	# seed_trimmed

:stop geometry definition:
