/* weapons.hpp

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

# ifndef WEAPONS_HPP_INCLUDED
# define WEAPONS_HPP_INCLUDED

class Weapon;
class Ship;

namespace weapons {
    enum WeaponType {wInsta=1, wAFK47=2, wBurner=4, wFist=8, wFlubba=16,
                     wShotgun=32, wRocketLauncher=64, wROFLE=128,
                     wH2OMG=256, wNoWeapon=512};

    Weapon* create    (WeaponType type, Ship* parent);
    Weapon* createNext(WeaponType type, Ship* parent);
    Weapon* createPrev(WeaponType type, Ship* parent);
}

# endif // WEAPONS_HPP_INCLUDED
