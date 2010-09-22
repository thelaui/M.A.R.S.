/* fragmentFlames.cpp

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

# include "Particles/fragmentFlames.hpp"

# include "System/timer.hpp"
# include "System/settings.hpp"

namespace fragmentFlames {

    namespace {
        std::list<FragmentFlame*> activeParticles_;
    }

    FragmentFlame::FragmentFlame(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity):
               Particle(spaceObjects::oFragmentFlame, location, 1.f, 0.f, sf::Randomizer::Random(0.15f, 0.25f)) {

        Vector2f distortion(Vector2f::randDirLen());
        location_ = location + distortion;
        velocity_ = velocity*0.5f + distortion*20.f;

        radius_ = direction.x_;

        color_.h(50.f);
        color_.v(0.6f);
        color_.s(0.8f);
    }

    void FragmentFlame::update() {
        float time = timer::frameTime();
        // update Color
        color_.h((-1.0/totalLifeTime_*lifeTime_+1)*60+350);
        color_.v(-1.0/totalLifeTime_*lifeTime_+1);

       // radius_ += time*20;

        location_ += velocity_*time;
        velocity_ += velocity_*(-10.f)*time;

        lifeTime_ += time;
    }

    void FragmentFlame::draw() const {
        color_.gl3f();
        glTexCoord2i(0, 0); glVertex2f(location_.x_-radius_, location_.y_-radius_);
        glTexCoord2i(0, 1); glVertex2f(location_.x_-radius_, location_.y_+radius_);
        glTexCoord2i(1, 1); glVertex2f(location_.x_+radius_, location_.y_+radius_);
        glTexCoord2i(1, 0); glVertex2f(location_.x_+radius_, location_.y_-radius_);
    }


    void spawn(Vector2f const& location, Vector2f const& direction, Vector2f const& sourceVelocity) {
        activeParticles_.push_back(new FragmentFlame(location, direction, sourceVelocity));
    }

    void draw() {
        glEnable(GL_TEXTURE_2D);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Fuel));
        glBegin(GL_QUADS);

        for (std::list<FragmentFlame*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it)
            (*it)->draw();

        glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void update() {
        std::list<FragmentFlame*>::iterator it = activeParticles_.begin();
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
        for (std::list<FragmentFlame*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it)
            delete *it;
        activeParticles_.clear();
    }
}

