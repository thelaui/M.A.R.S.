/* MiniFlameSmoke.cpp

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

# include "Particles/MiniFlameSmoke.hpp"

# include "System/timer.hpp"
# include "System/settings.hpp"

std::list<MiniFlameSmoke*> MiniFlameSmoke::activeParticles_;

MiniFlameSmoke::MiniFlameSmoke(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Color3f const& color, Player* damageSource):
           Particle<MiniFlameSmoke>(spaceObjects::oMiniFlameSmoke, location+Vector2f::randDirLen()*2.f, 4, 0, sf::Randomizer::Random(0.8f, 2.0f)) {

    color_ = Color3f(0.9, 0.8, 0.7);
}

void MiniFlameSmoke::update() {
    float time = timer::frameTime();
    Vector2f acceleration = physics::attract(this);

    color_.v(-0.6f/totalLifeTime_*lifeTime_+0.9f);
    // update Size
    radius_ = lifeTime_*8.f;

    location_ = location_ + velocity_*time - 1.f*acceleration*time*time;
    velocity_ = velocity_ - 1.f*acceleration*time + velocity_*(-2)*time;

    lifeTime_ += time;
}

void MiniFlameSmoke::draw() const {
    color_.gl4f(-0.4f/totalLifeTime_*lifeTime_+0.4f);
    const int posX = 1;
    const int posY = 0;
    glTexCoord2f(posX*0.125f,     posY*0.125f);     glVertex2f(location_.x_-radius_, location_.y_-radius_);
    glTexCoord2f(posX*0.125f,     (posY+1)*0.125f); glVertex2f(location_.x_-radius_, location_.y_+radius_);
    glTexCoord2f((posX+1)*0.125f, (posY+1)*0.125f); glVertex2f(location_.x_+radius_, location_.y_+radius_);
    glTexCoord2f((posX+1)*0.125f, posY*0.125f);     glVertex2f(location_.x_+radius_, location_.y_-radius_);
}

void MiniFlameSmoke::shockWave(SpaceObject* source, float strength, float radius) {
    for (std::list<MiniFlameSmoke*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it) {
        Vector2f direction((*it)->location_ - source->location());
        float distance = direction.length();
        if (distance < radius && direction != Vector2f()) {
            float intensity = radius-distance;
            direction = direction.normalize();
            direction *= intensity;
            (*it)->velocity_ += direction;
        }
    }
}



