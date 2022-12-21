:start geometry definition:

    :start geometry:
        name = the_planes    
        library = egs_planes
        type = EGS_Zplanes
        positions = -0.8 0.25 0.40
    :stop geometry:

    :start geometry:
        name = end_cap
        library = egs_spheres
        midpoint = 0 0 0.25
        type = EGS_cSpheres
        radii = 0.15
        :start media input:
            media = SS_SUS316_p8.02
        :stop media input:
    :stop geometry:

    :start geometry:
        name    = cladding_and_source
        library = egs_cones
        type    = EGS_ConeStack 
        axis    = 0 0 -0.9  0 0 1

        :start layer:
            thickness = 0.8
            top radii = 0.15
            bottom radii = 0.15
            media = SS_SUS316_p8.02
        :stop layer:

        :start layer:
            thickness = 0.1
            top radii = 0.05 0.075 0.15
            bottom radii = 0.05 0.075 0.15
            media = Co , AIR_TG43, SS_SUS316_p8.02
        :stop layer:
		
		 :start layer:
            thickness = 0.1
            top radii = 0.05 0.075 0.15
            bottom radii = 0.05 0.075 0.15
            media =  Co , AIR_TG43, SS_SUS316_p8.02
        :stop layer:
		
		:start layer:
            thickness = 0.15
            top radii = 0.15
            bottom radii = 0.15
            media = SS_SUS316_p8.02 
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
