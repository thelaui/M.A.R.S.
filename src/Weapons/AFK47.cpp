/* AFK47.cpp

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

# include "Weapons/AFK47.hpp"

# include "SpaceObjects/Ship.hpp"
# include "Particles/particles.hpp"
# include "Media/sound.hpp"
# include "Players/Player.hpp"

# include <SFML/Graphics.hpp>
# include <cfloat>

void AFK47::draw(float alpha) const {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    parent_->getOwner()->color().gl4f(alpha);
    const int posX = 0;
    const int posY = 28;
    glBegin(GL_QUADS);
        glTexCoord2f(posX*0.125f,     posY*0.03125f);    glVertex2f(0, parent_->radius()*0.95f);
        glTexCoord2f(posX*0.125f,    (posY+1)*0.03125f); glVertex2f(0, parent_->radius()*0.45f);
        glTexCoord2f((posX+1)*0.125f,(posY+1)*0.03125f); glVertex2f(parent_->radius()*1.5f, parent_->radius()*0.45f);
        glTexCoord2f((posX+1)*0.125f, posY*0.03125f);    glVertex2f(parent_->radius()*1.5f, parent_->radius()*0.95f);

        glTexCoord2f(posX*0.125f,     posY*0.03125f);    glVertex2f(0, -1.f*parent_->radius()*0.95f);
        glTexCoord2f(posX*0.125f,    (posY+1)*0.03125f); glVertex2f(0, -1.f*parent_->radius()*0.45f);
        glTexCoord2f((posX+1)*0.125f,(posY+1)*0.03125f); glVertex2f(parent_->radius()*1.5f, -1.f*parent_->radius()*0.45f);
        glTexCoord2f((posX+1)*0.125f, posY*0.03125f);    glVertex2f(parent_->radius()*1.5f, -1.f*parent_->radius()*0.95f);
    glEnd();
}

void AFK47::fire() const {
    float time = timer::totalTime();
    if (time - timer_ > 0.1) {
        timer_ = time;
        float angleRad = parent_->rotation()*M_PI / 180;
        Vector2f faceDirection(std::cos(angleRad), std::sin(angleRad));
        particles::spawn(particles::pAmmoAFK47, Vector2f(parent_->location().x_+(faceDirection.x_*parent_->radius()*0.9 - faceDirection.y_*parent_->radius()*0.9), parent_->location().y_+( faceDirection.x_*parent_->radius()*0.7 + faceDirection.y_*parent_->radius()*0.7)), faceDirection, parent_->velocity(), Color3f(), parent_->getOwner());
        particles::spawn(particles::pAmmoAFK47, Vector2f(parent_->location().x_+(faceDirection.x_*parent_->radius()*0.9 + faceDirection.y_*parent_->radius()*0.9), parent_->location().y_+(-faceDirection.x_*parent_->radius()*0.7 + faceDirection.y_*parent_->radius()*0.7)), faceDirection, parent_->velocity(), Color3f(), parent_->getOwner());
        sound::playSound(sound::Laser, parent_->location());
    }
}

float AFK47::maxDistance() const {
    return FLT_MAX;
}

float AFK47::minDistance() const {
    return 0.f;
}

float AFK47::maxAngle() const {
    return 10.f;
}
