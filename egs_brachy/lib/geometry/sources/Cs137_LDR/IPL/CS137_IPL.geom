:start geometry definition:

    :start geometry:
        name    = the_seed
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -1.049 0 0 1

        :start layer:
            thickness = 0.08   # 
            top radii = 0.076  0.1525 
            bottom radii = 0.076  0.1525
            media = SS_AISI304_p7.9, Al
        :stop layer:
    
        :start layer:
            thickness = 0.1    # 
            top radii = 0.1525
            bottom radii = 0.1525
            media = SS_AISI304_p7.9
        :stop layer:

        :start layer:
            thickness = 0.11
            top radii = 0.1104 0.1195 0.1525  
            bottom radii = 0.1104 0.1195 0.1525 
            media = SS_AISI304_p7.9, AIR_TG43, SS_AISI304_p7.9
        :stop layer:

        :start layer:
            thickness = 0.009    # airgap
            top radii = 0.085 0.1104 0.1195 0.1525
            bottom radii = 0.085 0.1104 0.1195 0.1525
            media = AIR_TG43, SS_AISI304_p7.9, AIR_TG43, SS_AISI304_p7.9
        :stop layer:

        :start layer:
            thickness = 1.48   # active layer
            top radii = 0.076 0.085, 0.1104, 0.1195, 0.1525
            bottom radii = 0.076 0.085, 0.1104, 0.1195, 0.1525
            media = CS2O_1.47, AIR_TG43, SS_AISI304_p7.9, AIR_TG43, SS_AISI304_p7.9
        :stop layer:
		
		:start layer:
            thickness = 0.009    # airgap
            top radii = 0.085 0.1104 0.1195 0.1525
            bottom radii = 0.085 0.1104 0.1195 0.1525
            media = AIR_TG43, SS_AISI304_p7.9, AIR_TG43, SS_AISI304_p7.9
        :stop layer:
 
        :start layer:
             thickness = 0.11
            top radii = 0.1104 0.1195 0.1525  
            bottom radii = 0.1104 0.1195 0.1525 
            media = SS_AISI304_p7.9, AIR_TG43, SS_AISI304_p7.9
        :stop layer:
 
        :start layer:
             thickness = 0.09
            top radii = 0.1525  
            bottom radii = 0.1525 
            media = SS_AISI304_p7.9
        :stop layer:

    :stop geometry:

    simulation geometry = the_seed

:stop geometry definition:
