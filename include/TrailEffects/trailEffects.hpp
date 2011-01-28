/* trailEffects.hpp

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

# ifndef TRAILEFFECTS_HPP_INCLUDED
# define TRAILEFFECTS_HPP_INCLUDED

# include "System/Color3f.hpp"

class SpaceObject;
class Trail;

namespace trailEffects {
    void update();

    void draw();

    Trail* attach(SpaceObject* target, float timeStep, float duration, float width, Color3f const& color, bool persistant);

    void detach(SpaceObject* target);

    int  count();

    void clear();
}

# endif // TRAILEFFECTS_HPP_INCLUDED

