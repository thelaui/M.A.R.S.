/* MiniFlame.cpp

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

# include "Particles/MiniFlame.hpp"

# include "System/timer.hpp"
# include "System/settings.hpp"
# include "Particles/particles.hpp"

std::list<MiniFlame*> MiniFlame::activeParticles_;

MiniFlame::MiniFlame(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Color3f const& color, Player* damageSource):
           Particle<MiniFlame>(spaceObjects::oMiniFlame, location+Vector2f::randDir(), 1.f, 0.f, sf::Randomizer::Random(0.8f, 2.0f)) {

    flickerSpeed_ = sf::Randomizer::Random(0.01f, 1.f);
    timer_        = sf::Randomizer::Random(0.f, 1.f);
    radius_       = sf::Randomizer::Random(2.0f, 5.0f);

    color_.h(60+350);
    color_.v(0.2);
    color_.s(1);
}

void MiniFlame::update() {
    float time = timer::frameTime();
    // update Color
    color_.h((-1.0/totalLifeTime_*lifeTime_+1)*60+0);

    if (std::fmod(lifeTime_, flickerSpeed_) < 0.01f)
        color_ = Color3f(1.f, 0.8f, 0.3f);
    else {
        color_.s(1.f);
        color_.v(-0.2f/totalLifeTime_*lifeTime_+0.2f);
    }

    location_ += Vector2f::randDirLen()*0.3f;

    if (timer_ > 0.f)
        timer_ -= time;
    else {
        timer_ = 1.f;
         particles::spawn(particles::pMiniFlameSmoke, location_, velocity_);
         particles::spawn(particles::pHeat, location_, velocity_);
    }

    lifeTime_ += time;
}

void MiniFlame::draw() const {
    color_.gl3f();
    const int posX = 5;
    const int posY = 0;
    glTexCoord2f(posX*0.125f,     posY*0.125f);     glVertex2f(location_.x_-radius_, location_.y_-radius_);
    glTexCoord2f(posX*0.125f,     (posY+1)*0.125f); glVertex2f(location_.x_-radius_, location_.y_+radius_);
    glTexCoord2f((posX+1)*0.125f, (posY+1)*0.125f); glVertex2f(location_.x_+radius_, location_.y_+radius_);
    glTexCoord2f((posX+1)*0.125f, posY*0.125f);     glVertex2f(location_.x_+radius_, location_.y_-radius_);
}
