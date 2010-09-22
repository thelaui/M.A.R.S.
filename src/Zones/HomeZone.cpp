/* HomeZone.cpp

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

# include "Zones/HomeZone.hpp"

# include "SpaceObjects/SpaceObject.hpp"
# include "SpaceObjects/Ship.hpp"
# include "Players/players.hpp"
# include "Players/Player.hpp"
# include "Players/Team.hpp"
# include <cmath>

HomeZone::HomeZone(Vector2f const& location) :
        Zone(),
        radius_(350.f),
        location_(location) {
        if (location_.x_ < 640.f)
            homeSide_ = 0;
        else
            homeSide_ = 1;
        }

bool HomeZone::isInside(SpaceObject const& toBeChecked) const {
    return((toBeChecked.location()-location_).lengthSquare() <= radius_*radius_);
}

void HomeZone::draw() const {
    glColor4f(0.f, 0.f, 0.f, 0.f);

    glBlendFunc(GL_ONE, GL_ONE);
    glBegin(GL_TRIANGLE_FAN);
         glVertex2f(location_.x_, location_.y_);
         glColor4f(0.f, 0.f, 0.7f, 0.6f);
         for (double i=0; i<=2*M_PI; i+=M_PI*0.02)
            glVertex2f( location_.x_ + std::sin(i) * radius_, location_.y_ + std::cos(i) * radius_);
    glEnd();

}
