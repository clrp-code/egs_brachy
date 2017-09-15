:start geometry definition:

    :start geometry:
        library = egs_planes
        name = the_planes
        type = EGS_Zplanes
        positions = -0.465 0.185 0.235001
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = end_cap
        midpoint = 0 0 0.185
        type = EGS_cSpheres
        radii = 0.05

        :start media input:
            media = SS_AISI316L_p8.02
        :stop media input:
    :stop geometry:

    :start geometry:
        name    = cladding_and_source
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -0.475 0 0 1

        :start layer:
            thickness = 0.21
            top radii = 0.05
            bottom radii = 0.05
            media = SS_AISI316L_p5.0
        :stop layer:
        
        :start layer:
            thickness = 0.09
            bottom radii = 0.05
            media = SS_AISI316L_p8.02
        :stop layer:

        :start layer:
            thickness = 0.35
            top radii = 0.03 0.05            
            bottom radii = 0.03 0.05
            media = Ir, SS_AISI316L_p8.02
        :stop layer:

        :start layer:
            thickness = 0.02
            top radii = 0.05            
            bottom radii = 0.05
            media = SS_AISI316L_p8.02
        :stop layer:

    :stop geometry:

    :start geometry:
        name = the_seed
        library = egs_cdgeometry
        base geometry = the_planes
        set geometry = 0 cladding_and_source
        set geometry = 1 end_cap
        boundary tolerance = 1e-6
    :stop geometry:

    simulation geometry = the_seed

:stop geometry definition:
