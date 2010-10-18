/* TutBot.cpp

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

# include "Controllers/TutBot.hpp"

# include "SpaceObjects/Ship.hpp"
# include "Zones/zones.hpp"
# include "Players/Player.hpp"
# include "Players/Team.hpp"
# include "Items/items.hpp"

# include <cmath>

void TutBot::evaluate() {
    checkEnergy();
}

void TutBot::checkEnergy() {
    if(lastFrameLife_ - ship()->getLife() > 0.f)
        slave_->ship()->life_ -= lastFrameLife_ - ship()->getLife();
    if(lastFrameLife_ != ship()->getLife())
        lastFrameLife_ = ship()->getLife();
    if(shipDocked()) {
        actions_[BOT_LAND] = 0;
        actions_[BOT_CHARGE] = std::max(100 - ship()->getLife(), 100 - ship()->getFuel());  // max 100, min 0
        actions_[BOT_ESCAPE] = 10;
        if (weaponChangeTimer_ <= 0.f) {
            actions_[BOT_CHANGE_WEAPON] = 100;
            if (sf::Randomizer::Random(0, 10) <= 2)
                weaponChangeTimer_ =sf::Randomizer::Random(30.f, 60.f);
        }
    }
    else {
        actions_[BOT_LAND] = std::max(std::pow(100 - ship()->getLife(), 2), std::pow(100 - ship()->getFuel(), 2))/100;  // max 100, min 0
        actions_[BOT_CHARGE] = 0;
        actions_[BOT_ESCAPE] = 80;
    }     // max 100, min 0
}

