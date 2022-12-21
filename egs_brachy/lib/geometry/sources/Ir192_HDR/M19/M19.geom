:start geometry definition:

    :start geometry:
        name = the_planes
        library = egs_planes
        type = EGS_Zplanes
        positions = -30.341 0.1815 0.240001  # -30.341 for cable effect # -0.341 clrpv2
    :stop geometry:

    :start geometry:
        name = end_cap
        library = egs_spheres
        midpoint = 0 0 0.1815
        type = EGS_cSpheres
        radii = 0.0585
        :start media input:
            media = SS_AISI316L_p7.8  
        :stop media input:
    :stop geometry:

    :start geometry:
        name    = cladding_and_source
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -30.341 0 0 1     # -30.341 for cable effect # -0.341 clrpv2 #-0.351  Mch

        :start layer:
            thickness = 30.071 # 0.071 CLRPv2 for cable effect on F(r, theta) 30.71 cm cable effect is considered #0.081 Mch
            top radii = 0.0585
            bottom radii = 0.0585
            media = SS_AISI316L_p6.9   
        :stop layer:
        
        :start layer:
            thickness = 0.095
            top radii = 0.0585
            bottom radii = 0.0585
            media = SS_AISI316L_p7.8   
        :stop layer:

        :start layer:
            thickness = 0.35
            top radii = 0.0325 0.0425 0.0585
            bottom radii = 0.0325 0.0425 0.0585
            media = Ir, AIR_TG43, SS_AISI316L_p7.8
        :stop layer:
 
        :start layer:
            thickness = 0.065   #? 0.01 Mch
            top radii = 0.0585
            bottom radii = 0.0585
            media = SS_AISI316L_p7.8  
        :stop layer:
      
    :stop geometry:

	:start geometry:
		name = the_source
		library = egs_cdgeometry
		base geometry = the_planes
		set geometry = 0 cladding_and_source
		set geometry = 1 end_cap
	:stop geometry:

    simulation geometry = the_source

:stop geometry definition:
