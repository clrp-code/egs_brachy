:start geometry definition:

    :start geometry:
        library = egs_planes
        name = planes
        type = EGS_Zplanes
        positions = -0.25 -0.16 -0.055 0.055 0.16 0.25
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = sph_end_cap_1
        midpoint = 0 0  -0.16
        type = EGS_cSpheres
        radii = 0.09

        :start media input:
            media = SS_AISI316L_p7.8
            set medium = 0 0
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_cylinders
         name = end_cap_1
        midpoint = 0 0  0
        type = EGS_ZCylinders
         radii = 0.09
  
        :start media input:
            media = SS_AISI316L_p7.8
            set medium = 0 0
         :stop media input:
     :stop geometry:

    :start geometry:
        library = egs_cylinders
        name = cladding
        midpoint = 0 0 0
        type = EGS_ZCylinders
        radii = 0.055 0.09

        :start media input:
            media = AIR_TG43 SS_AISI316L_p7.8
            set medium = 0 0
            set medium = 1 1
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = sph_source_1
        midpoint = 0 0  0
        type = EGS_cSpheres 
        radii = 0.055

        :start media input:
            media = Borosilicate_glass__p7.8
            set medium = 0 0
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_genvelope
        name = seed_center
        base geometry = cladding
        inscribed geometries = sph_source_1 # sph_source_2 sph_source_3
    :stop geometry:

    :start geometry:
        library = egs_cylinders
        name = end_cap_2
        midpoint = 0 0  0
        type = EGS_ZCylinders
        radii = 0.09

        :start media input:
            media = SS_AISI316L_p7.8
            set medium = 0 0
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = sph_end_cap_2
        midpoint = 0 0  0.16
        type = EGS_cSpheres
        radii = 0.09

       :start media input:
            media = SS_AISI316L_p7.8
            set medium = 0 0
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_cdgeometry
        name = sourceCS
        base geometry = planes
        set geometry = 0 sph_end_cap_1
        set geometry = 1 end_cap_1
        set geometry = 2 seed_center
        set geometry = 3 end_cap_2
        set geometry = 4 sph_end_cap_2
    :stop geometry:

    simulation geometry = sourceCS

:stop geometry definition:
