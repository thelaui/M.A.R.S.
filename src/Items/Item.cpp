/* Item.cpp

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

# include "Items/Item.hpp"

# include "SpaceObjects/ships.hpp"
# include "SpaceObjects/Ship.hpp"

Ship* Item::update() {
    std::vector<Ship*> const& shipList = ships::getShips();
    for (std::vector<Ship*>::const_iterator it = shipList.begin(); it != shipList.end(); ++it)
        if ((*it)->getLife() > 0.f && ((*it)->location() - location_).lengthSquare() < std::pow(radius_ + (*it)->radius(),2)) {
            collected_ = true;
            return *it;
        }
    return NULL;
}

Vector2f const& Item::location() const {
    return location_;
}

float Item::radius() const {
    return radius_;
}
