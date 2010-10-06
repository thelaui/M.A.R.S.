/* MobileSpaceObject.hpp

Copyright (c) 2010 by Felix Lauer und Simon Schneegans

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

# ifndef MOBLIESPACEOBJECT_HPP_INCLUDED
# define MOBLIESPACEOBJECT_HPP_INCLUDED

# include "SpaceObjects/SpaceObject.hpp"

class MobileSpaceObject: public SpaceObject {
    public:
        MobileSpaceObject(spaceObjects::ObjectType type, Vector2f location, float radius, float mass):
                          SpaceObject(type, location, radius, mass),
                          velocity_(0.f, 0.f) {}

        Vector2f const& velocity() {return velocity_;}

        friend Vector2f physics::attract(MobileSpaceObject*);
        friend void     physics::collide(MobileSpaceObject*, int);
        friend void     physics::causeShockWave(SpaceObject* evilOne, float strength, float radius);

    protected:
        Vector2f velocity_;
};

# endif // MOBLIESPACEOBJECT_HPP_INCLUDED
