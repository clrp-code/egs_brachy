## -1.5cm <= X, Y, Z <= 1.5cm with 0.1cm voxels

:start geometry definition:

    :start geometry:

        name = xyz_3.0cmx3.0cmx3.0cm_1.0mm
        library = egs_ndgeometry
        type    = EGS_XYZGeometry

        x-slabs = -1.5, 0.1, 30
        y-slabs = -1.5, 0.1, 30
        z-slabs = -1.5, 0.1, 30

        :start media input:
            media = WATER_0.998
        :stop media input:

    :stop geometry:

    simulation geometry = xyz_3.0cmx3.0cmx3.0cm_1.0mm

:stop geometry definition:
