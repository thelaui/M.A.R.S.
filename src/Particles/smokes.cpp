/* Smoke.cpp

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

# include "Particles/smokes.hpp"

# include "System/timer.hpp"
# include "System/settings.hpp"

namespace smokes {

    namespace {
        std::list<Smoke*> activeParticles_;
    }

    Smoke::Smoke(Vector2f const& location, Vector2f const& direction):
               Particle(spaceObjects::oSmoke, location+Vector2f::randDirLen()*2.f, 4, 0, sf::Randomizer::Random(0.8f, 2.0f)*settings::C_globalParticleLifeTime/100.f) {

        color_ = Color3f(0.3, 0.3, 0.3);
    }

    void Smoke::update() {
        float time = timer::frameTime();
        Vector2f acceleration = physics::attract(this);

        color_.v(-0.5f/totalLifeTime_*lifeTime_+0.5f);
        // update Size
        radius_ = lifeTime_*5.f+2;

        location_ = location_ + velocity_*time - 1.f*acceleration*time*time;
        velocity_ = velocity_ - 1.f*acceleration*time + velocity_*(-2)*time;

        lifeTime_ += time;
    }

    void Smoke::draw() const {
        color_.gl4f(0.3);
        glTexCoord2i(0, 0); glVertex2f(location_.x_-radius_, location_.y_-radius_);
        glTexCoord2i(0, 1); glVertex2f(location_.x_-radius_, location_.y_+radius_);
        glTexCoord2i(1, 1); glVertex2f(location_.x_+radius_, location_.y_+radius_);
        glTexCoord2i(1, 0); glVertex2f(location_.x_+radius_, location_.y_-radius_);
    }


    void spawn(Vector2f const& location, Vector2f const& direction) {
        activeParticles_.push_back(new Smoke(location, direction));
    }

    void draw() {
        glEnable(GL_TEXTURE_2D);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Smoke));
        glBegin(GL_QUADS);

        for (std::list<Smoke*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it)
            (*it)->draw();

        glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void update() {
        std::list<Smoke*>::iterator it = activeParticles_.begin();
        while (it != activeParticles_.end()) {
            (*it)->update();
            if ((*it)->isDead()) {
                delete *it;
                it = activeParticles_.erase(it);
            }
            else ++it;
        }
    }

    void shockWave(SpaceObject* source, float strength, float radius) {
        for (std::list<Smoke*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it) {
            Vector2f direction((*it)->location_ - source->location());
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
        activeParticles_.clear();
    }
}

