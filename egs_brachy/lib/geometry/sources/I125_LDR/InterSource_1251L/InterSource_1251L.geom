# #            seed name: I125-InterSource_1251L
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
        name    = seed
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -0.225 0 0 1

        :start layer:
            thickness    =  0.015  #  
            top radii    = 0.025 0.033     # 0.025 +0.08 (walls thickness)
            bottom radii = 0.025 0.033
            media        = WATER_0.998 Ti  # If inside the cylinder seed hollow occupied with water
		  # media        = AIR_TG43 Ti     # If inside the cylinder seed hollow occupied with air
		 
        :stop layer:

        :start layer:
            thickness    = 0.015 # 
            top radii    = 0.025 0.029 0.029 0.033
            bottom radii = 0.025 0.029 0.0365 0.0405
            media        = WATER_0.998 Ti AIR_TG43 Ti        # If inside the cylinder seed hollow occupied with water
		 #  media        = AIR_TG43 Ti AIR_TG43 Ti           # If inside the cylinder seed hollow occupied with air
		  
        :stop layer:


        :start layer:
            thickness    = 0.01
            top radii    = 0.025 0.029 0.0365 0.0405
            bottom radii = 0.025 0.029 0.0365 0.0405
             media        = WATER_0.998 Ti AIR_TG43 Ti    # If inside the cylinder seed hollow occupied with water
		 #   media        = AIR_TG43 Ti AIR_TG43 Ti       # If inside the cylinder seed hollow occupied with air
		 
        :stop layer:

        :start layer:
            thickness    = 0.08
            top radii    = 0.025 0.029 0.0305 0.0365 0.0405
            bottom radii = 0.025 0.029 0.0305 0.0365 0.0405
             media        = WATER_0.998 Ti C85.7H14.3 AIR_TG43 Ti    # If inside the cylinder seed hollow occupied with water
		  #  media        = AIR_TG43 Ti C85.7H14.3 AIR_TG43 Ti      # If inside the cylinder seed hollow occupied with air
		 
        :stop layer:

        :start layer:
            thickness    = 0.0415
            top radii    = 0.025 0.029 0.0365 0.0405
            bottom radii = 0.025 0.029 0.0365 0.0405
            media        = WATER_0.998 Ti AIR_TG43 Ti     # If inside the cylinder seed hollow occupied with water
		 #  media        = AIR_TG43 Ti AIR_TG43 Ti        # If inside the cylinder seed hollow occupied with air
	
        :stop layer:

        :start layer:
            thickness    = 0.0385
            top radii    = 0.025 0.029 0.0335 0.0365 0.0405
            bottom radii = 0.025 0.029 0.0335 0.0365 0.0405
            media        = WATER_0.998 Ti Pt90Ir10 AIR_TG43 Ti    # If inside the cylinder seed hollow occupied with water
		 #   media        = AIR_TG43 Ti Pt90Ir10 AIR_TG43 Ti      # If inside the cylinder seed hollow occupied with air
		
        :stop layer:

        :start layer:
            thickness    = 0.05
            top radii    = 0.025 0.029 0.0335 0.0344 0.0365 0.0405
            bottom radii = 0.025 0.029 0.0335 0.0344 0.0365 0.0405
            media        = WATER_0.998 Ti Pt90Ir10 C85.7H14.3 AIR_TG43 Ti
		  #  media        = AIR_TG43 Ti Pt90Ir10 C85.7H14.3 AIR_TG43 Ti
		 
        :stop layer:

        :start layer:
            thickness    = 0.0385
            top radii    = 0.025 0.029 0.0335 0.0365 0.0405
            bottom radii = 0.025 0.029 0.0335 0.0365 0.0405
            media        = WATER_0.998 Ti Pt90Ir10 AIR_TG43 Ti
		 #  media        = AIR_TG43 Ti Pt90Ir10 AIR_TG43 Ti
		
        :stop layer:

        :start layer:
            thickness    = 0.0415
            top radii    = 0.025 0.029 0.0365 0.0405
            bottom radii = 0.025 0.029 0.0365 0.0405
            media        = WATER_0.998 Ti AIR_TG43 Ti
		  # media        = AIR_TG43 Ti AIR_TG43 Ti
		
        :stop layer:

        :start layer:
            thickness    = 0.08
            top radii    = 0.025 0.029 0.0305 0.0365 0.0405
            bottom radii = 0.025 0.029 0.0305 0.0365 0.0405
            media        = WATER_0.998 Ti C85.7H14.3 AIR_TG43 Ti
		#	media        = AIR_TG43 Ti C85.7H14.3 AIR_TG43 Ti
	
        :stop layer:

        :start layer:
            thickness    = 0.01
            top radii    = 0.025 0.029 0.0365 0.0405
            bottom radii = 0.025 0.029 0.0365 0.0405
             media        = WATER_0.998 Ti AIR_TG43 Ti
		 #   media        = AIR_TG43 Ti AIR_TG43 Ti
		
        :stop layer:

        :start layer:
            thickness    = 0.015 #  0.0265
            top radii    = 0.025 0.029 0.0365 0.0405
            bottom radii = 0.025 0.029 0.029 0.033
             media        = WATER_0.998 Ti AIR_TG43 Ti
		  #  media        = AIR_TG43 Ti AIR_TG43 Ti
		
        :stop layer:

        :start layer:
            thickness    = 0.015 # 0.035
            top radii    = 0.025 0.033
            bottom radii = 0.025 0.033
              media        = WATER_0.998 Ti
		 #    media        = AIR_TG43 Timedia        
	
        :stop layer:


    :stop geometry:


    simulation geometry = seed

:stop geometry definition:

:start view control:
    xmin = -0.5
    xmax = 0.5
    ymin = -0.5
    ymax = 0.5
    zmin = -1
    zmax = 1
:stop view control:
