/* Tutorial.cpp

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

# include "Games/Tutorial.hpp"

# include "Players/Team.hpp"
# include "System/settings.hpp"
# include "Media/music.hpp"
# include "Items/items.hpp"
# include "DecoObjects/decoObjects.hpp"
# include "Menu/menus.hpp"
# include "Tutorial/TutWindow01.hpp"
# include "Tutorial/TutWindow02.hpp"
# include "Tutorial/TutWindow03.hpp"

Tutorial::Tutorial():
    Game(games::gTutorial),
    state_(0) {

    music::playGameMusic();

    players::addPlayer (players::addTeam(settings::C_playerIColor), controllers::cPlayer1);
    settings::C_playerIIteamL = false;
    settings::C_playerIIteamR = false;
    players::assignHomes(spaceObjects::addHome(Vector2f(1300.f, 450.f), settings::C_playerIColor));
    players::createShips();

    spaceObjects::addPlanet(Vector2f(680.f, 300.f), 150.f);
    spaceObjects::addPlanet(Vector2f(280.f, 650.f), 80.f);
}

void Tutorial::update() {
    Game::update();
    switch (state_) {
        case 0:
            if (!menus::visible()) {
                menus::showWindow(TutWindow01::get());
                ++state_;
            } break;
        case 1:
            if (!menus::visible()) {
                menus::showWindow(TutWindow02::get());
                ++state_;
            } break;
        case 2:
            if (!menus::visible()) {
                menus::showWindow(TutWindow03::get());
                ++state_;
            } break;
    }
}

void Tutorial::draw() const {
    zones::draw();
    Game::draw();
}

void Tutorial::restart() {
    Game::restart();

    state_ = 0;

    players::assignHomes(spaceObjects::addHome(Vector2f(1300.f, 450.f), settings::C_playerIColor));
    players::createShips();

    spaceObjects::addPlanet(Vector2f(680.f, 300.f), 150.f);
    spaceObjects::addPlanet(Vector2f(280.f, 650.f), 80.f);
}










