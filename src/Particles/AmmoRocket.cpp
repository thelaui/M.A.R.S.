/* AmmoRocket.cpp

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

# include "Particles/AmmoRocket.hpp"

# include "System/timer.hpp"
# include "Media/sound.hpp"
# include "TrailEffects/trailEffects.hpp"
# include "Particles/particles.hpp"
# include "System/settings.hpp"
# include "SpaceObjects/Ship.hpp"
# include "SpaceObjects/ships.hpp"
# include "Shaders/postFX.hpp"

# include <cfloat>

std::list<AmmoRocket*> AmmoRocket::activeParticles_;

AmmoRocket::AmmoRocket(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Color3f const& color, Player* damageSource):
         Particle<AmmoRocket>(spaceObjects::oAmmoRocket, location, 10.f, 3.0f, FLT_MAX),
         timer_(1.f),
         target_(NULL),
         parent_(damageSource) {

    setDamageSource(damageSource);
    velocity_ = direction*300.f;
    location_ += velocity_*timer::frameTime()*1.2f;

    trailEffects::attach(this, 30, 2.f, 20.f, Color3f(0.6f, 0.2f, 0.f));
}

AmmoRocket::~AmmoRocket() {
    trailEffects::detach(this);
}


void AmmoRocket::update() {
    float const time = timer::frameTime();
    physics::collide(this, STATICS | MOBILES);

    if (target_) {
        if (target_->getLife() == 0.f)
            target_ = NULL;
        else {
            float const speed(velocity_.length());
            velocity_ /= speed;
            Vector2f const toTarget(target_->location() - location_);
            float const angle = toTarget.y_*velocity_.x_ - toTarget.x_*velocity_.y_;
            float const phi(time*5.f);
            if (angle > 0.f)
                velocity_ = Vector2f (std::cos(std::atan2(velocity_.y_, velocity_.x_)+phi), std::sin(std::atan2(velocity_.y_, velocity_.x_)+phi));
            else
                velocity_ = Vector2f (std::cos(std::atan2(velocity_.y_, velocity_.x_)-phi), std::sin(std::atan2(velocity_.y_, velocity_.x_)-phi));
            velocity_ *= speed;
        }
    }

    location_ += velocity_*time;

    Vector2f const faceDirection(velocity_.normalize());

    particles::spawnTimed(80.f/settings::C_globalParticleCount, particles::pFuel, location_-faceDirection*radius_*2.3f, faceDirection, velocity_);
    particles::spawnTimed(5.f/settings::C_globalParticleCount, particles::pHeatJet, location_-faceDirection*radius_*3.f, faceDirection, velocity_);

    lifeTime_ += time;
    timer_ -= time;
    if (timer_ < 0.f) {
        timer_ = 0.5f;
        std::vector<Ship*> const& ships (ships::getShips());
        float closest(FLT_MAX);
        for (std::vector<Ship*>::const_iterator it = ships.begin(); it != ships.end(); ++it) {
            float distance((location_ - (*it)->location()).lengthSquare());
            if ( distance < closest && (*it)->getLife() != 0.f) {
                target_ = *it;
                closest = distance;
            }
        }
    }
}

void AmmoRocket::draw() const {
    glColor3f(1.f, 1.f, 1.f);

    Vector2f direction(velocity_.normalize()*10.f);
    Vector2f normDirection(direction.y_, -1.f*direction.x_);
    const Vector2f topLeft(location_ + direction + normDirection), topRight(location_ + direction - normDirection), bottomLeft(location_ - 3*direction + normDirection), bottomRight(location_ - 3*direction - normDirection);

    const int posX = 0;
    const int posY = (static_cast<int>(lifeTime_*3.f) % 2) * 2 + 3;
    glTexCoord2f(posX*0.125f,    (posY+2)*0.125f); glVertex2f(topLeft.x_, topLeft.y_);
    glTexCoord2f((posX+4)*0.125f,(posY+2)*0.125f); glVertex2f(bottomLeft.x_, bottomLeft.y_);
    glTexCoord2f((posX+4)*0.125f, posY*0.125f);    glVertex2f(bottomRight.x_, bottomRight.y_);
    glTexCoord2f(posX*0.125f,     posY*0.125f);    glVertex2f(topRight.x_, topRight.y_);

    if (target_) {
        glColor3f(1.f, 0.7f, 0.9f);
        const int posX = 5;
        const int posY = 5;
        float const size = std::sin(lifeTime_*5.f)*4.f + 26.f;
        Vector2f const loc(target_->location());
        glTexCoord2f(posX*0.125f,    (posY+3)*0.125f); glVertex2f(loc.x_ - size, loc.y_ - size);
        glTexCoord2f((posX+3)*0.125f,(posY+3)*0.125f); glVertex2f(loc.x_ + size, loc.y_ - size);
        glTexCoord2f((posX+3)*0.125f, posY*0.125f);    glVertex2f(loc.x_ + size, loc.y_ + size);
        glTexCoord2f(posX*0.125f,     posY*0.125f);    glVertex2f(loc.x_ - size, loc.y_ + size);
    }
}

void AmmoRocket::onCollision(SpaceObject* with, Vector2f const& location,
                        Vector2f const& direction, Vector2f const& velocity) {
    particles::spawnMultiple(50, particles::pDust, location_);
    particles::spawnMultiple(20, particles::pExplode, location_);
    particles::spawnMultiple(5, particles::pBurningFragment, location_);
    particles::spawnMultiple(1, particles::pMiniFlame, location_);
    postFX::onExplosion();
    setDamageSource(parent_);
    physics::  causeShockWave(this, 50.f);
    particles::spawn(particles::pShockWave, location_);
    killMe();
}

