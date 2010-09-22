/* ammoROFLEs.hpp

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

# ifndef AMMOROFLE_HPP_INCLUDED
# define AMMOROFLE_HPP_INCLUDED

# include "Particles/Particle.hpp"

# include <list>

namespace ammoROFLEs {

    class AmmoROFLE: public Particle {
        public:
            AmmoROFLE(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Player* damageSource);

            void update();
            void draw() const;

            void onCollision(SpaceObject* with, Vector2f const& location,
                             Vector2f const& direction, Vector2f const& velocity);
    };

    void spawn(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Player* damageSource);
    void update();
    void draw();
    int  count();
    void clear();
}

# endif // AMMOROFLE_HPP_INCLUDED


