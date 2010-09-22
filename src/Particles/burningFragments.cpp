/* burningFragments.cpp

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

# include "Particles/burningFragments.hpp"

# include "System/timer.hpp"
# include "System/settings.hpp"
# include "Particles/particles.hpp"

namespace burningFragments {

    namespace {
        std::list<BurningFragment*> activeParticles_;
    }

    BurningFragment::BurningFragment(Vector2f const& location):
             Particle(spaceObjects::oBurningFragment, location, 1.f, 0, sf::Randomizer::Random(4.5f, 5.5f)),
             timer1_(0.25f),
             timer2_(0.f) {

        radius_   = sf::Randomizer::Random(1.0f, 5.0f);
        velocity_ = Vector2f::randDir()*sf::Randomizer::Random(400, 600);
    }

    void BurningFragment::update() {
        float time = timer::frameTime();
        Vector2f acceleration = physics::attract(this);
        physics::collide(this, STATICS | MOBILES);

        location_ += velocity_*time + acceleration*time*time*2;
        velocity_ += acceleration*time*2 + velocity_*-0.5f*time;

        if (timer1_ > 0)
            timer1_ -= time;
        else {
            timer1_ = lifeTime_*4.f/settings::C_globalParticleCount;
            particles::spawn(particles::pSmoke, location_, velocity_);
        }
        if (timer2_ > 0)
            timer2_ -= time;
        else {
            timer2_ = lifeTime_/settings::C_globalParticleCount;
            particles::spawn(particles::pFragmentFlame, location_, Vector2f(radius_,0), velocity_);
        }

        lifeTime_ += time;
    }

    void BurningFragment::onCollision(SpaceObject* with, Vector2f const& location,
                                      Vector2f const& direction, Vector2f const& velocity) {

        if (with->type() == spaceObjects::oHome | with->type() == spaceObjects::oSun | with->type() == spaceObjects::oPlanet) {
            particles::spawn(particles::pMiniFlame, location_);
            particles::spawn(particles::pMiniFlame, location_);
            particles::spawn(particles::pMiniFlame, location_);
            particles::spawnMultiple(2, particles::pMud, location, direction, velocity, Color3f(1.f, 0.8f, 0.2f));
        }
        killMe();
    }

    void spawn(Vector2f const& location) {
        activeParticles_.push_back(new BurningFragment(location));
    }

    void draw() {

        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glPointSize(1);
        glBegin(GL_POINTS);

        for (std::list<BurningFragment*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it)
            (*it)->draw();

        glEnd();
    }

    void update() {
        std::list<BurningFragment*>::iterator it = activeParticles_.begin();
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
        for (std::list<BurningFragment*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it)
            delete *it;
        activeParticles_.clear();
    }
}

