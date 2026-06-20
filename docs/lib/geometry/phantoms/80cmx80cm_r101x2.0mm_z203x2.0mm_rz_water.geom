## 0 <= r <= 40cm,  -40cm <= Z <= 40cm
## 0.2 cm voxels for 0 <= r <= 20.3cm and -20.3cm <= z <= 20.3cm

:start geometry definition:

    :start geometry:

        name = rz_80cmx80cm_r101x1.0mm_z203x1.0mm_water
        library = egs_rz

        number of shells = 1, 101, 1
        shell thickness =  0.1, 0.2, 59.7

        first plane = -40.
        number of slabs = 1, 203, 1
        slab thickness  = 19.7, 0.2, 19.7

        :start media input:
            media = WATER_0.998
        :stop media input:

    :stop geometry:

    simulation geometry = rz_80cmx80cm_r101x1.0mm_z203x1.0mm_water

:stop geometry definition:
