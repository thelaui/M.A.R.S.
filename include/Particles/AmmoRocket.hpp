/* AmmoRocket.hpp

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

# ifndef AMMOROCKET_HPP_INCLUDED
# define AMMOROCKET_HPP_INCLUDED

# include "Particles/Particle.hpp"

# include "System/Color3f.hpp"

class Ship;

class AmmoRocket: public Particle<AmmoRocket> {
    public:
        AmmoRocket(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Color3f const& color, Player* damageSource);
        ~AmmoRocket();

        void update();
        void draw() const;

        void onCollision(SpaceObject* with, Vector2f const& location,
                         Vector2f const& direction, Vector2f const& velocity);

        friend class Particle<AmmoRocket>;

    private:
        Color3f color_;
        float timer_;
        Ship* target_;
        Player* parent_;
        static std::list<AmmoRocket*> activeParticles_;
};

# endif // AMMOROCKET_HPP_INCLUDED
