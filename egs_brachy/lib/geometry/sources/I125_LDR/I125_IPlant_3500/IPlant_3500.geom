# #            seed name: I125-IPlant_3500
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

        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -0.225  0 0 1
        name    = seed  # seed_center

        # end weld cylinder thickness
        :start layer:
            thickness    = 0.025   
            top radii    = 0.04    
            bottom radii = 0.04    
            media        = Ti
        :stop layer:

        # air gap 
        :start layer:
           thickness    = 0.012       #  
           top radii    = 0.035 0.04  #  
           bottom radii = 0.035 0.04  #  
           media        = AIR_TG43 Ti
        :stop layer:
 

       # cone apex to cone base
        :start layer:
           thickness    = 0.065  
           top radii    = 0.00 0.02159 0.03175 0.03335 0.03385 0.035 0.04
           bottom radii = 0.0203 0.02159 0.03175 0.03335 0.03385 0.035 0.04 
           media        = Ag, AIR_TG43, quartz_2.21, IPlant_active, SiO2, AIR_TG43, Ti
        :stop layer:

       # cone apex to cone base 
        :start layer:
           thickness    = 0.246 # 
           bottom radii = 0.0203 0.02159 0.03175 0.03335 0.03385 0.035 0.04 
           media        = Ag, AIR_TG43, quartz_2.21, IPlant_active, SiO2, AIR_TG43, Ti
        :stop layer:

       # cone base to cone apex
        :start layer:
           thickness    = 0.065  
           bottom radii    = 0.00 0.02159 0.03175 0.03335 0.03385 0.035 0.04
           media        = Ag, AIR_TG43, quartz_2.21, IPlant_active, SiO2, AIR_TG43, Ti
        :stop layer:


        # air gap
        :start layer:
           thickness    = 0.012  # 0.009
           top radii    = 0.035 0.04  # 0.036195 0.04178
           bottom radii = 0.035 0.04  # 0.036195 0.04178
           media        = AIR_TG43 Ti
        :stop layer:

        # end weld cylinder
        :start layer:
            thickness    = 0.025   
            top radii    = 0.04  
            bottom radii = 0.04   
            media        = Ti
        :stop layer:

    :stop geometry:

    simulation geometry = seed
:stop geometry definition:

:start view control:
    xmin = -0.5
    xmax = 0.5
    ymin = -0.5
    ymax = 0.5
    zmin = -1
    zmax = 1
:stop view control:
