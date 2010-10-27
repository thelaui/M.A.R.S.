/* PowerUp.cpp

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

# include "Items/PowerUp.hpp"

# include "SpaceObjects/Ship.hpp"
# include "System/timer.hpp"
# include "Items/items.hpp"

void PowerUp::update() {
    lifeTime_ += timer::frameTime();

    if (lifeTime_ < totalLifeTime_)
        Item::update();
    if (lifeTime_ < M_PI/4.f)
        radius_ = std::sin(lifeTime_ * 2.f) * 15.f;
    else if (lifeTime_ > totalLifeTime_-0.38f)
        radius_ = -400.0*pow(lifeTime_+0.2-totalLifeTime_, 2)+25;
    else
        radius_ = 15.f;

    if (ship_ && ship_->getLife() == 0.f) {
        totalLifeTime_ = 0.f;
    }
}
