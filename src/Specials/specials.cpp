/* specials.cpp

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

# include "Specials/specials.hpp"

# include "Specials/Blast.hpp"
# include "Specials/Freezer.hpp"
# include "Specials/Heal.hpp"
# include "Specials/FireWall.hpp"
# include "Specials/Shocker.hpp"
# include "Specials/NoSpecial.hpp"
# include "SpaceObjects/Ship.hpp"
# include "Players/Player.hpp"
# include "System/settings.hpp"

namespace specials {

    Special* create(SpecialType type, Ship* parent) {
        int i(1), tmpType(type);
        while (i < sNoSpecial && !(settings::C_EnabledSpecials & tmpType)) {
            i *= 2;
            if ((tmpType*=2) > sNoSpecial)
                tmpType = 1;
        }
        type = static_cast<SpecialType>(tmpType);

        if  (parent->getOwner()->type() == controllers::cPlayer1 && type != sNoSpecial)
            settings::C_playerISpecial = type;
        else if  (parent->getOwner()->type()  == controllers::cPlayer2 && type != sNoSpecial)
            settings::C_playerIISpecial = type;

        switch (type) {
            case sHeal:      return new Heal(parent);
            case sBlast:     return new Blast(parent);
            case sFireWall:  return new FireWall(parent);
            case sFreeze:    return new Freezer(parent);
            case sShocker:   return new Shocker(parent);
            default:         return new NoSpecial(parent);
        }
    }

    Special* createNext(SpecialType type, Ship* parent) {
        int next(type == sNoSpecial ? 1 : type*2), i(1);

        while (i < sNoSpecial && !(settings::C_EnabledSpecials & next)) {
            i *= 2;
            if ((next*=2) > sNoSpecial)
                next = 1;
        }
        return create(static_cast<SpecialType>(next), parent);
    }

    Special* createPrev(SpecialType type, Ship* parent) {
        int next(type == 1 ? sNoSpecial : type*0.5), i(1);

        while (i < sNoSpecial && !(settings::C_EnabledSpecials & next)) {
            i *= 2;
            if ((next/=2) < 1)
                next = sNoSpecial;
        }
        return create(static_cast<SpecialType>(next), parent);
    }

}

