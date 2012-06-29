/* weapons.cpp

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

# include "Weapons/weapons.hpp"

# include "Weapons/AFK47.hpp"
# include "Weapons/Burner.hpp"
# include "Weapons/Fist.hpp"
# include "Weapons/Insta.hpp"
# include "Weapons/Flubba.hpp"
# include "Weapons/H2OMG.hpp"
# include "Weapons/ROFLE.hpp"
# include "Weapons/RocketLauncher.hpp"
# include "Weapons/Shotgun.hpp"
# include "Weapons/NoWeapon.hpp"
# include "SpaceObjects/Ship.hpp"
# include "Players/Player.hpp"
# include "System/settings.hpp"

namespace weapons {

    Weapon* create(WeaponType type, Ship* parent) {
        int i(1), tmpType(type);
        while (i < wNoWeapon && !(settings::C_EnabledWeapons & tmpType)) {
            i *= 2;
            if ((tmpType*=2) > wNoWeapon)
                tmpType = 1;
        }
        type = static_cast<WeaponType>(tmpType);

        if  (parent->getOwner()->type()  == controllers::cPlayer1 && type != wNoWeapon && type != wInsta)
            settings::C_playerIWeapon = type;
        else if  (parent->getOwner()->type()  == controllers::cPlayer2 && type != wNoWeapon && type != wInsta)
            settings::C_playerIIWeapon = type;

        switch (type) {
            case wAFK47:           return new AFK47(parent);
            case wBurner:          return new Burner(parent);
            case wFist:            return new Fist(parent);
            case wFlubba:          return new Flubba(parent);
            case wH2OMG:           return new H2OMG(parent);
            case wROFLE:           return new ROFLE(parent);
            case wRocketLauncher:  return new RocketLauncher(parent);
            case wInsta:           return new Insta(parent);
            case wShotgun:         return new Shotgun(parent);
            default:               return new NoWeapon(parent);
        }
    }

    Weapon* createNext(WeaponType type, Ship* parent) {
        int next(type == wNoWeapon ? 1 : type*2), i(1);

        while (i < wNoWeapon && !(settings::C_EnabledWeapons & next)) {
            i *= 2;
            if ((next*=2) > wNoWeapon)
                next = 1;
        }
        return create(static_cast<WeaponType>(next), parent);
    }

    Weapon* createPrev(WeaponType type, Ship* parent) {
        int next(type == 1 ? wNoWeapon : type*0.5), i(1);

        while (i < wNoWeapon && !(settings::C_EnabledWeapons & next)) {
            i *= 2;
            if ((next/=2) < 1)
                next = wNoWeapon;
        }
        return create(static_cast<WeaponType>(next), parent);
    }

}


