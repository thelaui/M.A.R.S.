/* BotControllerDraw.cpp

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

# include "System/settings.hpp"
# include "SpaceObjects/Ship.hpp"
# include "Players/Player.hpp"
# include "Teams/Team.hpp"
# include "SpaceObjects/Home.hpp"

# include <SFML/OpenGL.hpp>

void BotController::draw() {
    if (ship()->getLife() > 0) {
        const Vector2f shipLocation = ship()->location_;

         // draw AI-Paths
        if (settings::C_drawAIPath) {

            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glLineWidth(1);

            glBegin(GL_LINES);
            if (nextPathPoint_ == moveToPoint_) {
                glColor4f(0,1,0, 0.8f);
                glVertex2f(shipLocation.x_, shipLocation.y_);
                glVertex2f(nextPathPoint_.x_, nextPathPoint_.y_);
            }
            else {
                glColor4f(0,1,0, 0.8f);
                glVertex2f(shipLocation.x_, shipLocation.y_);
                glColor4f(1,0,0, 0.8f);
                glVertex2f(nextPathPoint_.x_, nextPathPoint_.y_);
                glVertex2f(nextPathPoint_.x_, nextPathPoint_.y_);
                glColor4f(0,1,0, 0.8f);
                glVertex2f(moveToPoint_.x_, moveToPoint_.y_);
            }
            glEnd();

            glPointSize(8);
            glBegin(GL_POINTS);
            glColor3f(0,1,0);
            Vector2f temp = aimDirection_*50 + shipLocation;
                glVertex2f(temp.x_, temp.y_);
            glEnd();
        }

        // draw Jobs
        if (settings::C_drawBotJobs) {
            switch (currentJob_.type_) {
                case Job::jLand: case Job::jCharge: {
                    Vector2f direction(slave_->team()->home()->location() - shipLocation);
                    decoObjects::drawArrow(shipLocation + direction*0.05f, shipLocation + direction*0.6f, Color3f(0.5f, 0.4f, 0.3f));
                    break;
                }
                case Job::jHeal: {
                    Vector2f direction(static_cast<Ship*>(currentJob_.object_)->location() - shipLocation);
                    decoObjects::drawArrow(shipLocation + direction*0.1f, shipLocation + direction*0.9f, Color3f(0.f, 0.6f, 0.f));
                    break;
                }
                case Job::jUnfreeze: {
                    Vector2f direction(static_cast<Ship*>(currentJob_.object_)->location() - shipLocation);
                    decoObjects::drawArrow(shipLocation + direction*0.1f, shipLocation + direction*0.9f, Color3f(0.2f, 0.2f, 0.6f));
                    break;
                }
                case Job::jAttackTarget: {
                    Vector2f direction(static_cast<Ship*>(currentJob_.object_)->location() - shipLocation);
                    decoObjects::drawArrow(shipLocation + direction*0.1f, shipLocation + direction*0.9f, Color3f(0.6f, 0.f, 0.f));
                    break;
                }
                case Job::jAttackAny: {
                    for (std::map<Ship*, float>::iterator it = aggroTable_.begin(); it != aggroTable_.end(); ++it) {
                        if (it->first == target_) {
                            Vector2f direction(it->first->location() - shipLocation);
                            decoObjects::drawArrow(shipLocation + direction*0.1f, shipLocation + direction*0.9f, Color3f(0.6f, 0.f, 0.f), 8.f);
                        }
                        else {
                            Vector2f direction(it->first->location() - shipLocation);
                            decoObjects::drawArrow(shipLocation + direction*0.1f, shipLocation + direction*0.9f, Color3f(it->second/120.f, it->second/240.f, 0), 8.f*it->second/120.f);
                        }
                    }
                    break;
                }
                case Job::jGetPUFuel: case Job::jGetPUHealth: case Job::jGetPUReverse:
                case Job::jGetPUShield: case Job::jGetPUSleep: {
                    Vector2f direction(*static_cast<Vector2f*>(currentJob_.object_) - shipLocation);
                    decoObjects::drawArrow(shipLocation + direction*0.1f, shipLocation + direction*0.9f, Color3f(0.6f, 0.f, 0.6f));
                    break;
                }
                case Job::jKickToEnemy: {
                    Vector2f direction(static_cast<Ball*>(currentJob_.object_)->location() - shipLocation);
                    decoObjects::drawArrow(shipLocation + direction*0.1f, shipLocation + direction*0.9f, Color3f(0.8f, 0.4f, 0.6f));
                    break;
                }
                case Job::jWaitForBall: {
                    Vector2f direction(static_cast<Ball*>(currentJob_.object_)->location() - shipLocation);
                    decoObjects::drawArrow(shipLocation + direction*0.1f, shipLocation + direction*0.9f, Color3f(0.4f, 0.3f, 0.35f));
                    break;
                }
                case Job::jKickOutHome: {
                    Vector2f direction(static_cast<Ball*>(currentJob_.object_)->location() - shipLocation);
                    decoObjects::drawArrow(shipLocation + direction*0.1f, shipLocation + direction*0.9f, Color3f(0.8f, 0.2f, 0.1f));
                    break;
                }
                case Job::jEscape:
                case Job::jProtectZone: {
                    Vector2f direction(nextRoutePoint_ - shipLocation);
                    decoObjects::drawArrow(shipLocation + direction*0.1f, shipLocation + direction*0.9f, Color3f(0.6f, 0.4f, 0.f));
                    break;
                }

                default:;
            }
        }
    }
}
