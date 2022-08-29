:start geometry definition:

    :start geometry:
        name = the_planes
        library = egs_planes
        type = EGS_Zplanes
        positions = -0.475 0.175 0.23
    :stop geometry:

    :start geometry:
        name = end_sphere
        library = egs_spheres
        midpoint = 0 0 0.175
        radii = 0.055
        :start media input:
            media = SS_AISI316L
            set medium = 0 0
        :stop media input:
    :stop geometry:

    :start geometry:
        name    = cladding_and_source
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -0.475 0 0 1   # HS
		        # 0 0 -0.485 0 0 1   # M.Ch

        :start layer:
            thickness =  0.205   # HS 
                       # 0.215    # M. Ch
            top radii = 0.055
            bottom radii = 0.055
            media = SS_AISI304
        :stop layer:
        
        :start layer:
            thickness = 0.095
            bottom radii = 0.055
            media = SS_AISI316L
        :stop layer:
                
        :start layer:
            thickness = 0.35
            top radii = 0.03 0.055
            bottom radii = 0.03 0.055
            media = Ir SS_AISI316L
        :stop layer:
        
  #     :start layer:
  #          thickness = 0.01  #  M. Ch.
  #          top radii = 0.055
  #          bottom radii = 0.055
  #          media = SS_AISI316L
  #      :stop layer:
        
    :stop geometry:

    :start geometry:
        name = the_seed
        library = egs_cdgeometry
        base geometry = the_planes
        set geometry = 0 cladding_and_source
        set geometry = 1 end_sphere
    :stop geometry:

    simulation geometry = the_seed

:stop geometry definition:
