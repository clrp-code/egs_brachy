#####################################################################
#
# IMPORTANT: Please note that the ends of this seed are, in reality,
# hollow. However, due to limitations in egs++, the ends of this
# geometry model are filled with water. To change the medium that
# fills the ends of the seed, change 'WATER_0.998' in 'end_cylinder'
# and in 'base_end_sphere' to whatever medium desired.
#
#####################################################################
:start geometry definition:

    # change 'WATER_0.998' to whatever medium the seed is inscribed
    # into
    :start geometry:
        name = end_cylinder
        library = egs_cylinders
        type = EGS_ZCylinders
        radii = 0.0306, 0.0413
        :start media input:
            media = WATER_0.998 Ti
            set medium = 0 0
            set medium = 1 1
        :stop media input:
    :stop geometry:

    # change 'WATER_0.998' to whatever medium the seed is inscribed
    # into
    :start geometry:
        name = base_end_sphere
        library = egs_spheres
        type = EGS_cSpheres
        radii = 0.0306, 0.0346

        :start media input:
            media = WATER_0.998 Ti # 
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
            thickness = 0.07453
            top radii = 0.0357, 0.0413
            bottom radii = 0.0357, 0.0413
            media = AIR_TG43, Ti
        :stop layer:
        
        :start layer:
            thickness = 0.00022
            top radii = 0.02822, 0.0357, 0.0413
            bottom radii = 0.02822, 0.0357, 0.0413
            media = Pd, AIR_TG43, Ti
        :stop layer:
        
        :start layer:
            thickness = 0.089
            top radii = 0.028, 0.02822, 0.0357, 0.0413
            bottom radii = 0.028, 0.02822, 0.0357, 0.0413
            media = Graphite2.26, Pd, AIR_TG43, Ti
        :stop layer:
        
        :start layer:
            thickness = 0.00022
            top radii = 0.02822, 0.0357, 0.0413
            bottom radii = 0.02822, 0.0357, 0.0413
            media = Pd, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.00653
            top radii = 0.0357, 0.0413
            bottom radii = 0.0357, 0.0413
            media = AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.109
            top radii = 0.025, 0.0357, 0.0413
            bottom radii = 0.02822, 0.0357, 0.0413
            media = Pb, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.00653
            top radii = 0.0357, 0.0413
            bottom radii = 0.0357, 0.0413
            media = AIR_TG43, Ti
        :stop layer:
       
        :start layer:
            thickness = 0.00022
            top radii = 0.02822, 0.0357, 0.0413
            bottom radii = 0.02822, 0.0357, 0.0413
            media = Pd, AIR_TG43, Ti
        :stop layer:
  
        :start layer:
            thickness = 0.089
            top radii = 0.028, 0.02822, 0.0357, 0.0413
            bottom radii = 0.028, 0.02822, 0.0357, 0.0413
            media = Graphite2.26, Pd, AIR_TG43, Ti
        :stop layer:
       
        :start layer:
            thickness = 0.00022
            top radii = 0.02822, 0.0357, 0.0413
            bottom radii = 0.02822, 0.0357, 0.0413
            media = Pd, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.07453
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
        name = the_seed
        library = egs_cdgeometry
        base geometry = the_planes
        set geometry = 0 end_cylinder
        set geometry = 1 source_and_spheres
        set geometry = 2 end_cylinder
        boundary tolerance = 1e-6
    :stop geometry:
    
    simulation geometry = the_seed

:stop geometry definition: