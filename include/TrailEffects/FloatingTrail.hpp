/* FloatingTrail.hpp

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

# ifndef FLOATINGTRAIL_HPP_INCLUDED
# define FLOATINGTRAIL_HPP_INCLUDED

# include "TrailEffects/Trail.hpp"
# include "System/Vector2f.hpp"
# include "System/Color3f.hpp"

# include <vector>

class SpaceObject;

class FloatingTrail: public Trail {
    public:
        FloatingTrail(SpaceObject* target, float timeStep, float duration, float width, Color3f const& color);

        void update();
        void draw() const;
        bool isDead() const;

    private:
        std::vector<Vector2f> points_;
        int frontIndex_;
        int length_;
        float timer_;
        float timeStep_;
        float width_;
        Color3f color_;
};

# endif //FLOATINGTRAIL_HPP_INCLUDED


