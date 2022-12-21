:start geometry definition:

    :start geometry:
        name = end_cap
        library = egs_spheres
        midpoint = 0 0 0.055
        type = EGS_cSpheres
        radii = 0.055

        :start media input:
            media = SS_AISI316L
        :stop media input:
    :stop geometry:

    :start geometry:
        name = air_gap
        library = egs_rz
        radii = 0.03
        z-planes = 0.05, 0.06

        :start media input:
            media = AIR_TG43
        :stop media input:
    :stop geometry:

    :start geometry:
        name = end_cap_with_air
        library = egs_genvelope
        base geometry = end_cap
        inscribed geometries = air_gap
    :stop geometry:

    :start geometry:
        name    = cladding_and_source
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -0.535 0 0 1

        :start layer:
            thickness = 0.475
            top radii = 0.055
            bottom radii = 0.055
            media = SS_AISI316L
        :stop layer:
        
        :start layer:
            thickness = 0.01
            top radii = 0.03 0.055
            bottom radii = 0.03 0.055
            media = AIR_TG43, SS_AISI316L
        :stop layer:

        :start layer:
            thickness = 0.1
            top radii = 0.025 0.03 0.055
            bottom radii = 0.025 0.03 0.055
            media = Ir, AIR_TG43, SS_AISI316L
        :stop layer:

        :start layer:
            thickness = 0.01
            top radii = 0.03 0.055
            bottom radii = 0.03 0.055
            media = AIR_TG43, SS_AISI316L
        :stop layer:
        
    :stop geometry:

    :start geometry:
        name = the_planes
        library = egs_planes
        type = EGS_Zplanes
        positions = -0.525 0.055 1.100001
    :stop geometry:

	:start geometry:
		name = the_source
		library = egs_cdgeometry
		base geometry = the_planes
		set geometry = 0 cladding_and_source
		set geometry = 1 end_cap_with_air
	:stop geometry:
	
    simulation geometry = the_source

:stop geometry definition:
