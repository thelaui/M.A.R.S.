/* CannonKeep.hpp

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

# ifndef CANNONKEEP_HPP_INCLUDED
# define CANNONKEEP_HPP_INCLUDED

# include "Games/Game.hpp"

/// Game: CannonKeep (CK).
/// A funny game, where both teams aim on keeping the control of
/// gigantic weapon of mass destruction in order to smash the
/// oponents home planet. The team which is the first to destroy
/// the others planet will win the match.

class CannonKeep: public Game {
    public:
        /// Ctor starts a new game.
        CannonKeep();

        /// Draws the game.
        void draw() const;

        /// Restarts the game.
        void restart();
};

# endif // CANNONKEEP_HPP_INCLUDED
