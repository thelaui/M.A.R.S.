/* PUHealth.cpp

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

# include "Items/PUHealth.hpp"

# include "SpaceObjects/Ship.hpp"
# include "Particles/particles.hpp"

void PUHealth::draw() const {
    if (!collected_) {
        PowerUp::draw();
    }
}

void PUHealth::refreshLifeTime() {
    lifeTime_ = totalLifeTime_;
    for (std::list<Ship*>::iterator it = ships_.begin(); it != ships_.end(); ++it)
        (*it)->life_ = 200.f;
    // direction is abused for texture coords
    particles::spawnMultiple(5, particles::pPowerUpCollect, location_, Vector2f(0,1));
}

