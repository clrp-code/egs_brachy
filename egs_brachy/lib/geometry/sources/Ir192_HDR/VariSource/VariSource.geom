#              Ir192, Varian, VariSource HDR
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
#       low-energy brachytherapy sources, Med. Phys. 47 (2020)
#
#
#
:start geometry definition:

    :start geometry:
        library = egs_spheres
        name = end_cap
        midpoint = 0 0 0.5705
        type = EGS_cSpheres
        radii = 0.0295

        :start media input:
            media = Ti44.4Ni55.6
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_rz
        name = cylinders
        radii = 0.017 0.0295
        z-planes = -1, -0.5 0.5, 0.5705

        :start media input:
            media = Ti44.4Ni55.6, Ir
            set medium = 1 1
        :stop media input:

    :stop geometry:

    :start geometry:
        library =  egs_gunion
        name = source_and_cladding
        geometries = cylinders end_cap
    :stop geometry:

    simulation geometry = source_and_cladding

:stop geometry definition:
