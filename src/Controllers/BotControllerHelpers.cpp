/* BotControllerHelpers.cpp

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
# include "Games/games.hpp"
# include "System/settings.hpp"
# include "SpaceObjects/balls.hpp"
# include "Players/Player.hpp"
# include "Teams/Team.hpp"
# include "Teams/teams.hpp"
# include "Particles/AmmoInsta.hpp"
# include "defines.hpp"

# include <cfloat>

int pathDepth = 0;

bool BotController::moveTo(Vector2f const& location, float stopFactor, bool avoidBall, float minDistance, bool goingToLand) {
    moveToPoint_ = location;
    nextPathPoint_ = calcPath(location, avoidBall);

    const Vector2f shipLocation = ship()->location_;
    const Vector2f shipVelocity = ship()->velocity_;
    const float    shipRotation = ship()->rotation_*M_PI/180.f;
    const Vector2f shipDirection = Vector2f(std::cos(shipRotation), std::sin(shipRotation));

    if (nextPathPoint_ == location) aimDirection_ = nextPathPoint_ - shipLocation - shipVelocity*stopFactor*shipVelocity.lengthSquare()*0.00003f;
    else                                aimDirection_ = nextPathPoint_ - shipLocation - shipVelocity*0.8f*shipVelocity.lengthSquare()*0.00003f;

    aimDirection_ = aimDirection_.normalize();

    turnTo(aimDirection_+shipLocation);

    Vector2f velocityInAimDirection = aimDirection_*(shipVelocity*aimDirection_);
    float distance = (nextPathPoint_-shipLocation).lengthSquare();

    int accelerate(0);
    if (velocityInAimDirection.lengthSquare() < 15000.f + strength_* 75000.f || shipVelocity*aimDirection_ < 0.f)
        if (distance > 2500 || (shipVelocity*aimDirection_ < 0.f && (distance > 250.f || !goingToLand)))
            if (spaceObjects::isOnLine(shipLocation, shipDirection, shipLocation+aimDirection_*50.f, 10.f))
                accelerate = 100;

    slaveUp(accelerate);

    return ((location - shipLocation).lengthSquare() < minDistance * minDistance);
}

bool BotController::turnTo(Vector2f const& location) {
    float    shipRotation = ship()->rotation_*M_PI/180.f;
    Vector2f aimDirection_ = location - ship()->location();
    float angle = aimDirection_.y_*std::cos(shipRotation)-aimDirection_.x_*std::sin(shipRotation);
    if (angle > 0) slaveRight(100);
    else           slaveLeft (100);
    return std::abs(angle) < 1.f;
}

Vector2f BotController::calcPath(Vector2f const& endPoint, bool avoidBall) {
    // get some useful data...
    Vector2f toEndPoint  = (endPoint - ship()->location()).normalize();
    Vector2f targetPoint =  endPoint;

    if (++pathDepth < 5) {
        // check for collision with planet
        SpaceObject const* obstacle = spaceObjects::getObstacle(ship()->location(), endPoint, avoidBall, 40.f);

        if (obstacle != NULL) {
            // special case: obstacle center is target point
            if (obstacle->location() == endPoint) {
                pathDepth = 0;
                Vector2f surfacePoint(obstacle->location() - toEndPoint*(obstacle->radius() + 20.f));
                int count(0);
                bool fits(false);
                std::vector<Ship*> const& allShips = ships::getShips();
                while(!fits && ++count < 6) {
                    fits = true;
                    for(std::vector<Ship*>::const_iterator it = allShips.begin(); it != allShips.end(); ++it) {
                        if((*it) != ship() && ((*it)->location() - surfacePoint).lengthSquare() < 225.f) {
                            fits = false;
                            Vector2f rotated;
                            float phi = std::pow(-1.f, count) * ((count+1)/2) * M_PI/6.f;
                            if(toEndPoint.x_ != 0.f)
                                rotated = Vector2f (std::cos(std::atan2(toEndPoint.y_, toEndPoint.x_)+phi), std::sin(std::atan2(toEndPoint.y_, toEndPoint.x_)+phi));
                            else
                                rotated = Vector2f (-std::sin(phi), std::cos(phi));
                            surfacePoint = (obstacle->location() - rotated*(obstacle->radius() + 20.f));
                            break;
                        }
                    }
                    if(surfacePoint.x_<0.f || surfacePoint.x_>SPACE_X_RESOLUTION)
                        fits = false;
                }
                if(surfacePoint.x_>0.f && surfacePoint.x_<SPACE_X_RESOLUTION)
                    return surfacePoint;
                else
                    return obstacle->location() - toEndPoint*(obstacle->radius() + 20.f);
            }

            // calculate alternative point near obstacle
            Vector2f obstacleToStart(ship()->location() - obstacle->location());
            Vector2f normalToPath(-toEndPoint.y_, toEndPoint.x_);
            Vector2f newEndPoint;
            // if path hits obstacle exactly in the center
            if (obstacleToStart * normalToPath == 0.f)
                newEndPoint = obstacle->location() - normalToPath*(obstacle->radius() + 60.f);
            else {
                normalToPath = (normalToPath * (obstacleToStart * normalToPath)).normalize();
                newEndPoint = obstacle->location() + normalToPath*(obstacle->radius() + 60.f);
            }
            // check whether new endPoint is in an obstacle again... (happens, whenever two obstacles are close to each other)
            SpaceObject const* newObstacle = spaceObjects::getObstacle(ship()->location(), newEndPoint, avoidBall, 40.f);

            // if a new obstacle was found, calculate the midpoint of both
            if (newObstacle != NULL && obstacle != newObstacle) {
                Vector2f obst1obst2 = (newObstacle->location() - obstacle->location()).normalize();
                // get points on surface of obstacles
                Vector2f obst1 = obstacle->   location() + obst1obst2*obstacle->   radius();
                Vector2f obst2 = newObstacle->location() - obst1obst2*newObstacle->radius();
                // get midpoint between these
                newEndPoint = (obst1 + obst2) / 2;
            }
            // calculate path to newly found endpoint recursivly
            targetPoint = calcPath(newEndPoint, avoidBall);
        }
    }
    pathDepth = 0;
    return targetPoint;
}

void BotController::shootEnemy(Ship* enemyShip) {
    Vector2f pathToEnemy = calcPath(enemyShip->location(), true);
    if (pathToEnemy == enemyShip->location())
        shootPoint(pathToEnemy);
}

void BotController::shootEnemies() {
    std::vector<Player*>const& enemies = teams::getEnemy(slave_->team())->members();
    for (std::vector<Player*>::const_iterator it = enemies.begin(); it != enemies.end(); ++it) {
        Vector2f pathToEnemy = calcPath((*it)->ship()->location(), false);
        if (pathToEnemy == (*it)->ship()->location()) {
            shootPoint(pathToEnemy);
            break;
        }
    }
}

void BotController::shootPoint(Vector2f const& location, bool avoidTeamMembers) {
    const float    maxDistance  (ship()->currentWeapon_->maxDistance());
    const float    minDistance  (ship()->currentWeapon_->minDistance());
    const float    maxAngle     (ship()->currentWeapon_->maxAngle());
    const float    shipRotation (ship()->rotation_*M_PI/180.f);
    const Vector2f shipDirection(Vector2f(std::cos(shipRotation), std::sin(shipRotation)));

    if (ship()->currentWeapon_->getType() == weapons::wInsta) {
        slaveFire(AmmoInsta::hitsAny(ship()->location() + shipDirection*ship()->radius(), shipDirection, slave_->team()));
    }
    else {
        const float    distance     ((location - ship()->location()).length());

        if(distance < maxDistance && distance > minDistance) {
            if (spaceObjects::isOnLine(ship()->location(), shipDirection, location, maxAngle)) {
                int doShoot(100);
                if(avoidTeamMembers) {
                    std::vector<Player*>const& teamMates = slave_->team()->members();
                    for (std::vector<Player*>::const_iterator it = teamMates.begin(); it != teamMates.end(); ++it) {
                        if (*it != slave_)
                            if(spaceObjects::isOnLine(ship()->location(), location - ship()->location(), (*it)->ship()->location(), 20.f)
                               && ((location - ship()->location()) > ((*it)->ship()->location() - ship()->location()))) {
                                doShoot = 0;
                                break;
                            }
                    }
                }
                slaveFire(doShoot);
            }
        }
    }
}



