## -1cm <= X, Y, Z <= 1cm with 0.2mm voxels

:start geometry definition:

    :start geometry:

        name = xyz_2.0cmx2.0cmx2.0cm_2mm
        library = egs_ndgeometry
        type    = EGS_XYZGeometry

        x-slabs = -1, 0.2, 10
        y-slabs = -1, 0.2, 10
        z-slabs = -1, 0.2, 10

        :start media input:
            media = WATER_0.998
        :stop media input:

    :stop geometry:

    simulation geometry = xyz_2.0cmx2.0cmx2.0cm_2mm

:stop geometry definition:
