# seed name: Co60- Co0.A86
# air kerma: 4.2361E-13

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
        name    = capsule
        library = egs_cones
        type    = EGS_ConeStack
        axis    = 0 0 0.196   0 0 -1

        :start layer:
           thickness    = 0.018
           top radii    = 0. 0.05
           bottom radii = 0.035 0.05
           media        = AIR_TG43 SS_AISI316L
        :stop layer:

        :start layer:
           thickness    = 0.003
           bottom radii = 0.035 0.05
           media        = AIR_TG43 SS_AISI316L
        :stop layer:

        :start layer:
           thickness    = 0.35
           top radii    = 0.025 0.035 0.05
           bottom radii = 0.025 0.035 0.05
           media        = Co AIR_TG43 SS_AISI316L
        :stop layer:
        
        :start layer:
           thickness    = 0.074
           top radii    = 0.05
           bottom radii = 0.05
           media        = SS_AISI316L
        :stop layer:

        :start layer:
           thickness    = 0.5
           bottom radii = 0.05
           media        = SS_AISI316L_p7.8
        :stop layer:

    :stop geometry:

    :start geometry:
        name = sph_end_cap
        library = egs_spheres
        midpoint = 0 0 0.200772727
        type = EGS_cSpheres
        radii = 0.050227273

        :start media input:
            media = SS_AISI316L
        :stop media input:
    :stop geometry:

    :start geometry:
        name = the_seed
        library = egs_gunion
        geometries = capsule sph_end_cap
    :stop geometry:

    simulation geometry = the_seed

:stop geometry definition:
