#               seed name: Pd103-CT20 CivaThin
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
        z-planes = -1.045, 1.045
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
					
        closed      = 0. 0.028   # width
        :start media input:
            media = CS10_Organpoly_Pd5% 
			#  each Pd region is filled with 5% natural pd and 95% organic polymer Which is consistent with 8  micorgram Pd of Reed et al paper Medphys 2014. {rho = 1.15, elements = H, C, N, O, PD ; mass fractions = 0.08075, 0.6156, 0.0475, 0.20615, 0.05} respectively
			
        :stop media input:
    :stop geometry:

    :start geometry:
        name = marker1R
        library = egs_gtransformed
        my geometry = base_marker
        :start transformation:
            translation = 0., 0.0055, 0.5 # CT20
			            # 0., 0.0055, 0.  # CT10
        :stop transformation:
    :stop geometry:
	:start geometry:
        name = marker1L
        library = egs_gtransformed
        my geometry = base_marker
        :start transformation:
            translation = 0., 0.0055, -0.5 # CT20
			            # 0., 0.0055, 0.   # CT10
        :stop transformation:
    :stop geometry:
    
    :start geometry:
        name = well1R
        library = egs_gtransformed
        my geometry = base_well
        :start transformation:
            translation = -0.014 , 0.0035, 0.111      # CT20 
			            # -0.014 , 0.0035, -0.389     # CT10
			            # -0.014 , 0.0035, -0.3885    # half of the width should be (-0.014), 0.0035 down, and 0.3885 offset from source center.  
        :stop transformation:
    :stop geometry:

    :start geometry:
        name = well2R
        library = egs_gtransformed
        my geometry = base_well
        :start transformation:
            translation = -0.014, 0.0035, 0.333      # CT20
			             # -0.014, 0.0035, -0.167    # CT10 Reed Dimension 2014 Brachytherapy
			           
        :stop transformation:
    :stop geometry:

    :start geometry:
        name = well3R
        library = egs_gtransformed
        my geometry = base_well
        :start transformation:
            translation = -0.014, 0.0035, 0.667  # CT20
			            # -0.014, 0.0035, 0.167  # Ct10
        :stop transformation:
    :stop geometry:

    :start geometry:
        name = well4R
        library = egs_gtransformed
        my geometry = base_well
        :start transformation:
            translation = -0.014, 0.0035, 0.889 # CT20
			            # -0.014, 0.0035, 0.389 # CT10
        :stop transformation:
    :stop geometry:
	
	:start geometry:
        name = well01L
        library = egs_gtransformed
        my geometry = base_well
        :start transformation:
            translation = -0.014 , 0.0035, -0.111   # CT20
        :stop transformation:
    :stop geometry:
	
	:start geometry:
        name = well02L
        library = egs_gtransformed
        my geometry = base_well
        :start transformation:
            translation = -0.014, 0.0035, -0.333    # CT20
			           
        :stop transformation:
    :stop geometry:
	
	:start geometry:
        name = well03L
        library = egs_gtransformed
        my geometry = base_well
        :start transformation:
            translation = -0.014, 0.0035, -0.667    # CT20
        :stop transformation:
    :stop geometry:
	
	:start geometry:
        name = well04L
        library = egs_gtransformed
        my geometry = base_well
        :start transformation:
            translation = -0.014, 0.0035, -0.889   # CT20
        :stop transformation:
    :stop geometry:

	
    :start geometry:
        name = the_source
        library = egs_genvelope
        base geometry = string
        inscribed geometries = well1R well2R well3R well4R marker1R marker1L well01L well02L well03L well04L
		#    
    :stop geometry:
    
    simulation geometry = the_source
    
:stop geometry definition:
