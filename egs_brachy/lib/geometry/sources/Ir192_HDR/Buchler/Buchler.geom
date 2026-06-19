# seed name: Ir192- Buchler
# air kerma: 1.09733E-13

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
        positions = -6.241 0.095 0.175001
    :stop geometry:

    :start geometry:
        name = end_cap
        library = egs_spheres
        midpoint = 0 0 0.095
        type = EGS_cSpheres
        radii = 0.08
        :start media input:
            media = SS_AISI321
        :stop media input:
    :stop geometry:

    :start geometry:
        name    = cladding_and_source
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -6.251 0 0 1    

        :start layer:
            thickness = 5.876
            top radii = 0.055
            bottom radii = 0.055
            media = SS_AISI301
        :stop layer:
        
        :start layer:
            thickness = 0.134
            top radii = 0.055 0.08
            bottom radii = 0.055 0.08
            media = SS_AISI301, SS_AISI321
        :stop layer:

        :start layer:
            thickness = 0.176
            top radii = 0.08
            bottom radii = 0.08
            media = SS_AISI321
        :stop layer:
 
        :start layer:
            thickness = 0.13
            top radii = 0.05 0.06 0.08
            bottom radii = 0.05 0.06 0.08
            media = Ir, AIR_TG43, SS_AISI321
        :stop layer:
                
        :start layer:
            thickness = 0.01
            top radii = 0.06 0.08
            bottom radii = 0.06 0.08
            media = AIR_TG43, SS_AISI321
        :stop layer:
 
        :start layer:
            thickness = 0.03
            top radii = 0.08
            bottom radii = 0.08
            media = SS_AISI321
        :stop layer:
              
    :stop geometry:
    
    :start geometry:
		name = the_source
		library = egs_cdgeometry
		base geometry = the_planes
		set geometry = 0 cladding_and_source
		set geometry = 1 end_cap
	:stop geometry:
	
    simulation geometry = the_source

:stop geometry definition:
