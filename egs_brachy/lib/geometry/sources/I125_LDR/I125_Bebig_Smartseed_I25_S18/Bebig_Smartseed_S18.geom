#            seed name: I125-Bebig_Smartseed_S18
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
#IBt-Bebig I-125 Smart-seed  brachytherapy source modeled based on the Abboud paper published in Medphys @ 2010. 
#
:start geometry definition:
     # The set of planes needed for the CD geometry to cut the source length
    :start geometry:
        library = egs_planes
        name = planes
        type = EGS_Zplanes
        positions = -0.225 0.225
    :stop geometry:

    :start geometry:
        library = egs_cylinders
		type = EGS_ZCylinders
        name = cladding
		midpoint = 0 0 0
        radii =  0.039

	    :start media input:
           media = Ti AIR_TG43    # PEEK AIR_TG43
           set medium = 0 0	
        :stop media input:
		 
    :stop geometry:

    :start geometry:
        library = egs_spheres
        type = EGS_cSpheres
	    name = end_sphere_1
        radii = 0.022
        midpoint = 0 0  -0.21

        :start media input:
            media = AIR_TG43  
        :stop media input:
    :stop geometry:

    :start geometry:
	    library = egs_spheres 
        type = EGS_cSpheres
		name = end_sphere_2
        radii = 0.022
        midpoint = 0 0  0.21
		
        :start media input:
            media = AIR_TG43
        :stop media input:
    :stop geometry:
	
	:start geometry:
        library = egs_gunion
        name = S
        geometries = end_sphere_1 end_sphere_2 cladding
    :stop geometry:
	
	:start geometry:
        library = egs_cdgeometry
        name = seed-body
        base geometry = planes
        set geometry = 0 S
    :stop geometry:
	
	:start geometry: 
        name    = core
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -0.14   0 0 1

        :start layer:
            thickness    = 0.0002
            top radii    = 0.0265 0.039
            bottom radii = 0.0265 0.039
            media        =  quartz_2.21 PEEK  
                           			
        :stop layer:

        :start layer:
            thickness    = 0.0023
            top radii    = 0.0263 0.0265 0.039
            bottom radii = 0.0263 0.0265 0.039
            media        = IPlant_active quartz_2.21 PEEK
			            
        :stop layer:
		
        :start layer:
            thickness    = 0.275
            top radii    = 0.024 0.0263 .0265 0.039
            bottom radii = 0.024 0.0263 .0265 0.039
            media        = LEAD_GLASS_4.8 IPlant_active quartz_2.21 PEEK 
                         # LEAD_GLASS means 18% Pbo and LEAD_GLASS_4.8 means 32% pbo into marker		
        :stop layer:
		
		:start layer:
            thickness    = 0.0023
            top radii    = 0.0263 .0265 0.039
            bottom radii = 0.0263 .0265 0.039
            media        =  IPlant_active quartz_2.21 PEEK 
        :stop layer:
		
         :start layer:
            thickness    = 0.0002
            top radii    = 0.0265 0.039
            bottom radii = 0.0265 0.039
            media        =  quartz_2.21 PEEK
        :stop layer:

     :stop geometry:
	 :start geometry:
        name = Smart-seed
        library = egs_genvelope
        base geometry = seed-body 
        inscribed geometries = core
    :stop geometry:
 
     simulation geometry = Smart-seed

:stop geometry definition:
