/* fuels.cpp

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

# include "Particles/fuels.hpp"

# include "System/timer.hpp"

namespace fuels {

    namespace {
        std::list<Fuel*> activeParticles_;
    }

    Fuel::Fuel(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity):
               Particle(spaceObjects::oFuel, location, 1.f, 0.f, sf::Randomizer::Random(0.2f, 0.4f)) {

        Vector2f distortion(Vector2f::randDirLen());
        location_ = location + distortion;
        velocity_ = velocity + direction*(-60.f) + distortion*20.f;

        color_.h(50.f);
        color_.v(0.6f);
        color_.s(0.8f);
    }

    void Fuel::update() {
        float time = timer::frameTime();
        physics::collide(this, STATICS);
        // update Color
        color_.h((-1.f/totalLifeTime_*lifeTime_+1.f)*60.f+350.f);
        color_.v(-0.6f/totalLifeTime_*lifeTime_+0.6f);
        color_.s(lifeTime_/100.f + 0.8f);

        // update Size
        radius_ = lifeTime_*30 + 1;

        location_ += velocity_*time;
        velocity_ += velocity_*(-1.f)*time;

        lifeTime_ += time;
    }

    void Fuel::draw() const {
        color_.gl3f();
        const int posX = 0;
        const int posY = 0;
        glTexCoord2f(posX*0.125f,     posY*0.125f);     glVertex2f(location_.x_-radius_, location_.y_-radius_);
        glTexCoord2f(posX*0.125f,     (posY+1)*0.125f); glVertex2f(location_.x_-radius_, location_.y_+radius_);
        glTexCoord2f((posX+1)*0.125f, (posY+1)*0.125f); glVertex2f(location_.x_+radius_, location_.y_+radius_);
        glTexCoord2f((posX+1)*0.125f, posY*0.125f);     glVertex2f(location_.x_+radius_, location_.y_-radius_);
    }


    void spawn(Vector2f const& location, Vector2f const& direction, Vector2f const& sourceVelocity) {
        activeParticles_.push_back(new Fuel(location, direction, sourceVelocity));
    }

    void draw() {
        for (std::list<Fuel*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it)
            (*it)->draw();
    }

    void update() {
        std::list<Fuel*>::iterator it = activeParticles_.begin();
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
        for (std::list<Fuel*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it)
            delete *it;
        activeParticles_.clear();
    }
}
