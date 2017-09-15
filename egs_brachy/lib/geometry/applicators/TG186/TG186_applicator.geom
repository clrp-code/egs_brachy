## Generic shielded applicator from the AAPM Working Group on
## Model-Based Dose Calculation Algorithms (MBDCA)
##
## This is commonly referred to as the TG-186 applicator.
##
## <img style="width: 200px" src="../lib/geometry/applicators/TG186/TG-186_applicator.png"/>
## <img style="width: 200px" src="../lib/geometry/applicators/TG186/TG-186_applicator_cross-section.png"/>

:start geometry definition:

    :start geometry:
        library = egs_planes
        name = planes_a
        type = EGS_Zplanes
        positions = -6.1 6.1 7.9
    :stop geometry:

    :start geometry:
        library = egs_cylinders
        name = body_a
        midpoint = 0 0 0
        type = EGS_ZCylinders
        radii = 1.8

        :start media input:
            media = PMMA
        :stop media input:
    :stop geometry:


    :start geometry:
        library = egs_cylinders
        name = rho_coordinates_a
        type = EGS_ZCylinders
        radii = 1.2
    :stop geometry:

   :start geometry:
        library   = egs_planes
        type      = EGS_Zplanes
        positions = -6.2 6.2
        name      = z_coordinates_a
   :stop geometry:

   :start geometry:
        library   = egs_iplanes
        axis      = 0 0 0   0 0 1
        angles    = 90
        name      = phi_coordinates_a
    :stop geometry:

   :start geometry:
        library   = egs_ndgeometry
        name      = 180degree_tungsten_shield_a
        dimensions = rho_coordinates_a z_coordinates_a phi_coordinates_a
        :start media input:
            media = AIR_TG43 DENSIMET_D176
            set medium =  0  0
           set medium =  1  1
      :stop media input:
   :stop geometry:


    :start geometry:
        library = egs_cylinders
        name = probe_a
        midpoint = 0 0 0
        type = EGS_ZCylinders
        radii = 0.05 0.16

        :start media input:
            media = AIR_TG43 SS_PROBE_RHO8.0
            set medium = 0 0
            set medium = 1 1
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_genvelope
        name = shield_and_probe_a
        base geometry = 180degree_tungsten_shield_a
        inscribed geometries = probe_a
    :stop geometry:

    :start geometry:
        library = egs_genvelope
        name = body_w_shield_and_probe_a
        base geometry = body_a
        inscribed geometries = shield_and_probe_a
    :stop geometry:


    :start geometry:
        library = egs_spheres
        name = end_cap_a
        midpoint = 0 0 6.1
        type = EGS_cSpheres
        radii = 1.8

        :start media input:
            media = PMMA
        :stop media input:
    :stop geometry:


    :start geometry:
        library = egs_cdgeometry
        name = applicator
        base geometry = planes_a
        set geometry = 0 body_w_shield_and_probe_a
        set geometry = 1 end_cap_a
    :stop geometry:

    simulation geometry = applicator

:stop geometry definition:

