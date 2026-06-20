## 0 <= r <= 40cm,  -40cm <= Z <= 40cm
## 0.01 cm voxels for 0 <= r <= 1.015cm and -1.015cm <= z <= 1.015cm


:start geometry definition:

    :start geometry:

        name = rz_80cmx80cm_r101x0.1mm_z203x0.1mm_water
        library = egs_rz

        number of shells = 1, 101, 1
        shell thickness =  0.005, 0.01, 38.985

        first plane = -40.
        number of slabs = 1, 203, 1
        slab thickness  = 38.985, 0.01, 38.985

        :start media input:
            media = WATER_0.998
        :stop media input:

    :stop geometry:

    simulation geometry = rz_80cmx80cm_r101x0.1mm_z203x0.1mm_water

:stop geometry definition:
