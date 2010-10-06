/* ammoShotguns.cpp

Copyright (c) 2010 by Felix Lauer und Simon Schneegans

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

# include "Particles/ammoShotguns.hpp"

# include "System/timer.hpp"
# include "Media/sound.hpp"

namespace ammoShotguns {

    namespace {
        std::list<AmmoShotgun*> activeParticles_;
    }

    AmmoShotgun::AmmoShotgun(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Player* damageSource):
             Particle(spaceObjects::oAmmoShotgun, location, 2.f, 0.5f, 1.5f),
             color_(1.f, 0.7f, 0.2f) {

        setDamageSource(damageSource);
        velocity_ = direction*900.f + Vector2f::randDirLen()*270.f;
        location_ += velocity_*timer::frameTime()*1.2f;
    }

    void AmmoShotgun::update() {
        float time = timer::frameTime();

        physics::collide(this, STATICS | MOBILES);
        Vector2f acceleration = physics::attract(this);

        location_ += velocity_*time + acceleration*time*time;
        velocity_ += acceleration*time + velocity_*-0.5*time;

        color_.v(-1.f/totalLifeTime_*lifeTime_+1.f);

        lifeTime_ += time;
    }

    void AmmoShotgun::draw() const {
        color_.gl3f();
        glVertex2f(location_.x_, location_.y_);
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


    void spawn(Vector2f const& location, Vector2f const& direction, Vector2f const& sourceVelocity, Player* damageSource) {
        activeParticles_.push_back(new AmmoShotgun(location, direction, sourceVelocity, damageSource));
    }

    void draw() {

        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glPointSize(2.f);
        glBegin(GL_POINTS);

        for (std::list<AmmoShotgun*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it)
            (*it)->draw();

        glEnd();
    }

    void update() {
        std::list<AmmoShotgun*>::iterator it = activeParticles_.begin();
        while (it != activeParticles_.end()) {
            (*it)->update();
            if ((*it)->isDead()) {
                delete *it;
                it = activeParticles_.erase(it);
            }
            else ++it;
        }
    }

    int count() {
        return activeParticles_.size();
    }

    void clear() {
        for (std::list<AmmoShotgun*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it)
            delete *it;
        activeParticles_.clear();
    }
}


