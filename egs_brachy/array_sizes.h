/*
################################################################################
#
#  egs_brachy array_sizes.h
#  Copyright (C) 2016 Rowan Thomson, Dave Rogers, Randle Taylor, and Marc
#  Chamberland
#
#  This file is part of egs_brachy
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
################################################################################
#
#  When egs_brachy is used for publications, please cite our paper:
#  M. J. P. Chamberland, R. E. P. Taylor, D. W. O. Rogers, and R. M. Thomson,
#  egs brachy: a versatile and fast Monte Carlo code for brachytherapy,
#  Phys. Med. Biol. 61, 8214-8231 (2016).
#
################################################################################
#
#  Author:        Randle Taylor, 2016
#
#  Contributors:  Marc Chamberland
#                 Dave Rogers
#                 Rowan Thomson
#
################################################################################
*/

/*****************************************************************************
 *
 *
 *   Define in this file the maximum number of media (MXMED) and the
 *   maximum number of particles on the stack (MXSTACK).
 *   This file gets included by the egsnrc fortran subroutines
 *   (egsnrc_$my_machine.F), the base application (egs_simple_application.cpp
 *   or egs_advanced_application.cpp in $HEN_HOUSE/egs++), and possible by
 *   the user code, if use is made of the particle stack or one of the
 *   structures that depends on the maximum number of media.
 *
 *****************************************************************************/

#ifndef ARRAY_SIZES_
#define ARRAY_SIZES_

/* don't forget to adjust corresponding values in egs_brachy.macros
then make clean; make */
#define MXMED 50
#define MXSTACK 200000
#endif
