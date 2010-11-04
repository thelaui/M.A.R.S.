/* RocketLauncher.hpp

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

# ifndef ROCKETLAUNCHER_HPP_INCLUDED
# define ROCKETLAUNCHER_HPP_INCLUDED

# include "Weapons/Weapon.hpp"

/// Weapon: ROCK'n'LOL.
/// A rocket launcher which aims at nearby enemies.

class RocketLauncher: public Weapon {
    public:
        /// Ctor which constructs the weapon.
        RocketLauncher(Ship* parent):
              Weapon(parent, "ROCK'n'LOL") {};

        /// Spawns some particles.
        void fire() const;

        /// Draws the weapon.
        void draw() const;

        /// Replaces this weapon by the next one.
        /// With this method it's possible to cycle through the weapons.
        void next();

        /// Replaces this weapon by the previous one.
        /// With this method it's possible to cycle through the weapons.
        void previous();
};

# endif // ROCKETLAUNCHER_HPP_INCLUDED




