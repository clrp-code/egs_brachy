# seed name: Yb169- 4140
# air kerma: 2.1245E-14

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
        library = egs_spheres
        name = end_cap
        midpoint = 0 0   0.195   #  0.1949 cm; The source length just reduce about 0.0001 cm to check the effect of boundry toching
        type = EGS_cSpheres
        radii = 0.045

        :start media input:
            media = SS_AISI316L_p7.8
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_rz
        name = cylinders
        radii = 0.0365 0.045
        z-planes = -0.435 -0.24 -0.18 0.18 0.19 0.1949 # 0.195

        :start media input:
            media = SS_AISI316L_p7.8 SS_AISI316L_p6.9 Yb169 AIR_TG43
            set medium = 0 1
            set medium = 2 2
            set medium = 3 3
            set medium = 5 1
        :stop media input:

    :stop geometry:

    :start geometry:
        library =  egs_gunion
        name = source_and_cladding
        geometries = cylinders end_cap
    :stop geometry:

    simulation geometry = source_and_cladding

:stop geometry definition:
