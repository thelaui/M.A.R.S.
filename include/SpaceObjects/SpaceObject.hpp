/* SpaceObject.hpp

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

# ifndef SPACEOBJECT_HPP_INCLUDED
# define SPACEOBJECT_HPP_INCLUDED

# include "System/Vector2f.hpp"
# include "Media/texture.hpp"
# include "physics.hpp"
# include "SpaceObjects/spaceObjects.hpp"

class Player;

class SpaceObject {
    public:
        SpaceObject(spaceObjects::ObjectType type, Vector2f location, float radius, float mass):
                    location_(location),
                    radius_(radius),
                    mass_(mass),
                    damageSource_(NULL),
                    type_(type) {}

        virtual void update() = 0;
        virtual void draw() const = 0;

        virtual void onCollision(SpaceObject* source, Vector2f const& location,
                                 Vector2f const& direction, Vector2f const& velocity) {};

        virtual void onShockWave(SpaceObject* source, float intensity) {};

        virtual void setDamageSource(Player* evilOne) {damageSource_ = evilOne;}
        Player*      damageSource() const             {return damageSource_;}

        spaceObjects::ObjectType type()     const {return type_;}
        Vector2f const&          location() const {return location_;}
        float                    radius()   const {return radius_;}

        friend Vector2f          physics::attract(MobileSpaceObject*);
        friend void              physics::collide(MobileSpaceObject*, int);
        friend void              physics::causeShockWave(SpaceObject* evilOne, float strength, float radius);

    protected:
        Vector2f location_;
        float radius_;
        float mass_;
        Player* damageSource_;

    private:
        spaceObjects::ObjectType type_;
};

# endif // SPACEOBJECT_HPP_INCLUDED
