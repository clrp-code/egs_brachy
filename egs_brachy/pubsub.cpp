/*
################################################################################
#
#  egs_brachy pubsub.cpp
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

/*
 * Adapted from http://www.cs.sjsu.edu/~pearce/modules/patterns/events/pubsubimp.htm
 * Programmer:      Pearce
 * Copyright (c):   2000, all rights reserved.
 */
#include "pubsub.h"

void Publisher::notify(EB_Message message, void *what, Subscriber *s) {
    if (notifyEnabled) {
        list<Subscriber *>::iterator p;
        for (p = subscribers[message].begin(); p != subscribers[message].end(); p++) {
            if (*p != s) {
                (*p)->update(message, what);
            }
        }
    }
    notifyEnabled = true;
}
