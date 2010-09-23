/* BotController.cpp

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
# include "System/timer.hpp"
# include "Items/items.hpp"
# include "Items/CannonControl.hpp"

# include <cfloat>

BotController::BotController(Player* slave, controllers::ControlType type):
    Controller(slave, type),
    actions_(10),
    target_(NULL),
    weaponChangeTimer_(sf::Randomizer::Random(0, 1)),
    evaluationTimer_(0.f),
    nextRoutePoint_(FLT_MAX, FLT_MAX),
    toCover_(NULL) {}

void BotController::update() {
    evaluationTimer_ += timer::frameTime();
    if (weaponChangeTimer_ > 0)
        weaponChangeTimer_ -= timer::frameTime();
    if(evaluationTimer_ > 0.2f) {
        evaluationTimer_ = 0.f;
        evaluate();
    }

    int action = BOT_CHARGE;
    for (int i=0; i<actions_.size(); ++i)
        if (actions_[i] > actions_[action])
            action = i;

    switch (action) {

        case BOT_CHARGE:             charge();                                              break;
        case BOT_LAND:               land();                                                break;
        case BOT_KICK_BALL_TE:       kickBallToEnemy();                                     break;
        case BOT_KICK_BALL_OH:       kickBallOutHome();                                     break;
        case BOT_WAIT_FOR_BALL:      waitForBall();                                         break;
        case BOT_ATTACK_TARGET:      attackTarget();                                        break;
        case BOT_PROTECT_ZONE:       protectZone();                                         break;
        case BOT_CHANGE_WEAPON:      switchToWeapon();                                      break;
        case BOT_GET_CANNON_CONTROL: moveTo(items::getCannonControl()->location(), 0.5f);   break;
        case BOT_ESCAPE:             escape();                                              break;
        default:;
    }
}

