/* BotControllerMovement.cpp

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

int pathDepth = 0;

void BotController::charge() const {
    Vector2f direction = ship()->location() - slave_->team()->home()->location();
    turnTo(direction + ship()->location());
    slaveFire(false);
    slaveUp(false);
}

void BotController::land() const {
    moveTo(slave_->team()->home()->location(), 5.f, true);
}

void BotController::kickBallToEnemy() const {
    Vector2f shipLocation = ship()->location();
    Vector2f ballLocation = balls::getBall()->location_;
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
            moveTo(targetPlanetLocation, 0, false);

            if (ballLocation == balls::getBall()->location_)
                slaveFire(true);
        }
        else {
            // rotate ship correctly
            turnTo(ballLocation);
        }
    }
    else {
        // move behind the ball
        Vector2f aimPosition = ballLocation + (ballLocation - targetPlanetLocation).normalize()*70 + ballVelocity*0.5f;
        moveTo(aimPosition, 1.f, true);
    }

    shootEnemy();
}

void BotController::kickBallOutHome() const {
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
            turnTo(ballLocation);
            slaveFire(true);
        }
        else {
            moveTo(shipLocation + ballShip*2.f, 0, false);
            slaveFire(true);;
        }
    }
    else {
        // go to planet surface, facing the ball
        Vector2f direction = (ballLocation - homeLocation).normalize();
        direction = direction * (slave_->team()->home()->radius() + ship()->radius()+30.f);
        Vector2f location = slave_->team()->home()->location()+direction;
        moveTo(location, 1.f, true);
    }
}

void BotController::waitForBall() const {
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
        if (target_->getLife() == 0.f)
            target_ = NULL;
    }
}

void BotController::protectZone() {
    TacticalZone* tmp(toCover_);
    toCover_ = zones::toProtect(slave_->team());
    if (tmp != toCover_ || nextRoutePoint_.x_ == FLT_MAX) {
            nextRoutePoint_ = toCover_->getRandomPoint();
    }
    if (moveTo(nextRoutePoint_, 1.5f, false, toCover_->radius_ / 4.f)) {
        nextRoutePoint_.x_ = FLT_MAX;
    }
}

bool BotController::moveTo(Vector2f const& location, float stopFactor, bool avoidBall, float minDistance) const {
    Vector2f targetPoint = calcPath(location, avoidBall);

    Vector2f shipLocation = ship()->location();
    Vector2f shipVelocity = ship()->velocity_;
    float    shipRotation = ship()->rotation_*M_PI/180.f;
    Vector2f shipDirection = Vector2f(std::cos(shipRotation), std::sin(shipRotation)).normalize();

    Vector2f aimDirection;
    if (targetPoint == location) aimDirection = targetPoint - shipLocation - shipVelocity*stopFactor*shipVelocity.length()*0.003f;
    else                         aimDirection = targetPoint - shipLocation - shipVelocity*0.8f*shipVelocity.length()*0.003f;

    aimDirection = aimDirection.normalize();

    turnTo(aimDirection+shipLocation);

    Vector2f velocityInAimDirection = aimDirection*(shipVelocity*aimDirection);
    float distance = (targetPoint-shipLocation).lengthSquare();

    bool accelerate(false);
    if ((velocityInAimDirection.length() < 300.f || shipVelocity.normalize()*aimDirection < 0.f)
     && (distance > 2500.f || (shipVelocity.normalize()*aimDirection < 0.f && distance > 250.f))
     && (spaceObjects::isOnLine(shipLocation, shipDirection, shipLocation+aimDirection*50.f, 10.f)))
           accelerate = true;

    slaveUp(accelerate);

    // draw lines
    if (settings::C_drawAIPath) {

        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glLineWidth(1);

        glBegin(GL_LINES);
        if (targetPoint == location) {
            glColor4f(0,1,0, 0.8f);
            glVertex2f(shipLocation.x_, shipLocation.y_);
            glVertex2f(targetPoint.x_, targetPoint.y_);
        }
        else {
            glColor4f(0,1,0, 0.8f);
            glVertex2f(shipLocation.x_, shipLocation.y_);
            glColor4f(1,0,0, 0.8f);
            glVertex2f(targetPoint.x_, targetPoint.y_);
            glVertex2f(targetPoint.x_, targetPoint.y_);
            glColor4f(0,1,0, 0.8f);
            glVertex2f(location.x_, location.y_);
        }
        glEnd();

        glPointSize(8);
        glBegin(GL_POINTS);
        glColor3f(0,1,0);
        Vector2f temp = aimDirection*50 + shipLocation;
            glVertex2f(temp.x_, temp.y_);
        glEnd();
    }

    return (distance < minDistance * minDistance);
}

void BotController::turnTo(Vector2f const& location) const {
    float    shipRotation = ship()->rotation_*M_PI/180.f;
    Vector2f aimDirection = location - ship()->location();
    if (aimDirection.y_*std::cos(shipRotation)-aimDirection.x_*std::sin(shipRotation) > 0) slaveRight(true);
    else                                                                                   slaveLeft (true);
}

Vector2f BotController::calcPath(Vector2f const& endPoint, bool avoidBall) const {
    // get some useful data...
    Vector2f toEndPoint  = (endPoint - ship()->location()).normalize();
    Vector2f targetPoint =  endPoint;

    if (++pathDepth < 5) {
        // check for collision with planet
        SpaceObject const* obstacle = spaceObjects::getObstacle(ship()->location(), endPoint, avoidBall, 30.f);

        if (obstacle != NULL) {
            // special case: obstacle center is target point
            if (obstacle->location() == endPoint) {
                pathDepth = 0;
                return obstacle->location() - toEndPoint*(obstacle->radius() + 20);
            }

            // calculate alternative point near obstacle
            Vector2f obstacleToStart(ship()->location() - obstacle->location());
            Vector2f normalToPath(-toEndPoint.y_, toEndPoint.x_);
            Vector2f newEndPoint;
            // if path hits obstacle exactly in the center
            if (obstacleToStart * normalToPath == 0)
                newEndPoint = obstacle->location() - toEndPoint*(obstacle->radius() + 40);
            else {
                normalToPath = (normalToPath * (obstacleToStart * normalToPath)).normalize();
                newEndPoint = obstacle->location() + normalToPath*(obstacle->radius() + 40);
            }
            // check whether new endPoint is in an obstacle again... (happens, whenever two obstacles are close to each other)
            SpaceObject const* newObstacle = spaceObjects::getObstacle(ship()->location(), newEndPoint, avoidBall, 30.f);

            // if a new obstacle was found, calculate the midpoint of both
            if (newObstacle != NULL) {
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

void BotController::shootEnemy(Ship* enemyShip) const {
    float    shipRotation = ship()->rotation_*M_PI/180.f;
    Vector2f shipDirection = Vector2f(std::cos(shipRotation), std::sin(shipRotation)).normalize();

    if (enemyShip == NULL) {
        std::vector<Player*>const& enemies = players::getEnemy(slave_->team())->members();
        for (std::vector<Player*>::const_iterator it = enemies.begin(); it != enemies.end(); ++it) {
            Vector2f pathToEnemy = calcPath((*it)->ship()->location(), false);
            if (pathToEnemy == (*it)->ship()->location()) {
                slaveFire(spaceObjects::isOnLine(ship()->location(), shipDirection, pathToEnemy, 10.f));
                break;
            }
        }
    }
    else {
        Vector2f pathToEnemy = calcPath(enemyShip->location(), true);
            if (pathToEnemy == enemyShip->location())
                slaveFire(spaceObjects::isOnLine(ship()->location(), shipDirection, pathToEnemy, 10.f));
    }
}

void BotController::shootBall() const {
    float    shipRotation = ship()->rotation_*M_PI/180.f;
    Vector2f shipDirection = Vector2f(std::cos(shipRotation), std::sin(shipRotation)).normalize();
    Vector2f pathToBall = calcPath(balls::getBall()->location(), false);
    if (pathToBall == balls::getBall()->location())
        slaveFire(spaceObjects::isOnLine(ship()->location(), shipDirection, pathToBall, 20.f));
}

void BotController::switchToWeapon() {
    slaveFire();
    slaveLeft();
    if(ship()->currentWeapon_->getName() == "Flubba")
        slaveLeft();
    slaveFire();
    actions_[BOT_CHANGE_WEAPON] = 0;
}

