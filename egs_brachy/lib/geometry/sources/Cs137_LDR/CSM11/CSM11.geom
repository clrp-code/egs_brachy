:start geometry definition:

    :start geometry:
        name = the_planes
        library = egs_planes
        type = EGS_Zplanes
        positions = -0.21001 0.1825 0.2275 0.31001
    :stop geometry:

    :start geometry:
        name        = end_cap
        library     = egs_spheres
        radii       = 0.0825
        midpoint    = 0 0 0.2275
        :start media input:
            media = SS_AISI316L_p7.8
        :stop media input:
    :stop geometry:
    
    :start geometry:
        name        = air_sphere
        library     = egs_spheres
        radii       = 0.0425
        midpoint    = 0 0 0.1825
        :start media input:
            media = AIR_TG43
        :stop media input:
    :stop geometry:

    :start geometry:
        name    = cladding_and_source
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -0.22 0 0 1

        :start layer:
            thickness = 0.06
            top radii = 0.0825
            bottom radii = 0.0825
            media = SS_AISI316L_p7.8
        :stop layer:
        
        :start layer:
            thickness = 0.32
            top radii = 0.0425 0.0825
            bottom radii = 0.0425 0.0825
            media = Pollucite SS_AISI316L_p7.8
        :stop layer:
        
        :start layer:
            thickness = 0.0225
            bottom radii = 0.0425 0.0825
            media = AIR_TG43 SS_AISI316L_p7.8
        :stop layer:

        :start layer:
            thickness = 0.145
            top radii = 0.0825
            bottom radii = 0.0825
            media = SS_AISI316L_p7.8
        :stop layer:
       
    :stop geometry:

    :start geometry:
        name = air_sphere_and_cladding
        library = egs_genvelope
        base geometry = cladding_and_source
        inscribed geometries = air_sphere
    :stop geometry:

	:start geometry:
		name = the_seed
		library = egs_cdgeometry
		base geometry = the_planes
		set geometry = 0 cladding_and_source
		set geometry = 1 air_sphere_and_cladding
		set geometry = 2 end_cap
	:stop geometry:
	
    simulation geometry = the_seed
    
:stop geometry definition: