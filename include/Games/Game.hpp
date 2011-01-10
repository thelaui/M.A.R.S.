/* Game.hpp

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

# ifndef GAME_HPP_INCLUDED
# define GAME_HPP_INCLUDED

# include "Games/games.hpp"

/// Base class for all games.
/// Provides some basic methods.

class Game {
    public:
        /// Ctor starts a game based on the given GameType.
        /// The previous game has to be deleted before creating
        /// a new one.
        Game(games::GameType const& type);

        /// Destructor clears all game related namespaces.
        /// E.g.: ships::clear(), balls::clear(), physics::clear(),
        /// particles::clear(), spaceObjects::clear(),
        /// controllers::clear(), players::clear(), zones::clear(),
        /// decoObjects::clear(), items::clear().
        ~Game();

        /// Updates the game.
        /// Has to be called every frame.
        /// Derived Games may extend this function.
        virtual void update();

        /// Draws the game.
        /// Derived Games may extend this function.
        virtual void draw() const;

        /// Restarts the game.
        /// All settings are kept. (most important: The teams aren't changed).
        /// Derived Games may extend this function.
        virtual void restart();

        /// Returns the game's GameType.
        games::GameType type() const;

        /// Returns the elapsed time since it has been created.
        float elapsedTime() const;

    protected:
        /// Populates the space with planets, suns and black holes.
        void createSpace() const;

    private:
        /// Stores the game's GameType
        games::GameType type_;

        /// Stores the time when the games was created.
        float startTime_;

        /// Stores the maximum points until the game finishes.
        int pointLimit_;
};

# endif // GAME_HPP_INCLUDED





