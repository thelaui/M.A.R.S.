/* Weapon.hpp

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

# ifndef WEAPON_HPP_INCLUDED
# define WEAPON_HPP_INCLUDED

# include "System/timer.hpp"
# include "Weapons/weapons.hpp"

class Ship;

/// A virtual base class for all weapons.
/// Provides a basic interface for all weapons, some base members and methodes.

class Weapon {
    public:
        /// Ctor which constructs the base Weapon.
        Weapon(weapons::WeaponType type, Ship* parent, sf::String name);

        /// This function is called whenever the weapon is fired.
        virtual void fire() const = 0;

        /// Draws the weapon.
        virtual void draw() const = 0;

        /// Replaces this weapon by the next one.
        /// With this method it's possible to cycle through the weapons.
        void next();

        /// Replaces this weapon by the previous one.
        /// With this method it's possible to cycle through the weapons.
        void previous();

        /// Returns the name of the Weapon.
        sf::String const& getName() const {return name_;}

        /// Returns the type of the Weapon.
        weapons::WeaponType getType() const {return type_;}

    protected:
        Ship* parent_;
        mutable float timer_;

    private:
        sf::String name_;
        weapons::WeaponType type_;
};

# endif // WEAPON_HPP_INCLUDED
