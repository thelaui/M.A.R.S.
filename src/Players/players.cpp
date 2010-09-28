/* players.cpp

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

# include "Players/players.hpp"

# include "Players/Player.hpp"
# include "System/settings.hpp"
# include "System/generateName.hpp"
# include "Players/Team.hpp"

namespace players {
    namespace {
        std::vector<Team*> allTeams_;
        std::vector<Player*> allPlayers_;
        Player* playerI_;
        Player* playerII_;
        bool initialized_(false);

        void initLocalPlayers() {
            playerI_  = new Player(&settings::C_playerIName,  &settings::C_playerIColor , &settings::C_playerIShip, controllers::cPlayer1);
            playerII_ = new Player(&settings::C_playerIIName, &settings::C_playerIIColor, &settings::C_playerIIShip, controllers::cPlayer2);
            initialized_ = true;
        }
    }

    Team* addTeam(Color3f const& color) {
        Team* newTeam = new Team(color);
        allTeams_.push_back(newTeam);
        return newTeam;
    }

    void addPlayer (Team* team, controllers::ControlType type) {
        switch (type) {
            case controllers::cPlayer1:
                if (!initialized_) initLocalPlayers();
                team->addMember(playerI_);
                allPlayers_.push_back(playerI_);
                break;
            case controllers::cPlayer2:
                if (!initialized_) initLocalPlayers();
                team->addMember(playerII_);
                allPlayers_.push_back(playerII_);
                break;
            default:
                Player* bot = new Player(generateName::bot(((long)team%INT_MAX)/97), Color3f::random(), rand()%9+1, type);
                team->addMember(bot);
                allPlayers_.push_back(bot);
        }
    }

    void assignHomes(Home* home) {
        if (allTeams_.size() > 0)
            for (std::vector<Team*>::iterator it = allTeams_.begin(); it != allTeams_.end(); ++it)
                (*it)->setHome(home);
        else
            std::cout << "Cant assign Home Planet! No Teams are specified!\n";
    }

    void assignHomes(Home* homeL, Home* homeR) {
        if (allTeams_.size() == 2) {
            allTeams_[0]->setHome(homeL);
            allTeams_[1]->setHome(homeR);
        }
        else
            std::cout << "Cant assign two Home Planets! There have to be exactly two Teams specified!\n";
    }

    void createShips() {
        // temporary list of all homes
        std::vector<Home*>const& homes = spaceObjects::getHomes();
        // temporary lists of all inhabitants of all
        for (std::vector<Home*>::const_iterator homeIt = homes.begin(); homeIt != homes.end(); ++homeIt) {
            std::vector<Player*> inhabitants;
            for (std::vector<Player*>::iterator playIt = allPlayers_.begin(); playIt != allPlayers_.end(); ++playIt) {
                if ((*playIt)->team()->home() == (*homeIt))
                    inhabitants.push_back(*playIt);
            }
            (*homeIt)->createShips(inhabitants);
        }
    }

    Player const* getPlayerI () {
        return playerI_;
    }

    Player const* getPlayerII() {
        return playerII_;
    }

    Team const* getTeamL() {
        return allTeams_[0];
    }

    Team const* getTeamR() {
        return allTeams_[1];
    }

    std::vector<Team*> const& getAllTeams() {
        return allTeams_;
    }

    Team const* getEnemy(Team const* checker) {
        return checker == allTeams_[0] ? allTeams_[1] : allTeams_[0];
    }

    int getFirstPoints() {
        int highest(0);
        for (std::vector<Team*>::iterator it = allTeams_.begin(); it != allTeams_.end(); ++it)
            if ((*it)->points() > highest)
                highest = (*it)->points();
        return highest;
    }

    int getSecondPoints() {
        int highest(getFirstPoints());
        int second(0);
        for (std::vector<Team*>::iterator it = allTeams_.begin(); it != allTeams_.end(); ++it)
            if ((*it)->points() > second && (*it)->points() != highest)
                second = (*it)->points();
        return second;
    }

    void resetPoints() {
        for (std::vector<Team*>::iterator it = allTeams_.begin(); it != allTeams_.end(); ++it)
            (*it)->resetPoints();
    }

    void clear() {
        for (std::vector<Team*>::iterator it = allTeams_.begin(); it != allTeams_.end(); ++it)
            delete *it;
        for (std::vector<Player*>::iterator it = allPlayers_.begin(); it != allPlayers_.end(); ++it)
            delete *it;
        allPlayers_.clear();
        allTeams_.clear();
        initialized_ = false;
    }
}
