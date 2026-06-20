:start geometry definition:

    :start geometry:
        name    = the_seed
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -1.045 0 0 1 

        :start layer:
            thickness = 0.15   # 
            top radii = 0.15
            bottom radii = 0.15
            media = SS_AISI_p7.8 
        :stop layer:
    
        :start layer:
            thickness = 1.5   # active Au core
            top radii = 0.04  0.15
            bottom radii = 0.04  0.15
            media = Au SS_AISI_p7.8
        :stop layer:

        :start layer:
            thickness = 0.44
            top radii = 0.15 
            bottom radii = 0.15
            media = SS_AISI_p7.8
        :stop layer:

    :stop geometry:

    simulation geometry = the_seed

:stop geometry definition:
