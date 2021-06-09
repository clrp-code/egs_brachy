# #            seed name: PharmaSeed_BT-125-2
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
        name = seed
        library = egs_cones
        type = EGS_ConeStack
        axis = 0 0 -0.225 0 0 1
        
		:start layer:
		    thickness = 0.05
		    top radii =0.04
		    bottom radii = 0.04
		    media = Ti
		:stop layer:
		
		:start layer:
            thickness = 0.01245
		    top radii = 0.034 0.04
			bottom radii = 0.034 0.04
		    media = AIR_TG43, Ti
        :stop layer: 
		
		:start layer:
		    thickness    = 0.00005
            top radii    = 0.02505 0.034 0.04
            bottom radii = 0.02505 0.034 0.04
            media        = I AIR_TG43 Ti
        :stop layer:

        :start layer:
           thickness    = 0.325
           top radii    = 0.025 0.02505 0.034 0.04
           bottom radii = 0.025 0.02505 0.034 0.04
           media        = Ag I AIR_TG43 Ti
        :stop layer:		

		:start layer:
            thickness    = 0.00005
            top radii    = 0.02505 0.034 0.04
            bottom radii = 0.02505 0.034 0.04
            media        = I AIR_TG43 Ti
        :stop layer
		
		:start layer:
            thickness = 0.01245
		    top radii = 0.034 0.04
			bottom radii = 0.034 0.04
		    media = AIR_TG43, Ti
        :stop layer:
		
		:start layer:
		    thickness = 0.05
		    top radii =0.04
		    bottom radii = 0.04
		    media = Ti
		:stop layer:
		
		:stop geometry:

    simulation geometry = seed

:stop geometry definition:
		
		

