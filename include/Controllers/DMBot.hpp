/* DMBot.hpp

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

# ifndef DMBOT_HPP_INCLUDED
# define DMBOT_HPP_INCLUDED

# include "Controllers/BotController.hpp"

# include "Players/Player.hpp"
# include "SpaceObjects/Ship.hpp"

# include <cfloat>

/// A bot for DeathMatch.
/// It uses his aggro-table to attack enemies.

class DMBot: public BotController {
    public:
        /// Constructs a BotController, attached to a Player.
        /// \param slave The Player, controlled by this bot.
        /// \param strength The individual strength of the bot. From 0 to 100.
        DMBot(Player* slave, float strength):
            BotController(slave, controllers::cDMBot, strength),
            lastFrameLife_(FLT_MAX) {}

    private:
        /// Evaluates the situation of the bot.
        /// Calls all other private member methods for this purpose.
        void evaluate();

        /// Checks whether an enemy has got more aggro than another.
        /// If so, the new one is attacked.
        void checkAggro();

        /// Checks life and fuel.
        /// Changes the priority of BOT_LAND.
        void checkEnergy();

        /// Checks whether the current target has been killed.
        /// Changes the priority of BOT_ATTACK_TARGET and BOT_START_FIGHT.
        void checkEnemies();

        /// Stores the Player's Ship's life.
        /// Important for realizing when life is lost.
        float lastFrameLife_;
};

# endif // DMBOT_HPP_INCLUDED
