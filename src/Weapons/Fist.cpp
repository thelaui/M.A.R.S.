/* Fist.cpp

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
# include "Media/sound.hpp"
# include "Players/Player.hpp"
# include "Particles/particles.hpp"

# include <SFML/Graphics.hpp>

void Fist::draw() const {
    float time = timer::totalTime();
    if (time - timer_ < 0.1f)
        position_ = (time - timer_)*20.f;
    else if (time - timer_ < 0.5f)
        position_ = (0.5f - (time - timer_))*5.f;
    else
        position_ = 0.f;

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
        glTexCoord2f(0.109375f, 0.953125f); glVertex2f(0.f,          parent_->radius_*0.5f);
        glTexCoord2f(0.140625f, 0.984375f); glVertex2f(0.f,     -1.f*parent_->radius_*0.5f);
        glTexCoord2f(0.234375f, 0.890625f); glVertex2f(position_*parent_->radius_*(position_+1)*1.2f, (-1.f*parent_->radius_*0.5f)*(1+position_)*0.7f);
        glTexCoord2f(0.203125f, 0.859375f); glVertex2f(position_*parent_->radius_*(position_+1)*1.2f, (     parent_->radius_*0.5f)*(1+position_)*0.7f);
    glEnd();

    parent_->getOwner()->color().gl3f();
    const float posX = 2;
    const float posY = 28;
    glBegin(GL_QUADS);
        glTexCoord2f(posX*0.125f,     posY*0.03125f);       glVertex2f(position_*parent_->radius_*(position_+1),       (     parent_->radius_*0.5f)*(1+position_));
        glTexCoord2f(posX*0.125f,    (posY+2)*0.03125f);    glVertex2f(position_*parent_->radius_*(position_+1),       (-1.f*parent_->radius_*0.5f)*(1+position_));
        glTexCoord2f((posX+0.5f)*0.125f,(posY+2)*0.03125f); glVertex2f(parent_->radius_*(1.f+position_)*(position_+1), (-1.f*parent_->radius_*0.5f)*(1+position_));
        glTexCoord2f((posX+0.5f)*0.125f, posY*0.03125f);    glVertex2f(parent_->radius_*(1.f+position_)*(position_+1), (     parent_->radius_*0.5f)*(1+position_));
    glEnd();
}

void Fist::fire() const {
    float time = timer::totalTime();
    if (time - timer_ > 0.5f) {
        timer_ = time;
        float angleRad = parent_->rotation_*M_PI / 180.f;
        Vector2f faceDirection(std::cos(angleRad), std::sin(angleRad));

        particles::spawn(particles::pAmmoFist, parent_->location_ + faceDirection*parent_->radius_, faceDirection, parent_->velocity_, Color3f(), parent_->owner_);

        parent_->velocity_ -= faceDirection*200.f;
        sound::playSound(sound::Pump, parent_->location_);
    }
}

void Fist::next() {
    parent_->currentWeapon_ = new RocketLauncher(parent_);
    delete this;
}

void Fist::previous() {
    parent_->currentWeapon_ = new Shotgun(parent_);
    delete this;
}


