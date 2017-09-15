## single 30cm voxel filled with water

:start geometry definition:

    :start geometry:

        name = xyz_30cmx30cmx30cm_box
        library = egs_box
        box size = 30

        :start media input:
            media = WATER_0.998
        :stop media input:

    :stop geometry:

    simulation geometry = xyz_30cmx30cmx30cm_box

:stop geometry definition:
