## 0 <= r <= 30cm,  -15.1cm <= Z <= 15.1cm
## 0.05 cm voxels for 0 <= r <= 5.075cm and -5.075cm <= z <= 5.075cm

:start geometry definition:

    :start geometry:

        name = rz_30cmx30cm_r101xz203_0.5mm_water
        library = egs_rz

        number of shells = 1, 101, 1
        shell thickness =  0.025, 0.05, 9.925

        first plane = -15.
        number of slabs = 1, 203, 1
        slab thickness  = 9.925, 0.05, 9.925

        :start media input:
            media = WATER_0.998
        :stop media input:

    :stop geometry:

    simulation geometry = rz_30cmx30cm_r101xz203_0.5mm_water

:stop geometry definition:
