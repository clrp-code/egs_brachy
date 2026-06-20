## -2.5cm <= X, Y, Z <= 2.5Cm with 0.2cm voxels

:start geometry definition:

    :start geometry:

        name = xyz_5.0cmx5.0cmx5.0cm_2mm
        library = egs_ndgeometry
        type    = EGS_XYZGeometry

        x-slabs = -2.5, 0.2, 25
        y-slabs = -2.5, 0.2, 25
        z-slabs = -2.5, 0.2, 25

        :start media input:
            media = WATER_0.998
        :stop media input:

    :stop geometry:

    simulation geometry = xyz_5.0cmx5.0cmx5.0cm_2mm

:stop geometry definition:
