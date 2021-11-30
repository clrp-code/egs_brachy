# #             seed name: Pd103- TheraSeed_200
# #             air kerma: 6.4261E-14
#
#  TheraSeed, 200 (Light seed) (6.4261+/-0.0006)E-14
#  TheraSeed, 200 (Heavy seed) (4.6940+/-0.0005)E-14
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
        name = end_cylinder
        library = egs_cylinders
        type = EGS_ZCylinders
        radii = 0.0306, 0.0413
        :start media input:
            media = WATER_0.998 Ti
			      # AIR_TG43_LD Ti    # If want to occupy the end-caps with low density air
            set medium = 0 0
            set medium = 1 1
        :stop media input:
    :stop geometry:

    :start geometry:
        name = base_end_sphere
        library = egs_spheres
        type = EGS_cSpheres
        radii = 0.0306, 0.0346

        :start media input:
            media = WATER_0.998 Ti
			      # AIR_TG43_LD Ti   # If want to occupy the end-caps with low density air
            set medium = 0 0
            set medium = 1 1
        :stop media input:
    :stop geometry:

    :start geometry:
        name = end_sphere_1
        library = egs_gtransformed
        my geometry = base_end_sphere
        :start transformation:
            translation = 0, 0, -0.1916
        :stop transformation:
    :stop geometry:

    :start geometry:
        name = end_sphere_2
        library = egs_gtransformed
        my geometry = base_end_sphere
        :start transformation:
            translation = 0, 0, 0.1916
        :stop transformation:
    :stop geometry:

    :start geometry:
        name    = cladding_and_source
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 0.225 0 0 -1

        :start layer:
	     	           #### 0.00022 micrometer active layer, light seed & 0.00105 micrometer for heavy seed 
            thickness = 0.07453 #   light seed  , if you want to simulate light seed use this dimension
            		  # 0.0737  #   heavy seed  , if you want to simulate heavy seed use this dimension
			#  
			# The value of 0.0737 is for 10.5 micrometer "heavy seed" 
			# Airgap between endcap start and Graphite 
            top radii = 0.0357, 0.0413
            bottom radii = 0.0357, 0.0413
            media = AIR_TG43, Ti
        :stop layer:
        
        :start layer:
            thickness = 0.000220 # 
			# 0.000220 for light seed and 0.00105 for heavy seed-Radioactive layer on surface of  #Graphite cylinder 
            top radii = 0.02822, 0.0357, 0.0413  # light seed  , if you want to simulate light seed use this dimension
			# 
			          # 0.02905 , 0.0357 , 0.0413 # heavy seed  , if you want to simulate heavy seed use this dimension
			# 
			#	    
               
            bottom radii = 0.02822, 0.0357, 0.0413   # light seed  , if you want to simulate light seed use this dimension
			# 
			             # 0.02905 , 0.0357 , 0.0413 #  heavy seed , if you want to simulate heavy seed use this dimension
            media = Pd, AIR_TG43, Ti
        :stop layer:
        
        :start layer:
            thickness = 0.089       
            top radii = 0.028, 0.02822, 0.0357, 0.0413  # light seed  , if you want to simulate light seed use this dimension
			# 
			# 0.028, 0.02905 , 0.0357, 0.0413           # heavy seed  , if you want to simulate heavy seed use this dimension
			# 
            bottom radii = 0.028, 0.02822, 0.0357, 0.0413  # light seed   , if you want to simulate light seed use this dimension
			# 
			# 0.028, 0.02905 , 0.0357, 0.0413              # heavy seed   , if you want to simulate heavy seed use this dimension
			# 
            media = Graphite2.26, Pd, AIR_TG43, Ti
        :stop layer:
        
        :start layer:
            thickness = 0.00022   # light seed  , if you want to simulate light seed use this dimension
			# 
			# 0.00105     # heavy seed
			# 
            top radii = 0.02822, 0.0357, 0.0413  # light seed  , if you want to simulate light seed use this dimension
			# 
			# 0.02905 , 0.0357 , 0.0413           # heavy seed  , if you want to simulate heavy seed use this dimension
            #
            bottom radii = 0.02822, 0.0357, 0.0413  # light seed  , if you want to simulate light seed use this dimension
			# 
			# 0.02905 , 0.0357 , 0.0413         # heavy seed  , if you want to simulate heavy seed use this dimension
			
            media = Pd, AIR_TG43, Ti
        :stop layer:

        :start layer: #  Air gap between graphit and Pb marker 
            thickness = 0.00653   # light seed   , if you want to simulate light seed use this dimension
			          # 0.0057    # heavy seed  , if you want to simulate heavy seed use this dimension
			#     
            top radii = 0.0357, 0.0413
            bottom radii = 0.0357, 0.0413
            media = AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.109
            top radii = 0.0255, 0.0357, 0.0413  # Pb diameter=0.0.051cm, radi is 0.0255cm, 
            bottom radii = 0.0255, 0.0357, 0.0413    
            media = Pb, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.00653   # light seed  , if you want to simulate light seed use this dimension
			#  
		             # 0.0057  # heavy seed    , if you want to simulate heavy seed use this dimension
            top radii = 0.0357, 0.0413
            bottom radii = 0.0357, 0.0413
            media = AIR_TG43, Ti
        :stop layer:
       
        :start layer:
            thickness = 0.00022   # light seed , if you want to simulate light seed use this dimension
			# 
			          # 0.00105  #   heavy seed  , if you want to simulate heavy seed use this dimension
 
            top radii = 0.02822, 0.0357, 0.0413   # light seed  , if you want to simulate light seed use this dimension
			#  
			# 0.02905 , 0.0357 , 0.0413           # heavy seed  , if you want to simulate heavy seed use this dimension
			
            bottom radii = 0.02822, 0.0357, 0.0413  # light seed  , if you want to simulate light seed use this dimension
			# 
			# 0.02905 , 0.0357 , 0.0413            # heavy seed   , if you want to simulate heavy seed use this dimension 
            media = Pd, AIR_TG43, Ti
        :stop layer:
  
        :start layer:
            thickness = 0.089 
            top radii = 0.028, 0.02822, 0.0357, 0.0413 # light seed   , if you want to simulate light seed use this dimension
			# 
			          # 0.028, 0.02905 , 0.0357, 0.0413 # heavy seed  , if you want to simulate heavy seed use this dimension

            bottom radii = 0.028, 0.02822, 0.0357, 0.0413  # light seed  , if you want to simulate light seed use this dimension
			# 
			             # 0.028, 0.02905 , 0.0357, 0.0413 # heavy seed  , if you want to simulate heavy seed use this dimension
			# 
            media = Graphite2.26, Pd, AIR_TG43, Ti
        :stop layer:
       
        :start layer:
            thickness = 0.00022   # light seed  , if you want to simulate light seed use this dimension
			# 
			# 0.00105             # heavy seed  , if you want to simulate heavy seed use this dimension
			#  
            top radii = 0.02822, 0.0357, 0.0413 # light seed  , if you want to simulate light seed use this dimension
			#  
			# 0.02905 , 0.0357 , 0.0413         # heavy seed  , if you want to simulate heavy seed use this dimension
			# 
            bottom radii = 0.02822, 0.0357, 0.0413  # light seed  , if you want to simulate light seed use this dimension
			# 
			# 0.02905 , 0.0357 , 0.0413             # heavy seed  , if you want to simulate heavy seed use this dimension
			#  
            media = Pd, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.07453 # light seed   , if you want to simulate light seed use this dimension
			# 
			# 0.0737            # heavy seed   , if you want to simulate heavy seed use this dimension

            top radii = 0.0357, 0.0413
            bottom radii = 0.0357, 0.0413
            media = AIR_TG43, Ti
        :stop layer:
        
    :stop geometry:

    :start geometry:
        name = source_and_spheres
        library = egs_gunion
        geometries = end_sphere_1 end_sphere_2 cladding_and_source
    :stop geometry:
    
	:start geometry:
        name = the_planes
        library = egs_planes
        type = EGS_Zplanes
        positions = -0.225, -0.1916, 0.1916, 0.225
    :stop geometry:

    :start geometry:
        name = seed
        library = egs_cdgeometry
        base geometry = the_planes
        set geometry = 0 end_cylinder
        set geometry = 1 source_and_spheres
        set geometry = 2 end_cylinder
    :stop geometry:
    
    simulation geometry = seed

:stop geometry definition: