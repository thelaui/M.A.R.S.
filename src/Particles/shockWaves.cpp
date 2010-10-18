/* shockWaves.cpp

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

# include "Particles/shockWaves.hpp"

# include "System/timer.hpp"
# include "System/settings.hpp"

namespace shockWaves {

    namespace {
        std::list<ShockWave*> activeParticles_;
    }

    ShockWave::ShockWave(Vector2f const& location):
               Particle(spaceObjects::oShockWave, location, 4, 0, sf::Randomizer::Random(0.6f, 1.f)) {
    }

    void ShockWave::update() {
        float time = timer::frameTime();

        // update Size
        radius_ = lifeTime_*700+20;

        lifeTime_ += time;
    }

    void ShockWave::draw() const {
        glColor4f(1.f, 1.f, 1.f, 1.f - lifeTime_/totalLifeTime_);
        glTexCoord2f(0, 0); glVertex2f(location_.x_-radius_, location_.y_-radius_);
        glTexCoord2f(0, 1); glVertex2f(location_.x_-radius_, location_.y_+radius_);
        glTexCoord2f(1, 1); glVertex2f(location_.x_+radius_, location_.y_+radius_);
        glTexCoord2f(1, 0); glVertex2f(location_.x_+radius_, location_.y_-radius_);
    }

    void spawn(Vector2f const& location) {
        activeParticles_.push_back(new ShockWave(location));
    }

    void draw() {
        for (std::list<ShockWave*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it)
            (*it)->draw();
    }

    void update() {
        std::list<ShockWave*>::iterator it = activeParticles_.begin();
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
        for (std::list<ShockWave*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it)
            delete *it;
        activeParticles_.clear();
    }
}



