#               seed name: Pd103-CT50 CivaThin
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
        name = string
        library = egs_rz
        radii = 0.03
        z-planes = -2.545, 2.545
        :start media input:
           media = CS10_Organic_polymer
        :stop media input:
    :stop geometry:

    :start geometry:
        name = base_marker
        library = egs_rz
        radii = 0.0125
        z-planes = -0.04, 0.04
        :start media input:
           media = Au
        :stop media input:
    :stop geometry:

    :start geometry:
        name        = base_well
        library     = egs_prism
        type        = EGS_PrismX
        points      = 0.0125  -0.105   0.0125  0.105   -0.0125  0.097   -0.0125  -0.097
		            # 0.01232 -0.1047  0.01232 0.1047  -0.01232 0.0965  -0.01232 -0.0965 
					
        closed      = 0. 0.028   # width
        :start media input:
            media = CS10_Organpoly_Pd5% # Each Pd region is filled with 5% natural pd and 95% organic polymer; {rho = 1.15, elements = H, C, N, O, PD ; mass fractions = 0.08075, 0.6156, 0.0475, 0.20615, 0.05} respectively 
       	   :stop media input:
		   
    :stop geometry:

    :start geometry:
        name = marker
        library = egs_gtransformed
        my geometry = base_marker
        :start transformation:
            translation = 0., 0.0055, 0.
        :stop transformation:
    :stop geometry:
	
	:start geometry:
        name = marker1
        library = egs_gtransformed
        my geometry = base_marker
        :start transformation:
            translation = 0., 0.0055, 1.0
        :stop transformation:
    :stop geometry:
	
	:start geometry:
        name = marker2
        library = egs_gtransformed
        my geometry = base_marker
        :start transformation:
            translation = 0., 0.0055, -1.0 
        :stop transformation:
    :stop geometry:
	
	:start geometry:
        name = marker3
        library = egs_gtransformed
        my geometry = base_marker
        :start transformation:
            translation = 0., 0.0055, 2.0
        :stop transformation:
    :stop geometry:
	
	:start geometry:
        name = marker4
        library = egs_gtransformed
        my geometry = base_marker
        :start transformation:
            translation = 0., 0.0055, -2.0 
        :stop transformation:
    :stop geometry:
	
    
    :start geometry:
        name = well1
        library = egs_gtransformed
        my geometry = base_well
        :start transformation:
            translation = -0.014 , 0.0035, -0.389
			            # -0.014 , 0.0035, -0.3885    # half of the width should be (-0.014), 0.0035 down, and 0.3885 offset from source center.  
        :stop transformation:
    :stop geometry:

    :start geometry:
        name = well2
        library = egs_gtransformed
        my geometry = base_well
        :start transformation:
            translation = -0.014, 0.0035, -0.167    # Reed Dimension 2014 Brachytherapy
			            # -0.014, 0.0035, -0.1665   # Rivard Dimension 2014 med phys
        :stop transformation:
    :stop geometry:

    :start geometry:
        name = well3
        library = egs_gtransformed
        my geometry = base_well
        :start transformation:
            translation = -0.014, 0.0035, 0.167
			            # -0.014, 0.0035, 0.1665
        :stop transformation:
    :stop geometry:

    :start geometry:
        name = well4
        library = egs_gtransformed
        my geometry = base_well
        :start transformation:
            translation = -0.014, 0.0035, 0.389
			            # -0.014, 0.0035, 0.3885
        :stop transformation:
    :stop geometry:
	
	:start geometry:
        name = well5
        library = egs_gtransformed
        my geometry = base_well
        :start transformation:
            translation = -0.014, 0.0035, 0.611
        :stop transformation:
    :stop geometry:
	
	:start geometry:
        name = well6
        library = egs_gtransformed
        my geometry = base_well
        :start transformation:
            translation = -0.014, 0.0035, -0.611
        :stop transformation:
    :stop geometry: 
	
	:start geometry:
        name = well7
        library = egs_gtransformed
        my geometry = base_well
        :start transformation:
            translation = -0.014, 0.0035, 0.833
        :stop transformation:
    :stop geometry:
	
	:start geometry:
        name = well8
        library = egs_gtransformed
        my geometry = base_well
        :start transformation:
            translation = -0.014, 0.0035, -0.833
        :stop transformation:
    :stop geometry:
	
	:start geometry:
        name = well9
        library = egs_gtransformed
        my geometry = base_well
        :start transformation:
            translation = -0.014, 0.0035, 1.167
        :stop transformation:
    :stop geometry:
	
	:start geometry:
        name = well10
        library = egs_gtransformed
        my geometry = base_well
        :start transformation:
            translation = -0.014, 0.0035, -1.167
        :stop transformation:
    :stop geometry:
	
	:start geometry:
        name = well11
        library = egs_gtransformed
        my geometry = base_well
        :start transformation:
            translation = -0.014, 0.0035, -1.389
        :stop transformation:
    :stop geometry:
	
	:start geometry:
        name = well12
        library = egs_gtransformed
        my geometry = base_well
        :start transformation:
            translation = -0.014, 0.0035, 1.389
        :stop transformation:
    :stop geometry:
	
	:start geometry:
        name = well13
        library = egs_gtransformed
        my geometry = base_well
        :start transformation:
            translation = -0.014, 0.0035, -1.611
        :stop transformation:
    :stop geometry:
	
	:start geometry:
        name = well14
        library = egs_gtransformed
        my geometry = base_well
        :start transformation:
            translation = -0.014, 0.0035, 1.611
        :stop transformation:
    :stop geometry:
	
	:start geometry:
        name = well15
        library = egs_gtransformed
        my geometry = base_well
        :start transformation:
            translation = -0.014, 0.0035, -1.833
        :stop transformation:
    :stop geometry:
	
	:start geometry:
        name = well16
        library = egs_gtransformed
        my geometry = base_well
        :start transformation:
            translation = -0.014, 0.0035, 1.833
        :stop transformation:
    :stop geometry:
	
	:start geometry:
        name = well17
        library = egs_gtransformed
        my geometry = base_well
        :start transformation:
            translation = -0.014, 0.0035, -2.167
        :stop transformation:
    :stop geometry:
	
	:start geometry:
        name = well18
        library = egs_gtransformed
        my geometry = base_well
        :start transformation:
            translation = -0.014, 0.0035, 2.167
        :stop transformation:
    :stop geometry:
	
	:start geometry:
        name = well19
        library = egs_gtransformed
        my geometry = base_well
        :start transformation:
            translation = -0.014, 0.0035, -2.389
        :stop transformation:
    :stop geometry:
	
	:start geometry:
        name = well20
        library = egs_gtransformed
        my geometry = base_well
        :start transformation:
            translation = -0.014, 0.0035, 2.389
        :stop transformation:
    :stop geometry:
	
	

    :start geometry:
        name = the_source
        library = egs_genvelope
        base geometry = string
        inscribed geometries = marker marker1 marker2 marker3 marker4 well1 well2 well3 well4 well5 well6 well7 well8 well9 well10 well11 well12 well13 well14 well15 well16 well17 well18 well19 well20
    :stop geometry:
    
    simulation geometry = the_source
    
:stop geometry definition:
