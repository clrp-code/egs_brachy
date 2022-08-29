:start geometry definition:

    :start geometry:

        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 0.261 0 0 -1
        name    = capsule_and_source

        :start layer:
            thickness    = 0.02
            top radii    = 0.0
            bottom radii = 0.055
            media        = SS_AISI316L_p7.8
        :stop layer:

        :start layer:
           thickness    = 0.066
           top radii    = 0.055
           bottom radii = 0.055
           media        = SS_AISI316L_p7.8
        :stop layer:

        :start layer:
           thickness    = 0.01
           top radii    = 0.035 0.055
           bottom radii    = 0.035 0.055
           media        = AIR_TG43 SS_AISI316L_p7.8
        :stop layer:

        :start layer:
           thickness    = 0.14
           top radii    = 0.03 0.035 0.055
           bottom radii = 0.03 0.035 0.055
           media        = Al AIR_TG43 SS_AISI316L_p7.8
        :stop layer:

        :start layer:
           thickness    = 0.05
           top radii    = 0.03 0.035 0.055
           bottom radii = 0.03 0.035 0.055
           media        = Ir70Pt30 AIR_TG43 SS_AISI316L_p7.8
        :stop layer:

        :start layer:
           thickness    = 0.05
           top radii    = 0.055
           bottom radii = 0.055
           media        = SS_AISI316L_p7.8
        :stop layer:

        :start layer:
           thickness    = 6
           top radii    = 0.055
           bottom radii = 0.055
           media        = SS_AISI304_p5.6
        :stop layer:

    :stop geometry:

    simulation geometry = capsule_and_source

:stop geometry definition:
