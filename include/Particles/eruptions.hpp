/* Eruption.hpp

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

# ifndef ERUPTION_HPP_INCLUDED
# define ERUPTION_HPP_INCLUDED

# include "Particles/Particle.hpp"
# include "System/Color3f.hpp"

# include <list>

namespace eruptions {

    class Eruption: public Particle {
        public:
            Eruption(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity);

            void update();
            void draw() const;

        private:
            Color3f color_;
    };

    void spawn(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity);
    void update();
    void draw();
    int  count();
    void clear();
}

# endif // ERUPTION_HPP_INCLUDED


