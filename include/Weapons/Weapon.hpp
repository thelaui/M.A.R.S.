/* Weapon.hpp

Copyright (c) 2010 by Felix Lauer und Simon Schneegans

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

# include <string>

class Ship;

class Weapon {
    public:
        Weapon(Ship* parent, std::string name):
               parent_(parent),
               timer_(0),
               name_(name) {}

        virtual void fire() const = 0;
        virtual void draw() const = 0;

        virtual void next()     = 0;
        virtual void previous() = 0;

        std::string const& getName() const {return name_;}

    protected:
        Ship* parent_;
        mutable float timer_;

    private:
        std::string name_;
};

# endif // WEAPON_HPP_INCLUDED
