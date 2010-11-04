/* trailEffects.hpp

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

# ifndef TRAILEFFECTS_HPP_INCLUDED
# define TRAILEFFECTS_HPP_INCLUDED

# include "System/Color3f.hpp"

class SpaceObject;

namespace trailEffects {
    void update();

    void draw();

    void attach(SpaceObject* target, int resolution, float length, float width, Color3f const& color);

    void detach(SpaceObject* target);

    int  count();

    void clear();
}

# endif // TRAILEFFECTS_HPP_INCLUDED

