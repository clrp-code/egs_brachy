#               seed name: Pd103-CT10 CivaThin
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
        #### HS Note: The CS10 and CT10 from CivaTech tech Inc are identical, except the CT10 has smaller outer diameter OD=0.06 cm
    :start geometry:
        name = Thin
        library = egs_rz
        radii = 0.03
        z-planes = -0.545, 0.545
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
            media = CS10_Organpoly_Pd5% 
			# HS Note: It supposed that each Pd region is filled with 5% natural pd and 95% organic polymer #  Result show 5% is the correct one {rho = 1.15, elements = H, C, N, O, PD ; mass fractions = 0.08075, 0.6156, 0.0475, 0.20615, 0.05} respectively
			
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
        name = the_source
        library = egs_genvelope
        base geometry = Thin
        inscribed geometries = well1 well2 well3 well4 marker
    :stop geometry:
    
    simulation geometry = the_source
    
:stop geometry definition:
