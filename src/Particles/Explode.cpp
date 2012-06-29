/* Explode.cpp

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

# include "Particles/Explode.hpp"

# include "System/timer.hpp"
# include "System/settings.hpp"
# include "System/randomizer.hpp"

std::list<Explode*> Explode::activeParticles_;

Explode::Explode(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Color3f const& color, Player* damageSource):
           Particle<Explode>(spaceObjects::oExplode, location, 4, 0, randomizer::random(0.4f, 0.8f)) {

    velocity_ = Vector2f::randDir()*200*randomizer::random(0.5f, 2.f);

    color_.h(50.f);
    color_.v(0.8f);
    color_.s(0.3f);
}

void Explode::update() {
    float time = timer::frameTime();

    // update Color
    color_.h((-1.0f/totalLifeTime_*lifeTime_+1.f)*60.f);
    color_.v(-0.8f/totalLifeTime_*lifeTime_+0.8f);
    color_.s(lifeTime_*10.f + 0.3f);

    // update Size
    radius_ = lifeTime_*50 + 2;

    location_ += velocity_*time;
    velocity_ += velocity_*(-6.f)*time;

    lifeTime_ += time;
}

void Explode::draw() const {
    color_.gl4f(0.5);
    const int posX = 6;
    const int posY = 0;
    glTexCoord2f(posX*0.125f,     posY*0.125f);     glVertex2f(location_.x_-radius_, location_.y_-radius_);
    glTexCoord2f(posX*0.125f,     (posY+2)*0.125f); glVertex2f(location_.x_-radius_, location_.y_+radius_);
    glTexCoord2f((posX+2)*0.125f, (posY+2)*0.125f); glVertex2f(location_.x_+radius_, location_.y_+radius_);
    glTexCoord2f((posX+2)*0.125f, posY*0.125f);     glVertex2f(location_.x_+radius_, location_.y_-radius_);
}

