/* numbers.hpp

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

# ifndef NUMBERS_HPP_INCLUDED
# define NUMBERS_HPP_INCLUDED

# include "System/Vector2f.hpp"
# include "System/Color3f.hpp"

# include <list>

namespace numbers {

    class Number {
        public:
            Number(Vector2f const* location, int value);

            bool update();
            void draw() const;

        private:
            int value_;
            Color3f color_;
            Vector2f const* sourceLocation_;
            float lifeTime_;
    };

    void spawn(Vector2f const* location, int value);
    void update();
    void draw();
    void clear();
}

# endif // NUMBERS_HPP_INCLUDED



