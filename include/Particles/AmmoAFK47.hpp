/* AmmoAFK47.hpp

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

# ifndef AMMOAFK47_HPP_INCLUDED
# define AMMOAFK47_HPP_INCLUDED

# include "Particles/Particle.hpp"

class AmmoAFK47: public Particle<AmmoAFK47> {
    public:
        AmmoAFK47(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Color3f const& color, Player* damageSource);

        void update();
        void draw() const;

        void onCollision(SpaceObject* with, Vector2f const& location,
                         Vector2f const& direction, Vector2f const& velocity);

        friend class Particle<AmmoAFK47>;

    private:
        static std::list<AmmoAFK47*> activeParticles_;
};

# endif // AMMOAFK47_HPP_INCLUDED

