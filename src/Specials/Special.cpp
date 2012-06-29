/* Special.cpp

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

# include "Specials/Special.hpp"

# include "SpaceObjects/Ship.hpp"

Special::Special(specials::SpecialType type, Ship* parent, sf::String name):
    parent_(parent),
    timer_(0),
    type_(type),
    name_(name) {}

void Special::next() {
    parent_->currentSpecial_ = specials::createNext(type_, parent_);
    delete this;
}

void Special::previous() {
    parent_->currentSpecial_ = specials::createPrev(type_, parent_);
    delete this;
}

void Special::stop() {
    timer_ = 0.f;
}
