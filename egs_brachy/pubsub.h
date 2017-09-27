/*
################################################################################
#
#  egs_brachy pubsub.h
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

/*! \file pubsub.h
 *
 * \brief A simple pub/sub module to allow various egs_brachy classes to subscribe to particle
 * events.
 *
 * Adapted from http://www.cs.sjsu.edu/~pearce/modules/patterns/events/pubsubimp.htm
 * */
#ifndef PUBSUB_H
#define PUBSUB_H
#include <list>
#include <string>
#include <map>
using namespace std;
class Publisher;

enum EB_Message {

    NEW_HISTORY,
    PARTICLE_INITIALIZED,
    PARTICLE_TAKING_STEP,
    PARTICLE_TOOK_STEP,
    PARTICLE_ESCAPING_SOURCE,
    PARTICLE_ESCAPED_SOURCE,
    PARTICLE_ESCAPING_GEOM,
    PARTICLE_ESCAPED_GEOM,
    PHOTON_SCATTER_EVENT, // scatter occuring anywhere
    NON_SOURCE_PHOTON_SCATTER_EVENT // scatter occuring outside source

};

typedef pair<bool, EB_Message> SendMessage;

class Subscriber {
public:
    virtual ~Subscriber() {}
    virtual void update(EB_Message message, void *what = 0) = 0;
};

class Publisher {
public:
    Publisher() {
        notifyEnabled = true;
    }
    virtual ~Publisher() { }
    void subscribe(Subscriber *s, EB_Message message) {
        subscribers[message].push_back(s);
    }
    void unsubscribe(Subscriber *s, EB_Message message) {
        subscribers[message].remove(s);
    }
    void notify(EB_Message message, void *what = 0, Subscriber *s = 0);
    void setNotifyEnabled(bool flag) {
        notifyEnabled = flag;
    }
    bool getNotifyEnabled() const {
        return notifyEnabled;
    }
private:
    map<EB_Message, list<Subscriber *> > subscribers;
    bool notifyEnabled;
};
#endif
