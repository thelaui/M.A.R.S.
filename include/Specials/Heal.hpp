/* Heal.hpp

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

# ifndef HEAL_HPP_INCLUDED
# define HEAL_HPP_INCLUDED

# include "Specials/Special.hpp"

/// Special: Heal.
/// A healing wave.

class Heal: public Special {
    public:
        /// Ctor which constructs the special.
        Heal(Ship* parent):
              Special(parent, sf::String("HEAL")),
              radius_(0.f) {};

        /// Heals nearby friends.
        void activate() const;

        /// Draws the special.
        void draw() const;

        /// Replaces this special by the next one.
        /// With this method it's possible to cycle through the specials.
        void next();

        /// Replaces this special by the previous one.
        /// With this method it's possible to cycle through the specials.
        void previous();

    private:
        mutable float radius_;
};

# endif // HEAL_HPP_INCLUDED



