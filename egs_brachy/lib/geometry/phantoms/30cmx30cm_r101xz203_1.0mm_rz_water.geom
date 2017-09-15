## 0 <= r <= 30cm,  -15.1cm <= Z <= 15.1cm
## 1cm voxels for 0 <= r <= 10.15cm and -10.15cm <= z <= 10.15cm

:start geometry definition:

    :start geometry:

        name = rz_30cmx30cm_r101xz203_1.0mm_water
        library = egs_rz

        number of shells = 1, 101, 1
        shell thickness =  0.05, 0.1, 4.85

        first plane = -15.
        number of slabs = 1, 203, 1
        slab thickness  = 4.85, 0.1, 4.85

        :start media input:
            media = WATER_0.998
        :stop media input:

    :stop geometry:

    simulation geometry = rz_30cmx30cm_r101xz203_1.0mm_water

:stop geometry definition:
