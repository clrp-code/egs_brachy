## A 10cm^3 water phantom with a single region

:start geometry definition:

    :start geometry:

        name = xyz_10cmx10cmx10cm_box
        library = egs_box
        box size = 10

        :start media input:
            media = WATER_0.998
        :stop media input:

    :stop geometry:

    simulation geometry = xyz_10cmx10cmx10cm_box

:stop geometry definition:
