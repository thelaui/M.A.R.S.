/* TeamDeathMatch.cpp

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

# include "Games/TeamDeathMatch.hpp"

# include "Players/Team.hpp"
# include "System/settings.hpp"
# include "Media/music.hpp"

TeamDeathMatch::TeamDeathMatch():
    Game(games::gTeamDeathMatch) {

    music::playGameMusic();

    Team* myTeamL = NULL;
    Team* myTeamR = NULL;

    if (settings::C_playerIteamL) {
        myTeamL = new Team(settings::C_playerITeamColor);
        players::addPlayer(myTeamL, controllers::cPlayer1);
    }
    else if (settings::C_playerIteamR) {
        myTeamR = new Team(settings::C_playerITeamColor);
        players::addPlayer(myTeamR, controllers::cPlayer1);
    }

    if (settings::C_playerIIteamL) {
        if (!myTeamL) myTeamL = new Team(settings::C_playerIITeamColor);
        players::addPlayer(myTeamL, controllers::cPlayer2);
    }
    else if (settings::C_playerIIteamR) {
        if (!myTeamR) myTeamR = new Team(settings::C_playerIITeamColor);
        players::addPlayer(myTeamR, controllers::cPlayer2);
    }

    if (!myTeamR && !myTeamL) {
        Color3f rand = Color3f::random();
        myTeamL = new Team(rand.inverted());
        myTeamR = new Team(rand);

    }
    else if (!myTeamL) {
        myTeamL = new Team(myTeamR->color().inverted());
    }
    else if (!myTeamR) {
        myTeamR = new Team(myTeamL->color().inverted());
    }

    players::addTeam(myTeamL);
    players::addTeam(myTeamR);

    for (int i=0; i<settings::C_botsLeft;  ++i)     players::addPlayer(myTeamL, controllers::cDMBot);
    for (int i=0; i<settings::C_botsRight; ++i)     players::addPlayer(myTeamR, controllers::cDMBot);

    Home* homeL = spaceObjects::addHome(HOME_LEFT,  myTeamL->color());
    Home* homeR = spaceObjects::addHome(HOME_RIGHT, myTeamR->color());

    players::assignHomes(homeL, homeR);
    players::createShips();

    createSpace();
    zones::createRaster(4,3);
}

void TeamDeathMatch::draw() const {
    if (settings::C_drawZones) zones::draw();
    Game::draw();
}

void TeamDeathMatch::restart() {
    Game::restart();

    Home* homeL = spaceObjects::addHome(HOME_LEFT,  players::getTeamL()->color());
    Home* homeR = spaceObjects::addHome(HOME_RIGHT, players::getTeamR()->color());

    players::assignHomes(homeL, homeR);
    players::createShips();

    createSpace();
    zones::createRaster(4,3);
}











