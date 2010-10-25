/* PUShield.hpp

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

# ifndef PUSHIELD_HPP_INCLUDED
# define PUSHIELD_HPP_INCLUDED

# include "System/Vector2f.hpp"
# include "Items/PowerUp.hpp"

class Ship;

class PUShield: public PowerUp{
    public:
        PUShield(Vector2f const& location, float lifeTime):
            PowerUp(items::iShield, location, lifeTime){}

        void draw() const;
};

# endif // PUSHIELD_HPP_INCLUDED







