/* AmmoBurner.cpp

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

# include "Particles/AmmoBurner.hpp"

# include "System/timer.hpp"
# include "SpaceObjects/ships.hpp"
# include "SpaceObjects/balls.hpp"
# include "SpaceObjects/Ship.hpp"
# include "Media/sound.hpp"
# include "System/randomizer.hpp"

std::list<AmmoBurner*> AmmoBurner::activeParticles_;

AmmoBurner::AmmoBurner(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Color3f const& color, Player* damageSource):
         Particle<AmmoBurner>(spaceObjects::oAmmoBurner, location, 1.f, 0.f, randomizer::random(0.4f, 0.5f)) {

    setDamageSource(damageSource);
    Vector2f distortion(Vector2f::randDirLen()*3.f);
    location_ = location + direction*randomizer::random(0.f, 20.f);
    velocity_ = velocity + direction*700.f + distortion*70.f;

    color_.h(50.f);
    color_.v(0.8f);
    color_.s(0.3f);
}

void AmmoBurner::update() {
    float time = timer::frameTime();
    physics::collide(this, STATICS);
    // update Color
    color_.h((-1.0f/totalLifeTime_*lifeTime_+1.f)*60.f);
    color_.v(-0.8f/totalLifeTime_*lifeTime_+0.8f);
    color_.s(lifeTime_*10.f + 0.3f);

    // update Size
    radius_ = lifeTime_*50 + 2;

    location_ += velocity_*time;
    velocity_ += velocity_*(-4.f)*time;

    lifeTime_ += time;

    // check for collisions with ships
    std::vector<Ship*>const& shipsList = ships::getShips();
    for (std::vector<Ship*>::const_iterator it = shipsList.begin(); it != shipsList.end(); ++it)
        if ((location_ - (*it)->location()).lengthSquare() < std::pow(radius_ + (*it)->radius(), 2) && (*it)->collidable())
            (*it)->onCollision(this, location_, velocity_, velocity_);

    // check for collisions with ball
    Ball* ball = balls::getBall();
    if (ball)
        if ((location_ - ball->location()).lengthSquare() < std::pow(radius_ + ball->radius(), 2))
            ball->onCollision(this, location_, velocity_, velocity_);
}

void AmmoBurner::draw() const {
    color_.gl3f();
    const int posX = 0;
    const int posY = 0;
    glTexCoord2f(posX*0.125f,     posY*0.125f);     glVertex2f(location_.x_-radius_, location_.y_-radius_);
    glTexCoord2f(posX*0.125f,     (posY+1)*0.125f); glVertex2f(location_.x_-radius_, location_.y_+radius_);
    glTexCoord2f((posX+1)*0.125f, (posY+1)*0.125f); glVertex2f(location_.x_+radius_, location_.y_+radius_);
    glTexCoord2f((posX+1)*0.125f, posY*0.125f);     glVertex2f(location_.x_+radius_, location_.y_-radius_);
}

void AmmoBurner::onCollision(SpaceObject* with, Vector2f const& location,
                        Vector2f const& direction, Vector2f const& velocity) {
    float strength = (velocity-velocity_).length();

    if (strength > 50.f) {

        switch (with->type()) {
            case spaceObjects::oShip:
                sound::playSound(sound::LaserCollide, location, (strength-50)/3);
                break;

            case spaceObjects::oBall:
                sound::playSound(sound::LaserCollide, location, (strength-50)/3);
                break;

            case spaceObjects::oPlanet: case spaceObjects::oHome:
                sound::playSound(sound::LaserCollide, location, (strength-50)/3);
                break;

            case spaceObjects::oSun:
                sound::playSound(sound::LaserCollide, location, (strength-50)/3);
                break;

            default:;
        }
    }
    killMe();
}

