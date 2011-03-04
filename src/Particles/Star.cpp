/* Star.cpp

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

# include "Particles/Star.hpp"

# include "System/timer.hpp"
# include "System/settings.hpp"
# include "defines.hpp"

std::list<Star*> Star::activeParticles_;

Star::Star(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Color3f const& color, Player* damageSource):
           Particle<Star>(spaceObjects::oStar, location, 0.f, 0.f, 1.f) {

    location_    += Vector2f::randDir()*sf::Randomizer::Random(0.2f, 5.f);
    depth_       =  0.2f/(location-location_).lengthSquare();
    velocity_    =  (location_ - location)*depth_*2.5f;

    color_ = Color3f::random();
    color_.s(0.2f);
    alpha_ = 0.f;
}

void Star::update() {
    update(timer::frameTime());
}

void Star::update(float time) {
    location_ += velocity_*time;
    velocity_ += velocity_*time*depth_*0.01;
    radius_   += time*depth_*0.25f;

    alpha_ = radius_/16.f-0.5f;
    if (alpha_ > 1.f) alpha_ = 1.f;
    if (alpha_ < 0.f) alpha_ = 0.f;


    if (location_.x_ < -radius_ || location_.x_ > SPACE_X_RESOLUTION + radius_ || location_.y_ < -radius_ || location_.y_ > SPACE_Y_RESOLUTION + radius_)
        killMe();
}

void Star::draw() const {
    color_.gl4f(alpha_);
    glTexCoord2f(0, 0); glVertex2f(location_.x_-radius_, location_.y_-radius_);
    glTexCoord2f(0, 1); glVertex2f(location_.x_-radius_, location_.y_+radius_);
    glTexCoord2f(1, 1); glVertex2f(location_.x_+radius_, location_.y_+radius_);
    glTexCoord2f(1, 0); glVertex2f(location_.x_+radius_, location_.y_-radius_);
}

void Star::init() {
    for (std::list<Star*>::iterator it=activeParticles_.begin(); it!=activeParticles_.end(); ++it)
        delete *it;
    activeParticles_.clear();

    for (int i=0; i<2000; ++i) {
        spawn(Vector2f(SPACE_X_RESOLUTION*0.5f, SPACE_Y_RESOLUTION*0.5f), Vector2f(), Vector2f(), Color3f(), NULL);

        for (std::list<Star*>::iterator it=activeParticles_.begin(); it!=activeParticles_.end(); ++it)
            (*it)->update(0.05f);
    }
}

