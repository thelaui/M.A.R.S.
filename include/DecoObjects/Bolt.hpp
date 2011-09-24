/* Bolt.hpp

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

# ifndef BOLT_HPP_INCLUDED
# define BOLT_HPP_INCLUDED

# include "DecoObjects/DecoObject.hpp"

class SpaceObject;

/// An object for drawing bolts between ships.

class Bolt: public DecoObject {
    public:
        Bolt(SpaceObject *from, SpaceObject *to, float width):
            from_(from),
            to_(to),
            width_(width),
            maxLifeTime_(0.3f),
            lifeTime_(0.f) {}

        void draw() const;

    private:
        SpaceObject *from_, *to_;
        float width_;
        mutable float maxLifeTime_, lifeTime_;
};

# endif // BOLT_HPP_INCLUDED


