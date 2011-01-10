/* Zone.hpp

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

# ifndef ZONE_HPP
# define ZONE_HPP

# include "System/Vector2f.hpp"

class SpaceObject;

/// Basic interface for all Zones.

class Zone {
    public:
        /// Returns true, if the given SpaceObject is inside this Zone.
        virtual bool isInside(SpaceObject const& toBeChecked) const = 0;

        /// Draws the zone.
        /// Only for debugging information.
        virtual void draw() const = 0;
};

# endif
