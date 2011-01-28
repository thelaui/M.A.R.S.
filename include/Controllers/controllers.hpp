/* controllers.hpp

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

# ifndef CONTROLLERS_HPP_INCLUDED
# define CONTROLLERS_HPP_INCLUDED

# include <SFML/Window.hpp>

class Player;
class BotController;
class KeyController;

/// A namespace which controls all Ships.
/// Local Player's Ships are controlled by KeyControllers, Bots
/// are controlled by BotControllers.

namespace controllers {
    /// A list of all supported control types.
    enum ControlType {
        cBot,           ///< A bot controlled by AI.
        cPlayer1,       ///< Local player one.
        cPlayer2        ///< Local player two.
    };

    /// Updates all controllers.
    void update();

    /// Notifies KeyControllers of single key events.
    void singleKeyEvent(sf::Key::Code keyCode);

    /// Draws some debugging stuff.
    /// Like lines, showing where the bot heads.
    void draw();

    /// Adds a controller to the game.
    /// \param slave The Player, controlled by this bot.
    /// \param type The type of the Controller.
    /// \param strength The individual strength of the bot. From 0 to 100.
    BotController* addBotController(Player* slave, float strength = 1.f);
    KeyController* addKeyController(Player* slave);

    /// Resets Controllers.
    /// Should be called, when a game restarts.
    void resetBots();

    /// Deletes all Controllers.
    void clear();
}

# endif // CONTROLLERS_HPP_INCLUDED

