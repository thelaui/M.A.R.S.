/* cannonBalls.cpp

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

# include "Particles/cannonBalls.hpp"

# include "System/timer.hpp"
# include "Media/sound.hpp"

namespace cannonBalls {

    namespace {
        std::list<CannonBall*> activeParticles_;
    }

    CannonBall::CannonBall(Vector2f const& location, Vector2f const& direction):
             Particle(spaceObjects::oCannonBall, location, 15.f, 3.0f, 5.0f) {

        velocity_ = direction*800.f;
    }

    void CannonBall::update() {
        float time = timer::frameTime();
        location_ += velocity_*time;
    }

    void CannonBall::draw() const {
        glColor3f(1.0f, 0.5f, 0.2f);
        glTexCoord2i(0, 0); glVertex2f(location_.x_-radius_, location_.y_-radius_);
        glTexCoord2i(0, 1); glVertex2f(location_.x_-radius_, location_.y_+radius_);
        glTexCoord2i(1, 1); glVertex2f(location_.x_+radius_, location_.y_+radius_);
        glTexCoord2i(1, 0); glVertex2f(location_.x_+radius_, location_.y_-radius_);
    }

    void spawn(Vector2f const& location, Vector2f const& direction) {
        activeParticles_.push_back(new CannonBall(location, direction));
    }

    void draw() {
        for (std::list<CannonBall*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it)
            (*it)->draw();
    }

    void update() {
        std::list<CannonBall*>::iterator it = activeParticles_.begin();
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
        for (std::list<CannonBall*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it)
            delete *it;
        activeParticles_.clear();
    }
}


