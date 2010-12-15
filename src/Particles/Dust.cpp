/* Dust.cpp

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

# include "Particles/Dust.hpp"

# include "System/timer.hpp"
# include "System/settings.hpp"

std::list<Dust*> Dust::activeParticles_;

Dust::Dust(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Color3f const& color, Player* damageSource):
           Particle<Dust>(spaceObjects::oDust, location, 4, 0, sf::Randomizer::Random(0.5f, 1.5f)*settings::C_globalParticleLifeTime/100.f) {

    velocity_ = Vector2f::randDir()*250*sf::Randomizer::Random(1.f, 1.2f)*sf::Randomizer::Random(1.f, 1.2f);

    color_ = Color3f::random();
    color_.s(0.0);
}

void Dust::update() {
    float time = timer::frameTime();

    color_.v(-0.7/totalLifeTime_*lifeTime_+0.7);
    // update Size
    radius_ = lifeTime_/totalLifeTime_*5+5;

    location_ = location_ + velocity_*time;
    velocity_ = velocity_ + velocity_*(-2.f)*time;

    lifeTime_ += time;
}

void Dust::draw() const {
    color_.gl4f(-0.3/totalLifeTime_*lifeTime_+0.3);
    const int posX = 1;
    const int posY = 0;
    glTexCoord2f(posX*0.125f,     posY*0.125f);     glVertex2f(location_.x_-radius_, location_.y_-radius_);
    glTexCoord2f(posX*0.125f,     (posY+1)*0.125f); glVertex2f(location_.x_-radius_, location_.y_+radius_);
    glTexCoord2f((posX+1)*0.125f, (posY+1)*0.125f); glVertex2f(location_.x_+radius_, location_.y_+radius_);
    glTexCoord2f((posX+1)*0.125f, posY*0.125f);     glVertex2f(location_.x_+radius_, location_.y_-radius_);
}

void Dust::shockWave(Vector2f const& location, float strength, float radius) {
    for (std::list<Dust*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it) {
        Vector2f direction((*it)->location_ - location);
        float distance = direction.length();
        if (distance < radius && direction != Vector2f()) {
            float intensity = radius-distance;
            direction = direction.normalize();
            direction *= intensity;
            (*it)->velocity_ += direction;
        }
    }
}


