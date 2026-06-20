## 0 <= r <= 40cm,  -40cm <= Z <= 40cm
## 0.05 cm voxels for 0 <= r <= 5.075cm and -5.075cm <= z <= 5.075cm

:start geometry definition:

    :start geometry:

        name = rz_80cmx80cm_r101x0.5mm_z203x0.5mm_water
        library = egs_rz

        number of shells = 1, 101, 1
        shell thickness =  0.025, 0.05, 34.925

        first plane = -40.
        number of slabs = 1, 203, 1
        slab thickness  = 34.925, 0.05, 34.925

        :start media input:
            media = WATER_0.998
        :stop media input:

    :stop geometry:

    simulation geometry = rz_80cmx80cm_r101x0.5mm_z203x0.5mm_water

:stop geometry definition:
