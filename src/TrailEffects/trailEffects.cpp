/* trailEffects.cpp

Copyright (c) 2010 - 2011 by Felix Lauer and Simon Schneegans

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option)
any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>. */

# include "TrailEffects/trailEffects.hpp"

# include "TrailEffects/Trail.hpp"

# include <vector>
# include <set>

namespace trailEffects {
    namespace {
        std::vector<Trail*> trails_;
        std::set<SpaceObject*> toBeDetached_;
    }

    void update() {
        std::vector<Trail*>::iterator it = trails_.begin();
        while (it != trails_.end()) {
            if ((*it)->isDead()) {
                delete *it;
                it = trails_.erase(it);
            }
            else {
                if (toBeDetached_.find((*it)->target()) != toBeDetached_.end())
                    (*it)->detach();
                (*it)->update();
                ++it;
            }
        }
        toBeDetached_.clear();
    }

    void draw() {
        for (std::vector<Trail*>::iterator it = trails_.begin(); it != trails_.end(); ++it)
            (*it)->draw();
    }

    void attach(SpaceObject* target, int resolution, float length, float width, Color3f const& color) {
        trails_.push_back(new Trail(target, resolution, length, width, color));
    }

    void detach(SpaceObject* target) {
        toBeDetached_.insert(target);
    }

    int  count() {
        return trails_.size();
    }

    void clear() {
        for (std::vector<Trail*>::iterator it = trails_.begin(); it != trails_.end(); ++it)
            delete *it;
        trails_.clear();
    }
}


