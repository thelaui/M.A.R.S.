/* ammoROFLEs.cpp

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

# include "Particles/ammoROFLEs.hpp"

# include "System/timer.hpp"
# include "Media/sound.hpp"

namespace ammoROFLEs {

    namespace {
        std::list<AmmoROFLE*> activeParticles_;
    }

    AmmoROFLE::AmmoROFLE(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Player* damageSource):
             Particle(spaceObjects::oAmmoROFLE, location, 1.f, 3.0f, 5.0f) {

        setDamageSource(damageSource);
        velocity_ = direction*1300.f;
        location_ += velocity_*timer::frameTime()*1.2f;
    }

    void AmmoROFLE::update() {
        float time = timer::frameTime();
        physics::collide(this, STATICS | MOBILES);
        location_ += velocity_*time;

        lifeTime_ += time;
    }

    void AmmoROFLE::draw() const {
        Vector2f laser0 = location_ - velocity_.normalize()*30.f*2.f;
        Vector2f laser1 = location_ - velocity_.normalize()*30.f;
        //Vector2f laser2 is location_
        Vector2f laser3 = location_ + velocity_.normalize()*30.f;

        glColor3f(0.f, 0.f, 0.f);
        glVertex2f(laser0.x_, laser0.y_);
        glColor3f(1.0f, 0.3f, 0.3f);
        glVertex2f(laser1.x_, laser1.y_);
        glVertex2f(location_.x_, location_.y_);
        glVertex2f(laser1.x_, laser1.y_);
        glVertex2f(location_.x_, location_.y_);
        glColor3f(0.f, 0.f, 0.f);
        glVertex2f(laser3.x_, laser3.y_);
    }

    void AmmoROFLE::onCollision(SpaceObject* with, Vector2f const& location,
                                Vector2f const& direction, Vector2f const& velocity) {
        float strength = (velocity-velocity_).length();

        if (strength > 50.f) {

            switch (with->type()) {
                case spaceObjects::oShip:
                    sound::playSound(sound::SniperShipCollide, location, (strength-50)/3);
                    break;

                case spaceObjects::oBall:
                    sound::playSound(sound::SniperShipCollide, location, (strength-50)/3);
                    break;

                case spaceObjects::oPlanet: case spaceObjects::oHome:
                    sound::playSound(sound::SniperPlanetCollide, location, (strength-50)/3);
                    break;

                case spaceObjects::oSun:
                    sound::playSound(sound::SniperPlanetCollide, location, (strength-50)/3);
                    break;

                default:;
            }
        }
        killMe();
    }


    void spawn(Vector2f const& location, Vector2f const& direction, Vector2f const& sourceVelocity, Player* damageSource) {
        activeParticles_.push_back(new AmmoROFLE(location, direction, sourceVelocity, damageSource));
    }

    void draw() {

        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glLineWidth(2.f);
        glBegin(GL_LINES);

        for (std::list<AmmoROFLE*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it)
            (*it)->draw();

        glEnd();
    }

    void update() {
        std::list<AmmoROFLE*>::iterator it = activeParticles_.begin();
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
        for (std::list<AmmoROFLE*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it)
            delete *it;
        activeParticles_.clear();
    }
}

