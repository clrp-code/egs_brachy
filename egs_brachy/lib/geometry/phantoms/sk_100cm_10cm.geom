:start geometry definition:

    :start geometry:

        name = sk_100cm_10cm_wafac_voxel
        library = egs_ndgeometry
        type    = EGS_XYZGeometry

        x-planes = -1, 1, 100, 100.05
        y-planes = -6, -5, 5, 6
        z-planes = -6, -5, 5, 6

        # We want to score in the WAFAC 0.05cmx10cmx10cm voxel
        # Its region number is given by:
        # ir = ix + nx*iy + nx*ny*iz
        # ir = (2) + (3)*(1) + (3)*(3)*(1) = 14
        :start media input:
            media = vacuum, AIR_TG43
            set medium = 14 1   # set region 14 to AIR_TG43
        :stop media input:

    :stop geometry:

    simulation geometry = sk_100cm_10cm_wafac_voxel

:stop geometry definition:
