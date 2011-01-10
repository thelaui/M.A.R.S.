/* physics.hpp

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

# ifndef PHYSICS_HPP_INCLUDED
# define PHYSICS_HPP_INCLUDED

# include "System/Vector2f.hpp"

# include <vector>
# include <list>

# define MOBILES   1
# define STATICS   2
# define PARTICLES 4

class SpaceObject;
class MobileSpaceObject;
class Player;

namespace physics {
    void     collide        (MobileSpaceObject* source, int with);
    Vector2f attract        (MobileSpaceObject* attracted);
    void     causeShockWave (Player* damageSource, Vector2f const& location, float strength, float radius, float damage);

    void addMobileObject    (MobileSpaceObject* source);
    void removeMobileObject (MobileSpaceObject* source);
    void addStaticObject    (SpaceObject* source);
    void removeStaticObject (SpaceObject* source);
    void addGravitySource   (SpaceObject* source);
    void clear              ();
}

# endif // PHYSICS_HPP_INCLUDED
