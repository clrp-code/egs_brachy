# #            seed name: I125-EchoSeed_6733
#
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
#       Habib Safigholi, Marc J.P. Chamberland, Randle E.P. Taylor,
#       Christian H. Allen, Martin P. Martinov, D. W. O. Rogers and Rowan
#       M. Thomson,  Update of the CLRP TG-43 parameter database for
#       low-energy brachytherapy sources, Med. Phys. 47 (2020) 4656-4669
#
#

# Here the  125I coating is assumed to have a thickness of 2.0 μm on both the cylindrical surface and end faces

:start geometry definition:

    :start geometry:
        name = end_sphere1
        library = egs_spheres
        radii = 0.04
        midpoint = 0, 0, -0.185

        :start media input:
            media = Ti
        :stop media input:
    :stop geometry:

    :start geometry:
        name = end_sphere2
        library = egs_spheres
        radii = 0.04
        midpoint = 0, 0, 0.185

        :start media input:
            media = Ti
        :stop media input:
    :stop geometry:

    :start geometry:
        name = end_inner_sphere1
        library = egs_spheres
        radii = 0.035
        midpoint = 0, 0, -0.16

        :start media input:
            media = AIR_TG43
        :stop media input:
    :stop geometry:

    :start geometry:
        name = end_inner_sphere2
        library = egs_spheres
        radii = 0.035
        midpoint = 0, 0, 0.16

        :start media input:
            media = AIR_TG43
        :stop media input:
    :stop geometry:

    :start geometry:
        name = end_cyl1
        library = egs_rz
        radii = 0.04
        z-planes = -0.185, -0.160001
        :start media input:
            media = Ti
        :stop media input:
    :stop geometry:

    :start geometry:
        name = end_cyl2
        library = egs_rz
        radii = 0.04
        z-planes = 0.159999, 0.185
        :start media input:
            media = Ti
        :stop media input:
    :stop geometry:

    :start geometry:
        name    = cladding_and_source
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -0.1601 0 0 1

        :start layer:
            thickness = 0.0099
            top radii = 0.035 0.04
            bottom radii = 0.035 0.04
            media = AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.0002
            top radii = 0.0252 0.035 0.04
            bottom radii = 0.0252 0.035 0.04
            media = AgI, AIR_TG43, Ti
        :stop layer:
        
        :start layer:
            thickness = 0.005
            top radii = 0.025 0.0252 0.0325 0.04
            bottom radii = 0.025 0.0252 0.0325 0.04
            media = Ag, AgI, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.0218181818181818
            top radii = 0.025 0.0252 0.0325 0.0375
            bottom radii = 0.025 0.0252 0.0325 0.0375
            media = Ag, AgI, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.005
            top radii = 0.025 0.0252 0.0325 0.04
            bottom radii = 0.025 0.0252 0.0325 0.04
            media = Ag, AgI, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.0218181818181818
            top radii = 0.025 0.0252 0.035 0.04
            bottom radii = 0.025 0.0252 0.035 0.04
            media = Ag, AgI, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.005
            top radii = 0.025 0.0252 0.0325 0.04
            bottom radii = 0.025 0.0252 0.0325 0.04
            media = Ag, AgI, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.0218181818181818
            top radii = 0.025 0.0252 0.0325 0.0375
            bottom radii = 0.025 0.0252 0.0325 0.0375
            media = Ag, AgI, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.005
            top radii = 0.025 0.0252 0.0325 0.04
            bottom radii = 0.025 0.0252 0.0325 0.04
            media = Ag, AgI, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.0218181818181818
            top radii = 0.025 0.0252 0.035 0.04
            bottom radii = 0.025 0.0252 0.035 0.04
            media = Ag, AgI, AIR_TG43, Ti
        :stop layer:
 
        :start layer:
            thickness = 0.005
            top radii = 0.025 0.0252 0.0325 0.04
            bottom radii = 0.025 0.0252 0.0325 0.04
            media = Ag, AgI, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.0218181818181818
            top radii = 0.025 0.0252 0.0325 0.0375
            bottom radii = 0.025 0.0252 0.0325 0.0375
            media = Ag, AgI, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.005
            top radii = 0.025 0.0252 0.0325 0.04
            bottom radii = 0.025 0.0252 0.0325 0.04
            media = Ag, AgI, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.0218181818181818
            top radii = 0.025 0.0252 0.035 0.04
            bottom radii = 0.025 0.0252 0.035 0.04
            media = Ag, AgI, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.005
            top radii = 0.025 0.0252 0.0325 0.04
            bottom radii = 0.025 0.0252 0.0325 0.04
            media = Ag, AgI, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.0218181818181818
            top radii = 0.025 0.0252 0.0325 0.0375
            bottom radii = 0.025 0.0252 0.0325 0.0375
            media = Ag, AgI, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.005
            top radii = 0.025 0.0252 0.0325 0.04
            bottom radii = 0.025 0.0252 0.0325 0.04
            media = Ag, AgI, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.0218181818181818
            top radii = 0.025 0.0252 0.035 0.04
            bottom radii = 0.025 0.0252 0.035 0.04
            media = Ag, AgI, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.005
            top radii = 0.025 0.0252 0.0325 0.04
            bottom radii = 0.025 0.0252 0.0325 0.04
            media = Ag, AgI, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.0218181818181818
            top radii = 0.025 0.0252 0.0325 0.0375
            bottom radii = 0.025 0.0252 0.0325 0.0375
            media = Ag, AgI, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.005
            top radii = 0.025 0.0252 0.0325 0.04
            bottom radii = 0.025 0.0252 0.0325 0.04
            media = Ag, AgI, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.0218181818181818
            top radii = 0.025 0.0252 0.035 0.04
            bottom radii = 0.025 0.0252 0.035 0.04
            media = Ag, AgI, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.005
            top radii = 0.025 0.0252 0.0325 0.04
            bottom radii = 0.025 0.0252 0.0325 0.04
            media = Ag, AgI, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.0218181818181818
            top radii = 0.025 0.0252 0.0325 0.0375
            bottom radii = 0.025 0.0252 0.0325 0.0375
            media = Ag, AgI, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.005
            top radii = 0.025 0.0252 0.0325 0.04
            bottom radii = 0.025 0.0252 0.0325 0.04
            media = Ag, AgI, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.0002
            top radii = 0.0252 0.035 0.04
            bottom radii = 0.0252 0.035 0.04
            media = AgI, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.0099
            top radii = 0.035 0.04
            bottom radii = 0.035 0.04
            media = AIR_TG43, Ti
        :stop layer:

    :stop geometry:
    
    :start geometry:
        name = the_planes
        library = egs_planes
        type = EGS_Zplanes
        positions = -0.2250001 -0.16 0.16 0.2250001
    :stop geometry:

    :start geometry:
        name = end1
        library = egs_gunion
        geometries = end_inner_sphere1 end_cyl1 end_sphere1
    :stop geometry:

    :start geometry:
        name = end2
        library = egs_gunion
        geometries = end_inner_sphere2 end_cyl2 end_sphere2
    :stop geometry:

	:start geometry:
		name = the_seed
		library = egs_cdgeometry
		base geometry = the_planes
		set geometry = 0 end1
		set geometry = 1 cladding_and_source
		set geometry = 2 end2
	:stop geometry:
	
    simulation geometry = the_seed
:stop geometry definition: