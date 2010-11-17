/* AggroBot.hpp

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

# ifndef AGGROBOT_HPP_INCLUDED
# define AGGROBOT_HPP_INCLUDED

# include "Controllers/BotController.hpp"

/// An aggressive bot for SpaceBall.
/// This bot tries to score by smashing the ball into
/// the opponents Home.

class AggroBot: public BotController {
    public:
        /// Constructs an aggressive BotController, attached to a Player.
        /// \param slave The Player, controlled by this bot.
        /// \param strength The individual strength of the bot. From 0 to 100.
        AggroBot(Player* slave, float strength):
            BotController(slave, controllers::cAggroBot, strength) {}

    private:
        /// Evaluates the situation of the bot.
        /// Calls all other private member methods for this purpose.
        void evaluate();

        /// Checks life and fuel.
        /// Changes the priority of BOT_LAND.
        void checkEnergy();

        /// Checks the position of the ball.
        /// Changes the priority of BOT_KICK_BALL_TE, BOT_KICK_BALL_OH
        /// and BOT_WAIT_FOR_BALL.
        void checkBall();

        /// Checks, whether the Home has been destroyed already.
        /// If so, BOT_PROTECT_ZONE is set to 100, which prevents the
        /// bot from trying to land.
        void checkHome();
};

# endif // AGGROBOT_HPP_INCLUDED




