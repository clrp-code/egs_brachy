#               seed name: Cs131-Proxcelan_CS1_rev2
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
:start geometry definition:

    :start geometry:
        name    = the_seed
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 0.226 0 0 -1
		
          # Ti end cap
		  
        :start layer:
            thickness = 0.01
            top radii = 0.0415
            bottom radii = 0.0415
            media = Ti
        :stop layer:
        
        :start layer:
            thickness = 0.016
            top radii = 0.0355 0.0415
            bottom radii = 0.0355 0.0415
            media = Ar Ti
        :stop layer:

        :start layer:
            thickness = 0.4
            top radii = 0.0125 0.015 0.020 0.031 0.0355 0.0415
            bottom radii = 0.0125 0.015 0.020 0.031 0.0355 0.0415
            media = Au Ar  Pyrex_2.4 Pyrex_2.4_Cs Ar Ti
        :stop layer:
        
        :start layer:
            thickness = 0.016
            top radii = 0.0355 0.0415
            bottom radii = 0.0355 0.0415
            media = Ar Ti
        :stop layer:

        :start layer:
            thickness = 0.01
            top radii = 0.0415
            bottom radii = 0.0415
            media = Ti
        :stop layer:

    :stop geometry:
    
    simulation geometry = the_seed
    
:stop geometry definition: