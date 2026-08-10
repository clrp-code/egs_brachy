#      Plaque name: Iris-270-No-Lips
#
#   Copyright (C) 2026, Rowan M Thomson
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
#       Phys Med Biol 61(2016) 8214 - 8231
# and
#     Djedouani M, Viner A, Fletcher EM, Thomson RM. 
#     Advancing iris melanoma brachytherapy: Eye plaque models for Monte Carlo simulations and 3D dosimetric datasets. 
#	  Med Phys. 2026;e70439. https://doi.org/10.1002/mp.70439
#
#
#---------------------------------------------------------------------
:start geometry definition:

    :start geometry:
        name = two_spheres
        library = egs_spheres
        midpoint = 0,0,0
        radii = 1.455, 1.505

        :start media input:
            media = WATER_0.998, WATER_0.998
            set medium = 0 0
            set medium = 1 1
        :stop media input:
    :stop geometry:

    :start geometry:
        name = middle_sphere
        library = egs_spheres
        midpoint = 0,0,0
        radii = 1.455

        :start media input:
            media = WATER_0.998
            set medium = 0 0
        :stop media input:
    :stop geometry:

    :start geometry:
        name = middle_cylinder
        library = egs_cylinders
        type = EGS_ZCylinders
        radii = 1.150

        :start media input:
            media = WATER_0.998
            set medium = 0 0
        :stop media input:
    :stop geometry:

    :start geometry:
        name = middle
        library = egs_gunion
        geometries = middle_sphere middle_cylinder
    :stop geometry:

#---------------------------------------------------------------------
	
			:start geometry:
				library = egs_cylinders
				type 	= EGS_ZCylinders
				name = plaque_hole_WATER_0.998
				radii	= 0.55
					:start media input:
						media = WATER_0.998  
					:stop media input:
			:stop geometry:
	
			:start geometry:
				library = egs_planes
				type 	= EGS_Zplanes
				name 	= hole_boarders
				positions = 1.123788 1.455 
			:stop geometry:
		
			:start geometry: 
				library = egs_cdgeometry
				name 	= hole_WATER_0.998
				base geometry = hole_boarders
				set geometry = 0 plaque_hole_WATER_0.998
			:stop geometry:
	
	
		:start geometry:
			library = egs_cylinders
			type 	= EGS_ZCylinders
			name = plaque_hole
			radii	= 0.5
				:start media input:
					media = WATER_0.998
				:stop media input:
		:stop geometry:
	
		:start geometry:
			library = egs_gunion
			name 	= iris_hole
			geometries = plaque_hole hole_WATER_0.998
		:stop geometry:
		
#----------------------------------------------------------------------------------

				:start geometry:
					library = egs_planes
					type 	= EGS_Zplanes
					name 	= transitions_mold_chunk
					positions = 0.55038998 0.97084002 1.60002     
				:stop geometry:
		
				:start geometry:
					library = egs_spheres
					name 	= start_spheres_mold_chunk
					midpoint = 0 0 0                         
					radii	= 1.505002                                          
					:start media input:
						media = WATER_0.998 				
					:stop media input:
				:stop geometry:
			
				:start geometry:
					library   = egs_cylinders
					type      = EGS_ZCylinders
					name      = the_cylinders_mold_chunk
					radii     = 1.150002                
					:start media input:
						media = WATER_0.998                 
					:stop media input:
				:stop geometry:
		
				:start geometry: 
					library = egs_cdgeometry
					name 	= eye_plaque_mold_chunk
					base geometry = transitions_mold_chunk
					set geometry = 1 start_spheres_mold_chunk    
					set geometry = 0 the_cylinders_mold_chunk      
				:stop geometry:
		
		
			:start geometry:
				library   = egs_planes
				type      = EGS_Xplanes
				positions = 0.05 2
				name      = chunk_limit_x
			:stop geometry:
			
			:start geometry:
				library   = egs_planes
				type      = EGS_Yplanes
				positions = 0.05 2
				name      = chunck_limit_y
			:stop geometry:
			
			:start geometry:
				library   = egs_ndgeometry
				name      = plaque_chunk
				dimensions = chunk_limit_x chunck_limit_y eye_plaque_mold_chunk
				:start media input:
					media = WATER_0.998
				:stop media input:
			:stop geometry:
			
			:start geometry:
				library = egs_gtransformed
				my geometry = plaque_chunk
				:start transformation:
					rotation = 3.14159 0 2.356
				:stop transformation:
				name = rotated_chunk
			:stop geometry:

#---------------------------------------------------------------------------------


    :start geometry:
        name = plaque_planes
        library = egs_planes
        type = EGS_Zplanes
        positions = -1.505, -0.970837268, -0.952378601, -0.550363516

    :stop geometry:

    :start geometry:
        name = plaque_not_hollow
        library = egs_cdgeometry
        base geometry = plaque_planes
        set geometry = 0 two_spheres
        set geometry = 1 middle
    :stop geometry:
	
		:start geometry:
			library = egs_gunion
			name 	= eye_plaque_w_hole
			geometries =  rotated_chunk iris_hole plaque_not_hollow  	
		:stop geometry:

    :start geometry:
        name = base_sphere
        library = egs_spheres
        type = shell
        radii = 1.23, 1.505
    :stop geometry:

    :start geometry:
        name = the_plaque
        library = egs_cdgeometry
        base geometry = base_sphere
        set geometry = 0 eye_plaque_w_hole
    :stop geometry:
	

	:start geometry:
		library = egs_planes
		type 	= EGS_Zplanes
		name 	= cleanup_planes
		positions =  -1.62   -0.95
	:stop geometry:
	
	:start geometry: 
		library = egs_cdgeometry
		name 	= centered_model
		base geometry = cleanup_planes
		set geometry = 0 the_plaque
	:stop geometry:

	simulation geometry = centered_model

:stop geometry definition:

:start view control:
    xmin = -2
    xmax = 2
    ymin = -2
    ymax = 2
    zmin = -2
    zmax = 2
:stop view control:
