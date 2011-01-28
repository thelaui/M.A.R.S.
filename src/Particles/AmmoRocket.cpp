/* AmmoRocket.cpp

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

# include "Particles/AmmoRocket.hpp"

# include "System/timer.hpp"
# include "Media/sound.hpp"
# include "TrailEffects/trailEffects.hpp"
# include "Particles/particles.hpp"
# include "System/settings.hpp"
# include "SpaceObjects/Ship.hpp"
# include "SpaceObjects/ships.hpp"
# include "Shaders/postFX.hpp"
# include "Players/Player.hpp"

# include <cfloat>

std::list<AmmoRocket*> AmmoRocket::activeParticles_;

AmmoRocket::AmmoRocket(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Color3f const& color, Player* damageSource):
         Particle<AmmoRocket>(spaceObjects::oAmmoRocket, location, 10.f, 3.0f, FLT_MAX),
         timer_(1.f),
         target_(NULL),
         parent_(damageSource),
         rotation_(0.f),
         life_(50.f),
         frozen_(0.f),
         visible_(true) {

    setDamageSource(damageSource);
    velocity_ = direction*300.f;
    location_ += velocity_*timer::frameTime()*1.2f;

    trailEffects::attach(this, 0.05, 2.5f, 20.f, Color3f(0.6f, 0.2f, 0.f), false);
}

AmmoRocket::~AmmoRocket() {
    trailEffects::detach(this);
}


void AmmoRocket::update() {
    float const time = timer::frameTime();
    if (timer_ >= 0.5)
        physics::collide(this, STATICS | MOBILES | PARTICLES);
    else
        physics::collide(this, STATICS | MOBILES);

    if (target_) {
        if (target_->getLife() == 0.f)
            target_ = NULL;
        else {
            float const speed(velocity_.length());
            velocity_ /= speed;
            Vector2f const toTarget(target_->location() - location_);
            rotation_ = toTarget.y_*velocity_.x_ - toTarget.x_*velocity_.y_;
            float const phi(time*5.f);
            if (rotation_ > 0.f)
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

    if (life_<=0.f) {
        visible_ = false;
        particles::spawnMultiple(50, particles::pDust, location_);
        particles::spawnMultiple(20, particles::pExplode, location_);
        particles::spawnMultiple(5, particles::pBurningFragment, location_);
        particles::spawnMultiple(1, particles::pMiniFlame, location_);
        postFX::onExplosion();
        setDamageSource(parent_);
        physics::  causeShockWave(damageSource(), location_, 150.f, 300.f, 5.f);
        particles::spawn(particles::pShockWave, location_);
        killMe();
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
    float strength(velocity.length());
    float amount(0.f), unfreeze(0.f);

    switch (with->type()) {
        case spaceObjects::oAmmoFist:
            target_ = NULL;
            timer_ = 0.5f;
            break;

        case spaceObjects::oAmmoAFK47:
            amount = strength*0.02f;
            unfreeze = 1.f;
            break;

        case spaceObjects::oAmmoROFLE:
            amount = life_;
            unfreeze = 10.f;
            break;

        case spaceObjects::oAmmoShotgun:
            amount = strength*0.01f;
            unfreeze = 1.f;
            break;

        case spaceObjects::oAmmoFlubba:
            amount = sf::Randomizer::Random(5.5f, 8.f);
            unfreeze = 10.f;
            break;

        case spaceObjects::oMiniAmmoFlubba:
            amount = sf::Randomizer::Random(1.7f, 3.f);
            break;

        case spaceObjects::oAmmoBurner:
            amount = timer::frameTime();
            unfreeze = 1.f;
            break;

        case spaceObjects::oAmmoRocket:
            unfreeze = 40.f;
            life_ = 0.f;
            break;

        case spaceObjects::oShip: {
            if (totalLifeTime_ <= 0.5) {
                if (dynamic_cast<Ship*>(with) != parent_->ship())
                    amount = life_;
            }
            else
                amount = life_;
            break;
        }

        default:
            amount = life_;
            unfreeze = 10;

    }

    if (frozen_ <= 0)
        life_ -= amount;
    else frozen_ -= unfreeze;
}

