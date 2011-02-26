/* MenuGame.cpp

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

# include "Games/MenuGame.hpp"

# include "Teams/GITeam.hpp"
# include "System/settings.hpp"
# include "Media/music.hpp"
# include "Hud/hud.hpp"
# include "Menu/menus.hpp"
# include "Menu/ChooseLanguage.hpp"
# include "Players/players.hpp"
# include "SpaceObjects/Home.hpp"
# include "Teams/teams.hpp"

MenuGame::MenuGame():
    Game(games::gMenu) {

    settings::C_EnabledWeapons  = weapons::wInsta;
    settings::C_EnabledSpecials = specials::sNoSpecial;

    music::playMenuMusic();

    Color3f rand = Color3f::random();

    Team* myTeamL = teams::addTeam(new GITeam(rand));
    Team* myTeamR = teams::addTeam(new GITeam(rand.inverted()));

    for (int i=0; i<4;  ++i)    players::addPlayer(myTeamL, controllers::cBot);
    for (int i=0; i<4; ++i)     players::addPlayer(myTeamR, controllers::cBot);

    Home* homeL = spaceObjects::addHome(HOME_LEFT,  myTeamL->color());
    Home* homeR = spaceObjects::addHome(HOME_RIGHT, myTeamR->color());

    teams::assignHomes(homeL, homeR);
    players::createShips();

    menus::showMain();

    if (settings::C_showSelectLanguage) {
        menus::showWindow(ChooseLanguage::get());
        settings::C_showSelectLanguage = false;
    }

    spaceObjects::populateSpace(40.f, 5.f, 2);
    zones::createRaster(4,3);
}

void MenuGame::restart() {
    Game::restart();

    Home* homeL = spaceObjects::addHome(HOME_LEFT,  teams::getTeamL()->color());
    Home* homeR = spaceObjects::addHome(HOME_RIGHT, teams::getTeamR()->color());

    teams::assignHomes(homeL, homeR);
    players::createShips();

    menus::showMain();

    spaceObjects::populateSpace(40.f, 5.f, 2);
    zones::createRaster(4,3);
}

/*
MenuGame::MenuGame():
    Game(games::gMenu) {

    settings::C_EnabledWeapons  = settings::C_EnabledWeaponsByUser;
    settings::C_EnabledSpecials = settings::C_EnabledSpecialsByUser;

    music::playMenuMusic();

    Color3f rand = Color3f::random();

    Team* myTeamL = teams::addTeam(new TDMTeam(rand));
    Team* myTeamR = teams::addTeam(new TDMTeam(rand.inverted()));

    for (int i=0; i<5;  ++i)    players::addPlayer(myTeamL, controllers::cBot);
    for (int i=0; i<5; ++i)     players::addPlayer(myTeamR, controllers::cBot);

    Home* homeL = spaceObjects::addHome(HOME_LEFT,  myTeamL->color());
    Home* homeR = spaceObjects::addHome(HOME_RIGHT, myTeamR->color());

    teams::assignHomes(homeL, homeR);
    players::createShips();

    menus::showMain();

    if (settings::C_showSelectLanguage) {
        menus::showWindow(ChooseLanguage::get());
        settings::C_showSelectLanguage = false;
    }

    spaceObjects::populateSpace(5.f, 10.f, 4);
    zones::createRaster(4,3);
}

void MenuGame::restart() {
    Game::restart();

    Home* homeL = spaceObjects::addHome(HOME_LEFT,  teams::getTeamL()->color());
    Home* homeR = spaceObjects::addHome(HOME_RIGHT, teams::getTeamR()->color());

    teams::assignHomes(homeL, homeR);
    players::createShips();

    menus::showMain();

    spaceObjects::populateSpace(5.f, 10.f, 4);
    zones::createRaster(4,3);
}
*/









