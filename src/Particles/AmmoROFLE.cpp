/* AmmoROFLE.cpp

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

# include "Particles/AmmoROFLE.hpp"

# include "System/timer.hpp"
# include "Media/sound.hpp"
# include "TrailEffects/trailEffects.hpp"

std::list<AmmoROFLE*> AmmoROFLE::activeParticles_;

AmmoROFLE::AmmoROFLE(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Color3f const& color, Player* damageSource):
         Particle<AmmoROFLE>(spaceObjects::oAmmoROFLE, location, 1.f, 3.0f, 3.0f) {

    setDamageSource(damageSource);
    velocity_ = direction*1300.f;
    location_ += velocity_*timer::frameTime()*1.2f;

    trailEffects::attach(this, 0.05, 1.f, 4.f, Color3f(1.f, 0.2f, 0.f), false);
}

AmmoROFLE::~AmmoROFLE() {
    trailEffects::detach(this);
}


void AmmoROFLE::update() {
    float time = timer::frameTime();
    physics::collide(this, STATICS | MOBILES);
    location_ += velocity_*time;

    lifeTime_ += time;

    if (location_.x_ < -100 || location_.x_ > 1280 + 100 || location_.y_ < -100 || location_.y_ > 800 + 100) {
        killMe();
    }
}

void AmmoROFLE::draw() const {
    glColor3f(1.f, 1.f, 1.f);

    Vector2f direction(velocity_*0.025f);
    Vector2f normDirection(direction.y_, -1.f*direction.x_);
    normDirection *= 0.05f;

    const Vector2f topLeft(location_ + direction + normDirection), topRight(location_ + direction - normDirection), bottomLeft(location_ - 3*direction + normDirection), bottomRight(location_ - 3*direction - normDirection);

    const int posX = 0;
    const float posY = 2.5f;
    glTexCoord2f(posX*0.125f,       posY*0.125f);       glVertex2f(topLeft.x_, topLeft.y_);
    glTexCoord2f(posX*0.125f,      (posY+0.5f)*0.125f); glVertex2f(bottomLeft.x_, bottomLeft.y_);
    glTexCoord2f((posX+3.f)*0.125f,(posY+0.5f)*0.125f); glVertex2f(bottomRight.x_, bottomRight.y_);
    glTexCoord2f((posX+3.f)*0.125f, posY*0.125f);       glVertex2f(topRight.x_, topRight.y_);
}

void AmmoROFLE::onCollision(SpaceObject* with, Vector2f const& location,
                        Vector2f const& direction, Vector2f const& velocity) {
    float strength = (velocity-velocity_).length();

    if (strength > 50.f) {

        switch (with->type()) {
            case spaceObjects::oShip:
                sound::playSound(sound::SniperShipCollide, location, (strength-50)/3);
                break;

            case spaceObjects::oBall:
                sound::playSound(sound::SniperShipCollide, location, (strength-50)/3);
                break;

            case spaceObjects::oPlanet: case spaceObjects::oHome:
                sound::playSound(sound::SniperPlanetCollide, location, (strength-50)/3);
                break;

            case spaceObjects::oSun:
                sound::playSound(sound::SniperPlanetCollide, location, (strength-50)/3);
                break;

            default:;
        }
    }
    killMe();
}


