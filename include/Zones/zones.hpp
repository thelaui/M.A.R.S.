/* zones.hpp

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

# ifndef ZONES_HPP_INCLUDED
# define ZONES_HPP_INCLUDED

# define NO_ZONE 0
# define OWN_HOME 1
# define OWN_TEAM 2
# define ENEMY_HOME 3
# define ENEMY_TEAM 4

# include "System/Vector2f.hpp"
# include "Zones/Zone.hpp"

# include <vector>

class Team;
class TacticalZone;
class RasterZone;

namespace zones {
    Zone* addTeamZone(Vector2f const& location);
    Zone* addHomeZone(Vector2f const& location);
    void detectTacticalZones();
    void createRaster(int dimX, int dimY);
    void update();
    void draw();

    TacticalZone* toProtect(Team* checker);
    RasterZone*   freeZone();
    float totalTacticalArea(short homeSide);

    short isInside(Team* checker, SpaceObject const& toBeChecked);

    void clear();
}

# endif // ZONES_HPP_INCLUDED

