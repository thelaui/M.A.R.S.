/* CKBot.cpp

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

# include "Controllers/CKBot.hpp"

# include "SpaceObjects/Ship.hpp"
# include "Zones/zones.hpp"
# include "Players/Player.hpp"
# include "Players/Team.hpp"

# include <cmath>

void CKBot::evaluate() {
    checkEnergy();
    checkCannonController();
}

void CKBot::checkEnergy() {
    if(shipDocked()) {
        actions_[BOT_LAND] = 0;
        actions_[BOT_CHARGE] = std::max(100 - ship()->getLife(), 100 - ship()->getFuel());  // max 100, min 0
    }
    else {
        actions_[BOT_LAND] = std::max(std::pow(100 - ship()->getLife(), 2), std::pow(100 - ship()->getFuel(), 2))/100;  // max 100, min 0
        actions_[BOT_CHARGE] = 0;
    };     // max 100, min 0
}

void CKBot::checkCannonController() {
    std::vector<Player*>const& enemies = players::getEnemy(slave_->team())->members();
    for (std::vector<Player*>::const_iterator it = enemies.begin(); it != enemies.end(); ++it)
        if ((*it)->ship()->getCollectedItems()[0] == true) {
            target_ = (*it)->ship();
            actions_[BOT_ATTACK_TARGET] = 80;
            break;
        }
}
