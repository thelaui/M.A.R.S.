/* CKTeam.cpp

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

# include "Teams/CKTeam.hpp"

# include "Teams/teams.hpp"
# include "Players/Player.hpp"
# include "Items/items.hpp"
# include "Items/PowerUp.hpp"
# include "SpaceObjects/ships.hpp"
# include "Games/games.hpp"
# include "Items/CannonControl.hpp"

void CKTeam::createJobs() {
    checkEnemies();
    checkPowerUps();
    checkControl();

    for (int i=0; i<botControllers_.size(); ++i) {
        addJob(Job(Job::jLand, 4));
        addJob(Job(Job::jCharge, 4));
    }
}

void CKTeam::checkEnemies() {
    std::vector<Ship*> ships = ships::getShips();
    bool existAny(false);

    for (std::vector<Ship*>::const_iterator it = ships.begin(); it != ships.end(); ++it)
        if ((*it)->getOwner()->team() != this && (*it)->attackable()) {
            existAny = true;
            break;
        }

    if (existAny) {
        for (int i=0; i<botControllers_.size(); ++i)
            addJob(Job(Job::jAttackAny, 60));
    }
    else {
        for (int i=0; i<botControllers_.size(); ++i)
            addJob(Job(Job::jEscape, 6));
    }
}

void CKTeam::checkPowerUps() {
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
                case items::puFuel:     addJob(Job(Job::jGetPUFuel,    70, &powerUpLocations_.back())); break;
                case items::puHealth:   addJob(Job(Job::jGetPUHealth,  70, &powerUpLocations_.back())); break;
                case items::puReverse:  if (existAny) addJob(Job(Job::jGetPUReverse, 70, &powerUpLocations_.back())); break;
                case items::puShield:   addJob(Job(Job::jGetPUShield,  70, &powerUpLocations_.back())); break;
                default:                if (existAny) addJob(Job(Job::jGetPUSleep,   70, &powerUpLocations_.back())); break;
            }
        }
    }
}

void CKTeam::checkControl() {
    CannonControl* control(items::getCannonControl());
    if (control) {
        Player* carrier(control->getCarrier());

        if (carrier) {
            if (carrier->team() == this) {
                addJob(Job(Job::jEscape, 100));
                for (int i=0; i<botControllers_.size()-1; ++i)
                    addJob(Job(Job::jAttackAny, 80));
            }
            else {
                for (int i=0; i<botControllers_.size(); ++i)
                    addJob(Job(Job::jAttackTarget, 80, carrier->ship()));
            }
        }
        else {
            addJob(Job(Job::jGetControl, 100));
        }
    }

}

