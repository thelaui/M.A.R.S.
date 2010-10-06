/* Cannon.hpp

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

# ifndef CANNON_HPP_INCLUDED
# define CANNON_HPP_INCLUDED

# include "DecoObjects/DecoObject.hpp"

class Cannon: public DecoObject {
    public:
        Cannon();

        void update();
        void draw() const;

    private:
        float timer_;
        float rotation_;
};

# endif // CANNON_HPP_INCLUDED




