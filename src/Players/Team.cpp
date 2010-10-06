/* Team.cpp

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

# include "Players/Team.hpp"

# include "Zones/zones.hpp"

Team::Team(Color3f const& color):
    home_(NULL),
    color_(color),
    homeZone_(NULL),
    teamZone_(NULL),
    points_(0) {}

void Team::addMember(Player* player) {
    members_.push_back(player);
    player->team_ = this;
}

void Team::setHome(Home* home) {
    home_ = home;
    homeZone_ = zones::addHomeZone(home->location());
    teamZone_ = zones::addTeamZone(home->location());
}

void Team::resetPoints() {
    points_ = 0;
}
