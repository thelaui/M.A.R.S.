/* SpaceBall.cpp

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

# include "Games/SpaceBall.hpp"

# include "Players/Team.hpp"
# include "System/settings.hpp"
# include "Media/music.hpp"
# include "SpaceObjects/balls.hpp"

SpaceBall::SpaceBall():
    Game(games::gSpaceBall) {

    music::playGameMusic();

    Color3f rand = Color3f::random();
    Color3f randInv = rand;
    rand.h(rand.h() + 180.f);

    Team* myTeamL = players::addTeam(rand);
    Team* myTeamR = players::addTeam(randInv);

    if      (settings::C_playerIteamL)  players::addPlayer  (myTeamL, controllers::cPlayer1);
    else if (settings::C_playerIteamR)  players::addPlayer  (myTeamR, controllers::cPlayer1);
    if      (settings::C_playerIIteamL) players::addPlayer  (myTeamL, controllers::cPlayer2);
    else if (settings::C_playerIIteamR) players::addPlayer  (myTeamR, controllers::cPlayer2);

    for (int i=0; i<settings::C_botsLeft/2;  ++i)                       players::addPlayer(myTeamL, controllers::cAggroBot);
    for (int i=settings::C_botsLeft/2; i<settings::C_botsLeft;  ++i)    players::addPlayer(myTeamL, controllers::cDefBot);
    for (int i=0; i<settings::C_botsRight/2;  ++i)                      players::addPlayer(myTeamR, controllers::cAggroBot);
    for (int i=settings::C_botsRight/2; i<settings::C_botsRight;  ++i)  players::addPlayer(myTeamR, controllers::cDefBot);

    Home* homeL = spaceObjects::addHome(HOME_LEFT,  myTeamL->color());
    Home* homeR = spaceObjects::addHome(HOME_RIGHT, myTeamR->color());

    players::assignHomes(homeL, homeR);
    players::createShips();

    balls::addBall();

    createSpace();

    zones::detectTacticalZones();
}

void SpaceBall::draw() const {
    if (settings::C_drawZones) zones::draw();
    Game::draw();
}

void SpaceBall::restart() {
    Game::restart();

    Home* homeL = spaceObjects::addHome(HOME_LEFT,  players::getTeamL()->color());
    Home* homeR = spaceObjects::addHome(HOME_RIGHT, players::getTeamR()->color());

    players::assignHomes(homeL, homeR);
    players::createShips();

    balls::addBall();

    createSpace();

    zones::detectTacticalZones();
}









