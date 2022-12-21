:start geometry definition:

    :start geometry:
        library = egs_spheres
        name = end_cap
        midpoint = 0 0   0.195   #  0.1949 cm; The source length just reduce about 0.0001 cm to check the effect of boundry toching
        type = EGS_cSpheres
        radii = 0.045

        :start media input:
            media = SS_AISI316L_p7.8
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_rz
        name = cylinders
        radii = 0.0365 0.045
        z-planes = -0.435 -0.24 -0.18 0.18 0.19 0.1949 # 0.195

        :start media input:
            media = SS_AISI316L_p7.8 SS_AISI316L_p6.9 Yb169 AIR_TG43
            set medium = 0 1
            set medium = 2 2
            set medium = 3 3
            set medium = 5 1
        :stop media input:

    :stop geometry:

    :start geometry:
        library =  egs_gunion
        name = source_and_cladding
        geometries = cylinders end_cap
    :stop geometry:

    simulation geometry = source_and_cladding

:stop geometry definition:
