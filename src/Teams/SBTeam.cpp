/* SBTeam.cpp

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

# include "Teams/SBTeam.hpp"

# include "Items/PowerUp.hpp"
# include "Items/items.hpp"
# include "SpaceObjects/balls.hpp"
# include "SpaceObjects/ships.hpp"
# include "SpaceObjects/Ship.hpp"
# include "Players/Player.hpp"
# include "System/settings.hpp"

# include <list>

void SBTeam::createJobs() {
    checkBall();
    checkEnemies();
    checkPowerUps();

     for (int i=0; i<botControllers_.size(); ++i) {
        addJob(Job(Job::jLand, 20));
        addJob(Job(Job::jCharge, 20));
    }
}

void SBTeam::checkEnemies() {
    std::vector<Ship*> ships = ships::getShips();
    bool existAny(false);

    for (std::vector<Ship*>::const_iterator it = ships.begin(); it != ships.end(); ++it)
        if ((*it)->getOwner()->team() != this && (*it)->attackable()) {
            existAny = true;
            break;
        }

    if (existAny)
        for (int i=0; i<botControllers_.size(); ++i)
            addJob(Job(Job::jAttackAny, 5));
}

void SBTeam::checkPowerUps() {
    std::vector<Ship*> ships = ships::getShips();
    bool existAny(false);

    for (std::vector<Ship*>::const_iterator it = ships.begin(); it != ships.end(); ++it)
        if ((*it)->getOwner()->team() != this && (*it)->attackable()) {
            existAny = true;
            break;
        }

    powerUpLocations_.clear();
    std::list<PowerUp*> const& powerUps = items::getPowerUps();
    for (std::list<PowerUp*>::const_iterator it=powerUps.begin(); it!=powerUps.end(); ++it) {
        if (!(*it)->isCollected()) {
            powerUpLocations_.push_back((*it)->location());
            switch ((*it)->type()) {
                case items::puFuel:                   addJob(Job(Job::jGetPUFuel,    30, &powerUpLocations_.back())); break;
                case items::puHealth:                 addJob(Job(Job::jGetPUHealth,  30, &powerUpLocations_.back())); break;
                case items::puReverse:  if (existAny) addJob(Job(Job::jGetPUReverse, 40, &powerUpLocations_.back())); break;
                case items::puShield:                 addJob(Job(Job::jGetPUShield,  30, &powerUpLocations_.back())); break;
                default:                if (existAny) addJob(Job(Job::jGetPUSleep,   40, &powerUpLocations_.back())); break;
            }
        }
    }
}

void SBTeam::checkBall() {
    Ball* ball = balls::getBall();
    if (ball) {
        if (!ball->isVisible()) {
            int waitCount(settings::C_iDumb*(botControllers_.size()+1)/200);
            for (int i=0; i<waitCount; ++i)
                addJob(Job(Job::jWaitForBall, 5, ball));
        }
        else {
            int ballZone(zones::isInside(this, *ball));
            switch (ballZone) {
                case OWN_HOME:
                    for (int i=0; i<botControllers_.size(); ++i)
                        addJob(Job(Job::jKickOutHome, 90, ball));
                    break;

                case OWN_TEAM: {
                    std::map<float, TacticalZone*> zones(zones::toProtect(this));
                    std::map<float, TacticalZone*>::iterator currentZone = zones.begin();
                    int protectJobs(botControllers_.size()*0.5);
                    while(protectJobs > 0) {
                        for (int i=0; i<(protectJobs+1)*0.5; ++i)
                            addJob(Job(Job::jProtectZone, 30,  currentZone->second));
                        protectJobs /= 2;
                        if (++currentZone == zones.end())
                            currentZone = zones.begin();
                    }
                    for (int i=botControllers_.size()*0.5f; i<botControllers_.size(); ++i)
                        addJob(Job(Job::jKickToEnemy, 60, ball));
                    for (int i=0; i<botControllers_.size()*0.5; ++i)
                        addJob(Job(Job::jKickOutHome, 40, ball));

                    break;
                }
                default: {
                    std::map<float, TacticalZone*> zones(zones::toProtect(this));
                    std::map<float, TacticalZone*>::iterator currentZone = zones.begin();
                    int protectJobs(botControllers_.size()*0.4);
                    while(protectJobs > 0) {
                        for (int i=0; i<(protectJobs+1)*0.5; ++i)
                            addJob(Job(Job::jProtectZone, 20,  currentZone->second));
                        protectJobs /= 2;
                        if (++currentZone == zones.end())
                            currentZone = zones.begin();
                    }
                    for (int i=0; i<botControllers_.size()*0.6f; ++i)
                        addJob(Job(Job::jKickToEnemy, 60, ball));
                }
            }
        }
    }
}
