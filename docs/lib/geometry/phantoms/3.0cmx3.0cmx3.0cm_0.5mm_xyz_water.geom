## -1.5cm <= X, Y, Z <= 1.5cm with 0.5cm voxels

:start geometry definition:

    :start geometry:

        name = xyz_3.0cmx3.0cmx3.0cm_0.5mm
        library = egs_ndgeometry
        type    = EGS_XYZGeometry

        x-slabs = -1.5, 0.05, 60
        y-slabs = -1.5, 0.05, 60
        z-slabs = -1.5, 0.05, 60

        :start media input:
            media = WATER_0.998
        :stop media input:

    :stop geometry:

    simulation geometry = xyz_3.0cmx3.0cmx3.0cm_0.5mm

:stop geometry definition:
