/* KeyController.cpp

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

# include "Controllers/KeyController.hpp"

# include "System/window.hpp"
# include "System/settings.hpp"
# include "SpaceObjects/Ship.hpp"
# include "Players/Player.hpp"
# include "Teams/Team.hpp"

KeyController::KeyController(Player* slave):
    Controller(slave) {}

void KeyController::update() const {
    if (type_ == controllers::cPlayer1) {
        slaveUp   (window::isKeyDown(settings::C_playerIup));
        slaveLeft (window::isKeyDown(settings::C_playerIleft));
        slaveRight(window::isKeyDown(settings::C_playerIright));
        slaveFire (window::isKeyDown(settings::C_playerIfire));
        slaveSpecial (window::isKeyDown(settings::C_playerISpecialKey));
    }

    else if (type_ == controllers::cPlayer2) {
        slaveUp   (window::isKeyDown(settings::C_playerIIup));
        slaveLeft (window::isKeyDown(settings::C_playerIIleft));
        slaveRight(window::isKeyDown(settings::C_playerIIright));
        slaveFire (window::isKeyDown(settings::C_playerIIfire));
        slaveSpecial (window::isKeyDown(settings::C_playerIISpecialKey));
    }
}

void KeyController::update(Key const& key) const {
    if (type_ == controllers::cPlayer1 && key.strength_ == 100) {
        if      (key == settings::C_playerIup)    slaveUp();
        else if (key == settings::C_playerIleft)  slaveLeft();
        else if (key == settings::C_playerIright) slaveRight();
        else if (key == settings::C_playerIfire)  slaveFire();
        else if (key == settings::C_playerISpecialKey)  slaveSpecial();
    }

    else if (type_ == controllers::cPlayer2 && key.strength_ == 100) {
        if      (key == settings::C_playerIIup)    slaveUp();
        else if (key == settings::C_playerIIleft)  slaveLeft();
        else if (key == settings::C_playerIIright) slaveRight();
        else if (key == settings::C_playerIIfire)  slaveFire();
        else if (key == settings::C_playerIISpecialKey)  slaveSpecial();
    }
}


void KeyController::evaluate() {
    if (std::max(100 - ship()->getLife(), 100 - ship()->getFuel()) > 30) {
        slave_->team()->addJob(Job(Job::jHeal, std::max(100 - ship()->getLife(), 100 - ship()->getFuel()), ship()));
        slave_->team()->addJob(Job(Job::jHeal, std::max(100 - ship()->getLife(), 100 - ship()->getFuel()), ship()));
    }

    if (ship()->frozen_ > 0) {
        slave_->team()->addJob(Job(Job::jUnfreeze, 90, ship()));
        slave_->team()->addJob(Job(Job::jUnfreeze, 90, ship()));
        slave_->team()->addJob(Job(Job::jUnfreeze, 90, ship()));
    }
}
