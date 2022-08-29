# MicroSelectron v2 -mHDR-V2r
:start geometry definition:

    :start geometry:
        name    = Ir_source
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -0.18   0 0 1

        :start layer:
           thickness    = 0.005
           top radii    =  0.03 0.045
           bottom radii =  0.03 0.045
           media        = AIR_TG43 SS_AISI316L
        :stop layer:

        :start layer:
           thickness    = 0.34
		      top radii = 0.025 0.03 0.045 
           bottom radii = 0.025 0.03 0.045
           media        = Co AIR_TG43 SS_AISI316L
        :stop layer:

        :start layer:
            thickness    = 0.01
		       top radii = 0.025 0.03 0.045
            bottom radii = 0.025 0.027 0.042
            media        = Co AIR_TG43 SS_AISI316L
        :stop layer:
		
		:start layer:
            thickness  = 0.01
			# 0.0065
		    top radii  =  0.027 0.041  
            bottom radii =  0.024 0.039
            media        = AIR_TG43 SS_AISI316L
        :stop layer:

    :stop geometry:

    :start geometry:
        name = sph_end_cap
        library = egs_spheres
        midpoint = 0 0 0.165  # 
        type = EGS_cSpheres
        radii = 0.045

        :start media input:
            media = SS_AISI316L
        :stop media input:
    :stop geometry:

    :start geometry:
        name    = cladding
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -0.265   0 0 1
#		0 0 -0.2935   0 0 1
# 		0 0 -0.45   0 0 1

        :start layer:
            thickness    = 0.02
			# 0.2
            top radii    = 0.036
            bottom radii = 0.036
            media        = SS_AISI316L_rho4.81
        :stop layer: 

        :start layer:
            thickness    = 0.02
			top radii = 0.036 0.045
            bottom radii = 0.036 0.045
            media        = SS_AISI316L_rho4.81 SS_AISI316L
        :stop layer:

        :start layer:
           thickness = 0.045  #   
           top radii = 0.036 0.045
           bottom radii = 0.036 0.045
            media        = SS_AISI316L SS_AISI316L
        :stop layer:
		:start layer:
           thickness = 1.99  #   
           top radii = 0.036 0.045
            bottom radii = 0.036 0.045
            media        = SS_AISI316L SS_AISI316L
        :stop layer:

    :stop geometry: 

    :start geometry:
        name = the_planes
        library = egs_planes
        type = EGS_Zplanes
        positions = -0.265  0.165   0.205001
    :stop geometry:

	:start geometry:
		name = capsule
		library = egs_cdgeometry
		base geometry = the_planes
		set geometry = 0 cladding
		set geometry = 1 sph_end_cap
	:stop geometry:
	
    :start geometry:
        library = egs_genvelope
        name = the_seed
        base geometry = capsule
        inscribed geometries = Ir_source
    :stop geometry:

    simulation geometry = the_seed

:stop geometry definition:
