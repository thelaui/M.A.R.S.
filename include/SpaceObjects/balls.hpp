/* balls.hpp

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

# ifndef BALLS_HPP_INCLUDED
# define BALLS_HPP_INCLUDED

# include "System/Vector2f.hpp"
# include "SpaceObjects/Ball.hpp"

# include <vector>

namespace balls {
    void addBall(Vector2f const& location = Vector2f(0, 0));

    void update();
    void draw();

    Ball* getBall();

    void clear();
}

# endif // BALLS_HPP_INCLUDED
