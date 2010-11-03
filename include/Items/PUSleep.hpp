/* PUSleep.hpp

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

# ifndef PUSLEEP_HPP_INCLUDED
# define PUSLEEP_HPP_INCLUDED

# include "System/Vector2f.hpp"
# include "Items/PowerUp.hpp"

# include <SFML/System.hpp>

class PUSleep: public PowerUp{
    public:
        PUSleep(Vector2f const& location):
            PowerUp(items::puSleep, location, 15.f, sf::Randomizer::Random(8.f, 12.f), 2, 0, Color3f(0.6f, 1.f, 0.4f)){}

        void draw() const;

    private:
        void refreshLifeTime();
};

# endif // PUSLEEP_HPP_INCLUDED








