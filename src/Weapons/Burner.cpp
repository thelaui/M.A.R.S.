/* Burner.cpp

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

# include "Weapons/Burner.hpp"

# include "SpaceObjects/Ship.hpp"
# include "Particles/particles.hpp"
# include "Media/sound.hpp"
# include "Players/Player.hpp"

# include <SFML/Graphics.hpp>

void Burner::draw(float alpha) const {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    const int posX = 0;
    const int posY = 30;
    glBegin(GL_QUADS);
        glTexCoord2f(posX*0.125f,     posY*0.03125f);    glVertex2f(0,      parent_->radius()*0.3f);
        glTexCoord2f(posX*0.125f,    (posY+1)*0.03125f); glVertex2f(0, -1.f*parent_->radius()*0.3f);
        glTexCoord2f((posX+1)*0.125f,(posY+1)*0.03125f); glVertex2f(parent_->radius()*3.f, -1.f*parent_->radius()*0.3f);
        glTexCoord2f((posX+1)*0.125f, posY*0.03125f);    glVertex2f(parent_->radius()*3.f,      parent_->radius()*0.3f);
    glEnd();
}

void Burner::fire() const {
    float time = timer::totalTime();
    if (time - timer_ > 0.05f) {
        timer_ = time;
        float angleRad = parent_->rotation()*M_PI / 180.f;
        Vector2f faceDirection(std::cos(angleRad), std::sin(angleRad));
        for (int i=0; i<20; ++i) {
            particles::spawn(particles::pAmmoBurner, parent_->location() + faceDirection*parent_->radius()*1.5f, faceDirection, parent_->velocity(), Color3f(), parent_->getOwner());
            particles::spawn(particles::pHeatBurner, parent_->location() + faceDirection*parent_->radius()*1.5f, faceDirection, parent_->velocity());
        }
        parent_->velocity() -= faceDirection*10.f;
    }
}

float Burner::maxDistance() const {
    return 200.f;
}

float Burner::minDistance() const {
    return 0.f;
}

float Burner::maxAngle() const {
    return 10.f;
}
