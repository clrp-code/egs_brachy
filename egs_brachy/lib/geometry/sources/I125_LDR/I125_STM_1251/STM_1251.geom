# #            seed name: I125-STM_1251
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
:start geometry definition:

    :start geometry:
        name    = the_seed
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -0.2275 0 0 1

        :start layer:
            thickness = 0.013   # Habib Note: Ti end Cap thick
            top radii = 0.0405
            bottom radii = 0.0405
            media = Ti
        :stop layer:
    
        :start layer:
            thickness = 0.0235583    # Habib Note: Air gap thickness between the Ti end cap and active layer
            top radii = 0.0325 0.0405
            bottom radii = 0.0325 0.0405
            media = AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.0000017   # hABIB nOTE : I active layer
            top radii = 0.0259417 0.0325 0.0405
            bottom radii = 0.0259417 0.0325 0.0405
            media = I, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.00025    # Habib note : Cu underlayer thickness
            top radii = 0.02594 0.0259417 0.0325 0.0405
            bottom radii = 0.02594 0.0259417 0.0325 0.0405
            media = Cu, I, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.00019    # Habib Note : Ni substrate layer
            top radii = 0.02569 0.02594 0.0259417 0.0325 0.0405
            bottom radii = 0.02569 0.02594 0.0259417 0.0325 0.0405
            media = Ni, Cu, I, AIR_TG43, Ti
        :stop layer:
		
 # Habib Note: AU and AL thickness- The Correct Au radi= 0.01805 cm while the correct one is 0.01805 cm
 # which is by mistake considered as Diameter in all new and old CLRP data and related publication 
 
        :start layer: 
		
            thickness = 0.381    
			top radii    = 0.009 0.0255 0.02569 0.02594 0.0259417 0.0325 0.0405 
			bottom radii = 0.009 0.0255 0.02569 0.02594 0.0259417 0.0325 0.0405 
            media = Au, Al, Ni, Cu, I, AIR_TG43, Ti 
			
        :stop layer:
 
        :start layer:
            thickness = 0.00019
            top radii = 0.02569 0.02594 0.0259417 0.0325 0.0405
            bottom radii = 0.02569 0.02594 0.0259417 0.0325 0.0405
            media = Ni, Cu, I, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.00025
            top radii = 0.02594 0.0259417 0.0325 0.0405
            bottom radii = 0.02594 0.0259417 0.0325 0.0405
            media = Cu, I, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.0000017
            top radii = 0.0259417 0.0325 0.0405
            bottom radii = 0.0259417 0.0325 0.0405
            media = I, AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.0235583
            top radii = 0.0325 0.0405
            bottom radii = 0.0325 0.0405
            media = AIR_TG43, Ti
        :stop layer:

        :start layer:
            thickness = 0.013
            top radii = 0.0405
            bottom radii = 0.0405
            media = Ti
        :stop layer:

    :stop geometry:

    simulation geometry = the_seed

:stop geometry definition:
