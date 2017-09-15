:start geometry definition:

    :start geometry:
        library = egs_spheres
        name = shell
        midpoint = 0 0 0
        type = EGS_cSpheres
        radii = 0.001, 0.0011

        :start media input:
            media = AIR_TG43_LD, Ti10W90
            set medium = 0 0
            set medium = 1 1
        :stop media input:
    :stop geometry:

    simulation geometry = shell

:stop geometry definition:
