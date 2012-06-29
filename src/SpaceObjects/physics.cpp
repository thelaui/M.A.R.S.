/* physics.cpp

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

# include "SpaceObjects/physics.hpp"

# include "SpaceObjects/SpaceObject.hpp"
# include "SpaceObjects/MobileSpaceObject.hpp"
# include "Particles/particles.hpp"
# include "System/timer.hpp"

# include <vector>
# include <cmath>

namespace physics {

    namespace {
        std::vector<SpaceObject*>       staticObjects_;
        std::vector<MobileSpaceObject*> mobileObjects_;
        std::vector<SpaceObject*>       gravitySources_;
    }

    void collide (MobileSpaceObject* object, int with) {
        // collision with planets
        if (with & STATICS) {
            // check for collision with each static object
            for (std::vector<SpaceObject*>::iterator it = staticObjects_.begin(); it != staticObjects_.end(); ++it) {
                // if object touches obstacle
                if (((*it)->location_ - object->location_).lengthSquare() < std::pow((*it)->radius_ + object->radius_, 2)) {
                    const Vector2f impactDirection = ((*it)->location_ - object->location_).normalize();
                    object->location_ = (*it)->location_ - impactDirection*((*it)->radius_ + object->radius_);

                    // if object is moving towards obstacle... avoiding double interactions
                    if ((object->velocity()*impactDirection) > 0) {
                        const Vector2f velocityBefore = (object->velocity() * impactDirection) * impactDirection;
                        object->velocity() = object->velocity() - velocityBefore*0.6 - velocityBefore;

                        const Vector2f impactLocation = (*it)->location_ - impactDirection*(*it)->radius_;
                        (*it)-> onCollision(object, impactLocation, impactDirection, velocityBefore);
                        object->onCollision(*it,    impactLocation, impactDirection, velocityBefore);
                        break;
                    }
                }
            }
        }
        // collision with ships and balls
        if (with & MOBILES) {
            // check for collision with each mobile object
            for (std::vector<MobileSpaceObject*>::iterator it = mobileObjects_.begin(); it != mobileObjects_.end(); ++it) {
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

                    const float minDistSquared = std::pow(object->radius_ + (*it)->radius_, 2);
                    // if objects are moving
                    if (source->velocity().lengthSquare() > 0) {
                        const Vector2f velocitySourceNorm  ( source->velocity().normalize());
                        const Vector2f velocitySourceOrtho (-velocitySourceNorm.y_, velocitySourceNorm.x_);
                        const Vector2f centerDist          ( velocitySourceOrtho * ((source->location_ - target->location_)*velocitySourceOrtho));

                        // if path of object crosses target object
                        if (centerDist.lengthSquare() < minDistSquared) {
                            const Vector2f lastFrameLocation = source->location_ - source->velocity()*timer::frameTime()*0.6f;
                            const Vector2f chordMidPoint     = target->location_ + centerDist;

                            // if path of object has intersected with target within the last frame
                            if ( ((source->location_ - target->location_             ).lengthSquare() < minDistSquared)
                              || ((lastFrameLocation - target->location_             ).lengthSquare() < minDistSquared)
                              || ((chordMidPoint - source->location_)*(chordMidPoint - lastFrameLocation) < 0.f)) {

                                if (source->velocity()*(target->location_ - lastFrameLocation) > 0) {

                                    const Vector2f moveOutDist (velocitySourceNorm * std::sqrt(minDistSquared - centerDist.lengthSquare()));
                                    source->location_ = chordMidPoint - moveOutDist;

                                    const Vector2f impactLocation  ((source->location_ + target->location_)*0.5f);
                                    const Vector2f impactDirection ((target->location_ - source->location_).normalize());

                                    // calculate velocity of objects in direction of impact before collision
                                    const Vector2f velocitySourceBefore = impactDirection * (source->velocity() * impactDirection);
                                    const Vector2f velocityTargetBefore = impactDirection * (target->velocity() * impactDirection);

                                    if ((velocitySourceBefore - velocityTargetBefore).lengthSquare() > 1000) {
                                        // calculate velocity of objects in direction of impact after collision
                                        const Vector2f velocitySourceAfter = (velocitySourceBefore*source->mass_ + velocityTargetBefore*target->mass_ - (velocitySourceBefore - velocityTargetBefore)*target->mass_) / (source->mass_ + target->mass_);
                                        const Vector2f velocityTargetAfter = (velocitySourceBefore*source->mass_ + velocityTargetBefore*target->mass_ - (velocityTargetBefore - velocitySourceBefore)*source->mass_) / (source->mass_ + target->mass_);

                                        // calculate collision result
                                        target->onCollision(source, impactLocation, impactDirection, velocitySourceBefore);
                                        source->onCollision(target, impactLocation, impactDirection, velocityTargetBefore);

                                        // add to orthongonal speed component of initial velocity
                                        // special case: Collision with rofle bullets is not physically correct, for improved gameplay
                                        if ((source->type() == spaceObjects::oAmmoROFLE) | (target->type() == spaceObjects::oAmmoROFLE))
                                            target-> velocity() += (0.05f*source->velocity()*source->mass_ + (velocityTargetAfter - velocityTargetBefore) * 0.6);
                                        else {
                                            source->velocity() += (velocitySourceAfter - velocitySourceBefore) * 0.8;
                                            target->velocity() += (velocityTargetAfter - velocityTargetBefore) * 0.8;
                                        }
                                    }
                                    else {
                                        const Vector2f velocityAfter = (velocitySourceBefore*source->mass_ + velocityTargetBefore*target->mass_) / (source->mass_ + target->mass_);
                                        target->onCollision(source, impactLocation, impactDirection, Vector2f());
                                        source->onCollision(target, impactLocation, impactDirection, Vector2f());
                                        source->velocity() = source->velocity() - velocitySourceBefore + velocityAfter;
                                        target->velocity() = target->velocity() - velocityTargetBefore + velocityAfter;
                                    }
                                }
                            }
                        }
                    }
                    else if ((source->location_ - target->location_).lengthSquare() < minDistSquared) {
                        // if objects intersect, but aren't moving
                        const Vector2f normDistance = (target->location_ - source->location_).normalize();
                        const float    moveOutDist  = ((target->radius_ + source->radius_) - (source->location_ - target->location_).length())*0.5;
                        source->location_ = target->location_ - normDistance*moveOutDist;
                        target->location_ = source->location_ + normDistance*moveOutDist;
                    }
                }
            }
        }
        // collision with particles
        if (with & PARTICLES) {
            particles::collideWith(object);
        }
    }

    Vector2f attract (MobileSpaceObject* attracted) {
        Vector2f totalAcceleration;
        for (std::vector<SpaceObject*>::iterator it = gravitySources_.begin(); it != gravitySources_.end(); ++it) {
            float distanceSquared = (attracted->location_ - (*it)->location_).lengthSquare();
            if (distanceSquared > 100.f)
                totalAcceleration += ((*it)->location_ - attracted->location_) * (*it)->mass_ / distanceSquared;
        }
        return totalAcceleration;
    }

    void causeShockWave(Player* damageSource, Vector2f const& location, float strength, float radius, float damage) {
        for (std::vector<MobileSpaceObject*>::iterator it = mobileObjects_.begin(); it != mobileObjects_.end(); ++it) {
            Vector2f direction((*it)->location_ - location);
            float distance = direction.length();
            if (distance < radius && direction != Vector2f()) {
                float intensity = ((radius-distance)/radius);
                direction /= distance;
                direction *= (intensity*strength);
                (*it)->velocity() += direction;
                (*it)->onShockWave(damageSource, intensity*damage);
            }
        }
        particles::shockWave(location, strength, radius);
    }

    void addMobileObject (MobileSpaceObject* object) {
        mobileObjects_.push_back(object);
    }

    void removeMobileObject (MobileSpaceObject* object) {
        for (std::vector<MobileSpaceObject*>::iterator it = mobileObjects_.begin(); it != mobileObjects_.end(); ++it)
            if(*it == object) {
                mobileObjects_.erase(it);
                break;
            }
    }

    void addStaticObject (SpaceObject* object) {
        staticObjects_.push_back(object);
    }

    void removeStaticObject (SpaceObject* object) {
        for (std::vector<SpaceObject*>::iterator it = staticObjects_.begin(); it != staticObjects_.end(); ++it)
            if(*it == object) {
                staticObjects_.erase(it);
                break;
            }
    }

    void addGravitySource (SpaceObject* object) {
        gravitySources_.push_back(object);
    }

    std::vector<SpaceObject*> const& getGravitySources() {
        return gravitySources_;
    }

    void clear() {
        staticObjects_.clear();
        mobileObjects_.clear();
        gravitySources_.clear();
    }
}
