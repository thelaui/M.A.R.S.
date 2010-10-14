/* players.hpp

Copyright (c) 2010 by Felix Lauer und Simon Schneegans

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

# ifndef PLAYERS_HPP_INCLUDED
# define PLAYERS_HPP_INCLUDED

# include "System/Color3f.hpp"
# include "Controllers/controllers.hpp"

class Team;
class Home;
class Player;

namespace players {
    Team* addTeam(Color3f const& = Color3f::random());

    void addPlayer (Team* team, controllers::ControlType type);

    void assignHomes(Home* home);
    void assignHomes(Home* homeL, Home* homeR);

    void assignZones();

    void createShips();

    void resetTeamPoints();
    void resetPlayerPoints();
    void clear();

    Player const* getPlayerI ();
    Player const* getPlayerII();

    Team   const* getTeamL();
    Team   const* getTeamR();
    std::vector<Team*> const& getAllTeams();
    Team   const*             getEnemy(Team const* checker);

    int getFirstPoints();
    int getSecondPoints();
}

# endif // PLAYERS_HPP_INCLUDED
