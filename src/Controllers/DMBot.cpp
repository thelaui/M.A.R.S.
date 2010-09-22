/* DMBot.cpp

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

# include "Controllers/DMBot.hpp"

# include "SpaceObjects/Ship.hpp"
# include "Zones/zones.hpp"
# include "Players/Player.hpp"

# include <cmath>

void DMBot::evaluate() {
    checkEnergy();
}

void DMBot::checkEnergy() {
    actions_[BOT_LAND] = 100;
    //actions_[BOT_LAND] = (std::pow((100 - ship()->getLife()) * (100 - ship()->getFuel()), 2))/1000000;     // max 100, min 0
}


