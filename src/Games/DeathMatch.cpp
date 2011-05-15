/* DeathMatch.cpp

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

# include "Games/DeathMatch.hpp"

# include "Teams/DMTeam.hpp"
# include "Players/players.hpp"
# include "System/settings.hpp"
# include "Media/music.hpp"
# include "Players/players.hpp"
# include "SpaceObjects/spaceObjects.hpp"
# include "Teams/teams.hpp"
# include "System/randomizer.hpp"

DeathMatch::DeathMatch():
    Game(games::gDeathMatch) {

    settings::C_EnabledWeapons  = settings::C_EnabledWeaponsByUser;
    settings::C_EnabledSpecials = settings::C_EnabledSpecialsByUser;

    music::play();

    if (settings::C_playerIteamL  | settings::C_playerIteamR)
        players::addPlayer (teams::addTeam(new DMTeam(settings::C_playerITeamColor)), controllers::cPlayer1);
    if (settings::C_playerIIteamL | settings::C_playerIIteamR)
        players::addPlayer (teams::addTeam(new DMTeam(settings::C_playerIITeamColor)), controllers::cPlayer2);

    for (int i=0; i<settings::C_botsDeath; ++i) {
        Team* newTeam = teams::addTeam(new DMTeam());
        Color3f color(newTeam->color());
        color.h(newTeam->color().h()+10*randomizer::random(-5, 5));
        color.v(newTeam->color().v()+randomizer::random(-0.5f, 0.5f));
        players::addPlayer(newTeam, controllers::cBot, color);
    }

    teams::assignHomes(spaceObjects::addHome(HOME_MIDDLE, 100, Color3f(0.9f, 0.7f, 1.0f)));
    players::createShips();

    spaceObjects::populateSpace(5.f, 10.f, 4);
    zones::createRaster(4,3);
}
void DeathMatch::draw() const {
    if (settings::C_drawZones) zones::draw();
    Game::draw();

}

void DeathMatch::restart() {
    Game::restart();

    teams::assignHomes(spaceObjects::addHome(HOME_MIDDLE, 100, Color3f(1.f, 1.f, 1.f)));
    players::createShips();

    spaceObjects::populateSpace(5.f, 10.f, 4);
    zones::createRaster(4,3);
}










