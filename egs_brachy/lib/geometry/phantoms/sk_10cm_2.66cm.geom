## 0.05x2.66x2.66 cm voxel at 10cm away along transverse axis
## for scoring air kerma strength

:start geometry definition:

    :start geometry:

        name = sk_10cm_2.66cm_wafac_voxel
        library = egs_ndgeometry
        type    = EGS_XYZGeometry

        x-planes = -1, 1, 10, 10.05
        y-planes = -2, -1.33, 1.33, 2
        z-planes = -2, -1.33, 1.33, 2

        # We want to score in the WAFAC 0.05cmx2.66cmx2.66cm voxel
        # Its region number is given by:
        # ir = ix + nx*iy + nx*ny*iz
        # ir = (2) + (3)*(1) + (3)*(3)*(1) = 14
        :start media input:
            media = vacuum, AIR_TG43
            set medium = 14 1   # set region 14 to AIR_TG43
        :stop media input:

    :stop geometry:

    simulation geometry = sk_10cm_2.66cm_wafac_voxel

:stop geometry definition:
