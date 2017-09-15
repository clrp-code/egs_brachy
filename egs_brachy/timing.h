/*
################################################################################
#
#  egs_brachy timing.h
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

#ifndef EGS_BRACHY_TIMER_
#define EGS_BRACHY_TIMER_

#include <sstream>
#include <iomanip>
#include "egs_timer.h"


class EB_Timer {

    string name;
    EGS_Timer timer;
    EGS_Float start_time;
    EGS_Float stop_time;
    int nested_level;

public:
    EB_Timer(string tname, int level) {
        nested_level = level;
        name = tname;
        start_time = -1;
        stop_time = -1;
    }

    void start() {
        timer.start();
        start_time = timer.time();
    }

    void stop() {
        stop_time = timer.time();
    }

    EGS_Float getElapsedTime() {
        return timer.time();
    }

    EGS_Float getStartTime() {
        return start_time;
    }

    EGS_Float getStop() {
        return stop_time;
    }

    EGS_Float getDuration() {
        if (stop_time >= 0 && start_time >=0) {
            return stop_time - start_time;
        }
        return -1;
    }

    string getName() {
        return name;
    }

    bool isStopped() {
        return getStop() >=0;
    }

    bool isRunning() {
        return !isStopped();
    }

    int getLevel() {
        return nested_level;
    }

};


class EB_TimingTree {

    vector<EB_Timer *> running_blocks;
    vector<EB_Timer *> stopped_blocks;

    int level;

public:
    EB_TimingTree(): level(0) {};

    ~EB_TimingTree() {
        for (vector<EB_Timer *>::iterator it = running_blocks.begin() ; it != running_blocks.end(); ++it) {
            delete(*it);
        }
        running_blocks.clear();

        for (vector<EB_Timer *>::iterator it = stopped_blocks.begin() ; it != stopped_blocks.end(); ++it) {
            delete(*it);
        }
        stopped_blocks.clear();
    }

    void addTimer(string name) {
        EB_Timer *timer = new EB_Timer(name, level);
        timer->start();
        running_blocks.push_back(timer);
        level++;
    }

    void stopTimer() {
        if (running_blocks.empty()) {
            level = 0;
            return;
        }

        running_blocks.back()->stop();

        vector<EB_Timer *>::iterator back = running_blocks.end();
        --back;
        copy(back, running_blocks.end(), back_inserter(stopped_blocks));
        running_blocks.pop_back();
        if (running_blocks.empty()) {
            level = 0;
        } else {
            level = running_blocks.back()->getLevel()+1;
        }

    }

    void outputInfo() {
        string sep(80, '=');
        egsInformation("\n\nTiming Blocks\n%s\n", sep.c_str());
        for (vector<EB_Timer *>::reverse_iterator it = running_blocks.rbegin(); it != running_blocks.rend(); ++it) {
            string indent(4*((*it)->getLevel()),' ');
            egsInformation("%s%s is still running\n", indent.c_str(), (*it)->getName().c_str());
        }
        for (vector<EB_Timer *>::reverse_iterator it = stopped_blocks.rbegin(); it != stopped_blocks.rend(); ++it) {
            string indent(4*((*it)->getLevel()),' ');

            stringstream output;
            output <<  indent << (*it)->getName();

            stringstream out_time;
            out_time << std::scientific << std::setprecision(2) << (*it)->getDuration() << " CPU s";

            size_t len = output.str().size() + out_time.str().size();
            string padding;
            if (len <= 80) {
                padding = string(80-len, ' ');
            }
            output << padding << out_time.str() ;

            egsInformation("%s\n", output.str().c_str());
        }
    }

};

#endif
