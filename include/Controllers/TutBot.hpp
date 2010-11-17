/* TutBot.hpp

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

# ifndef TUTBOT_HPP_INCLUDED
# define TUTBOT_HPP_INCLUDED

# include "Controllers/BotController.hpp"

# include <cfloat>

/// A very dumb bot for the tutorial.
/// It just floats around.

class TutBot: public BotController {
    public:
        /// Constructs a dumb BotController, attached to a Player.
        /// \param slave The Player, controlled by this bot.
        /// \param strength The individual strength of the bot. From 0 to 100.
        TutBot(Player* slave, float strength):
            BotController(slave, controllers::cTutBot, strength) {}

    private:
        /// Evaluates the situation of the bot.
        /// Calls all other private member methods for this purpose.
        void evaluate();

        /// Checks life and fuel.
        /// Changes the priority of BOT_LAND.
        void checkEnergy();
};

# endif // TUTBOT_HPP_INCLUDED

