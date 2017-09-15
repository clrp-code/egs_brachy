## 0 <= r <= 40cm,  -40cm <= Z <= 40cm
## 0.1 cm voxels for 0 <= r <= 10.15cm and -10.15cm <= z <= 10.15cm

:start geometry definition:

    :start geometry:

        name = rz_80cmx80cm_r101x1.0mm_z203x1.0mm_water
        library = egs_rz

        number of shells = 1, 101, 1
        shell thickness =  0.05, 0.1, 29.85

        first plane = -40.
        number of slabs = 1, 203, 1
        slab thickness  = 29.85, 0.1, 29.85

        :start media input:
            media = WATER_0.998
        :stop media input:

    :stop geometry:

    simulation geometry = rz_80cmx80cm_r101x1.0mm_z203x1.0mm_water

:stop geometry definition:
