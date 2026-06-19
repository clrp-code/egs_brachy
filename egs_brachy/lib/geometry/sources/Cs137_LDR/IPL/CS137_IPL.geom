# seed name: Cs137- IPL
# air kerma: 2.2446E-13

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
        name    = the_seed
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 -1.049 0 0 1

        :start layer:
            thickness = 0.08   # 
            top radii = 0.076  0.1525 
            bottom radii = 0.076  0.1525
            media = SS_AISI304_p7.9, Al
        :stop layer:
    
        :start layer:
            thickness = 0.1    # 
            top radii = 0.1525
            bottom radii = 0.1525
            media = SS_AISI304_p7.9
        :stop layer:

        :start layer:
            thickness = 0.11
            top radii = 0.1104 0.1195 0.1525  
            bottom radii = 0.1104 0.1195 0.1525 
            media = SS_AISI304_p7.9, AIR_TG43, SS_AISI304_p7.9
        :stop layer:

        :start layer:
            thickness = 0.009    # airgap
            top radii = 0.085 0.1104 0.1195 0.1525
            bottom radii = 0.085 0.1104 0.1195 0.1525
            media = AIR_TG43, SS_AISI304_p7.9, AIR_TG43, SS_AISI304_p7.9
        :stop layer:

        :start layer:
            thickness = 1.48   # active layer
            top radii = 0.076 0.085, 0.1104, 0.1195, 0.1525
            bottom radii = 0.076 0.085, 0.1104, 0.1195, 0.1525
            media = CS2O_1.47, AIR_TG43, SS_AISI304_p7.9, AIR_TG43, SS_AISI304_p7.9
        :stop layer:
		
		:start layer:
            thickness = 0.009    # airgap
            top radii = 0.085 0.1104 0.1195 0.1525
            bottom radii = 0.085 0.1104 0.1195 0.1525
            media = AIR_TG43, SS_AISI304_p7.9, AIR_TG43, SS_AISI304_p7.9
        :stop layer:
 
        :start layer:
             thickness = 0.11
            top radii = 0.1104 0.1195 0.1525  
            bottom radii = 0.1104 0.1195 0.1525 
            media = SS_AISI304_p7.9, AIR_TG43, SS_AISI304_p7.9
        :stop layer:
 
        :start layer:
             thickness = 0.09
            top radii = 0.1525  
            bottom radii = 0.1525 
            media = SS_AISI304_p7.9
        :stop layer:

    :stop geometry:

    simulation geometry = the_seed

:stop geometry definition:
