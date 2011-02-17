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

# include "System/timer.hpp"
# include "Items/items.hpp"
# include "Items/CannonControl.hpp"
# include "Teams/Team.hpp"
# include "System/settings.hpp"
# include "Players/BotPlayer.hpp"
# include "Teams/teams.hpp"
# include "SpaceObjects/Ball.hpp"
# include "SpaceObjects/Home.hpp"
# include "Zones/TacticalZone.hpp"

# include <cfloat>

BotController::BotController(Player* slave, float strength):
    Controller(slave),
    target_(NULL),
    weaponChangeTimer_(sf::Randomizer::Random(0.f, 0.5f)),
    specialChangeTimer_(sf::Randomizer::Random(0.f, 0.5f)),
    currentJob_(Job::jCharge, 1),
    nextRoutePoint_(FLT_MAX, FLT_MAX),
    toCover_(NULL),
    strength_(strength) {}

void BotController::update() {
    if(aggroTable_.empty()) {
        std::vector<Team*> const& teams = teams::getAllTeams();
        for (std::vector<Team*>::const_iterator it = teams.begin(); it != teams.end(); ++it) {
            if ((*it) != slave_->team()) {
                std::vector<Player*> const& players = (*it)->members();
                for (std::vector<Player*>::const_iterator it = players.begin(); it != players.end(); ++it)
                    aggroTable_.insert(std::make_pair((*it)->ship(), 0.f));
            }
        }
    }

    if (weaponChangeTimer_ > 0)
        weaponChangeTimer_ -= timer::frameTime();

    if (specialChangeTimer_ > 0)
        specialChangeTimer_ -= timer::frameTime();

    performJob();
}

void BotController::performJob() {
    if (ship()->docked_ && (weaponChangeTimer_ < 0.5f || specialChangeTimer_ < 0.5f) && currentJob_.type_ != Job::jKickOutHome) {
        charge();
    }
    else {
        switch (currentJob_.type_) {
            case Job::jAttackTarget:       attackTarget();                                        break;
            case Job::jAttackAny:          attackAny();                                           break;
            case Job::jHeal:               heal();                                                break;
            case Job::jUnfreeze:           unfreeze();                                            break;
            case Job::jGetPUFuel:
              case Job::jGetPUHealth:
              case Job::jGetPUReverse:
              case Job::jGetPUShield:
              case Job::jGetPUSleep:       getPowerUp();                                          break;
            case Job::jKickOutHome:        kickBallOutHome();                                     break;
            case Job::jKickToEnemy:        kickBallToEnemy();                                     break;
            case Job::jWaitForBall:        waitForBall();                                         break;
            case Job::jProtectZone:        protectZone();                                         break;
            case Job::jLand:               land();                                                break;
            case Job::jCharge:             charge();                                              break;
            case Job::jEscape:             escape();                                              break;
            case Job::jGetControl:         getControl();                                          break;
            default:;
        }
    }
}

void BotController::evaluate() {
    checkEnergy();
    checkCloseEnemies();
    checkAggro();
    checkSpecial();
    checkControl();
}

void BotController::applyForJob(std::multimap<Job, std::multimap<short, BotController*> >& jobMap) {
    for (std::multimap<Job, std::multimap<short, BotController*> >::iterator it=jobMap.begin(); it!=jobMap.end(); ++it) {
        short need(0);

        short didThisJobLAstTimeToo(0);
        if (currentJob_.type_ == it->first.type_)
            didThisJobLAstTimeToo = 10;

        if (ship()->getLife() > 0 && ship()->frozen_ <= 0.f) {
            switch (it->first.type_) {
                case Job::jAttackAny: {
                    need = 5;
                    break;
                }
                case Job::jCharge: {
                    if (ship()->docked_) {
                        float life = 100.f - std::pow(ship()->getLife(),5)*0.00000001f;
                        float fuel = 100.f - std::pow(ship()->getFuel(),5)*0.00000001f;
                        need = std::max(life,fuel);
                    }
                    break;
                }
                case Job::jLand: {
                    float life = std::pow(100.f - ship()->getLife(),3)*0.0001f;
                    float fuel = std::pow(100.f - ship()->getFuel(),3)*0.0001f;
                    need = std::max(life,fuel) + didThisJobLAstTimeToo;
                    break;
                }
                case Job::jHeal: {
                    // 20 - 100, based on fragstars and distance
                    if (ship()->currentSpecial_->getType() == specials::sHeal && ship()->fragStars_ > 0) {
                        Ship* target = static_cast<Ship*>(it->first.object_);
                        if (target != ship()) {
                            float dist((target->location()-ship()->location()).length());
                            dist *= 0.1f;
                            dist = 50 - dist;
                            if (dist < 0) dist = 0;
                            need = dist + (10 * ship()->fragStars_) + didThisJobLAstTimeToo;
                        }
                    }
                    break;
                }
                case Job::jUnfreeze: {
                    // 20 - 100, based on distance
                    Ship* target = static_cast<Ship*>(it->first.object_);
                    float dist = ((target->location()-ship()->location()).length());
                    dist *= 0.1f;
                    dist = 100 - dist;
                    if (dist < 0) dist = 0;
                    need = 1 + dist + didThisJobLAstTimeToo;
                    break;
                }
                case Job::jAttackTarget: {
                    // 20 - 100, based on distance and much life
                    Ship* target = static_cast<Ship*>(it->first.object_);
                    float dist = ((target->location()-ship()->location()).length());
                    dist *= 0.1f;
                    dist = 50 - dist;
                    if (dist < 0) dist = 0;
                    float life = ship()->getLife()*0.5f;
                    need = dist + life + didThisJobLAstTimeToo;
                    break;
                }
                case Job::jGetPUShield: case Job::jGetPUHealth: {
                    // 0 - 100, based on distance and few life
                    float dist = ((*static_cast<Vector2f*>(it->first.object_)-ship()->location()).length());
                    dist *= 0.2f;
                    dist = 100 - dist;
                    if (dist < 0) dist = 0;
                    float life = 100 - ship()->getLife();
                    need = dist*life*0.01 + didThisJobLAstTimeToo;
                    break;
                }
                case Job::jGetPUReverse: case Job::jGetPUSleep: {
                    // 0 - 70, based on distance
                    float dist = ((*static_cast<Vector2f*>(it->first.object_)-ship()->location()).length());
                    dist *= 0.3f;
                    dist = 70 - dist;
                    if (dist < 0) dist = 0;
                    need = dist + didThisJobLAstTimeToo;
                    break;
                }
                case Job::jGetPUFuel: {
                    // 0 - 100, based on distance and few fuel
                    float dist = ((*static_cast<Vector2f*>(it->first.object_)-ship()->location()).length());
                    dist *= 0.2f;
                    dist = 100 - dist;
                    if (dist < 0) dist = 0;
                    float fuel = 100 - ship()->getFuel();
                    need = dist*fuel*0.01f + didThisJobLAstTimeToo;
                    break;
                }
                case Job::jKickToEnemy: {
                    // 0 - 100, based on distance and few fuel
                    float dist = ((static_cast<Ball*>(it->first.object_)->location()-ship()->location()).length());
                    dist *= 0.075f;
                    dist = 50 - dist;
                    if (dist < 0) dist = 0;
                    Vector2f ballLocation (calcPath(static_cast<Ball*>(it->first.object_)->location(), false));
                    Vector2f targetPlanetLocation = calcPath(teams::getEnemy(slave_->team())->home()->location(), false);
                    float dir = spaceObjects::isOnLine(ship()->location(), ballLocation-ship()->location(), targetPlanetLocation, teams::getEnemy(slave_->team())->home()->radius()*2.f) ? 50 : 0;
                    need = 1 + dist + dir + didThisJobLAstTimeToo;
                    if (need > 85)
                        need = 85;
                    break;
                }
                case Job::jKickOutHome: {
                    // 0 - 100, based on distance and few fuel
                    float dist = ((static_cast<Ball*>(it->first.object_)->location()-ship()->location()).length());
                    dist *= 0.1f;
                    dist = 50 - dist;
                    if (dist < 0) dist = 0;
                    Vector2f ballLocation (calcPath(static_cast<Ball*>(it->first.object_)->location(), false));
                    Vector2f targetPlanetLocation = calcPath(teams::getEnemy(slave_->team())->home()->location(), false);
                    float dir = spaceObjects::isOnLine(ballLocation, ship()->location() - ballLocation, slave_->team()->home()->location(), slave_->team()->home()->radius()) ? 50 : 0;
                    need = dist + dir + didThisJobLAstTimeToo;
                    break;
                }
                case Job::jProtectZone: {
                    // 0 - 100, based on distance and few fuel
                    float dist = ((static_cast<TacticalZone*>(it->first.object_)->location()-ship()->location()).length());
                    dist *= 0.1f;
                    dist = 100 - dist;
                    if (dist < 0) dist = 0;
                    if (currentJob_.type_ == it->first.type_ && currentJob_.object_ == it->first.object_)
                        didThisJobLAstTimeToo = 10;
                    else
                        didThisJobLAstTimeToo = 0;
                    need = 1 + dist + didThisJobLAstTimeToo;
                    if (need > 60)
                        need = 60;
                    break;
                }
                case Job::jWaitForBall: {
                    // 0 - 100, based on distance and few fuel
                    float dist = (static_cast<Ball*>(it->first.object_)->location()-ship()->location()).length();
                    dist *= 0.1f;
                    dist = 100 - dist;
                    if (dist < 0) dist = 0;
                    need = 10 + dist + didThisJobLAstTimeToo;
                    break;
                }
                case Job::jEscape: {
                    need = 10 + didThisJobLAstTimeToo;
                    CannonControl* control(items::getCannonControl());
                    if (control) {
                        Player* carrier(control->getCarrier());
                        if (carrier == slave_)
                            need = 100;
                    }
                    break;
                }
                case Job::jGetControl: {
                    // 20 - 100, based on distance and much life
                    CannonControl* control(items::getCannonControl());
                    if (control) {
                        float dist = ((control->location()-ship()->location()).length());
                        dist *= 0.1f;
                        dist = 50 - dist;
                        if (dist < 0) dist = 0;
                        float life = ship()->getLife()*0.5f;
                        need = dist + life + didThisJobLAstTimeToo;
                    }
                    break;
                }
                default:;

            }

            if (need > 100)
                need = 100;
        }
        it->second.insert(std::make_pair(need*it->first.priority_, this));
    }
}

void BotController::reset() {
    target_ = NULL;
    weaponChangeTimer_ =  0.f;
    specialChangeTimer_ = 0.f;
    nextRoutePoint_ = Vector2f(FLT_MAX, FLT_MAX);
    aggroTable_.clear();
    currentJob_ = Job(Job::jCharge, 1);
    for (std::map<Ship*, float>::iterator it = aggroTable_.begin(); it != aggroTable_.end(); ++it)
        it->second = 0.f;
}

void BotController::assignJob(Job const& job) {
    currentJob_=job;
}
