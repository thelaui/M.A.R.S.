/* Eruption.cpp

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

# include "Particles/Eruption.hpp"

# include "System/timer.hpp"
# include "System/randomizer.hpp"

std::list<Eruption*> Eruption::activeParticles_;

Eruption::Eruption(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Color3f const& color, Player* damageSource):
         Particle<Eruption>(spaceObjects::oEruption, location, 1.f, 0.f, randomizer::random(1.0f, 2.5f)),
         color_(Color3f(1.0f, 0.9f, 0.2f)) {

    Vector2f distortion(Vector2f::randDir()*randomizer::random(0.9f, 1.1f));
    velocity_ = velocity*50.f + distortion*70.f;
}

void Eruption::update() {
    // update Color
    color_.v(-1.0f/totalLifeTime_*lifeTime_+1.f);

    float time = timer::frameTime();
    Vector2f acceleration = physics::attract(this);

    location_ += velocity_*time + acceleration*0.5*time*time;
    velocity_ += acceleration*0.5*time + velocity_*-2.f*time;

    lifeTime_ += time;
}

void Eruption::draw() const {
    color_.gl3f();
    const int posX = 0;
    const int posY = 1;
    glTexCoord2f(posX*0.125f,     posY*0.125f);     glVertex2f(location_.x_-radius_, location_.y_-radius_);
    glTexCoord2f(posX*0.125f,     (posY+1)*0.125f); glVertex2f(location_.x_-radius_, location_.y_+radius_);
    glTexCoord2f((posX+1)*0.125f, (posY+1)*0.125f); glVertex2f(location_.x_+radius_, location_.y_+radius_);
    glTexCoord2f((posX+1)*0.125f, posY*0.125f);     glVertex2f(location_.x_+radius_, location_.y_-radius_);
}
