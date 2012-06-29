/* AmmoInsta.hpp

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

# ifndef AMMOINSTA_HPP_INCLUDED
# define AMMOINSTA_HPP_INCLUDED

# include "Particles/Particle.hpp"

class Trail;
class Team;

class AmmoInsta: public Particle<AmmoInsta> {
    public:
        AmmoInsta(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Color3f const& color, Player* damageSource);
        ~AmmoInsta();

        void update();
        void draw() const;

        void onCollision(SpaceObject* with, Vector2f const& location,
                         Vector2f const& direction, Vector2f const& velocity);

        static int hitsAny(Vector2f const& location, Vector2f const& direction, Team* team);

        friend class Particle<AmmoInsta>;

    private:
        static std::list<AmmoInsta*> activeParticles_;
        Trail* trail_;
        Color3f color_;
};

# endif // AMMOINSTA_HPP_INCLUDED


