:start geometry definition:

    :start geometry:

        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 0.271 0 0 -1
        name    = capsule_and_source

        :start layer:
            thickness    = 0.0143
            top radii    = 0.0
            bottom radii = 0.055
            media        = SS_AISI316L
        :stop layer:

        :start layer:
           thickness    = 0.0717
           top radii    = 0.055
           bottom radii = 0.055
           media        = SS_AISI316L
        :stop layer:

        :start layer:
           thickness    = 0.01
           top radii    = 0.035 0.055
           bottom radii    = 0.035 0.055
           media        = AIR_TG43 SS_AISI316L
        :stop layer:

        :start layer:
           thickness    = 0.35
           top radii    = 0.03 0.035 0.055
           bottom radii = 0.03 0.035 0.055
           media        = Ir AIR_TG43 SS_AISI316L
        :stop layer:

        :start layer:
           thickness    = 0.05
           top radii    = 0.055
           bottom radii = 0.055
           media        = SS_AISI316L
        :stop layer:

        :start layer:
           thickness    = 6.
           top radii    = 0.055
           bottom radii = 0.055
           media        = SS_AISI304_p5.6
        :stop layer:

    :stop geometry:

    simulation geometry = capsule_and_source

:stop geometry definition:
