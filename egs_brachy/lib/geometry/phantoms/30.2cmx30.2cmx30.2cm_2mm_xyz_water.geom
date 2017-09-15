## -15.1cm <= X, Y, Z <= 15.1cm with 0.2mm voxels

:start geometry definition:

    :start geometry:

        name = xyz_30.2cmx30.2cmx30.2cm_2mm
        library = egs_ndgeometry
        type    = EGS_XYZGeometry

        x-slabs = -15.1, 0.2, 151
        y-slabs = -15.1, 0.2, 151
        z-slabs = -15.1, 0.2, 151

        :start media input:
            media = WATER_0.998
        :stop media input:

    :stop geometry:

    simulation geometry = xyz_30.2cmx30.2cmx30.2cm_2mm

:stop geometry definition:
