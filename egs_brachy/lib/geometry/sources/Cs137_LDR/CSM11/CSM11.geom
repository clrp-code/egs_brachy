# seed name: Cs137- CSM11
# air kerma: 2.2523E-13

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
        name = the_planes
        library = egs_planes
        type = EGS_Zplanes
        positions = -0.21001 0.1825 0.2275 0.31001
    :stop geometry:

    :start geometry:
        name        = end_cap
        library     = egs_spheres
        radii       = 0.0825
        midpoint    = 0 0 0.2275
        :start media input:
            media = SS_AISI316L_p7.8
        :stop media input:
    :stop geometry:
    
    :start geometry:
        name        = air_sphere
        library     = egs_spheres
        radii       = 0.0425
        midpoint    = 0 0 0.1825
        :start media input:
            media = AIR_TG43
        :stop media input:
    :stop geometry:

    :start geometry:
        name    = cladding_and_source
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -0.22 0 0 1

        :start layer:
            thickness = 0.06
            top radii = 0.0825
            bottom radii = 0.0825
            media = SS_AISI316L_p7.8
        :stop layer:
        
        :start layer:
            thickness = 0.32
            top radii = 0.0425 0.0825
            bottom radii = 0.0425 0.0825
            media = Pollucite SS_AISI316L_p7.8
        :stop layer:
        
        :start layer:
            thickness = 0.0225
            bottom radii = 0.0425 0.0825
            media = AIR_TG43 SS_AISI316L_p7.8
        :stop layer:

        :start layer:
            thickness = 0.145
            top radii = 0.0825
            bottom radii = 0.0825
            media = SS_AISI316L_p7.8
        :stop layer:
       
    :stop geometry:

    :start geometry:
        name = air_sphere_and_cladding
        library = egs_genvelope
        base geometry = cladding_and_source
        inscribed geometries = air_sphere
    :stop geometry:

	:start geometry:
		name = the_seed
		library = egs_cdgeometry
		base geometry = the_planes
		set geometry = 0 cladding_and_source
		set geometry = 1 air_sphere_and_cladding
		set geometry = 2 end_cap
	:stop geometry:
	
    simulation geometry = the_seed
    
:stop geometry definition: