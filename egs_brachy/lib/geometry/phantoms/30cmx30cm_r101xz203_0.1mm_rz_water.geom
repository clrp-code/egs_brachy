## 0 <= r <= 30cm,  -15.1cm <= Z <= 15.1cm
## 0.01 cm voxels for 0 <= r <= 1.015cm and -1.015cm <= z <= 1.015cm

:start geometry definition:

    :start geometry:

        name = rz_30cmx30cm_r101xz203_0.1mm_water
        library = egs_rz

        number of shells = 1, 101, 1
        shell thickness =  0.005, 0.01, 13.985

        first plane = -15.
        number of slabs = 1, 203, 1
        slab thickness  = 13.985, 0.01, 13.985

        :start media input:
            media = WATER_0.998
        :stop media input:

    :stop geometry:

    simulation geometry = rz_30cmx30cm_r101xz203_0.1mm_water

:stop geometry definition:
