# seed name: Ir192- microSelectron-v1
# air kerma: 1.15751E-13

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

:start geometry definition:

    :start geometry:
        name = the_planes
        library = egs_planes
        type = EGS_Zplanes
        positions = -0.475 0.175 0.23
    :stop geometry:

    :start geometry:
        name = end_sphere
        library = egs_spheres
        midpoint = 0 0 0.175
        radii = 0.055
        :start media input:
            media = SS_AISI316L
            set medium = 0 0
        :stop media input:
    :stop geometry:

    :start geometry:
        name    = cladding_and_source
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -0.475 0 0 1   # HS
		        # 0 0 -0.485 0 0 1   # M.Ch

        :start layer:
            thickness =  0.205   # HS 
                       # 0.215    # M. Ch
            top radii = 0.055
            bottom radii = 0.055
            media = SS_AISI304
        :stop layer:
        
        :start layer:
            thickness = 0.095
            bottom radii = 0.055
            media = SS_AISI316L
        :stop layer:
                
        :start layer:
            thickness = 0.35
            top radii = 0.03 0.055
            bottom radii = 0.03 0.055
            media = Ir SS_AISI316L
        :stop layer:
        
  #     :start layer:
  #          thickness = 0.01  #  M. Ch.
  #          top radii = 0.055
  #          bottom radii = 0.055
  #          media = SS_AISI316L
  #      :stop layer:
        
    :stop geometry:

    :start geometry:
        name = the_seed
        library = egs_cdgeometry
        base geometry = the_planes
        set geometry = 0 cladding_and_source
        set geometry = 1 end_sphere
    :stop geometry:

    simulation geometry = the_seed

:stop geometry definition:
