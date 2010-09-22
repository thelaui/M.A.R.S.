/* Shotgun.cpp

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

void Shotgun::draw() const {
    glLineWidth(1);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_LINES);
        glColor4f(1.0, 1.0, 0.2, 0);
            glVertex2f(parent_->radius_*0.2,  0);
        glColor4f(1.0, 1.0, 0.2, 0.5);
            glVertex2f(parent_->radius_*1.0, 0);
            glVertex2f(parent_->radius_*1.0,  0);
        glColor4f(1.0, 1.0, 0.2, 0);
            glVertex2f(parent_->radius_*2.0, 0);
    glEnd();
}

void Shotgun::fire() const {
    float time = timer::totalTime();
    if (time - timer_ > 1.0) {
        timer_ = time;
        float angleRad = parent_->rotation_*M_PI / 180.f;
        Vector2f faceDirection(std::cos(angleRad), std::sin(angleRad));
        for (int i=0; i<25; ++i)
            particles::spawn(particles::pAmmoShotgun, parent_->location_ + faceDirection*parent_->radius_, faceDirection, parent_->velocity_, Color3f(), parent_->owner_);
        parent_->velocity_ -= faceDirection*200.f;
        sound::playSound(sound::Pump, parent_->location_);
    }
}

void Shotgun::next() {
    parent_->currentWeapon_ = new AFK47(parent_);
    delete this;
}

void Shotgun::previous() {
    parent_->currentWeapon_ = new ROFLE(parent_);
    delete this;
}

