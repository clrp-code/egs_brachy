# seed name: Cs137- CDC-3
# air kerma: 2.254E-13

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
        library = egs_planes
        name = planes
        type = EGS_Zplanes
        positions = -0.25 -0.165 -0.164 0.164 0.165  0.25
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = sph_end_cap_1
        midpoint = 0 0  -0.16
        type = EGS_cSpheres
        radii = 0.09

        :start media input:
            media = SS_AISI316L_p7.8
            set medium = 0 0
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_cylinders
         name = end_cap_1
        midpoint = 0 0  0
        type = EGS_ZCylinders
         radii = 0.09
  
        :start media input:
            media = SS_AISI316L_p7.8
            set medium = 0 0
         :stop media input:
     :stop geometry:

    :start geometry:
        library = egs_cylinders
        name = cladding
        midpoint = 0 0 0
        type = EGS_ZCylinders
        radii = 0.055 0.09

        :start media input:
            media = AIR_TG43 SS_AISI316L_p7.8
            set medium = 0 0
            set medium = 1 1
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = sph_source_1
        midpoint = 0 0  0
        type = EGS_cSpheres 
        radii = 0.055

        :start media input:
            media = Borosilicate_glass__p7.8
            set medium = 0 0
        :stop media input:
    :stop geometry:
	  :start geometry:
        library = egs_spheres
        name = sph_source_2
        midpoint = 0 0  0.11
        type = EGS_cSpheres 
        radii = 0.055

        :start media input:
            media = Borosilicate_glass__p7.8
            set medium = 0 0
        :stop media input:
    :stop geometry:
	 :stop geometry:
	  :start geometry:
        library = egs_spheres
        name = sph_source_3
        midpoint = 0 0  -0.11
        type = EGS_cSpheres 
        radii = 0.055

        :start media input:
            media = Borosilicate_glass__p7.8
            set medium = 0 0
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_genvelope
        name = seed_center
        base geometry = cladding
        inscribed geometries = sph_source_1 sph_source_2 sph_source_3
    :stop geometry:

    :start geometry:
        library = egs_cylinders
        name = end_cap_2
        midpoint = 0 0  0
        type = EGS_ZCylinders
        radii = 0.09

        :start media input:
            media = SS_AISI316L_p7.8
            set medium = 0 0
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_spheres
        name = sph_end_cap_2
        midpoint = 0 0  0.16
        type = EGS_cSpheres
        radii = 0.09

       :start media input:
            media = SS_AISI316L_p7.8
            set medium = 0 0
        :stop media input:
    :stop geometry:

    :start geometry:
        library = egs_cdgeometry
        name = sourceCS
        base geometry = planes
        set geometry = 0 sph_end_cap_1
        set geometry = 1 end_cap_1
        set geometry = 2 seed_center
        set geometry = 3 end_cap_2
        set geometry = 4 sph_end_cap_2
    :stop geometry:

    simulation geometry = sourceCS

:stop geometry definition:
