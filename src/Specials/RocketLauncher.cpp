/* RocketLauncher.cpp

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

# include "Specials/specials.hpp"

# include "SpaceObjects/Ship.hpp"
# include "Particles/particles.hpp"
# include "Media/sound.hpp"
# include "Players/Player.hpp"

# include <SFML/Graphics.hpp>

void RocketLauncher::draw() const {
}

void RocketLauncher::activate() const {
    if (parent_->fragStars_ > 0) {
        if (parent_->fragStars_ == 1) {
            float angleRad = parent_->rotation_*M_PI / 180.f;
            Vector2f faceDirection(std::cos(angleRad), std::sin(angleRad));
            particles::spawn(particles::pAmmoRocket, parent_->location_ + faceDirection*parent_->radius_, faceDirection, parent_->velocity_, Color3f(), parent_->owner_);
            particles::spawnMultiple(10.f, particles::pDust,  parent_->location_ + faceDirection*parent_->radius_, parent_->velocity_);
            parent_->velocity_ -= faceDirection*100.f;
            sound::playSound(sound::Pump, parent_->location_);
        }
        else {
            float angleRad = (parent_->rotation_ -22.5f)*M_PI / 180.f;
            for (int i=parent_->fragStars_; i!=0; --i) {
                Vector2f faceDirection(std::cos(angleRad), std::sin(angleRad));
                particles::spawn(particles::pAmmoRocket, parent_->location_ + faceDirection*parent_->radius_, faceDirection, parent_->velocity_, Color3f(), parent_->owner_);
                particles::spawnMultiple(10.f, particles::pDust,  parent_->location_ + faceDirection*parent_->radius_, parent_->velocity_);
                parent_->velocity_ -= faceDirection*100.f/i;
                angleRad += M_PI_4/parent_->fragStars_;
            }
            sound::playSound(sound::Pump, parent_->location_);
        }
        parent_->fragStars_ = 0;
    }
}

void RocketLauncher::next() {
    parent_->currentSpecial_ = new Heal(parent_);
    delete this;
}

void RocketLauncher::previous() {
    parent_->currentSpecial_ = new Blast(parent_);
    delete this;
}


