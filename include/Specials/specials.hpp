/* specials.hpp

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

# ifndef SPECIALS_HPP_INCLUDED
# define SPECIALS_HPP_INCLUDED

class Special;
class Ship;

namespace specials {
    enum SpecialType {sFreeze=1, sHeal=2, sBlast=4, sNoSpecial=8};

    Special* create    (SpecialType type, Ship* parent);
    Special* createNext(SpecialType type, Ship* parent);
    Special* createPrev(SpecialType type, Ship* parent);
}

# endif // SPECIALS_HPP_INCLUDED

