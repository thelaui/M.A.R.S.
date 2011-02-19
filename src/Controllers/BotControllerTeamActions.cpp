/* BotControllerTeamActions.cpp

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
# include "Games/games.hpp"
# include "System/settings.hpp"
# include "SpaceObjects/balls.hpp"
# include "SpaceObjects/ships.hpp"
# include "Players/Player.hpp"
# include "Teams/Team.hpp"
# include "System/window.hpp"
# include "Zones/TacticalZone.hpp"
# include "Zones/RasterZone.hpp"
# include "SpaceObjects/Home.hpp"
# include "Teams/teams.hpp"
# include "Items/items.hpp"
# include "Items/CannonControl.hpp"

void BotController::kickBallToEnemy() {
    Vector2f ballLocation = balls::getBall()->location_;
    if(balls::getBall()->atStart()) {
        moveTo(ballLocation, 0.f, false, 0.f);
        ballLocation = calcPath(ballLocation, false);
        if (ballLocation == balls::getBall()->location_)
            shootPoint(ballLocation);
    }
    else {
        Vector2f shipLocation = ship()->location();
        Vector2f ballVelocity = balls::getBall()->velocity_;
        Vector2f targetPlanetLocation = teams::getEnemy(slave_->team())->home()->location();
        float    shipRotation = ship()->rotation_*M_PI/180.f;
        Vector2f shipDirection(std::cos(shipRotation), std::sin(shipRotation));
        shipDirection = shipDirection.normalize();
        targetPlanetLocation = calcPath(targetPlanetLocation, false);
        ballLocation = calcPath(ballLocation, false);

        // is ship, ball and target planet in row?
        if (spaceObjects::isOnLine(shipLocation, ballLocation-shipLocation, targetPlanetLocation, teams::getEnemy(slave_->team())->home()->radius()*1.5f)) {
            // if so, is the ship correctly oriented
            if (shipDirection*(ballLocation - shipLocation) > 0.8f) {
                // kick ball
                moveTo(targetPlanetLocation, 0.f, false);
                if (ballLocation == balls::getBall()->location_)
                    shootPoint(ballLocation);
            }
            else
                turnTo(ballLocation);
        }
        else {
            // move behind the ball
            Vector2f aimPosition = ballLocation + (ballLocation - targetPlanetLocation).normalize()*70 + ballVelocity*0.5f;
            moveTo(aimPosition, 0.2f, true);
        }
        shootEnemies();
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
    Vector2f targetPlanetLocation = teams::getEnemy(slave_->team())->home()->location();
    targetPlanetLocation = calcPath(targetPlanetLocation, false);
    ballLocation = calcPath(ballLocation, false);

    Vector2f planetBall = (ballLocation - targetPlanetLocation).normalize();
    Vector2f aimPosition = ballLocation + planetBall*50;
    moveTo(aimPosition, 1.5f, true);

    shootEnemies();
}


void BotController::attackAny() {
    if (target_ && !target_->attackable()) {
        aggroTable_[target_] = 0.f;
        target_ = NULL;
    }

    if (target_) {
        moveTo(target_->location(), 0.f);
        shootEnemy(target_);
    }
    else {
        std::vector<Ship*> const& ships = ships::getShips();
        float maxDistance(FLT_MAX);
        for (std::vector<Ship*>::const_iterator it = ships.begin(); it != ships.end(); ++it) {
            if ((*it)->owner_->team() != slave_->team() && (*it)->attackable()) {
                float distance = ((*it)->location() - ship()->location_).lengthSquare()*(*it)->getLife();
                if(distance < maxDistance) {
                    maxDistance = distance;
                    target_ = (*it);
                }
            }
        }
        if (target_)
            aggroTable_[target_] = 100.f;
    }
}

void  BotController::attackTarget() {
    if (currentJob_.object_) {
        moveTo(static_cast<Ship*>(currentJob_.object_)->location(), 0.f);
        shootEnemy(static_cast<Ship*>(currentJob_.object_));
    }
}

void BotController::protectZone() {
    if (nextRoutePoint_.x_ == FLT_MAX) {
        toCover_ = static_cast<TacticalZone*>(currentJob_.object_);
        nextRoutePoint_ = toCover_->getRandomPoint();
    }
    if (moveTo(nextRoutePoint_, 0.5f, false, toCover_->radius_ / 4.f + 10.f)) {
        nextRoutePoint_.x_ = FLT_MAX;
    }
}

void BotController::escape() {
    if (nextRoutePoint_.x_ == FLT_MAX)
        nextRoutePoint_ = zones::freeZone()->getRandomPoint();
    if (moveTo(nextRoutePoint_, 0.4f, false, 100.f))
        nextRoutePoint_.x_ = FLT_MAX;
    shootEnemies();
}

void  BotController::heal() {
    if (currentJob_.object_) {
        if (moveTo(static_cast<Ship*>(currentJob_.object_)->location(), 0.f, true, 20.f + 30.f*ship()->fragStars_))
            slaveSpecial(true);
    }
}

void  BotController::unfreeze() {
    if (currentJob_.object_) {
        target_=NULL;
        moveTo(static_cast<Ship*>(currentJob_.object_)->location(), 0.f);
        shootEnemy(static_cast<Ship*>(currentJob_.object_));
    }
}

void BotController::getPowerUp() {
    if (currentJob_.object_) {
        moveTo(*static_cast<Vector2f*>(currentJob_.object_), 1.f);
        shootEnemies();
    }
}

void BotController::getControl() {
    CannonControl* control(items::getCannonControl());
    if (control) {
        moveTo(control->location(), 1.f);
        shootEnemies();
    }
}
