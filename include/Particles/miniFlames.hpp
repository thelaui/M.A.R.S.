/* miniFlames.hpp

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

# ifndef MINIFLAMES_HPP_INCLUDED
# define MINIFLAMES_HPP_INCLUDED

# include "Particles/Particle.hpp"
# include "System/Color3f.hpp"

# include <list>

namespace miniFlames {

    class MiniFlame: public Particle {
        public:
            MiniFlame(Vector2f const& location);

            void update();
            void draw() const;

        private:
            Color3f color_;
            float flickerSpeed_;
            float timer_;
    };

    void spawn(Vector2f const& location);
    void update();
    void draw();
    int  count();
    void clear();
}

# endif // MINIFLAMES_HPP_INCLUDED



