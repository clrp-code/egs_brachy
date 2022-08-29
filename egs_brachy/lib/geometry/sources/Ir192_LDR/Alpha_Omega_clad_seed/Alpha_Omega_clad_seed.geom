:start geometry definition:

    :start geometry:
        name = the_seed
        library = egs_rz
        radii = 0.015 0.025
        z-planes = -0.15 0.15
        :start media input:
           media = Pt90Ir10 Pt  
           set medium = 0 0
           set medium = 1 1
        :stop media input:
    :stop geometry:

    simulation geometry = the_seed

:stop geometry definition:
