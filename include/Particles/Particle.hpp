/* Particle.hpp

Copyright (c) 2010 - 2011 by Felix Lauer and Simon Schneegans

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
# include "System/timer.hpp"

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

        static void collideWith(MobileSpaceObject* object) {
             // check for collision with each mobile object
            for (typename std::list<Derived*>::iterator it = Derived::activeParticles_.begin(); it != Derived::activeParticles_.end(); ++it) {
                // don't check for self collision
                if (*it != object) {
                    // get faster object
                    MobileSpaceObject *source, *target;
                    if (object->velocity() > (*it)->velocity()) {
                        source = object;
                        target = *it;
                    }
                    else {
                        source = *it;
                        target = object;
                    }

                    const float minDistSquared = std::pow(object->radius() + (*it)->radius(), 2);
                    // if objects are moving
                    if (source->velocity().lengthSquare() > 0) {
                        const Vector2f velocitySourceNorm  ( source->velocity().normalize());
                        const Vector2f velocitySourceOrtho (-velocitySourceNorm.y_, velocitySourceNorm.x_);
                        const Vector2f centerDist          ( velocitySourceOrtho * ((source->location() - target->location())*velocitySourceOrtho));

                        // if path of object crosses target object
                        if (centerDist.lengthSquare() < minDistSquared) {
                            const Vector2f lastFrameLocation = source->location() - source->velocity()*timer::frameTime()*0.6f;
                            const Vector2f chordMidPoint     = target->location() + centerDist;

                            // if path of object has intersected with target within the last frame
                            if ( ((source->location() - target->location()             ).lengthSquare() < minDistSquared)
                              || ((lastFrameLocation - target->location()            ).lengthSquare() < minDistSquared)
                              || ((chordMidPoint - source->location())*(chordMidPoint - lastFrameLocation) < 0.f)) {

                                if (source->velocity()*(target->location() - lastFrameLocation) > 0) {

                                    const Vector2f impactLocation  ((source->location() + target->location())*0.5f);
                                    const Vector2f impactDirection ((target->location() - source->location()).normalize());

                                    // calculate velocity of objects in direction of impact before collision
                                    const Vector2f velocitySourceBefore = impactDirection * (source->velocity() * impactDirection);
                                    const Vector2f velocityTargetBefore = impactDirection * (target->velocity() * impactDirection);

                                    // calculate velocity of objects in direction of impact after collision
                                    const Vector2f velocitySourceAfter = (velocitySourceBefore*source->mass() + velocityTargetBefore*target->mass() - (velocitySourceBefore - velocityTargetBefore)*target->mass()) / (source->mass() + target->mass());
                                    const Vector2f velocityTargetAfter = (velocitySourceBefore*source->mass() + velocityTargetBefore*target->mass() - (velocityTargetBefore - velocitySourceBefore)*source->mass()) / (source->mass() + target->mass());

                                    // calculate collision result
                                    target->onCollision(source, impactLocation, impactDirection, velocitySourceBefore);
                                    source->onCollision(target, impactLocation, impactDirection, velocityTargetBefore);

                                    // add to orthongonal speed component of initial velocity
                                    // special case: Collision with rofle bullets is not physically correct, for improved gameplay
                                    if ((source->type() == spaceObjects::oAmmoROFLE) | (target->type() == spaceObjects::oAmmoROFLE))
                                        target-> velocity() += (0.05f*source->velocity()*source->mass() + (velocityTargetAfter - velocityTargetBefore) * 0.6);
                                    else {
                                        source->velocity() += (velocitySourceAfter - velocitySourceBefore) * 0.8;
                                        target->velocity() += (velocityTargetAfter - velocityTargetBefore) * 0.8;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

    protected:
        /// Stores the targeted life time for ths particle.
        float totalLifeTime_;

        /// Stores the actual life time of the particle.
        float lifeTime_;
};


# endif // PARTICLE_HPP_INCLUDED


