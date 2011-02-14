/* TeamDeathMatch.cpp

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

# include "Games/TeamDeathMatch.hpp"

# include "Teams/TDMTeam.hpp"
# include "System/settings.hpp"
# include "Media/music.hpp"
# include "Players/players.hpp"
# include "SpaceObjects/spaceObjects.hpp"
# include "Teams/teams.hpp"

TeamDeathMatch::TeamDeathMatch():
    Game(games::gTeamDeathMatch) {

    settings::C_EnabledWeapons  = settings::C_EnabledWeaponsByUser;
    settings::C_EnabledSpecials = settings::C_EnabledSpecialsByUser;

    music::playGameMusic();

    Team* myTeamL = NULL;
    Team* myTeamR = NULL;

    if (settings::C_playerIteamL) {
        myTeamL = new TDMTeam(settings::C_playerITeamColor);
        players::addPlayer(myTeamL, controllers::cPlayer1);
    }
    else if (settings::C_playerIteamR) {
        myTeamR = new TDMTeam(settings::C_playerITeamColor);
        players::addPlayer(myTeamR, controllers::cPlayer1);
    }

    if (settings::C_playerIIteamL) {
        if (!myTeamL) myTeamL = new TDMTeam(settings::C_playerIITeamColor);
        players::addPlayer(myTeamL, controllers::cPlayer2);
    }
    else if (settings::C_playerIIteamR) {
        if (!myTeamR) myTeamR = new TDMTeam(settings::C_playerIITeamColor);
        players::addPlayer(myTeamR, controllers::cPlayer2);
    }

    if (!myTeamR && !myTeamL) {
        Color3f rand = Color3f::random();
        myTeamL = new TDMTeam(rand.inverted());
        myTeamR = new TDMTeam(rand);

    }
    else if (!myTeamL) {
        myTeamL = new TDMTeam(myTeamR->color().inverted());
    }
    else if (!myTeamR) {
        myTeamR = new TDMTeam(myTeamL->color().inverted());
    }

    teams::addTeam(myTeamL);
    teams::addTeam(myTeamR);

    for (int i=0; i<settings::C_botsLeft;  ++i)     players::addPlayer(myTeamL, controllers::cBot);
    for (int i=0; i<settings::C_botsRight; ++i)     players::addPlayer(myTeamR, controllers::cBot);

    Home* homeL = spaceObjects::addHome(HOME_LEFT,  myTeamL->color());
    Home* homeR = spaceObjects::addHome(HOME_RIGHT, myTeamR->color());

    teams::assignHomes(homeL, homeR);
    players::createShips();

    spaceObjects::populateSpace();
    zones::createRaster(4,3);
}

void TeamDeathMatch::draw() const {
    if (settings::C_drawZones) zones::draw();
    Game::draw();
}

void TeamDeathMatch::restart() {
    Game::restart();

    Home* homeL = spaceObjects::addHome(HOME_LEFT,  teams::getTeamL()->color());
    Home* homeR = spaceObjects::addHome(HOME_RIGHT, teams::getTeamR()->color());

    teams::assignHomes(homeL, homeR);
    players::createShips();

    spaceObjects::populateSpace();
    zones::createRaster(4,3);
}











