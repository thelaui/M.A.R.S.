/* FragmentFlame.cpp

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

# include "Particles/FragmentFlame.hpp"

# include "System/timer.hpp"
# include "System/settings.hpp"

std::list<FragmentFlame*> FragmentFlame::activeParticles_;

FragmentFlame::FragmentFlame(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Color3f const& color, Player* damageSource):
           Particle<FragmentFlame>(spaceObjects::oFragmentFlame, location, 1.f, 0.f, sf::Randomizer::Random(0.15f, 0.25f)) {

    Vector2f distortion(Vector2f::randDirLen());
    location_ = location + distortion;
    velocity_ = velocity*0.5f + distortion*20.f;

    radius_ = sf::Randomizer::Random(1.f, 3.f);

    color_.h(50.f);
    color_.v(0.6f);
    color_.s(0.8f);
}

void FragmentFlame::update() {
    float time = timer::frameTime();
    // update Color
    color_.h((-1.0/totalLifeTime_*lifeTime_+1)*60+350);
    color_.v(-1.0/totalLifeTime_*lifeTime_+1);

   // radius_ += time*20;

    location_ += velocity_*time;
    velocity_ += velocity_*(-10.f)*time;

    lifeTime_ += time;
}

void FragmentFlame::draw() const {
    color_.gl3f();
    const int posX = 5;
    const int posY = 0;
    glTexCoord2f(posX*0.125f,     posY*0.125f);     glVertex2f(location_.x_-radius_, location_.y_-radius_);
    glTexCoord2f(posX*0.125f,     (posY+1)*0.125f); glVertex2f(location_.x_-radius_, location_.y_+radius_);
    glTexCoord2f((posX+1)*0.125f, (posY+1)*0.125f); glVertex2f(location_.x_+radius_, location_.y_+radius_);
    glTexCoord2f((posX+1)*0.125f, posY*0.125f);     glVertex2f(location_.x_+radius_, location_.y_-radius_);
}

