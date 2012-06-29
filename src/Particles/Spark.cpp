/* Spark.cpp

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

# include "Particles/Spark.hpp"

# include "System/timer.hpp"
# include "System/randomizer.hpp"

std::list<Spark*> Spark::activeParticles_;

Spark::Spark(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Color3f const& color, Player* damageSource):
         Particle<Spark>(spaceObjects::oSpark, location, 1, 0, randomizer::random(0.3f, 0.4f)),
         color_(color) {

    Vector2f distortion(Vector2f::randDirLen());
    velocity_ = direction + velocity_*0.5f + distortion*150.f;

    if (randomizer::random(0, 1) == 0) color_ = Color3f(1.0f, 0.8f, 0.0f);
}

void Spark::update() {
    // update Color
    color_.v(-1.0/totalLifeTime_*lifeTime_+1);

    float time = timer::frameTime();

    location_ += velocity_*time;
    velocity_ -= velocity_*time;

    lifeTime_ += time;
}

void Spark::draw() const {
    color_.gl3f();
    const int posX = 0;
    const int posY = 1;
    glTexCoord2f(posX*0.125f,     posY*0.125f);     glVertex2f(location_.x_-radius_, location_.y_-radius_);
    glTexCoord2f(posX*0.125f,     (posY+1)*0.125f); glVertex2f(location_.x_-radius_, location_.y_+radius_);
    glTexCoord2f((posX+1)*0.125f, (posY+1)*0.125f); glVertex2f(location_.x_+radius_, location_.y_+radius_);
    glTexCoord2f((posX+1)*0.125f, posY*0.125f);     glVertex2f(location_.x_+radius_, location_.y_-radius_);
}

