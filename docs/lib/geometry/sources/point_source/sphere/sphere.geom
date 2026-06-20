:start geometry definition:

    :start geometry:
        library = egs_spheres
        name = point
        midpoint = 0 0 0
        type = EGS_cSpheres
        radii = 0.0001

        :start media input:
            media = AIR_TG43_LD
            set medium = 0 0
        :stop media input:
    :stop geometry:

    simulation geometry = point

:stop geometry definition:
