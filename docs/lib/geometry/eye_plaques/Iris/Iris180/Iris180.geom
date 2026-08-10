#      Plaque name: Iris-180
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
			library = egs_planes
			type 	= EGS_Zplanes
			name 	= Transitions
			positions = 0.55039 0.95238 0.97084 1.6    
		:stop geometry:
		
		:start geometry:
			library = egs_spheres
			name 	= start_spheres
			midpoint = 0 0 0                           
			radii	= 1.455 1.505                           
			:start media input:
				media = SILASTIC, MODULAY 				
				set medium = 1 1
				set medium = 2 2 
			:stop media input:
		:stop geometry:
		
				:start geometry:
					library	 = egs_spheres
					midpoint =  0 0 0		         
					radii	 = 1.455
					name	 = sphere_cut
					:start media input:
						media = SILASTIC				
					:stop media input:
				:stop geometry:
				
				:start geometry:
					library = egs_cylinders
					type 	= EGS_ZCylinders
					name 	= cylinder_cut
					radii 	= 1.15
					:start media input:
						media = MODULAY 				
					:stop media input:
				:stop geometry:
		

			:start geometry:
				library = egs_gunion
				name 	= sphere_cylinder
				geometries = sphere_cut cylinder_cut
			:stop geometry:
		

		:start geometry:
			library   = egs_cylinders
			type      = EGS_ZCylinders
			name      = the_cylinders
			radii     = 1.10 1.15
			:start media input:
				media = SILASTIC, MODULAY
				set medium = 1 1
				set medium = 2 2
			:stop media input:
		:stop geometry:
		
		:start geometry: 
			library = egs_cdgeometry
			name 	= eye_plaque_no_eye
			base geometry = Transitions
			set geometry = 2 start_spheres     
			set geometry = 1 sphere_cylinder           
			set geometry = 0 the_cylinders     
		:stop geometry:
		
#---------------------------------------------------------------------
	
			:start geometry:
				library = egs_cylinders
				type 	= EGS_ZCylinders
				name = plaque_hole_modulay
				radii	= 0.55
					:start media input:
						media = MODULAY  
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
				name 	= hole_modulay
				base geometry = hole_boarders
				set geometry = 0 plaque_hole_modulay
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
			geometries = plaque_hole hole_modulay
		:stop geometry:
	
#---------------------------------------------------------------------

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
						media = MODULAY 				
					:stop media input:
				:stop geometry:
			
			
				:start geometry:
					library   = egs_cylinders
					type      = EGS_ZCylinders
					name      = the_cylinders_mold_chunk
					radii     = 1.150002                
					:start media input:
						media = MODULAY                 
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
				type      = EGS_Yplanes
				positions = 0.05 2
				name      = chunk_limit_y
			:stop geometry:
		
			:start geometry:
				library   = egs_ndgeometry
				name      = plaque_chunk
				dimensions = chunk_limit_y eye_plaque_mold_chunk
				:start media input:
					media = WATER_0.998
				:stop media input:
			:stop geometry:
			
#---------------------------------------------------------------------

				:start geometry:
					library = egs_planes
					type 	= EGS_Zplanes
					name 	= transitions_mold
					positions = 0.55038999 0.97084001 1.60001     
				:stop geometry:
		

				:start geometry:
					library = egs_spheres
					name 	= start_spheres_mold
					midpoint = 0 0 0                         
					radii	= 1.505001                                      
					:start media input:
						media = MODULAY 				 
					:stop media input:
				:stop geometry:
			
				:start geometry:
					library   = egs_cylinders
					type      = EGS_ZCylinders
					name      = the_cylinders_mold
					radii     = 1.150001                
					:start media input:
						media = MODULAY
					:stop media input:
				:stop geometry:
		
			
				:start geometry: 
					library = egs_cdgeometry
					name 	= eye_plaque_mold
					base geometry = transitions_mold
					set geometry = 1 start_spheres_mold    
					set geometry = 0 the_cylinders_mold      
				:stop geometry:
				
		
			
			
				:start geometry:
					library = egs_planes
					type 	= EGS_Yplanes
					name 	= cut_boarder_strip
					positions = 0 0.051           
				:stop geometry:
			
			
				:start geometry: 
					library = egs_cdgeometry
					name 	= boarders                  
					base geometry = cut_boarder_strip 
					set geometry = 0 eye_plaque_mold        
				:stop geometry:
				
#---------------------------------------------------------------------

		:start geometry:
			library = egs_gunion
			name 	= eye_plaque_w_hole
			geometries = plaque_chunk iris_hole boarders eye_plaque_no_eye  		
		:stop geometry:
	

		:start geometry: 
			library = egs_spheres
			name 	= dome_cutout
			midpoint = 0 0 0      
			type 	= shell
			radii 	= 1.23, 1.505
		:stop geometry:
	

		:start geometry: 
			library = egs_cdgeometry
			name 	= full_eye_plaque
			base geometry = dome_cutout
			set geometry = 0 eye_plaque_w_hole
		:stop geometry:
		

	:start geometry:
		library = egs_planes
		type 	= EGS_Zplanes
		name 	= cleanup_planes
		positions = 0.55039 1.62    
	:stop geometry:
	
	:start geometry: 
		library = egs_cdgeometry
		name 	= centered_model
		base geometry = cleanup_planes
		set geometry = 0 full_eye_plaque
	:stop geometry:



	:start geometry:
		library = egs_gtransformed
		my geometry = centered_model
		:start transformation:
			rotation = 3.14159 0 3.14159
		:stop transformation:
		name = rotated_model
	:stop geometry:

	simulation geometry = rotated_model

:stop geometry definition:

:start view control:
    xmin = -2
    xmax = 2
    ymin = -2
    ymax = 2
    zmin = -2
    zmax = 2
:stop view control: