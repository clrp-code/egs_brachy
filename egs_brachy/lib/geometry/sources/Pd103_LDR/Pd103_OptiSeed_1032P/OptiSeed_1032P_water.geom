 # It assumed the cups in the end cups are filled with water

 :start geometry definition:

    :start geometry:
        library = egs_planes
        name = planes
        type = EGS_Zplanes
        positions = -0.25 0.25
    :stop geometry:

    :start geometry:
        library = egs_rz
        name = cylinders
        radii = 0.02   0.0225 0.040
        z-planes = -0.25 -0.19 -0.12 -0.1 0.1 0.12 0.19 0.25

        :start media input:
            media = C85.7H14.3 PolySty AIR_TG43 Au
            set medium = 1 1
            set medium = 2 2
            set medium = 3 3
            set medium = 4 2
            set medium = 5 1
            set medium = 10 3
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = end_sphere_1
        type = EGS_cSpheres
        radii = 0.02
        midpoint = 0 0  -0.235

        :start media input:
            media = WATER_0.998   # WATER IN THE ENDCAP cups
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = end_sphere_2
        type = EGS_cSpheres
        radii = 0.02
        midpoint = 0 0  0.235

        :start media input:
            media = WATER_0.998   # WATER IN THE ENDCAP cups
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_gunion
        name = seed
        geometries = end_sphere_1 end_sphere_2 cylinders
    :stop geometry:

    :start geometry:
        library = egs_cdgeometry
        name = seed_trimmed
        base geometry = planes
        set geometry = 0 seed
    :stop geometry:

    simulation geometry = seed_trimmed

:stop geometry definition:
