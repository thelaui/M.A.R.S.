/* AmmoShotgun.cpp

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

# include "Particles/AmmoShotgun.hpp"

# include "System/timer.hpp"
# include "Media/sound.hpp"
# include "TrailEffects/trailEffects.hpp"
# include "System/randomizer.hpp"

std::list<AmmoShotgun*> AmmoShotgun::activeParticles_;

AmmoShotgun::AmmoShotgun(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Color3f const& color, Player* damageSource):
         Particle<AmmoShotgun>(spaceObjects::oAmmoShotgun, location, 2.5f, 0.5f, randomizer::random(0.3f, 0.7f)),
         color_(1.f, 0.7f, 0.2f) {

    setDamageSource(damageSource);
    velocity_ = direction*1000.f + Vector2f::randDirLen()*400.f;
    location_ += velocity_*timer::frameTime()*1.2f;

    trailEffects::attach(this, 0.02f, 0.1f, 4.f, Color3f(0.3f, 0.2f, 0.05f), false);
}

AmmoShotgun::~AmmoShotgun() {
    trailEffects::detach(this);
}

void AmmoShotgun::update() {
    float time = timer::frameTime();

    physics::collide(this, STATICS | MOBILES);
    Vector2f acceleration = physics::attract(this)*15;

    location_ += velocity_*time + acceleration*time*time;
    velocity_ += acceleration*time - velocity_*time;

    color_.v(-1.f/totalLifeTime_*lifeTime_+1.f);

    lifeTime_ += time;
}

void AmmoShotgun::draw() const {
    color_.gl3f();
    const int posX = 0;
    const int posY = 1;
    glTexCoord2f(posX*0.125f,     posY*0.125f);     glVertex2f(location_.x_-radius_, location_.y_-radius_);
    glTexCoord2f(posX*0.125f,     (posY+1)*0.125f); glVertex2f(location_.x_-radius_, location_.y_+radius_);
    glTexCoord2f((posX+1)*0.125f, (posY+1)*0.125f); glVertex2f(location_.x_+radius_, location_.y_+radius_);
    glTexCoord2f((posX+1)*0.125f, posY*0.125f);     glVertex2f(location_.x_+radius_, location_.y_-radius_);
}

void AmmoShotgun::onCollision(SpaceObject* with, Vector2f const& location,
                        Vector2f const& direction, Vector2f const& velocity) {
    float strength = (velocity-velocity_).length();

    if (strength > 50.f) {

        switch (with->type()) {
            case spaceObjects::oShip:
                sound::playSound(sound::Particle, location, (strength-50)/3);
                break;

            case spaceObjects::oBall:
                sound::playSound(sound::Particle, location, (strength-50)/3);
                break;

            case spaceObjects::oPlanet: case spaceObjects::oHome:
                sound::playSound(sound::Particle, location, (strength-50)/3);
                break;

            case spaceObjects::oSun:
                sound::playSound(sound::Particle, location, (strength-50)/3);
                break;

            default:;
        }
    }
}
