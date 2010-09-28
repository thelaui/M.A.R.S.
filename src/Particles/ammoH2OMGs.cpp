/* ammoH2OMGs.cpp

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

# include "Particles/ammoH2OMGs.hpp"

# include "System/timer.hpp"
# include "System/settings.hpp"
# include "Particles/particles.hpp"

namespace ammoH2OMGs {

    namespace {
        std::list<AmmoH2OMG*> activeParticles_;
    }

    AmmoH2OMG::AmmoH2OMG(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Player* damageSource):
               Particle(spaceObjects::oAmmoH2OMG, location, 8.f, 0.4f, sf::Randomizer::Random(9.f, 11.f)) {

        setDamageSource(damageSource);
        velocity_ = velocity + direction*400.f + Vector2f::randDirLen()*50.f;
        location_ += velocity_*timer::frameTime()*1.2f;

        radius_ = sf::Randomizer::Random(3.f, 6.f);

        color_ = Color3f(sf::Randomizer::Random(0.6f, 0.8f), sf::Randomizer::Random(0.6f, 0.8f), sf::Randomizer::Random(0.9f, 1.0f));
    }

    void AmmoH2OMG::update() {
        float time = timer::frameTime();

        Vector2f acceleration = physics::attract(this);
        physics::collide(this, STATICS | MOBILES);

        // update Size
        if (lifeTime_ > totalLifeTime_-0.3f)
            radius_ = -400.0f*pow(lifeTime_+0.2f-totalLifeTime_, 2)+9.f;

        location_ += velocity_*time + acceleration*time*time*5.f;
        velocity_ += acceleration*time*5.f;

        lifeTime_ += time;

        if (lifeTime_ > totalLifeTime_) {
            particles::spawnMultiple(2, particles::pMud, location_, Vector2f(), Vector2f(), color_);
        }
    }

    void AmmoH2OMG::draw() const {
        color_.gl4f(1.0f);
        const int posX = 5;
        const int posY = 1;
        glTexCoord2f(posX*0.125f,     posY*0.125f);     glVertex2f(location_.x_-radius_, location_.y_-radius_);
        glTexCoord2f(posX*0.125f,     (posY+1)*0.125f); glVertex2f(location_.x_-radius_, location_.y_+radius_);
        glTexCoord2f((posX+1)*0.125f, (posY+1)*0.125f); glVertex2f(location_.x_+radius_, location_.y_+radius_);
        glTexCoord2f((posX+1)*0.125f, posY*0.125f);     glVertex2f(location_.x_+radius_, location_.y_-radius_);
    }

    void AmmoH2OMG::onCollision(SpaceObject* with, Vector2f const& location,
                             Vector2f const& direction, Vector2f const& velocity) {


    }

    void spawn(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Player* damageSource) {
        activeParticles_.push_back(new AmmoH2OMG(location, direction, velocity, damageSource));
    }

    void draw() {
        for (std::list<AmmoH2OMG*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it)
            (*it)->draw();
    }

    void update() {
        std::list<AmmoH2OMG*>::iterator it = activeParticles_.begin();
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
        for (std::list<AmmoH2OMG*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it)
            delete *it;
        activeParticles_.clear();
    }
}



