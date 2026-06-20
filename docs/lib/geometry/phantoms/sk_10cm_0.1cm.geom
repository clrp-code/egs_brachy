## 0.05x0.1x0.1 cm voxel at 10cm away along transverse axis
## for scoring air kerma strength

:start geometry definition:

    :start geometry:

        name = sk_10cm_0.1cm_point_voxel
        library = egs_ndgeometry
        type    = EGS_XYZGeometry

        x-planes = -1, 1, 10, 10.05
        y-planes = -1, -0.05, 0.05, 1
        z-planes = -1, -0.05, 0.05, 1

        # We want to score in the WAFAC 0.05cmx0.1cmx0.1cm voxel
        # Its region number is given by:
        # ir = ix + nx*iy + nx*ny*iz
        # ir = (2) + (3)*(1) + (3)*(3)*(1) = 14
        :start media input:
            media = vacuum, AIR_TG43
            set medium = 14 1   # set region 14 to AIR_TG43
        :stop media input:

    :stop geometry:

    simulation geometry = sk_10cm_0.1cm_point_voxel

:stop geometry definition:
