/* BotController.cpp

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

# include "Controllers/BotController.hpp"

# include "SpaceObjects/Ship.hpp"
# include "SpaceObjects/ships.hpp"
# include "Zones/zones.hpp"
# include "Players/Player.hpp"
# include "Teams/Team.hpp"
# include "System/settings.hpp"
# include "Items/items.hpp"
# include "Items/CannonControl.hpp"

# include <cmath>

void BotController::checkAggro() {
    if (ship()->collidable()) {
        if(lastFrameLife_ - ship()->getLife() > 0.f && ship()->damageSource()) {
            if(ship()->damageSource() != slave_ && slave_->team() != ship()->damageSource()->team() && ship()->damageSource()->ship()->attackable())
                aggroTable_[ship()->damageSource()->ship()] += (lastFrameLife_ - ship()->getLife()) * 30.f;
        }

        if(lastFrameLife_ != ship()->getLife())
            lastFrameLife_ = ship()->getLife();

        // process aggroTable
        float maxAggro(-1.f);
        for (std::map<Ship*, float>::iterator it = aggroTable_.begin(); it != aggroTable_.end(); ++it) {
            if(it->second > 0.f && !it->first->collidable())
                it->second = 0.f;
            if(it->second > 0.f && it->first != target_)
                it->second -= 5.f;
            if(it->second > maxAggro && it->first->collidable())
                maxAggro = it->second;
        }
        // if there is one enemy with a lot more aggro than the current
        // target, attack this one
        if (maxAggro > 120.f)
            for (std::map<Ship*, float>::iterator it = aggroTable_.begin(); it != aggroTable_.end(); ++it) {
                if(it->second == maxAggro && it->first->attackable()) {
                    it->second = 100.f;
                    target_ = it->first;
                }
                else
                    it->second /= (maxAggro/100.f);
            }

        // normalize aggro to 100 for the target
        if (target_) {
            float targetAggro(aggroTable_[target_]);
            for (std::map<Ship*, float>::iterator it = aggroTable_.begin(); it != aggroTable_.end(); ++it) {
                it->second /= (targetAggro/100.f);
            }
        }
    }
}

void BotController::checkEnergy() {
    if(!ship()->collidable()) {
        for (std::map<Ship*, float>::iterator it = aggroTable_.begin(); it != aggroTable_.end(); ++it)
            it->second = 0.f;
        target_ = NULL;
    }
    else {
        if (!ship()->docked_ && std::max(100 - ship()->getLife(), 100 - ship()->getFuel()) > 30) {
            slave_->team()->addJob(Job(Job::jHeal, std::max(100 - ship()->getLife(), 100 - ship()->getFuel()), ship()));
            slave_->team()->addJob(Job(Job::jHeal, std::max(100 - ship()->getLife(), 100 - ship()->getFuel()), ship()));
        }

        if (ship()->frozen_ > 0) {
            slave_->team()->addJob(Job(Job::jUnfreeze, 90, ship()));
            slave_->team()->addJob(Job(Job::jUnfreeze, 90, ship()));
            slave_->team()->addJob(Job(Job::jUnfreeze, 90, ship()));
        }
    }
}

void BotController::checkSpecial() {
    if (ship()->fragStars_ > 0 && sf::Randomizer::Random(0, 10) == 1)
        switch (ship()->currentSpecial_->getType()) {
            case specials::sHeal:
                if (ship()->getLife() < 10)
                    slaveSpecial(100);
                break;

            case specials::sBlast: {
                int decision(0);
                float blastRadius = ship()->fragStars_*100.f + 50.f;
                blastRadius *= blastRadius;
                std::vector<Ship*> const& ships(ships::getShips());
                for (std::vector<Ship*>::const_iterator it=ships.begin(); it!=ships.end(); ++it) {
                    if ((*it)->collidable() && (*it)->frozen_ <= 0 && (*it) != slave_->ship()) {
                        float distance(((*it)->location_-ship()->location_).lengthSquare());
                        if (distance <= blastRadius) {
                            if ((*it)->owner_->team() == slave_->team())
                                --decision;
                            else ++decision;
                        }
                    }
                }
                if (decision > 0)
                    slaveSpecial(100);
                break;
            }

            default: {
                int decision(0);
                float freezeRadius = ship()->fragStars_*50.f + 150.f;
                freezeRadius *= freezeRadius;
                std::vector<Ship*> const& ships(ships::getShips());
                for (std::vector<Ship*>::const_iterator it=ships.begin(); it!=ships.end(); ++it) {
                    if ((*it)->attackable() && (*it) != slave_->ship()) {
                        float distance(((*it)->location_-ship()->location_).lengthSquare());
                        if (distance <= freezeRadius) {
                            if ((*it)->owner_->team() == slave_->team())
                                --decision;
                            else ++decision;
                        }
                    }
                }
                if (decision > 0)
                    slaveSpecial(100);
                break;
            }
        }
}

void BotController::checkCloseEnemies() {
    std::vector<Ship*> const& ships(ships::getShips());
    for (std::vector<Ship*>::const_iterator it=ships.begin(); it!=ships.end(); ++it) {
        if ((*it)->attackable() && (*it)->owner_->team() != slave_->team()) {
            float aggroGain(90.f - (*it)->getLife()*0.9);
            float distance(((*it)->location_-ship()->location_).length()*0.01f);
            aggroGain -= distance;
            if (aggroGain < 0.f) aggroGain = 0.f;
            aggroGain *= settings::C_iDumb*0.01f;
            aggroTable_[*it] += aggroGain;
        }
    }
}
void BotController::checkControl() {
    CannonControl* control(items::getCannonControl());
    if (control) {
        Player* carrier(control->getCarrier());
        if (carrier == slave_) {
            for (std::map<Ship*, float>::iterator it = aggroTable_.begin(); it != aggroTable_.end(); ++it)
                if (it->second > 0)
                    slave_->team()->addJob(Job(Job::jAttackTarget, it->second, it->first));
        }
    }
}
