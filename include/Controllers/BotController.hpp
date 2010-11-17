/* BotController.hpp

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

# ifndef BOTCONTROLLER_HPP_INCLUDED
# define BOTCONTROLLER_HPP_INCLUDED

# define BOT_CHARGE 0
# define BOT_LAND 1
# define BOT_KICK_BALL_TE 2
# define BOT_KICK_BALL_OH 3
# define BOT_WAIT_FOR_BALL 4
# define BOT_ATTACK_TARGET 5
# define BOT_PROTECT_ZONE 6
# define BOT_CHANGE_WEAPON 7
# define BOT_GET_CANNON_CONTROL 8
# define BOT_ESCAPE 9
# define BOT_START_FIGHT 10

# include "Controllers/Controller.hpp"

# include "System/Vector2f.hpp"

# include <map>

class TacticalZone;

/// A basic controller with artificial intelligence.
/// It provides some basic actions for bots, like landing,
/// kicking the ball, switching weapons, attacking enemies
/// and so on. Each BotController has got a value from 0 to 100
/// for each of these actions, describing it's priority. It will
/// perform the action with the highest priority.
/// Twice a second the pure virtual protected member function
/// evaluate is called, which will change the priorities.
/// Furthermore, each BotController has got an aggro-table which
/// stores a threat-value for each opponent. Enemies gain threat
/// when they attack tha Player controlled by this bot. The enemiy
/// with the highest threat is most likely to be attacked.

class BotController: public Controller {
    public:
        /// Constructs the BotController, initializing it's members.
        /// \param slave The Player, controlled by this bot.
        /// \param type The type of the Controller.
        /// \param strength The individual strength of the bot. From 0 to 100.
        BotController(Player* slave, controllers::ControlType type, float strength);

        /// Performs the action with the highest priority.
        /// And calls evaluate twice a second.
        /*virtual*/ void update();

        /// Draws some debugging stuff.
        /// Like lines, showing where the bot heads.
        void draw();

        /// Resets private members.
        /// Should be called, when a game restarts.
        void reset();

    protected:
        /// Changes the priorities.
        /// Derived bots should implements this in different ways.
        virtual void evaluate() = 0;

        /// A vector storing all priorities.
        std::vector<int> actions_;

        /// The current target of attacks.
        Ship* target_;

        /// Stores a threat-value for each opponent.
        std::map<Ship*, float> aggroTable_;

        /// Stores time until weapons may be changed.
        float    weaponChangeTimer_;

    private:
        // actions
        void     charge();
        void     land();
        void     kickBallToEnemy();
        void     kickBallOutHome();
        void     waitForBall();
        void     attackTarget();
        void     protectZone();
        void     switchToWeapon();
        void     escape();
        void     startFight();

        // helper functions
        bool     turnTo(Vector2f const& location);
        bool     moveTo(Vector2f const& location, float stopFactor, bool avoidBall = true, float minDistance = 10.f, bool goingToLand = false);
        Vector2f calcPath(Vector2f const& endPoint, bool avoidBall);
        void     shootEnemy(Ship* enemyShip = NULL);
        void     shootPoint(Vector2f const& location, bool avoidTeamMembers = true);

        float evaluationTimer_;
        Vector2f nextRoutePoint_;
        TacticalZone* toCover_;
        const float strength_;

        //path variables
        Vector2f aimDirection_;
        Vector2f moveToPoint_;
        Vector2f nextPathPoint_;
};

# endif // BOTCONTROLLER_HPP_INCLUDED


