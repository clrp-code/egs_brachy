:start geometry definition:

    :start geometry:
        name    = Ir_source
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -0.18   0 0 1

        :start layer:
           thickness    = 0.006
           top radii    = 0.0265 0.03251
           bottom radii = 0.0325 0.03251
           media        = Ir SS_AISI316L
        :stop layer:

        :start layer:
           thickness    = 0.348
           bottom radii = 0.0325 0.03251
           media        = Ir SS_AISI316L
        :stop layer:

        :start layer:
           thickness    = 0.006
           bottom radii = 0.0265 0.03251
           media        = Ir SS_AISI316L
        :stop layer:

    :stop geometry:

    :start geometry:
        name = sph_end_cap
        library = egs_spheres
        midpoint = 0 0 0.155
        type = EGS_cSpheres
        radii = 0.045

        :start media input:
            media = SS_AISI316L
        :stop media input:
    :stop geometry:

    :start geometry:
        name    = cladding
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -0.45   0 0 1

        :start layer:
            thickness    = 0.2
            top radii    = 0.035
            bottom radii = 0.035
            media        = SS_AISI316L_rho4.81
        :stop layer:

        :start layer:
            thickness    = 0.015
            bottom radii = 0.045
            media        = SS_AISI316L
        :stop layer:

        :start layer:
           thickness    = 0.40
           bottom radii = 0.045
           media        = SS_AISI316L
        :stop layer:

    :stop geometry:

    :start geometry:
        name = the_planes
        library = egs_planes
        type = EGS_Zplanes
        positions = -0.455001 0.155 0.200001
    :stop geometry:

	:start geometry:
		name = capsule
		library = egs_cdgeometry
		base geometry = the_planes
		set geometry = 0 cladding
		set geometry = 1 sph_end_cap
		boundary tolerance = 1e-6
	:stop geometry:
	
    :start geometry:
        library = egs_genvelope
        name = the_seed
        base geometry = capsule
        inscribed geometries = Ir_source
    :stop geometry:

    simulation geometry = the_seed

:stop geometry definition:
