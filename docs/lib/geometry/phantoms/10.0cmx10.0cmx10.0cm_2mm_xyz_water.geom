## -5.0cm <= X, Y, Z <= 5.0cm with 0.2cm voxels

:start geometry definition:

    :start geometry:

        name = xyz_10.0cmx10.0cmx10.0cm_2mm
        library = egs_ndgeometry
        type    = EGS_XYZGeometry

        x-slabs = -5, 0.2, 50
        y-slabs = -5, 0.2, 50
        z-slabs = -5, 0.2, 50

        :start media input:
            media = WATER_0.998
        :stop media input:

    :stop geometry:

    simulation geometry = xyz_10.0cmx10.0cmx10.0cm_2mm

:stop geometry definition:
