/* balls.cpp

Copyright (c) 2010 by Felix Lauer and Simon Schneegans

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

# include "SpaceObjects/balls.hpp"

# include "SpaceObjects/Home.hpp"

namespace balls {
    namespace {
        Ball* ball_(NULL);
    }

    void addBall(Vector2f const& location) {
        // temporary list of all homes
        std::vector<Home*>const& homes = spaceObjects::getHomes();

        if (homes.size() >= 2 && location == Vector2f(0,0)) {
            Vector2f midPoint;
            for (std::vector<Home*>::const_iterator it = homes.begin(); it != homes.end(); ++it)
                midPoint += (*it)->location();
            midPoint /= homes.size();
            ball_ = new Ball(midPoint);
        }
        else
            ball_ = new Ball(location);
    }

    void update() {
        if (ball_)
            ball_->update();
    }

    void draw() {
        if (ball_)
            ball_->draw();
    }

    Ball* getBall() {
        return ball_;
    }

    void clear() {
        if (ball_) {
            delete ball_;
            ball_ = NULL;
        }
    }
}

