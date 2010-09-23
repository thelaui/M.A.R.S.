/* BotController.hpp

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

# include "Controllers/Controller.hpp"

# include "System/Vector2f.hpp"

class TacticalZone;

class BotController: public Controller {
    public:
        BotController(Player* slave, controllers::ControlType type);

        /*virtual*/ void update() ;

    protected:
        virtual void evaluate() = 0;

        virtual void checkEnergy() = 0;

        std::vector<int> actions_;
        Ship* target_;

        float    weaponChangeTimer_;

        bool     moveTo(Vector2f const& location, float stopFactor, bool avoidBall = true, float minDistance = 10.f) const;

    private:
        // actions
        void     charge() const;
        void     land() const;
        void     kickBallToEnemy() const;
        void     kickBallOutHome() const;
        void     waitForBall() const;
        void     attackTarget();
        void     protectZone();
        void     switchToWeapon();
        void     escape();

        // helper functions
        void     turnTo(Vector2f const& location) const;
        Vector2f calcPath(Vector2f const& endPoint, bool avoidBall) const;
        void     shootEnemy(Ship* enemyShip = NULL) const;
        void     shootBall() const;

        mutable float evaluationTimer_;
        Vector2f nextRoutePoint_;
        TacticalZone* toCover_;
};

# endif // BOTCONTROLLER_HPP_INCLUDED


