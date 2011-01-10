/* BotPlayer.cpp

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

# include "Players/BotPlayer.hpp"

# include "Controllers/controllers.hpp"
# include "System/settings.hpp"

BotPlayer::BotPlayer(std::pair<sf::String, int>const& nameStrength, Color3f const& color, int  graphic, controllers::ControlType controlType):
    Player(controlType),
    name_(nameStrength.first),
    color_(color),
    graphic_(graphic) {

        controllers::addController(controlType, this, static_cast<float>(nameStrength.second+settings::C_iDumb)*0.005f);
}

