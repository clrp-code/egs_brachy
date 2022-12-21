:start geometry definition:

    :start geometry:
        name    = capsule
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 0.196   0 0 -1

        :start layer:
           thickness    = 0.018
           top radii    = 0. 0.05
           bottom radii = 0.035 0.05
           media        = AIR_TG43 SS_AISI316L
        :stop layer:

        :start layer:
           thickness    = 0.003
           bottom radii = 0.035 0.05
           media        = AIR_TG43 SS_AISI316L
        :stop layer:

        :start layer:
           thickness    = 0.35
           top radii    = 0.025 0.035 0.05
           bottom radii = 0.025 0.035 0.05
           media        = Co AIR_TG43 SS_AISI316L
        :stop layer:
        
        :start layer:
           thickness    = 0.074
           top radii    = 0.05
           bottom radii = 0.05
           media        = SS_AISI316L
        :stop layer:

        :start layer:
           thickness    = 0.5
           bottom radii = 0.05
           media        = SS_AISI316L_p7.8
        :stop layer:

    :stop geometry:

    :start geometry:
        name = sph_end_cap
        library = egs_spheres
        midpoint = 0 0 0.200772727
        type = EGS_cSpheres
        radii = 0.050227273

        :start media input:
            media = SS_AISI316L
        :stop media input:
    :stop geometry:

    :start geometry:
        name = the_seed
        library = egs_gunion
        geometries = capsule sph_end_cap
    :stop geometry:

    simulation geometry = the_seed

:stop geometry definition:
