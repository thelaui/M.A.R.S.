/* BotControllerActions.cpp

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

# include "Controllers/BotController.hpp"

# include "SpaceObjects/Ship.hpp"
# include "Games/games.hpp"
# include "System/settings.hpp"
# include "SpaceObjects/balls.hpp"
# include "Players/Player.hpp"
# include "Players/Team.hpp"
# include "System/window.hpp"
# include "Zones/TacticalZone.hpp"
# include "Zones/RasterZone.hpp"

void BotController::charge() {
    Vector2f direction = ship()->location() - slave_->team()->home()->location();
    turnTo(direction + ship()->location());
    slaveFire(false);
    slaveUp(false);
}

void BotController::land() {
    moveTo(slave_->team()->home()->location(), 50.f, true, 0.f, true);
}

void BotController::kickBallToEnemy() {
    Vector2f ballLocation = balls::getBall()->location_;
    if(balls::getBall()->atStart())
        moveTo(ballLocation, 0.f, false, 0.f);
    else {
        Vector2f shipLocation = ship()->location();
        Vector2f ballVelocity = balls::getBall()->velocity_;
        Vector2f targetPlanetLocation = players::getEnemy(slave_->team())->home()->location();
        float    shipRotation = ship()->rotation_*M_PI/180.f;
        Vector2f shipDirection(std::cos(shipRotation), std::sin(shipRotation));
        shipDirection = shipDirection.normalize();
        targetPlanetLocation = calcPath(targetPlanetLocation, false);
        ballLocation = calcPath(ballLocation, false);

        // is ship, ball and target planet in row?
        if (spaceObjects::isOnLine(shipLocation, ballLocation-shipLocation, targetPlanetLocation, players::getEnemy(slave_->team())->home()->radius()*1.5f)) {
            // if so, is the ship correctly oriented
            if (shipDirection*(ballLocation - shipLocation) > 0.8f) {
                // kick ball
                moveTo(targetPlanetLocation, 0.f, false);

                if (ballLocation == balls::getBall()->location_ && !balls::getBall()->atStart())
                    shootPoint(ballLocation);
            }
            else {
                // rotate ship correctly
                turnTo(ballLocation);
            }
        }
        else {
            // move behind the ball
            Vector2f aimPosition = ballLocation + (ballLocation - targetPlanetLocation).normalize()*70 + ballVelocity*0.5f;
            moveTo(aimPosition, 0.2f, true);
        }

        shootEnemy();
    }
}

void BotController::kickBallOutHome() {
    Vector2f shipLocation = ship()->location();
    Vector2f ballLocation = balls::getBall()->location();
    float    shipRotation = ship()->rotation_*M_PI/180;
    Vector2f shipDirection = Vector2f (std::cos(shipRotation), std::sin(shipRotation));
    Vector2f homeLocation = slave_->team()->home()->location();
    shipDirection = shipDirection.normalize();
    Vector2f ballShip = shipLocation - ballLocation;

    // is ship between ball and planet?
    if (spaceObjects::isOnLine(ballLocation, ballShip, homeLocation, slave_->team()->home()->radius())) {
        // wait a bit and face ball
        if (ballShip.lengthSquare() > 2500.f) {
            if (turnTo(ballLocation))
                shootPoint(ballLocation, false);
        }
        else {
            moveTo(shipLocation - ballShip*2.f, 0, false);
            if (spaceObjects::isOnLine(ballLocation, ballShip, shipLocation, 15.f))
                shootPoint(ballLocation, false);
        }
    }
    else {
        // go to planet surface, facing the ball
        Vector2f direction = (ballLocation - homeLocation).normalize();
        direction = direction * (slave_->team()->home()->radius() + ship()->radius()+30.f);
        Vector2f location = slave_->team()->home()->location()+direction;
        moveTo(location, 0.2f, true);
    }
}

void BotController::waitForBall() {
    Vector2f ballLocation = balls::getBall()->location_;
    Vector2f targetPlanetLocation = players::getEnemy(slave_->team())->home()->location();
    targetPlanetLocation = calcPath(targetPlanetLocation, false);
    ballLocation = calcPath(ballLocation, false);

    Vector2f planetBall = (ballLocation - targetPlanetLocation).normalize();
    Vector2f aimPosition = ballLocation + planetBall*50;
    moveTo(aimPosition, 1.5f, true);

    shootEnemy();
}


void BotController::attackTarget() {
    if (target_ != NULL) {
        moveTo(target_->location(), 0.f, true);
        shootEnemy(target_);
        if (target_->getLife() == 0.f) {
            aggroTable_[target_] = 0.f;
            float maxAggro(0.f);
            for (std::map<Ship*, float>::iterator it = aggroTable_.begin(); it != aggroTable_.end(); ++it) {
                if(it->second > maxAggro && it->first->getLife() > 0.f)
                    maxAggro = it->second;
            }

            if(maxAggro == 0.f) {
                float closest(FLT_MAX);
                for (std::map<Ship*, float>::iterator it = aggroTable_.begin(); it != aggroTable_.end(); ++it)
                    if(((it->first->location() - ship()->location()).lengthSquare() < closest) && (it->first->getLife() > 0.f)) {
                        closest = (it->first->location() - ship()->location()).lengthSquare();
                        target_ = it->first;
                    }
                if(closest == FLT_MAX)
                    target_ = NULL;
                else
                    aggroTable_[target_] = 100.f;
            }
            else
                for (std::map<Ship*, float>::iterator it = aggroTable_.begin(); it != aggroTable_.end(); ++it) {
                    if((it->second == maxAggro) && (it->first->getLife() > 0.f)) {
                        it->second = 100.f;
                        target_ = it->first;
                    }
                    else
                        it->second /= (maxAggro/100.f);
            }
        }
    }
}

void BotController::protectZone() {
    if (nextRoutePoint_.x_ == FLT_MAX) {
        toCover_ = zones::toProtect(slave_->team());
        nextRoutePoint_ = toCover_->getRandomPoint();
    }
    if (moveTo(nextRoutePoint_, 0.5f, false, toCover_->radius_ / 4.f + 10.f)) {
        nextRoutePoint_.x_ = FLT_MAX;
    }
}

void BotController::switchToWeapon() {
    slaveFire();
    slaveLeft();
    if(ship()->currentWeapon_->getName() == "FLUBBA")
        slaveLeft();
    slaveFire();
    actions_[BOT_CHANGE_WEAPON] = 0;
}

void BotController::escape() {
    if (nextRoutePoint_.x_ == FLT_MAX)
        nextRoutePoint_ = zones::freeZone()->getRandomPoint();
    if (moveTo(nextRoutePoint_, 0.4f, false, 100.f))
        nextRoutePoint_.x_ = FLT_MAX;
}

void  BotController::startFight() {
    if (nextRoutePoint_.x_ == FLT_MAX)
        nextRoutePoint_ = zones::freeZone()->getRandomPoint();
    if (moveTo(nextRoutePoint_, 0.9f, false, 250.f)) {
        std::vector<Team*> const& enemies = players::getAllTeams();
        if(!enemies.empty()) {
            float distance(FLT_MAX);
            for (std::vector<Team*>::const_iterator it = enemies.begin(); it != enemies.end(); ++it)
                if((*it) != slave_->team()) {
                    std::vector<Player*> const& enemyMembers = (*it)->members();
                    for (std::vector<Player*>::const_iterator it = enemyMembers.begin(); it != enemyMembers.end(); ++it) {
                        if(((*it)->ship()->location() - ship()->location_).lengthSquare() < distance) {
                            distance = ((*it)->ship()->location()-ship()->location_).lengthSquare();
                            target_ = (*it)->ship();
                        }
                    }
                }
            if (target_)
                aggroTable_[target_] = 100.f;
        }
     }
}
