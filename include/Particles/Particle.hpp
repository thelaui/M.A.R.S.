/* Particle.hpp

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

class Particle: public MobileSpaceObject {
    public:
        Particle(spaceObjects::ObjectType const& type, Vector2f const& location, float radius, float mass, float totalLifeTime):
                 MobileSpaceObject(type, location, radius, mass),
                 totalLifeTime_(totalLifeTime),
                 lifeTime_(0) {}

        void killMe() {lifeTime_ = totalLifeTime_;}
        bool isDead() {return (lifeTime_ >= totalLifeTime_);}

    protected:
        float totalLifeTime_;
        float lifeTime_;
};


# endif // PARTICLE_HPP_INCLUDED


