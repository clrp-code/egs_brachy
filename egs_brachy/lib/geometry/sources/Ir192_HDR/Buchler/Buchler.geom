:start geometry definition:

    :start geometry:
        name = the_planes
        library = egs_planes
        type = EGS_Zplanes
        positions = -6.241 0.095 0.175001
    :stop geometry:

    :start geometry:
        name = end_cap
        library = egs_spheres
        midpoint = 0 0 0.095
        type = EGS_cSpheres
        radii = 0.08
        :start media input:
            media = SS_AISI321
        :stop media input:
    :stop geometry:

    :start geometry:
        name    = cladding_and_source
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -6.251 0 0 1    

        :start layer:
            thickness = 5.876
            top radii = 0.055
            bottom radii = 0.055
            media = SS_AISI301
        :stop layer:
        
        :start layer:
            thickness = 0.134
            top radii = 0.055 0.08
            bottom radii = 0.055 0.08
            media = SS_AISI301, SS_AISI321
        :stop layer:

        :start layer:
            thickness = 0.176
            top radii = 0.08
            bottom radii = 0.08
            media = SS_AISI321
        :stop layer:
 
        :start layer:
            thickness = 0.13
            top radii = 0.05 0.06 0.08
            bottom radii = 0.05 0.06 0.08
            media = Ir, AIR_TG43, SS_AISI321
        :stop layer:
                
        :start layer:
            thickness = 0.01
            top radii = 0.06 0.08
            bottom radii = 0.06 0.08
            media = AIR_TG43, SS_AISI321
        :stop layer:
 
        :start layer:
            thickness = 0.03
            top radii = 0.08
            bottom radii = 0.08
            media = SS_AISI321
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
