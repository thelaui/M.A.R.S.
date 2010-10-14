/* Player.cpp

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

# include "Players/Player.hpp"

# include "Controllers/controllers.hpp"

Player::Player(sf::String const& name, Color3f const& color, int graphic, controllers::ControlType controlType):
    ship_(NULL),
    name_(name),
    nameRef_(NULL),
    points_(0),
    frags_(0),
    suicides_(0),
    deaths_(0),
    teamKills_(0),
    reputation_(0),
    color_(color),
    colorRef_(NULL),
    graphic_(graphic),
    graphicRef_(NULL),
    team_(NULL),
    controlType_(controlType) {

        controllers::addController(controlType, this);
}

Player::Player(sf::String* name, Color3f* color, int* graphic, controllers::ControlType controlType):
    ship_(NULL),
    name_(*name),
    nameRef_(name),
    points_(0),
    frags_(0),
    suicides_(0),
    deaths_(0),
    teamKills_(0),
    reputation_(0),
    color_(*color),
    colorRef_(color),
    graphic_(*graphic),
    graphicRef_(graphic),
    team_(NULL),
    controlType_(controlType) {

        controllers::addController(controlType, this);
}

void Player::resetPoints() {
    points_ = 0;
    frags_ = 0;
    suicides_ = 0;
    deaths_ = 0;
    teamKills_ = 0;
    reputation_ = 0;
}
