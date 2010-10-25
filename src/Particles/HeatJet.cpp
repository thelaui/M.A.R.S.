/* HeatJet.cpp

Copyright (c) 2010 by Felix Lauer and Simon Schneegans

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

# include "Particles/HeatJet.hpp"

# include "System/timer.hpp"
# include "System/settings.hpp"

std::list<HeatJet*> HeatJet::activeParticles_;

HeatJet::HeatJet(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Color3f const& color, Player* damageSource):
           Particle<HeatJet>(spaceObjects::oHeatJet, location, 1.f, 0.f, sf::Randomizer::Random(0.5f, 1.0f)) {

    Vector2f distortion(Vector2f::randDirLen());
    location_ = location + distortion;
    velocity_ = velocity + direction*(-60.f) + distortion*20.f;

}

void HeatJet::update() {
    float time = timer::frameTime();

    // update Size
    radius_ = lifeTime_*30 + 1;

    location_ += velocity_*time;

    lifeTime_ += time;
}

void HeatJet::draw() const {
    glColor4f(1.f, 1.f, 1.f, 1.f - lifeTime_/totalLifeTime_);
    const int posX = 3;
    const int posY = 1;
    glTexCoord2f(posX*0.125f,     posY*0.125f);     glVertex2f(location_.x_-radius_, location_.y_-radius_);
    glTexCoord2f(posX*0.125f,     (posY+2)*0.125f); glVertex2f(location_.x_-radius_, location_.y_+radius_);
    glTexCoord2f((posX+2)*0.125f, (posY+2)*0.125f); glVertex2f(location_.x_+radius_, location_.y_+radius_);
    glTexCoord2f((posX+2)*0.125f, posY*0.125f);     glVertex2f(location_.x_+radius_, location_.y_-radius_);
}


