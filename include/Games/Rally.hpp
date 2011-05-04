/* Rally.hpp

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

# ifndef RALLY_HPP_INCLUDED
# define RALLY_HPP_INCLUDED

# include "Games/Game.hpp"

/// Game: Rally (RLY).
/// Be the first to reach a goal.

class Track;

class Rally: public Game {
    public:
        /// Ctor starts a new game.
        Rally();
        ~Rally();

        /// Draws the game.
        void draw() const;

        /// Restarts the game.
        void restart();

    private:
        Track* track_;
};

# endif // RALLY_HPP_INCLUDED








