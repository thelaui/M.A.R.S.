/* TacticalZone.cpp

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


# include "Zones/TacticalZone.hpp"

# include "SpaceObjects/SpaceObject.hpp"
# include "SpaceObjects/spaceObjects.hpp"
# include "SpaceObjects/Ship.hpp"
# include "Players/players.hpp"
# include "Players/Player.hpp"
# include "Players/Team.hpp"
# include <SFML/System.hpp>

# include <cmath>

TacticalZone::TacticalZone(Vector2f const& location, float radius) :
    radius_(radius),
    location_(location),
    covered_(false),
    shipCount_(0) {
        if (location_.x_ < 640.f)
            homeSide_ = 0;
        else
            homeSide_ = 1;
    }

bool TacticalZone::isInside(SpaceObject const& toBeChecked) const {
    return((toBeChecked.location()-location_).lengthSquare() <= radius_*radius_);
}

void TacticalZone::update() {
    shipCount_ = 0;
    if (homeSide_ == 0) {
        std::vector<Player*> const& players = players::getTeamL()->members();
        for (std::vector<Player*>::const_iterator it = players.begin(); it != players.end(); ++it)
            if (isInside(*(*it)->ship()))
                ++shipCount_;
        shipCount_ /= players.size();
    }
    else {
        std::vector<Player*> const& players = players::getTeamR()->members();
        for (std::vector<Player*>::const_iterator it = players.begin(); it != players.end(); ++it)
            if (isInside(*(*it)->ship()))
                ++shipCount_;
        shipCount_ /= players.size();
    }


    if (shipCount_ != 0  && radius_ * radius_ / zones::totalTacticalArea(homeSide_) < shipCount_)
        covered_ = true;
    else
        covered_ = false;
}

void TacticalZone::draw() const {
    glColor4f(0.f, 0.f, 0.f, 0.f);

    glBlendFunc(GL_ONE, GL_ONE);
    glBegin(GL_TRIANGLE_FAN);
         glVertex2f(location_.x_, location_.y_);
         if (covered_)
            glColor4f(0.7f, 0.5f, 0.f, 0.6f);
         else
            glColor4f(0.7f, 0.f, 0.f, 0.6f);
         for (double i=0; i<=2*M_PI; i+=M_PI*0.02)
            glVertex2f( location_.x_ + std::sin(i) * radius_, location_.y_ + std::cos(i) * radius_);
    glEnd();

}

Vector2f TacticalZone::getRandomPoint() const {
    Vector2f randomPoint;
    for (int i=0; i<100; ++i) {
        randomPoint = Vector2f(location_ + Vector2f::randDir()*(radius_ - 20.f));
        if (   randomPoint.x_ > 0.f
            && randomPoint.x_ < 1280.f
            && randomPoint.y_ > 0.f
            && randomPoint.y_ < 800.f) {
            bool fits = true;
            for (std::vector<SpaceObject*>::const_iterator it = spaceObjects::getObjects().begin(); it != spaceObjects::getObjects().end(); ++it) {
                if ((randomPoint - (*it)->location()).lengthSquare() < std::pow((*it)->radius() + 50, 2))
                    fits = false;
            }
            if (fits) break;
        }
    }
    return randomPoint;
}

Vector2f const& TacticalZone::location() const {
    return location_;
}

bool TacticalZone::covered() const {
    return covered_;
}
