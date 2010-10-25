/* Explode.cpp

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

# include "Particles/Explode.hpp"

# include "System/timer.hpp"
# include "System/settings.hpp"

std::list<Explode*> Explode::activeParticles_;

Explode::Explode(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Color3f const& color, Player* damageSource):
           Particle<Explode>(spaceObjects::oExplode, location, 4, 0, sf::Randomizer::Random(0.3f, 0.45f)) {

    velocity_ = Vector2f::randDir()*150*sf::Randomizer::Random(0.2f, 2.f);

    color_ = Color3f(1.0f, 0.5f, 0.2f);
}

void Explode::update() {
    float time = timer::frameTime();

    color_.v(-1.0/totalLifeTime_*lifeTime_+1);
    // update Size
    radius_ = lifeTime_*100+5;

    location_ = location_ + velocity_*time;
    velocity_ = velocity_ + velocity_*(-2)*time;

    lifeTime_ += time;
}

void Explode::draw() const {
    color_.gl4f(0.2);
    const int posX = 6;
    const int posY = 0;
    glTexCoord2f(posX*0.125f,     posY*0.125f);     glVertex2f(location_.x_-radius_, location_.y_-radius_);
    glTexCoord2f(posX*0.125f,     (posY+2)*0.125f); glVertex2f(location_.x_-radius_, location_.y_+radius_);
    glTexCoord2f((posX+2)*0.125f, (posY+2)*0.125f); glVertex2f(location_.x_+radius_, location_.y_+radius_);
    glTexCoord2f((posX+2)*0.125f, posY*0.125f);     glVertex2f(location_.x_+radius_, location_.y_-radius_);
}

