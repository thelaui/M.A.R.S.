/* Flubba.cpp

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

# include "Weapons/weapons.hpp"

# include "SpaceObjects/Ship.hpp"
# include "Particles/particles.hpp"
# include "Media/sound.hpp"
# include "Players/Player.hpp"

# include <SFML/Graphics.hpp>

void Flubba::draw() const {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor3f(1.0f, 1.0f, 1.0f);
    const int posX = 0;
    const int posY = 29;
    glBegin(GL_QUADS);
        glTexCoord2f(posX*0.125f,     posY*0.03125f);    glVertex2f(0,      parent_->radius_*0.4f);
        glTexCoord2f(posX*0.125f,    (posY+1)*0.03125f); glVertex2f(0, -1.f*parent_->radius_*0.4f);
        glTexCoord2f((posX+1)*0.125f,(posY+1)*0.03125f); glVertex2f(parent_->radius_*3.f, -1.f*parent_->radius_*0.4f);
        glTexCoord2f((posX+1)*0.125f, posY*0.03125f);    glVertex2f(parent_->radius_*3.f,      parent_->radius_*0.4f);
    glEnd();
}

void Flubba::fire() const {
    float time = timer::totalTime();
    if (time - timer_ > 0.3f) {
        timer_ = time;
        float angleRad = parent_->rotation_*M_PI / 180.f;
        Vector2f faceDirection(std::cos(angleRad), std::sin(angleRad));
        particles::spawn(particles::pAmmoFlubba, parent_->location_ + faceDirection*parent_->radius_, faceDirection, parent_->velocity_, Color3f(), parent_->owner_);
        sound::playSound(sound::Blub, parent_->location_);
    }
}

void Flubba::next() {
    parent_->currentWeapon_ = new ROFLE(parent_);
    delete this;
}

void Flubba::previous() {
    parent_->currentWeapon_ = new AFK47(parent_);
    delete this;
}

