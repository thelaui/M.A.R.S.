/* Burner.cpp

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

void Burner::draw() const {
    glLineWidth(5);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glBegin(GL_LINES);
        glColor3f(1.0, 0.4, 0.0);
            glVertex2f(parent_->radius_*0.5f, 0.f);
        glColor3f(0.0, 0.0, 0.0);
            glVertex2f(parent_->radius_*3.0f, 0.f);
    glEnd();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Burner::fire() const {
    float time = timer::totalTime();
    if (time - timer_ > 0.05f) {
        timer_ = time;
        float angleRad = parent_->rotation_*M_PI / 180.f;
        Vector2f faceDirection(std::cos(angleRad), std::sin(angleRad));
        for (int i=0; i<20; ++i) {
            particles::spawn(particles::pAmmoBurner, parent_->location_ + faceDirection*parent_->radius_, faceDirection, parent_->velocity_, Color3f(), parent_->owner_);
            particles::spawn(particles::pHeatBurner, parent_->location_ + faceDirection*parent_->radius_, faceDirection, parent_->velocity_);
        }
        parent_->velocity_ -= faceDirection*10.f;
    }
}

void Burner::next() {
    parent_->currentWeapon_ = new AFK47(parent_);
    delete this;
}

void Burner::previous() {
    parent_->currentWeapon_ = new Shotgun(parent_);
    delete this;
}


