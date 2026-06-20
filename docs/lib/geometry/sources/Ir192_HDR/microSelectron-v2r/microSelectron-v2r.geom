# seed name: Ir192- microSelectron-v2r
# air kerma: 1.16329E-13

#  Copyright (C) 2020, Rowan M Thomson and D.W.O. Rogers
#
# This file is part of the egs_brachy package to be used with EGSnrc
#
#  egs_brachy is free software: you can redistribute it and/or modify it
#  under the terms of the GNU Affero General Public License as published
#  by the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  egs_brachy is distributed in the hope that it will be useful, but
#  WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Affero General Public License for more details:
#  <http://www.gnu.org/licenses/>.
#
# If you use this file, please cite
#       M Chamberland, R E P Taylor, D W O Rogers and R M Thomson
#       egs_brachy: a versatile and fast Monte Carlo code for brachytherapy
#       Phys Med Biol 61(2016) 8214 -- 8231
# and
#       Habib Safigholi, Marc J.P. Chamberland, Randle E.P. Taylor, Martin P. Martinov,
#       D. W. O. Rogers and Rowan M. Thomson,  Update of the CLRP Monte Carlo TG-43
#       parameter database for high-energy brachytherapy sources, Med. Phys. 50 (2023)
#       1928-1941
#
#

# MicroSelectron v2 -mHDR-V2r
:start geometry definition:

    :start geometry:
        name    = Ir_source
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -0.185   0 0 1

        :start layer:
           thickness    = 0.0135
           top radii    =  0.035 0.045
           bottom radii =  0.035 0.045
           media        = AIR_TG43 SS_AISI316L
        :stop layer:

        :start layer:
           thickness    = 0.34
		   top radii = 0.03 0.035 0.045
           bottom radii = 0.03 0.035 0.045
           media        = Ir AIR_TG43 SS_AISI316L
        :stop layer:

        :start layer:
            thickness    = 0.01
		    top radii = 0.03 0.035 0.045
            bottom radii = 0.03 0.031 0.041
            media        = Ir AIR_TG43 SS_AISI316L
        :stop layer:
		
		:start layer:
            thickness  = 0.0065
		    top radii  =  0.031 0.041  
            bottom radii =  0.026 0.039
            media        = AIR_TG43 SS_AISI316L
        :stop layer:

    :stop geometry:

    :start geometry:
        name = sph_end_cap
        library = egs_spheres
        midpoint = 0 0 0.1614  # 
        type = EGS_cSpheres
        radii = 0.045

        :start media input:
            media = SS_AISI316L
        :stop media input:
    :stop geometry:

    :start geometry:
        name    = cladding
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -0.2935   0 0 1
# 		0 0 -0.45   0 0 1

        :start layer:
            thickness    = 0.04
			# 0.2
            top radii    = 0.035
            bottom radii = 0.035
            media        = SS_AISI316L_rho4.81
        :stop layer: 

        :start layer:
            thickness    = 0.015
            bottom radii = 0.045
            media        = SS_AISI316L
        :stop layer:

        :start layer:
           thickness = 0.40  #   
           bottom radii = 0.045
           media        = SS_AISI316L
        :stop layer:

    :stop geometry:

    :start geometry:
        name = the_planes
        library = egs_planes
        type = EGS_Zplanes
        positions = -0.2935  0.1614  .200001
    :stop geometry:

	:start geometry:
		name = capsule
		library = egs_cdgeometry
		base geometry = the_planes
		set geometry = 0 cladding
		set geometry = 1 sph_end_cap
	:stop geometry:
	
    :start geometry:
        library = egs_genvelope
        name = the_seed
        base geometry = capsule
        inscribed geometries = Ir_source
    :stop geometry:

    simulation geometry = the_seed

:stop geometry definition:
