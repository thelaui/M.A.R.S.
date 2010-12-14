/* KeyController.cpp

Copyright (c) 2010 by Felix Lauer and Simon Schneegans

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

# include "Controllers/KeyController.hpp"

# include "System/window.hpp"
# include "System/settings.hpp"

KeyController::KeyController(controllers::ControlType type, Player* slave):
    Controller(slave, type) {}

void KeyController::update(sf::Input const& input) const {
    if (type_ == controllers::cPlayer1) {
        slaveUp   (input.IsKeyDown(settings::C_playerIup));
        slaveLeft (input.IsKeyDown(settings::C_playerIleft));
        slaveRight(input.IsKeyDown(settings::C_playerIright));
        slaveFire (input.IsKeyDown(settings::C_playerIfire));
        slaveSpecial (input.IsKeyDown(settings::C_playerIspecial));
    }

    else if (type_ == controllers::cPlayer2) {
        slaveUp   (input.IsKeyDown(settings::C_playerIIup));
        slaveLeft (input.IsKeyDown(settings::C_playerIIleft));
        slaveRight(input.IsKeyDown(settings::C_playerIIright));
        slaveFire (input.IsKeyDown(settings::C_playerIIfire));
        slaveSpecial (input.IsKeyDown(settings::C_playerIIspecial));
    }
}

void KeyController::update(sf::Key::Code keyCode) const {
    if (type_ == controllers::cPlayer1) {
        if      (keyCode == settings::C_playerIup)    slaveUp();
        else if (keyCode == settings::C_playerIleft)  slaveLeft();
        else if (keyCode == settings::C_playerIright) slaveRight();
        else if (keyCode == settings::C_playerIfire)  slaveFire();
        else if (keyCode == settings::C_playerIspecial)  slaveSpecial();
    }

    else if (type_ == controllers::cPlayer2) {
        if      (keyCode == settings::C_playerIIup)    slaveUp();
        else if (keyCode == settings::C_playerIIleft)  slaveLeft();
        else if (keyCode == settings::C_playerIIright) slaveRight();
        else if (keyCode == settings::C_playerIIfire)  slaveFire();
        else if (keyCode == settings::C_playerIIspecial)  slaveSpecial();
    }
}
