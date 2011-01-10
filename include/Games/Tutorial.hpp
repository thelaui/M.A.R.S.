/* Tutorial.hpp

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

# ifndef TUTORIAL_HPP_INCLUDED
# define TUTORIAL_HPP_INCLUDED

# include "Games/Game.hpp"

class Home;
class Player;

/// Game: Tutorial.
/// This is the tutorial which, in fact,  is a special game.

class Tutorial: public Game {
    public:
        /// Ctor starts the tutorial.
        Tutorial();

        /// Updates the game.
        void update();

        /// Draws the game.
        void draw() const;

        /// Restarts the game.
        void restart();

    private:
        /// State of the tutorial.
        /// Increases step by step when advancing in th tutorial.
        int state_;

        /// Used for jumps between states.
        int savedState_;

        /// Pointer to the evil left-hand planet.
        Home* evilHome_;

        /// Pointer to the weak opponent.
        Player* evilPlayer1_;

        /// Pointer to the strong opponent.
        Player* evilPlayer2_;

        /// Some data
        ///@{
        float timer_, deadTimer_;
        bool dead_, life_, fuel_, name_;
        ///@}
};

# endif // TUTORIAL_HPP_INCLUDED
