/* AmmoAFK47.cpp

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

# include "Particles/AmmoAFK47.hpp"

# include "System/timer.hpp"
# include "Media/sound.hpp"

std::list<AmmoAFK47*> AmmoAFK47::activeParticles_;

AmmoAFK47::AmmoAFK47(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Color3f const& color, Player* damageSource):
         Particle<AmmoAFK47>(spaceObjects::oAmmoAFK47, location, 1.f, 0.3f, 2.5f) {

    setDamageSource(damageSource);
    Vector2f distortion(Vector2f::randDir());
    velocity_ = direction*1000.f + distortion*7.f;
    location_ += velocity_*timer::frameTime()*1.2f;
}

void AmmoAFK47::update() {
    float time = timer::frameTime();
    physics::collide(this, STATICS | MOBILES);
    Vector2f acceleration = physics::attract(this)*10.f;

    location_ += velocity_*time + acceleration*time*time;
    velocity_ += acceleration*time;

    lifeTime_ += time;
}

void AmmoAFK47::draw() const {
    glColor3f(1.f, 1.f, 1.f);

    Vector2f direction(velocity_*0.016f);
    Vector2f normDirection(direction.y_, -1.f*direction.x_);
    normDirection *= 0.1f;

    const Vector2f topLeft(location_ + direction + normDirection), topRight(location_ + direction - normDirection), bottomLeft(location_ - 3*direction + normDirection), bottomRight(location_ - 3*direction - normDirection);

    const int posX = 0;
    const int posY = 2;
    glTexCoord2f(posX*0.125f,       posY*0.125f);       glVertex2f(topLeft.x_, topLeft.y_);
    glTexCoord2f(posX*0.125f,      (posY+0.5f)*0.125f); glVertex2f(bottomLeft.x_, bottomLeft.y_);
    glTexCoord2f((posX+3.f)*0.125f,(posY+0.5f)*0.125f); glVertex2f(bottomRight.x_, bottomRight.y_);
    glTexCoord2f((posX+3.f)*0.125f, posY*0.125f);       glVertex2f(topRight.x_, topRight.y_);
}

void AmmoAFK47::onCollision(SpaceObject* with, Vector2f const& location,
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
    if (with->type() != spaceObjects::oMiniAmmoFlubba)
        killMe();
}
