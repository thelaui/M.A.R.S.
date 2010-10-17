/* AggroBot.cpp

Copyright (c) 2010 by Felix Lauer und Simon Schneegans

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

# include "Controllers/AggroBot.hpp"

# include "SpaceObjects/Ship.hpp"
# include "Zones/zones.hpp"
# include "SpaceObjects/balls.hpp"
# include "Players/Player.hpp"
# include "Players/Team.hpp"

# include <cmath>

void AggroBot::evaluate() {
    checkEnergy();
    checkBall();
    checkHome();
}

void AggroBot::checkEnergy() {
    if(shipDocked()) {
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
    }
}

void AggroBot::checkBall() {
    if (!balls::getBall()->isVisible()) {
        actions_[BOT_KICK_BALL_TE] = 0;
        actions_[BOT_WAIT_FOR_BALL] = 1;
        actions_[BOT_KICK_BALL_OH] = 0;
    }
    else {
        switch (zones::isInside(slave_->team(), *balls::getBall())) {
            case OWN_HOME:
                actions_[BOT_KICK_BALL_TE] = actions_[BOT_KICK_BALL_TE]/2;
                if(zones::isInside(slave_->team(), *this->ship()) == OWN_HOME)
                    actions_[BOT_KICK_BALL_OH] = 100;
                break;
            case OWN_TEAM:
                actions_[BOT_KICK_BALL_TE] = (actions_[BOT_KICK_BALL_TE] + 30)/2;
                actions_[BOT_KICK_BALL_OH] = 0;
                break;
            case ENEMY_TEAM: actions_[BOT_KICK_BALL_TE] = (actions_[BOT_KICK_BALL_TE]+60)/2;  break;
            case ENEMY_HOME: actions_[BOT_KICK_BALL_TE] = (actions_[BOT_KICK_BALL_TE] +80)/2;  break;
            case NO_ZONE:;
        }
        if(balls::getBall()->atStart())
            actions_[BOT_KICK_BALL_TE] = 60;
    }
}

void AggroBot::checkHome() {
    if (slave_->team()->home()->getLife() == 0) {
        actions_[BOT_PROTECT_ZONE] = 100;
    }
}
