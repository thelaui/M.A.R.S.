/* DMBot.cpp

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

# include "Controllers/DMBot.hpp"

# include "SpaceObjects/Ship.hpp"
# include "Zones/zones.hpp"
# include "Players/Player.hpp"

# include <cmath>

void DMBot::evaluate() {
    checkEnergy();

    if (ship()->getLife() > 0.f) {
        checkEnemies();
        checkAggro();
    }
}

void DMBot::checkAggro() {
    float maxAggro(-1.f);
    for (std::map<Ship*, float>::iterator it = aggroTable_.begin(); it != aggroTable_.end(); ++it) {
        if(it->second > 0.f && it->first != target_)
            it->second -= 2.f;
        if(it->second > maxAggro && it->first->getLife() > 0.f)
            maxAggro = it->second;
    }
    if (maxAggro > 120.f)
        for (std::map<Ship*, float>::iterator it = aggroTable_.begin(); it != aggroTable_.end(); ++it) {
            if(it->second == maxAggro) {
                it->second = 100.f;
                target_ = it->first;
            }
            else
                it->second /= (maxAggro/100.f);
        }
}

void DMBot::checkEnergy() {
    if(ship()->getLife() == 0.f) {
        for (std::map<Ship*, float>::iterator it = aggroTable_.begin(); it != aggroTable_.end(); ++it)
            it->second = 0.f;
        target_ = NULL;
    }
    else if(shipDocked()) {
        actions_[BOT_LAND] = 0;
        actions_[BOT_CHARGE] = std::max(100 - ship()->getLife(), 100 - ship()->getFuel());  // max 100, min 0
        if (weaponChangeTimer_ <= 0.f) {
            actions_[BOT_CHANGE_WEAPON] = 100;
            if (sf::Randomizer::Random(0, 10) <= 2)
                weaponChangeTimer_ =sf::Randomizer::Random(30.f, 60.f);
        }
    }
    else {
        actions_[BOT_LAND] = std::max(std::pow(100 - ship()->getLife(), 2), std::pow(100 - ship()->getFuel(), 2))/100;  // max 100, min 0
        actions_[BOT_CHARGE] = 0;
    }     // max 100, min 0
}

void DMBot::checkEnemies() {
    if(lastFrameLife_ - ship()->getLife() > 0.f) {
        if(ship()->damageSource() != slave_ && ship()->damageSource()->ship() != target_
           && slave_->team() != ship()->damageSource()->team())
            aggroTable_[ship()->damageSource()->ship()] += (lastFrameLife_ - ship()->getLife()) * 30.f;
    }
    if(lastFrameLife_ != ship()->getLife())
        lastFrameLife_ = ship()->getLife();

    if(target_!=NULL) {
        if(shipDocked())
            actions_[BOT_ATTACK_TARGET] = 10;
        else
            actions_[BOT_ATTACK_TARGET] = 80;
    }
    else {
        actions_[BOT_ATTACK_TARGET] = 0;
        actions_[BOT_START_FIGHT] = 10;
    }
}
