/* SpaceBall.hpp

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

# ifndef SPACEBALL_HPP_INCLUDED
# define SPACEBALL_HPP_INCLUDED

# include "Games/Game.hpp"

/// Game: SpaceBall (SB).
/// The original M.A.R.S. GameType. A little bit like soccer: Each team tries to destroy
/// the opponents home planet by dropping a huge ball on it.

class SpaceBall: public Game {
    public:
        /// Ctor starts a new game.
        SpaceBall();

        /// Draws the game.
        void draw() const;

        /// Restarts the game.
        void restart();
};

# endif // SPACEBALL_HPP_INCLUDED






