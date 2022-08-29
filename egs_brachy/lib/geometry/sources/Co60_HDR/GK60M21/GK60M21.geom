:start geometry definition:

    :start geometry:

        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 0.259 0 0 -1
        name    = capsule_and_source

        :start layer:
            thickness    = 0.084
            top radii    = 0.05
            bottom radii = 0.05
            media        = SS_AISI316L_p7.8
        :stop layer:

        :start layer:
           thickness    = 0.35
            top radii    = 0.03 0.035 0.05
           bottom radii = 0.03 0.035 0.05
           media        = Co AIR_TG43 SS_AISI316L_p7.8
        :stop layer:

        :start layer:
           thickness    = 0.02
           top radii    = 0.035 0.05
           bottom radii = 0.000 0.05
           media        = AIR_TG43 SS_AISI316L_p7.8
        :stop layer:

        :start layer:
           thickness    = 0.04
           top radii    = 0.05
           bottom radii = 0.05
           media        = SS_AISI316L_p7.8
        :stop layer:

        :start layer:
           thickness    = 0.5
           top radii    = 0.05
           bottom radii = 0.05
           media        = SS_AISI316L_p6.9
        :stop layer:


    :stop geometry:

    simulation geometry = capsule_and_source

:stop geometry definition:
