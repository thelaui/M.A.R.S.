/* Eruption.cpp

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

# include "Particles/eruptions.hpp"

# include "System/timer.hpp"

namespace eruptions {

    namespace {
        std::list<Eruption*> activeParticles_;
    }

    Eruption::Eruption(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity):
             Particle(spaceObjects::oEruption, location, 1.f, 0.f, sf::Randomizer::Random(1.0f, 2.5f)),
             color_(Color3f(1.0f, 0.9f, 0.2f)) {

        Vector2f distortion(Vector2f::randDir()*sf::Randomizer::Random(0.9f, 1.1f));
        velocity_ = velocity*50.f + distortion*70.f;
    }

    void Eruption::update() {
        // update Color
        color_.v(-1.0f/totalLifeTime_*lifeTime_+1.f);

        float time = timer::frameTime();
        Vector2f acceleration = physics::attract(this);

        location_ += velocity_*time + acceleration*0.5*time*time;
        velocity_ += acceleration*0.5*time + velocity_*-2.f*time;

        lifeTime_ += time;
    }

    void Eruption::draw() const {
        color_.gl3f();
        glVertex2i(static_cast<int>(location_.x_), static_cast<int>(location_.y_));
    }


    void spawn(Vector2f const& location, Vector2f const& direction, Vector2f const& sourceVelocity) {
        activeParticles_.push_back(new Eruption(location, direction, sourceVelocity));
    }

    void draw() {

        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glPointSize(1.f);
        glBegin(GL_POINTS);

        for (std::list<Eruption*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it)
            (*it)->draw();

        glEnd();
    }

    void update() {
        std::list<Eruption*>::iterator it = activeParticles_.begin();
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
        for (std::list<Eruption*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it)
            delete *it;
        activeParticles_.clear();
    }
}
