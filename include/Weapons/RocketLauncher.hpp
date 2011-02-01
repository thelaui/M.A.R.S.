/* RocketLauncher.hpp

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

# ifndef ROCKETLAUNCHER_HPP_INCLUDED
# define ROCKETLAUNCHER_HPP_INCLUDED

# include "Weapons/Weapon.hpp"

/// Weapon: ROCK'n'LOL.
/// A rocket launcher which aims at nearby enemies.

class RocketLauncher: public Weapon {
    public:
        /// Ctor which constructs the weapon.
        RocketLauncher(Ship* parent):
              Weapon(weapons::wRocketLauncher, parent, "ROCK'n'LOL") {};

        /// Spawns some particles.
        void fire() const;

        /// Draws the weapon.
        void draw() const;

        /// Returns the maximum distance from which this weapon should be used.
        float maxDistance() const;

        /// Returns the minimum distance from which this weapon should be used.
        float minDistance() const;

        /// Returns the maximum angle from which this weapon should be used.
        float maxAngle()   const;
};

# endif // ROCKETLAUNCHER_HPP_INCLUDED




