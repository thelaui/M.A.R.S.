/* CKBot.hpp

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

# ifndef CKBOT_HPP_INCLUDED
# define CKBOT_HPP_INCLUDED

# include "Controllers/BotController.hpp"

/// A bot for CannonKeep.
/// This bot tries to gain control of the remote control of the Cannon.

class CKBot: public BotController {
    public:
        /// Constructs an BotController, attached to a Player.
        /// \param slave The Player, controlled by this bot.
        /// \param strength The individual strength of the bot. From 0 to 100.
        CKBot(Player* slave, float strength):
            BotController(slave, controllers::cCKBot, strength) {}

    private:
        /// Evaluates the situation of the bot.
        /// Calls all other private member methods for this purpose.
        void evaluate();

        /// Checks life and fuel.
        /// Changes the priority of BOT_LAND.
        void checkEnergy();

        /// Checks the position of the CannonControl.
        /// Changes the priority of BOT_GET_CANNON_CONTROL,
        /// BOT_ESCAPE and BOT_ATTACK_TARGET.
        void checkCannonController();
};

# endif // CKBOT_HPP_INCLUDED
