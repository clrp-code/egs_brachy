:start geometry definition:

    :start geometry:
        name = the_seed
        library = egs_rz
        radii = 0.005 0.015
        z-planes = -0.25 0.25
        :start media input:
           media = Pt75Ir25 Pt
           set medium = 0 0
           set medium = 1 1
        :stop media input:
    :stop geometry:

    simulation geometry = the_seed

:stop geometry definition:
