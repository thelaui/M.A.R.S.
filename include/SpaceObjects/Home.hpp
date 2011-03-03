/* Home.hpp

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

# ifndef HOME_HPP_INCLUDED
# define HOME_HPP_INCLUDED

# include "SpaceObjects/SpaceObject.hpp"
# include "Players/Player.hpp"

# include <float.h>

class Home: public SpaceObject {
    public:
        Home(Vector2f const& location, float radius, Color3f const& color);

        void update();
        void draw() const;
        void drawLife() const;

        int getLife() const;

        void createShips(std::vector<Player*>& inhabitants) const;

        void onCollision(SpaceObject* with, Vector2f const& location,
                         Vector2f const& direction, Vector2f const& velocity);

    private:
        void explode();

        Color3f color_;
        int life_;
        bool visible_;
};

# endif // HOME_HPP_INCLUDED

