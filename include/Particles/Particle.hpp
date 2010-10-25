/* Particle.hpp

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

# ifndef PARTICLE_HPP_INCLUDED
# define PARTICLE_HPP_INCLUDED

# include "SpaceObjects/MobileSpaceObject.hpp"

# include <list>

/// Base class for all particles.

template <typename Derived>
class Particle: public MobileSpaceObject {
    public:
        /// Base ctor of a Particle.
        Particle(spaceObjects::ObjectType const& type, Vector2f const& location, float radius, float mass, float totalLifeTime):
                 MobileSpaceObject(type, location, radius, mass),
                 totalLifeTime_(totalLifeTime),
                 lifeTime_(0) {}

        /// Calling this function will make the particle disappear in the next frame.
        /// Its life time is set equal to its total life time - therfore it will be deleted soon.
        void killMe() {
            lifeTime_ = totalLifeTime_;
        }

        /// Returns true, when the particle exist as long as it should.
        bool isDead() {
            return (lifeTime_ >= totalLifeTime_);
        }

        static int count() {
            return Derived::activeParticles_.size();
        }

        static void clear() {
            for (typename std::list<Derived*>::iterator it = Derived::activeParticles_.begin(); it != Derived::activeParticles_.end(); ++it)
                delete *it;
            Derived::activeParticles_.clear();
        }

        static void updateAll() {
            typename std::list<Derived*>::iterator it = Derived::activeParticles_.begin();
            while (it != Derived::activeParticles_.end()) {
                (*it)->update();
                if ((*it)->isDead()) {
                    delete *it;
                    it = Derived::activeParticles_.erase(it);
                }
                else ++it;
            }
        }

        static void drawAll() {
            for (typename std::list<Derived*>::iterator it = Derived::activeParticles_.begin(); it != Derived::activeParticles_.end(); ++it)
                (*it)->draw();
        }

        static void spawn(Vector2f const& location, Vector2f const& direction, Vector2f const& sourceVelocity, Color3f const& color, Player* damageSource) {
            Derived::activeParticles_.push_back(new Derived(location, direction, sourceVelocity, color, damageSource));
        }

    protected:
        /// Stores the targeted life time for ths particle.
        float totalLifeTime_;

        /// Stores the actual life time of the particle.
        float lifeTime_;
};


# endif // PARTICLE_HPP_INCLUDED


