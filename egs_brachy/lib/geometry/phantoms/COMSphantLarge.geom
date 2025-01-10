:start geometry definition:
	:start geometry:
		name = water_phantom
		library = egs_ndgeometry
		type    = EGS_XYZGeometry
		x-slabs = -1.475  0.05  59
		y-slabs = -1.475  0.05  59
		z-slabs = -0.075  0.05  59
		
        :start media input:
            media = WATER_0.998
        :stop media input:
    :stop geometry:
	simulation geometry = water_phantom
:stop geometry definition: