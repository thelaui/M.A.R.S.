/* items.cpp

Copyright (c) 2010 by Felix Lauer und Simon Schneegans

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

# include "Items/items.hpp"

# include "Items/CannonControl.hpp"
# include "SpaceObjects/spaceObjects.hpp"
# include "SpaceObjects/Home.hpp"

namespace items {

    namespace {
        CannonControl* cannonControl_(NULL);
    }
    void update() {
        if (cannonControl_)
            cannonControl_->update();
    }

    void draw() {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::PowerUps));

        if (cannonControl_)
            cannonControl_->draw();

        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void addCannonControl() {
        // temporary list of all homes
        std::vector<Home*>const& homes = spaceObjects::getHomes();

        if (homes.size() >= 2) {
            Vector2f midPoint;
            for (std::vector<Home*>::const_iterator it = homes.begin(); it != homes.end(); ++it)
                midPoint += (*it)->location();
            midPoint /= homes.size();
            cannonControl_ = new CannonControl(midPoint);
        }
        else
            cannonControl_ = new CannonControl(Vector2f());
    }

    CannonControl* getCannonControl() {
        return cannonControl_;
    }

    void clear() {
        if (cannonControl_) {
            delete cannonControl_;
            cannonControl_ = NULL;
        }
    }

}
