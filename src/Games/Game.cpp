/* Game.cpp

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

# include "Games/Game.hpp"

# include "SpaceObjects/spaceObjects.hpp"
# include "SpaceObjects/balls.hpp"
# include "SpaceObjects/ships.hpp"
# include "Particles/particles.hpp"
# include "System/timer.hpp"
# include "System/settings.hpp"
# include "Media/music.hpp"
# include "Players/players.hpp"
# include "Players/Team.hpp"
# include "Hud/hud.hpp"
# include "Zones/zones.hpp"
# include "DecoObjects/decoObjects.hpp"
# include "Items/items.hpp"
# include "Menu/menus.hpp"
# include "System/window.hpp"
# include "Media/music.hpp"
# include "Shaders/postFX.hpp"
# include "SpaceObjects/stars.hpp"

Game::Game(games::GameType const& type):
    type_(type),
    startTime_(timer::totalTime()) {
        switch (type_) {
            case games::gSpaceBall: case games::gCannonKeep: pointLimit_ = settings::C_pointLimit;    break;
            case games::gDeathMatch:                         pointLimit_ = settings::C_pointLimitDM;  break;
            case games::gMenu:                               pointLimit_ = 9999999;                   break;
            default:                                         pointLimit_ = settings::C_pointLimitTDM;
        }

    hud::init();
    stars::init();
    }

Game::~Game() {
    ships::clear();
    balls::clear();
    physics::clear();
    particles::clear();
    spaceObjects::clear();
    controllers::clear();
    players::clear();
    zones::clear();
    decoObjects::clear();
    items::clear();
}

void Game::update() {
    music::update();
    if ((!menus::visible()) || (type_ == games::gMenu)) {
        hud::update();
        if (players::getFirstPoints() < pointLimit_) {
            controllers::update();
            if ((elapsedTime() > 4.f) || (type_ == games::gTutorial))
                ships::update();
            balls::update();
            spaceObjects::update();
        }
        else if(window::getInput().IsMouseButtonDown(sf::Mouse::Left)) {
            games::restart();
            hud::displayStats(false);
        }
        else
            hud::displayStats();
        particles::update();
        zones::update();
        decoObjects::update();
        items::update();
        postFX::update();
    }
    else
        startTime_ += timer::frameTime();
}

void Game::draw() const {
    particles::drawLower();
    ships::draw();
    balls::draw();
    spaceObjects::draw();
    particles::drawHigher();
    decoObjects::draw();
    items::draw();
    controllers::draw();
}

void Game::restart() {
    ships::clear();
    balls::clear();
    physics::clear();
    particles::clear();
    spaceObjects::clear();
    zones::clear();
    decoObjects::clear();
    items::clear();
    if (players::getFirstPoints() >= pointLimit_) {
        players::resetTeamPoints();
        players::resetPlayerPoints();
    }
    if (games::type() == games::gCannonKeep || games::type() == games::gSpaceBall)
        players::resetPlayerPoints();
    startTime_ = timer::totalTime();
    controllers::resetBots();
    stars::init();
}

games::GameType Game::type() const {
    return type_;
}

float Game::elapsedTime() const {
    return timer::totalTime() - startTime_;
}

void Game::createSpace() const {
    int sunCount(2), holeCount(1), planetCount(4);
    int count = rand()%5 + 1;
    if (count == 1) count += rand()%2;
    if (count == 5) count -= rand()%2;

    while (count > 0) {
        int type = rand()%7;
        switch (type) {
            case 0:
                if (holeCount > 0) {
                    spaceObjects::addBlackHole();
                    --count;
                    --holeCount;
                }
                break;
            case 1:
                if (sunCount > 0) {
                    spaceObjects::addSun();
                    --count;
                    --sunCount;
                }
                break;
            default:
                if (planetCount > 0) {
                    spaceObjects::addPlanet();
                    --count;
                    --planetCount;
                }
        }

    }
}




