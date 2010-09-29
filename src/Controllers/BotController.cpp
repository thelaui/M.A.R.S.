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
# include "Players/Team.hpp"

# include <cfloat>

BotController::BotController(Player* slave, controllers::ControlType type):
    Controller(slave, type),
    actions_(11),
    target_(NULL),
    weaponChangeTimer_(sf::Randomizer::Random(0, 1)),
    evaluationTimer_(0.f),
    nextRoutePoint_(FLT_MAX, FLT_MAX),
    toCover_(NULL) {}

void BotController::update() {
    if(aggroTable_.empty()) {
        std::vector<Team*> const& teams = players::getAllTeams();
        for (std::vector<Team*>::const_iterator it = teams.begin(); it != teams.end(); ++it) {
            if ((*it) != slave_->team()) {
                std::vector<Player*> const& players = (*it)->members();
                for (std::vector<Player*>::const_iterator it = players.begin(); it != players.end(); ++it)
                    aggroTable_.insert(std::make_pair((*it)->ship(), 0.f));
            }
        }
    }
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
        case BOT_START_FIGHT:        startFight();                                          break;
        default:;
    }
}

void BotController::reset() {
    target_ = NULL;
    weaponChangeTimer_ = sf::Randomizer::Random(0, 1);
    nextRoutePoint_ = Vector2f(FLT_MAX, FLT_MAX);
    aggroTable_.clear();
    for(int i=0; i<actions_.size(); ++i)
        actions_[i] = 0;
    for (std::map<Ship*, float>::iterator it = aggroTable_.begin(); it != aggroTable_.end(); ++it)
        it->second = 0.f;
}
