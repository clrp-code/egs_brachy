# seed name: Ir192- microSelectron-v1
# air kerma: 1.17522E-13

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
#       Habib Safigholi, Marc J.P. Chamberland, Randle E.P. Taylor, Martin P. Martinov,
#       D. W. O. Rogers and Rowan M. Thomson,  Update of the CLRP Monte Carlo TG-43
#       parameter database for high-energy brachytherapy sources, Med. Phys. 50 (2023)
#       1928-1941
#
#

:start geometry definition:

    :start geometry:
        name = end_cap
        library = egs_spheres
        midpoint = 0 0 0.055
        type = EGS_cSpheres
        radii = 0.055

        :start media input:
            media = SS_AISI316L
        :stop media input:
    :stop geometry:

    :start geometry:
        name = air_gap
        library = egs_rz
        radii = 0.03
        z-planes = 0.05, 0.06

        :start media input:
            media = AIR_TG43
        :stop media input:
    :stop geometry:

    :start geometry:
        name = end_cap_with_air
        library = egs_genvelope
        base geometry = end_cap
        inscribed geometries = air_gap
    :stop geometry:

    :start geometry:
        name    = cladding_and_source
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -0.535 0 0 1

        :start layer:
            thickness = 0.475
            top radii = 0.055
            bottom radii = 0.055
            media = SS_AISI316L
        :stop layer:
        
        :start layer:
            thickness = 0.01
            top radii = 0.03 0.055
            bottom radii = 0.03 0.055
            media = AIR_TG43, SS_AISI316L
        :stop layer:

        :start layer:
            thickness = 0.1
            top radii = 0.025 0.03 0.055
            bottom radii = 0.025 0.03 0.055
            media = Ir, AIR_TG43, SS_AISI316L
        :stop layer:

        :start layer:
            thickness = 0.01
            top radii = 0.03 0.055
            bottom radii = 0.03 0.055
            media = AIR_TG43, SS_AISI316L
        :stop layer:
        
    :stop geometry:

    :start geometry:
        name = the_planes
        library = egs_planes
        type = EGS_Zplanes
        positions = -0.525 0.055 1.100001
    :stop geometry:

	:start geometry:
		name = the_source
		library = egs_cdgeometry
		base geometry = the_planes
		set geometry = 0 cladding_and_source
		set geometry = 1 end_cap_with_air
	:stop geometry:
	
    simulation geometry = the_source

:stop geometry definition:
