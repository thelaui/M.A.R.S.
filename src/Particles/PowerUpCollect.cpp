/* PowerUpCollectcpp

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

# include "Particles/PowerUpCollect.hpp"

# include "System/timer.hpp"

std::list<PowerUpCollect*> PowerUpCollect::activeParticles_;

PowerUpCollect::PowerUpCollect(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Color3f const& color, Player* damageSource):
         Particle<PowerUpCollect>(spaceObjects::oPowerUpCollect, location, 4.f, 0.f, sf::Randomizer::Random(0.6f, 0.8f)),
         color_(1.f, 1.f, 1.f),
         texX_(direction.x_),
         texY_(direction.y_) {

    Vector2f distortion(Vector2f::randDirLen());
    velocity_ = distortion*150;
}

void PowerUpCollect::update() {
    // update Color
    color_.v(-1.0/totalLifeTime_*lifeTime_+1);

    float time = timer::frameTime();
    Vector2f acceleration = physics::attract(this);

    location_ += velocity_*time + acceleration*time*time*3;
    velocity_ += acceleration*time*3 + velocity_*-2*time;

    lifeTime_ += time;
}

void PowerUpCollect::draw() const {
    color_.gl3f();
    glTexCoord2f(texX_*0.15625f,     texY_*0.15625f);     glVertex2f(location_.x_-radius_, location_.y_-radius_);
    glTexCoord2f(texX_*0.15625f,     (texY_+1)*0.15625f); glVertex2f(location_.x_-radius_, location_.y_+radius_);
    glTexCoord2f((texX_+1)*0.15625f, (texY_+1)*0.15625f); glVertex2f(location_.x_+radius_, location_.y_+radius_);
    glTexCoord2f((texX_+1)*0.15625f, texY_*0.15625f);     glVertex2f(location_.x_+radius_, location_.y_-radius_);
}

