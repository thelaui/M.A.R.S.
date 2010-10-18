/* spaceObjects.cpp

Copyright (c) 2010 by Felix Lauer and Simon Schneegans

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

# include "SpaceObjects/spaceObjects.hpp"

# include "SpaceObjects/Planet.hpp"
# include "SpaceObjects/Sun.hpp"
# include "SpaceObjects/BlackHole.hpp"
# include "SpaceObjects/Home.hpp"
# include "SpaceObjects/balls.hpp"
# include "Items/CannonControl.hpp"
# include "Items/items.hpp"

# include <vector>

namespace spaceObjects {

    namespace {
        // objectList contains all homes as well, but for easy accessing
        // they are saved twice
        std::vector<SpaceObject*> objectList_;
        std::vector<Home*>        homeList_;

        Vector2f possiblePlanetLocation(int radius, float minDistance) {
            int tries(0);
            bool newPlanetFits(false);

            while (!newPlanetFits && ++tries < 500) {
                // 100 is min distance between edge and planet
                int randx = rand() % (1280 - 2*(100 + radius)) + 100 + radius;
                int randy = rand() % (800  - 2*(100 + radius)) + 100 + radius;
                Vector2f position(randx, randy);

                // check for collisions with other objects
                newPlanetFits = true;
                for (std::vector<SpaceObject*>::iterator it = objectList_.begin(); it != objectList_.end(); ++it)
                    if (((*it)->location() - position).lengthSquare() < std::pow((*it)->radius() + radius + 150, 2))
                        newPlanetFits = false;
                // check for collisions with balls
                Ball* ball = balls::getBall();
                if (ball) {
                    if ((ball->location() - position).lengthSquare() < std::pow(ball->radius() + radius + 50, 2))
                        newPlanetFits = false;
                }
                // check for collisions with cannoncontrol
                CannonControl* cannonControl = items::getCannonControl();
                if (cannonControl) {
                    if ((cannonControl->location() - position).lengthSquare() < std::pow(cannonControl->radius() + radius + 50, 2))
                        newPlanetFits = false;
                }

                if (newPlanetFits) return position;
            }
            return Vector2f(0,0);
        }
    }

    void update() {
        for (std::vector<SpaceObject*>::iterator it = objectList_.begin(); it != objectList_.end(); ++it) {
            (*it)->update();
        }
    }

    void draw() {
        for (std::vector<SpaceObject*>::iterator it = objectList_.begin(); it != objectList_.end(); ++it) {
            (*it)->draw();
        }
    }

    void addPlanet() {
        int radius = (rand() % 100 + 50);
        Vector2f position = possiblePlanetLocation(radius, 100);
        if (position != Vector2f(0,0)) addPlanet(position, radius);
    }

    void addPlanet(Vector2f const& location, float radius) {
        objectList_.push_back(new Planet(location, radius));
    }

    void addSun() {
        int radius = (rand() % 100 + 50);
        Vector2f position = possiblePlanetLocation(radius, 200);
        if (position != Vector2f(0,0)) objectList_.push_back(new Sun(position, radius));
    }

    void addBlackHole() {
        int radius = (rand() % 100 + 50);
        Vector2f position = possiblePlanetLocation(radius, 200);
        if (position != Vector2f(0,0)) objectList_.push_back(new BlackHole(position, radius));
    }

    Home* addHome(int where, Color3f const& color) {
        Vector2f position;
        switch (where) {
            case HOME_LEFT:  position = Vector2f(-50,     (rand()%550) + 100); break;
            case HOME_RIGHT: position = Vector2f(1280+50, (rand()%550) + 100); break;
            default:         position = possiblePlanetLocation(100, 100);
        }
        return addHome(position, color);
    }

    Home* addHome(Vector2f const& location, Color3f const& color) {
        Home* home = new Home(location, 100, color);
        objectList_.push_back(home);
        homeList_.push_back(home);
        return home;
    }

    std::vector<Home*>const& getHomes() {
        return homeList_;
    }

    std::vector<SpaceObject*> const& getObjects() {
        return objectList_;
    }

    SpaceObject const* getObstacle(Vector2f const& start, Vector2f const& end, bool avoidBall, float minDistance) {
        SpaceObject const* closest(NULL);
        float closestDistance = FLT_MAX;

        for (std::vector<SpaceObject*>::const_iterator it = objectList_.begin(); it != objectList_.end(); ++it) {
            float checkRadius = (*it)->radius();
            // increase radius, when start and end aren't close to object
            if ((start - (*it)->location()).lengthSquare() > std::pow((*it)->radius() + minDistance, 2)
             && (end   - (*it)->location()).lengthSquare() > std::pow((*it)->radius() + minDistance, 2))
                checkRadius += minDistance;

            if (isOnLine(start, end-start, (*it)->location(), checkRadius)) {
                // check if object is in between or endpoint inside of obstacle
                if ((end - start)*(end - (*it)->location()) >= 0.f || (end - (*it)->location()).lengthSquare() < std::pow((*it)->radius(), 2)) {
                    // hacky... should check for distańce to impactlocation, but does not...
                    float distance = (start - (*it)->location()).lengthSquare();
                    if (distance < closestDistance) {
                        closestDistance = distance;
                        closest = *it;
                    }
                }
            }
        }

        Ball* ball = balls::getBall();
        if (avoidBall && ball) {
            float checkRadius = ball->radius();
            // increase radius, when start and end aren't close to object
            if ((start - ball->location()).lengthSquare() > std::pow(ball->radius() + minDistance, 2)
             && (end   - ball->location()).lengthSquare() > std::pow(ball->radius() + minDistance, 2))
                checkRadius += minDistance;

            if (isOnLine(start, end-start, ball->location(), checkRadius)) {
                // check if object is in between
                if ((end - start)*(end - ball->location()) >= 0.f) {
                    // hacky... should check for distańce to impactlocation, but does not...
                    float distance = (start - ball->location()).lengthSquare();
                    if (distance < closestDistance) {
                        closestDistance = distance;
                        closest = ball;
                    }
                }
            }
        }

        return closest;
    }

    bool isOnLine(Vector2f source, Vector2f direction, Vector2f target, float radius) {
        return (direction*(target - source) > 0
                && (std::pow((direction*(target - source))/direction.lengthSquare(), 2) - ((target -source).lengthSquare() - std::pow(radius, 2))/direction.lengthSquare()) > 0);
    }

    void clear() {
        for (std::vector<SpaceObject*>::iterator it = objectList_.begin(); it != objectList_.end(); ++it)
            delete *it;
        objectList_.clear();
        homeList_.clear();
    }
}
