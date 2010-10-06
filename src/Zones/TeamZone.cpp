/* TeamZone.cpp

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

# include "Zones/TeamZone.hpp"
# include "SpaceObjects/SpaceObject.hpp"

bool TeamZone::isInside(SpaceObject const& toBeChecked) const {
    switch(homeSide_) {
        case 0:
            return(toBeChecked.location().x_ < 640.f);
        case 1:
            return(toBeChecked.location().x_ > 641.f);
        default :
            return false;
    }
}

void TeamZone::draw() const {
    switch(homeSide_) {
        case 0:

            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glBegin(GL_QUADS);
                glColor4f (0.f, 0.7f, 0.f, 0.f);
                glVertex2f(0.f, 0.f);
                glVertex2f(0.f, 960.f);
                glColor4f (0.f, 0.7f, 0.f, 0.3f);
                glVertex2f(639.f, 960.f);
                glVertex2f(639.f, 0.f);
            glEnd();

        break;
        case 1:

            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glBegin(GL_QUADS);
                glColor4f (0.f, 0.7f, 0.f, 0.3f);
                glVertex2f(641.f, 0.f);
                glVertex2f(641.f, 960.f);
                glColor4f (0.f, 0.7f, 0.f, 0.f);
                glVertex2f(1280.f, 960.f);
                glVertex2f(1280.f, 0.f);
            glEnd();

        break;
        default :;

    }
}
