:start geometry definition:

    :start geometry:

        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 0.097 0 0 -1
        name    = capsule_and_source

        :start layer:
            thickness    = 0.012120786775054704
            top radii    = 0.015
            bottom radii = 0.045 
            media        = SS_AISI316L_p7.8
        :stop layer:

        :start layer:
            thickness    = 0.0498792132249453
            top radii    = 0.045
            bottom radii = 0.045 
            media        = SS_AISI316L_p7.8
        :stop layer:

        :start layer:
            thickness    = 0.01
            top radii    = 0.035 0.045
            bottom radii = 0.035 0.045
            media        = AIR_TG43 SS_AISI316L_p7.8
        :stop layer:

        :start layer:
            thickness    = 0.1
            top radii    = 0.03 0.035 0.045
            bottom radii = 0.03 0.035 0.045
            media        = Ir AIR_TG43 SS_AISI316L_p7.8
        :stop layer:

        :start layer:
           thickness    = 0.01
           top radii    = 0.035 0.045
           bottom radii = 0.000 0.045
           media        = AIR_TG43 SS_AISI316L_p7.8
        :stop layer:

        :start layer:
           thickness    = 0.08
           top radii    = 0.045
           bottom radii = 0.045
           media        = SS_AISI316L_p7.8
        :stop layer:

        :start layer:
           thickness    = 0.5
           top radii    = 0.045
           bottom radii = 0.045
           media        = SS_AISI316L_p6.9
        :stop layer:


    :stop geometry:

    simulation geometry = capsule_and_source

:stop geometry definition:
