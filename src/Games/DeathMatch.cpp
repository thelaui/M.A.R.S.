/* DeathMatch.cpp

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

# include "Players/Team.hpp"
# include "System/settings.hpp"

DeathMatch::DeathMatch():
    Game(games::gDeathMatch) {

    if (settings::C_playerIteamL  | settings::C_playerIteamR)
        players::addPlayer (players::addTeam(), controllers::cPlayer1);
    if (settings::C_playerIIteamL | settings::C_playerIIteamR)
        players::addPlayer (players::addTeam(), controllers::cPlayer2);

    for (int i=0; i<settings::C_botsDeath; ++i) players::addPlayer(players::addTeam(), controllers::cDMBot);

    players::assignHomes(spaceObjects::addHome(HOME_MIDDLE, Color3f(1.f, 1.f, 1.f)));
    players::createShips();

    createSpace();
    zones::createRaster(4,3);
}
void DeathMatch::draw() const {
    if (settings::C_drawZones) zones::draw();
    Game::draw();

}

void DeathMatch::restart() {
    Game::restart();

    players::assignHomes(spaceObjects::addHome(HOME_MIDDLE, Color3f(1.f, 1.f, 1.f)));
    players::createShips();

    createSpace();
    zones::createRaster(4,3);
}










