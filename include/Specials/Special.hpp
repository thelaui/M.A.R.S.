/* Special.hpp

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

# ifndef SPECIAL_HPP_INCLUDED
# define SPECIAL_HPP_INCLUDED

# include "System/timer.hpp"
# include "Specials/specials.hpp"

class Ship;

/// A virtual base class for all specials.
/// Provides a basic interface for all specials, some base members and methodes.

class Special {
    public:
        /// Ctor which constructs the base special.
        Special(specials::SpecialType type, Ship* parent, sf::String name);

        /// This function is called whenever the special is activated.
        virtual void activate() const = 0;

        /// Draws the special.
        virtual void draw(float alpha) const = 0;

        /// Replaces this special by the next one.
        /// With this method it's possible to cycle through the specials.
        void next();

        /// Replaces this special by the previous one.
        /// With this method it's possible to cycle through the specials.
        void previous();

        /// Returns the active radius of the Special.
        virtual float radius() const = 0;

        /// Stops the current animation. Called when ship is killed.
        void stop();

        /// Returns the name of the special.
        sf::String const& getName() const {return name_;}

        /// Returns the type of the Special.
        specials::SpecialType getType() const {return type_;}

    protected:
        Ship* parent_;
        mutable float timer_;

    private:
        specials::SpecialType type_;
        sf::String name_;
};

# endif // SPECIAL_HPP_INCLUDED

