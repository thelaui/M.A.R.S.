/* KeyController.hpp

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

# ifndef KEYCONTROLLER_HPP_INCLUDED
# define KEYCONTROLLER_HPP_INCLUDED

# include "Controllers/Controller.hpp"

# include <SFML/Window.hpp>

/// A Controller which updates it's associated Ship based on keyboard events.
/// Used for local players.

class KeyController: public Controller {
    public:
        /// Constructs a KeyController, attached to a Player.
        /// \param slave The Player, controlled by the Keyboard.
        /// \param type Should be cPlayer1 or cPlayer2.
        KeyController(controllers::ControlType type, Player* slave);

        /// Updates the Controller, based on the current input state.
        void update(sf::Input const& input) const;

        /// Updates the Controller, based on single key events.
        void update(sf::Key::Code keyCode)  const;
};

# endif // KEYCONTROLLER_HPP_INCLUDED

