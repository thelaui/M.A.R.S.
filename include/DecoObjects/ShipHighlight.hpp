/* ShipHighlight.hpp

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

# ifndef SHIPHIGHLIGHT_HPP_INCLUDED
# define SHIPHIGHLIGHT_HPP_INCLUDED

# include "DecoObjects/DecoObject.hpp"

class Ship;

/// A rotating ring which can be attached to a Ship.
/// Human controlled Ships are highlighed to make it
/// easier to see the Ship

class ShipHighlight: public DecoObject {
    public:
        ShipHighlight(Ship* ship): ship_(ship) {}

        void draw() const;

    private:
        void draw(Vector2f const& location, float scale, float alpha) const;

        Ship* ship_;
};

# endif // SHIPHIGHLIGHT_HPP_INCLUDED


