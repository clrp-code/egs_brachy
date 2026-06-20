:start geometry definition:
	:start geometry:
		name = water_phantom
		library = egs_ndgeometry
		type    = EGS_XYZGeometry
		x-slabs = -1.275  0.05  51
		y-slabs = -1.275  0.05  51
		z-slabs = -0.075  0.05  51
		
        :start media input:
            media = WATER_0.998
        :stop media input:
    :stop geometry:
	simulation geometry = water_phantom
:stop geometry definition: