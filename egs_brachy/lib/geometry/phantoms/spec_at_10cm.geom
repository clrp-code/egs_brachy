:start geometry definition:

    :start geometry:

        name = rz_spec_at_10cm
        library = egs_rz

        radii = 0.05, 10, 10.005, 110
        z-planes = -100, -1, -0.0565, 0.0565, 1, 100

        #alternative way of defining the geometry; same as BrachyDose
#        number of shells = 1, 1, 1, 1
#        shell thickness =  0.05, 9.95, 0.005, 99.995
#
#        first plane = -100
#        number of slabs = 1, 1, 1, 1, 1
#        slab thickness  = 99, 0.9435, 0.113, 0.9435, 99

        :start media input:
            media = AIR_TG43
        :stop media input:

    :stop geometry:

    simulation geometry = rz_spec_at_10cm

:stop geometry definition:
