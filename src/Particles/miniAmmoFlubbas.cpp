/* miniAmmoFlubbas.cpp

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

# include "Particles/miniAmmoFlubbas.hpp"

# include "System/timer.hpp"
# include "System/settings.hpp"
# include "Particles/particles.hpp"
# include "Media/sound.hpp"

namespace miniAmmoFlubbas {

    namespace {
        std::list<MiniAmmoFlubba*> activeParticles_;
    }

    MiniAmmoFlubba::MiniAmmoFlubba(Vector2f const& location):
               Particle(spaceObjects::oMiniAmmoFlubba, location, 8.f, 1.8f, sf::Randomizer::Random(5.f, 7.f)) {

        velocity_ = Vector2f::randDirLen()*400;

        radius_ = sf::Randomizer::Random(2.f, 3.f);

        color_ = Color3f(sf::Randomizer::Random(0.0f, 0.4f), sf::Randomizer::Random(0.8f, 1.f), sf::Randomizer::Random(0.0f, 0.4f));
    }

    void MiniAmmoFlubba::update() {
        float time = timer::frameTime();

        physics::collide(this, STATICS | MOBILES);

        // update Size
        if (lifeTime_ > totalLifeTime_-0.2f)
            radius_ = -200.0*pow(lifeTime_+0.1-totalLifeTime_, 2)+4;

        location_ = location_ + velocity_*time;
        velocity_ = velocity_ + velocity_*(-8.f)*time;

        lifeTime_ += time;

        if (lifeTime_ > totalLifeTime_) {
            particles::spawnMultiple(2, particles::pMud, location_, Vector2f(), Vector2f(), color_);
            sound::playSound(sound::BlubPop, location_);
        }
    }

    void MiniAmmoFlubba::draw() const {
        color_.gl4f(0.8f);
        const int posX = 4;
        const int posY = 0;
        glTexCoord2f(posX*0.125f,     posY*0.125f);     glVertex2f(location_.x_-radius_, location_.y_-radius_);
        glTexCoord2f(posX*0.125f,     (posY+1)*0.125f); glVertex2f(location_.x_-radius_, location_.y_+radius_);
        glTexCoord2f((posX+1)*0.125f, (posY+1)*0.125f); glVertex2f(location_.x_+radius_, location_.y_+radius_);
        glTexCoord2f((posX+1)*0.125f, posY*0.125f);     glVertex2f(location_.x_+radius_, location_.y_-radius_);
    }

    void MiniAmmoFlubba::onCollision(SpaceObject* with, Vector2f const& location,
                             Vector2f const& direction, Vector2f const& velocity) {
        killMe();
    }

    void spawn(Vector2f const& location) {
        activeParticles_.push_back(new MiniAmmoFlubba(location));
    }

    void draw() {
        for (std::list<MiniAmmoFlubba*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it)
            (*it)->draw();
    }

    void update() {
        std::list<MiniAmmoFlubba*>::iterator it = activeParticles_.begin();
        while (it != activeParticles_.end()) {
            (*it)->update();
            if ((*it)->isDead()) {
                delete *it;
                it = activeParticles_.erase(it);
            }
            else ++it;
        }
    }

    void shockWave(Vector2f const& location, float strength, float radius) {
        for (std::list<MiniAmmoFlubba*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it) {
            Vector2f direction((*it)->location_ - location);
            float distance = direction.length();
            if (distance < radius && direction != Vector2f()) {
                float intensity = radius-distance;
                direction = direction.normalize();
                direction *= intensity;
                (*it)->velocity_ += direction;
            }
        }
    }

    int count() {
        return activeParticles_.size();
    }

    void clear() {
        for (std::list<MiniAmmoFlubba*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it)
            delete *it;
        activeParticles_.clear();
    }
}



