:start geometry definition:

    :start geometry:

        library = egs_cones 
        type    = EGS_ConeStack
        axis    = 0 0 0.2399 0 0 -1
        name    = capsule_and_source

        :start layer:
            thickness    = 0.0108
            top radii    = 0.017
            bottom radii = 0.0425
            media        = SS_AISI304
        :stop layer:

        :start layer:
           thickness    = 0.0491
           bottom radii = 0.0425
           media        = SS_AISI304
        :stop layer:

        :start layer:
           thickness    = 0.005
           top radii    = 0.0335 0.0425
           bottom radii = 0.0335 0.0425
           media        = AIR_TG43 SS_AISI304
        :stop layer:

        :start layer:
           thickness    = 0.35
           top radii    = 0.03 0.0335 0.0425
           bottom radii = 0.03 0.0335 0.0425
           media        = Ir AIR_TG43 SS_AISI304
        :stop layer:

        :start layer:
           thickness    = 0.005
           top radii    = 0.0335 0.0425
           bottom radii = 0.0335 0.0425
           media        = AIR_TG43 SS_AISI304
        :stop layer:

        :start layer:
           thickness    = 0.0401
           top radii    = 0.0425
           bottom radii = 0.025
           media        = SS_AISI304
        :stop layer:

        :start layer:
           thickness    = 0.5
           bottom radii = 0.025
           media        = SS_AISI304
        :stop layer:


    :stop geometry:

    simulation geometry = capsule_and_source

:stop geometry definition:
