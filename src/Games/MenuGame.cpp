/* MenuGame.cpp

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

# include "Games/MenuGame.hpp"

# include "Players/Team.hpp"
# include "System/settings.hpp"
# include "Media/music.hpp"
# include "Hud/hud.hpp"
# include "Menu/menus.hpp"
# include "Menu/ChooseLanguage.hpp"

MenuGame::MenuGame():
    Game(games::gMenu) {

    music::playMenuMusic();

    Color3f rand = Color3f::random();
    Color3f randInv = rand;
    rand.h(rand.h() + 180.f);

    Team* myTeamL = players::addTeam(rand);
    Team* myTeamR = players::addTeam(randInv);

    for (int i=0; i<5;  ++i)    players::addPlayer(myTeamL, controllers::cDMBot);
    for (int i=0; i<5; ++i)     players::addPlayer(myTeamR, controllers::cDMBot);

    Home* homeL = spaceObjects::addHome(HOME_LEFT,  myTeamL->color());
    Home* homeR = spaceObjects::addHome(HOME_RIGHT, myTeamR->color());

    players::assignHomes(homeL, homeR);
    players::createShips();

    menus::showMain();

    if (settings::C_showSelectLanguage) {
        menus::showWindow(ChooseLanguage::get());
        settings::C_showSelectLanguage = false;
    }

    createSpace();
    zones::createRaster(4,3);
}

void MenuGame::restart() {
    Game::restart();

    Home* homeL = spaceObjects::addHome(HOME_LEFT,  players::getTeamL()->color());
    Home* homeR = spaceObjects::addHome(HOME_RIGHT, players::getTeamR()->color());

    players::assignHomes(homeL, homeR);
    players::createShips();

    menus::showMain();

    createSpace();
    zones::createRaster(4,3);
}










