/* TutorialZone.cpp

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


# include "Zones/TutorialZone.hpp"

# include "SpaceObjects/SpaceObject.hpp"
# include "SpaceObjects/spaceObjects.hpp"
# include "SpaceObjects/Ship.hpp"
# include "Players/players.hpp"
# include "Players/Player.hpp"
# include "Teams/Team.hpp"
# include <SFML/System.hpp>

# include <cmath>

TutorialZone::TutorialZone(Vector2f const& location, float radius) :
    radius_(radius),
    location_(location) {}

bool TutorialZone::isInside(SpaceObject const& toBeChecked) const {
    return((toBeChecked.location()-location_).lengthSquare() <= radius_*radius_*0.7f);
}

void TutorialZone::draw() const {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glBegin(GL_TRIANGLE_FAN);
         glColor4f(1.f, 0.5f, 0.8f, (std::sin(timer::totalTime()*2.f) + 1.f) * 0.3f + 0.4f);
         glVertex2f(location_.x_, location_.y_);
         glColor4f(1.f, 0.5f, 0.8f, 0.f);
         for (double i=0; i<=2*M_PI; i+=M_PI*0.02)
            glVertex2f( location_.x_ + std::sin(i) * radius_, location_.y_ + std::cos(i) * radius_);
    glEnd();
}


