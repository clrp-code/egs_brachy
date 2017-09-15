:start geometry definition:
    # the set of planes needed for the CD geometry
    :start geometry:
        library = egs_planes
        name = planes
        type = EGS_Zplanes
        positions = -0.2275 -0.1875 0.1875 0.2275
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = sph_end_cap_1
        midpoint = 0 0  -0.1875
        type = EGS_cSpheres
        radii = 0.04

        :start media input:
            media = Ti
            set medium = 0 0
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_cylinders
        name = cladding
        midpoint = 0 0 0
        type = EGS_ZCylinders
        radii = 0.033 0.04

        :start media input:
            media = AIR_TG43 Ti
            set medium = 0 0
            set medium = 1 1
        :stop media input:
    :stop geometry:

:start geometry:

        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -0.140   0 0 1
        name    = source

        # source end cylinder
        :start layer:
            thickness    = 0.000175
            top radii    = 0.0175
            bottom radii = 0.017675
            media        = AgBrAgI_6.20
        :stop layer:

        # source end cone
        :start layer:
           thickness    = 0.007325
           top radii    = 0.0175 0.017675
           bottom radii = 0.024825 0.025
           media        = Ag AgBrAgI_6.20
        :stop layer:

       # mid source
        :start layer:
           thickness    = 0.265
           bottom radii = 0.024825 0.025
           media        =  Ag AgBrAgI_6.20
        :stop layer:

       # source end cone
        :start layer:
           thickness    = 0.007325
           bottom radii = 0.0175 0.017675
           media        = Ag AgBrAgI_6.20
        :stop layer:

       # source end cylinder
        :start layer:
           thickness    = 0.000175
           top radii    = 0.017675
           bottom radii = 0.0175
           media        = AgBrAgI_6.20
        :stop layer:

    :stop geometry:

    :start geometry:
        library = egs_genvelope
        name = seed_center
        base geometry = cladding
        inscribed geometries = source
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = sph_end_cap_2
        midpoint = 0 0  0.1875
        type = EGS_cSpheres
        radii = 0.04

        :start media input:
            media = Ti
            set medium = 0 0
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_cdgeometry
        name = seed
        base geometry = planes
        set geometry = 0 sph_end_cap_1
        set geometry = 1 seed_center
        set geometry = 2 sph_end_cap_2
        boundary tolerance = 1e-6
    :stop geometry:

    simulation geometry = seed
:stop geometry definition:
