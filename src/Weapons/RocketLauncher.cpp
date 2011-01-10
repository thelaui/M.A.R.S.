/* RocketLauncher.cpp

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

# include "Weapons/weapons.hpp"

# include "SpaceObjects/Ship.hpp"
# include "Particles/particles.hpp"
# include "Media/sound.hpp"
# include "Players/Player.hpp"

# include <SFML/Graphics.hpp>

void RocketLauncher::draw() const {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor3f(1.0f, 1.0f, 1.0f);
    const int posX = 0;
    const int posY = 30;
    glBegin(GL_QUADS);
        glTexCoord2f(posX*0.125f,     posY*0.03125f);    glVertex2f(0,      parent_->radius_*0.5f);
        glTexCoord2f(posX*0.125f,    (posY+1)*0.03125f); glVertex2f(0, -1.f*parent_->radius_*0.5f);
        glTexCoord2f((posX+1)*0.125f,(posY+1)*0.03125f); glVertex2f(parent_->radius_*2.f, -1.f*parent_->radius_*0.5f);
        glTexCoord2f((posX+1)*0.125f, posY*0.03125f);    glVertex2f(parent_->radius_*2.f,      parent_->radius_*0.5f);
    glEnd();
}

void RocketLauncher::fire() const {
    float time = timer::totalTime();
    if (time - timer_ > 4.0f) {
        timer_ = time;
        float angleRad = parent_->rotation_*M_PI / 180.f;
        Vector2f faceDirection(std::cos(angleRad), std::sin(angleRad));
        particles::spawn(particles::pAmmoRocket, parent_->location_ + faceDirection*parent_->radius_, faceDirection, parent_->velocity_, Color3f(), parent_->owner_);
        particles::spawnMultiple(10.f, particles::pDust,  parent_->location_ + faceDirection*parent_->radius_, parent_->velocity_);
        parent_->velocity_ -= faceDirection*400.f;
        sound::playSound(sound::Pump, parent_->location_);
    }
}

void RocketLauncher::next() {
    parent_->currentWeapon_ = new Burner(parent_);
    delete this;
}

void RocketLauncher::previous() {
    parent_->currentWeapon_ = new Fist(parent_);
    delete this;
}
