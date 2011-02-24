/* Player.hpp

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

# ifndef PLAYER_HPP_INCLUDED
# define PLAYER_HPP_INCLUDED

# include "SpaceObjects/Ship.hpp"
# include "Players/players.hpp"
# include "Controllers/Controller.hpp"

class Team;

class Player {
    public:
        Player(controllers::ControlType controlType);

        void                      resetPoints();

        Team*                     team()    const {return team_;}
        Ship*                     ship()    const {return ship_;}
        controllers::ControlType  type()    const {return controlType_;}

        virtual Color3f const&    color()   const = 0;
        virtual sf::String const& name()    const = 0;
        virtual int               graphic() const = 0;

        friend class Team;
        friend class Ship;
        friend class Controller;
        friend class TabStats;
        friend class Cannon;

    private:
        Ship* ship_;
        Team* team_;
        int points_, frags_, suicides_, deaths_, teamKills_, cannonShots_;
        controllers::ControlType controlType_;
};


# endif // PLAYER_HPP_INCLUDED
