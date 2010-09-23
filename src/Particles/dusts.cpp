/* Dust.cpp

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

# include "Particles/dusts.hpp"

# include "System/timer.hpp"
# include "System/settings.hpp"

namespace dusts {

    namespace {
        std::list<Dust*> activeParticles_;
    }

    Dust::Dust(Vector2f const& location):
               Particle(spaceObjects::oDust, location, 4, 0, sf::Randomizer::Random(0.5f, 1.5f)*settings::C_globalParticleLifeTime/100.f) {

        velocity_ = Vector2f::randDir()*250*sf::Randomizer::Random(1.f, 1.2f)*sf::Randomizer::Random(1.f, 1.2f);

        color_ = Color3f::random();
        color_.s(0.0);
    }

    void Dust::update() {
        float time = timer::frameTime();

        color_.v(-0.5/totalLifeTime_*lifeTime_+0.5);
        // update Size
        radius_ = lifeTime_/totalLifeTime_*5+5;

        location_ = location_ + velocity_*time;
        velocity_ = velocity_ + velocity_*(-2.f)*time;

        lifeTime_ += time;
    }

    void Dust::draw() const {
        color_.gl4f(0.3f);
        const int posX = 1;
        const int posY = 0;
        glTexCoord2f(posX*0.125f,     posY*0.125f);     glVertex2f(location_.x_-radius_, location_.y_-radius_);
        glTexCoord2f(posX*0.125f,     (posY+1)*0.125f); glVertex2f(location_.x_-radius_, location_.y_+radius_);
        glTexCoord2f((posX+1)*0.125f, (posY+1)*0.125f); glVertex2f(location_.x_+radius_, location_.y_+radius_);
        glTexCoord2f((posX+1)*0.125f, posY*0.125f);     glVertex2f(location_.x_+radius_, location_.y_-radius_);
    }

    void spawn(Vector2f const& location) {
        activeParticles_.push_back(new Dust(location));
    }

    void draw() {
        for (std::list<Dust*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it)
            (*it)->draw();
    }

    void update() {
        std::list<Dust*>::iterator it = activeParticles_.begin();
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
        for (std::list<Dust*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it) {
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
        for (std::list<Dust*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it)
            delete *it;
        activeParticles_.clear();
    }
}

