/* Freezer.hpp

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

# ifndef FREEZER_HPP_INCLUDED
# define FREEZER_HPP_INCLUDED

# include "Specials/Special.hpp"

/// Special: Freezer.
/// A Freezing wave.

class Freezer: public Special {
    public:
        /// Ctor which constructs the special.
        Freezer(Ship* parent):
              Special(parent, sf::String("FREEZER")),
              radius_(0.f) {};

        /// Freezes nearby ships.
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

# endif // FREEZER_HPP_INCLUDED




