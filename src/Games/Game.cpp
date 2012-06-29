/* Game.cpp

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

# include "Games/Game.hpp"

# include "SpaceObjects/spaceObjects.hpp"
# include "SpaceObjects/balls.hpp"
# include "SpaceObjects/ships.hpp"
# include "Particles/particles.hpp"
# include "System/timer.hpp"
# include "System/settings.hpp"
# include "Players/players.hpp"
# include "Teams/Team.hpp"
# include "Hud/hud.hpp"
# include "Zones/zones.hpp"
# include "DecoObjects/decoObjects.hpp"
# include "Items/items.hpp"
# include "Menu/menus.hpp"
# include "System/window.hpp"
# include "Media/announcer.hpp"
# include "Shaders/postFX.hpp"
# include "SpaceObjects/stars.hpp"
# include "TrailEffects/trailEffects.hpp"
# include "Teams/teams.hpp"

Game::Game(games::GameType const& type):
    type_(type),
    startTime_(timer::totalTime()),
    ended_(false) {
        switch (type_) {
            case games::gSpaceBall:  pointLimit_ = settings::C_pointLimitSB;    break;
            case games::gCannonKeep: pointLimit_ = settings::C_pointLimitCK;    break;
            case games::gDeathMatch: pointLimit_ = settings::C_pointLimitDM;  break;
            case games::gMenu:       pointLimit_ = 9999999;                   break;
            default:                 pointLimit_ = settings::C_pointLimitTDM;
        }

    hud::init();
    stars::init();
    }

Game::~Game() {
    items::clear();
    ships::clear();
    balls::clear();
    physics::clear();
    particles::clear();
    spaceObjects::clear();
    controllers::clear();
    teams::clear();
    players::clear();
    zones::clear();
    decoObjects::clear();
    trailEffects::clear();
    timer::resetSlowMotion();
}

void Game::update() {
    announcer::update();
    hud::update();
    if ((!menus::visible()) || (type_ == games::gMenu)) {
        spaceObjects::update();
        particles::update();
        items::update();
        postFX::update();
        trailEffects::update();

        if (teams::getFirstPoints() >= pointLimit_) {
            if (!ended_) {
                Team* best(NULL);
                int   most(0);
                for (std::vector<Team*>::const_iterator it = teams::getAllTeams().begin(); it != teams::getAllTeams().end(); ++it)
                    if (most < (*it)->points()) {
                        best = *it;
                        most = (*it)->points();
                    }
                if (best)
                    best->addVictory();
                ended_ = true;
            }
            hud::displayStats();

            if (type_ != games::gDeathMatch)
                hud::displayPoints();
        }
        else {
            decoObjects::update();
            ships::update();
            balls::update();
            teams::update();
            controllers::update();
            zones::update();
        }
    }
    else
        startTime_ += timer::frameTime();
}

void Game::draw() const {
    if (settings::C_StarField)
        particles::drawStars();
    particles::drawLower();
    trailEffects::draw();
    controllers::draw();
    ships::draw();
    balls::draw();
    spaceObjects::draw();
    particles::drawHigher();
    decoObjects::draw();
    items::draw();
}

void Game::restart() {
    items::clear();
    ships::clear();
    balls::clear();
    physics::clear();
    particles::clear();
    spaceObjects::clear();
    zones::clear();
    decoObjects::clear();
    trailEffects::clear();
    teams::resetTeamPoints();
    players::resetPlayerPoints();
    startTime_ = timer::totalTime();
    controllers::resetBots();
    stars::init();
    hud::displayStats(false);
    hud::displayPoints(false);
    timer::resetSlowMotion();
    ended_ = false;
}

games::GameType Game::type() const {
    return type_;
}

float Game::elapsedTime() const {
    return timer::totalTime() - startTime_;
}

bool Game::ended() const {
    return ended_;
}


