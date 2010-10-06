/* AFK47.cpp

Copyright (c) 2010 by Felix Lauer und Simon Schneegans

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

# include "Weapons/weapons.hpp"

# include "SpaceObjects/Ship.hpp"
# include "Particles/particles.hpp"
# include "Media/sound.hpp"
# include "Players/Player.hpp"

# include <SFML/Graphics.hpp>

void AFK47::draw() const {
    glLineWidth(1);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    parent_->owner_->color().gl3f();
    glBegin(GL_LINES);
        glVertex2f(parent_->radius_*0.3,  parent_->radius_* 0.625);
        glVertex2f(parent_->radius_*0.75, parent_->radius_* 0.625);
        glVertex2f(parent_->radius_*0.3,  parent_->radius_*-0.625);
        glVertex2f(parent_->radius_*0.75, parent_->radius_*-0.625);
    glEnd();
}

void AFK47::fire() const {
    float time = timer::totalTime();
    if (time - timer_ > 0.1) {
        timer_ = time;
        float angleRad = parent_->rotation_*M_PI / 180;
        Vector2f faceDirection(std::cos(angleRad), std::sin(angleRad));
        particles::spawn(particles::pAmmoAFK47, Vector2f(parent_->location_.x_+(faceDirection.x_*parent_->radius_*0.6 - faceDirection.y_*parent_->radius_*0.6), parent_->location_.y_+( faceDirection.x_*parent_->radius_*0.6 + faceDirection.y_*parent_->radius_*0.6)), faceDirection, parent_->velocity_, Color3f(), parent_->owner_);
        particles::spawn(particles::pAmmoAFK47, Vector2f(parent_->location_.x_+(faceDirection.x_*parent_->radius_*0.6 + faceDirection.y_*parent_->radius_*0.6), parent_->location_.y_+(-faceDirection.x_*parent_->radius_*0.6 + faceDirection.y_*parent_->radius_*0.6)), faceDirection, parent_->velocity_, Color3f(), parent_->owner_);
        sound::playSound(sound::Laser, parent_->location_);
    }
}

void AFK47::next() {
    parent_->currentWeapon_ = new Flubba(parent_);
    delete this;
}

void AFK47::previous() {
    parent_->currentWeapon_ = new Burner(parent_);
    delete this;
}
