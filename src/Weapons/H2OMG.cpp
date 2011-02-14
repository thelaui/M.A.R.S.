/* H2OMG.cpp

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

# include "Weapons/H2OMG.hpp"

# include "SpaceObjects/Ship.hpp"
# include "Particles/particles.hpp"
# include "Media/sound.hpp"
# include "Players/Player.hpp"

# include <SFML/Graphics.hpp>
# include <cfloat>

void H2OMG::draw(float alpha) const {
    glLineWidth(1);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_LINES);
        glColor4f(1.0, 1.0, 0.2, 0);
            glVertex2f(parent_->radius(),  0);
        glColor4f(1.0, 1.0, 0.2, 0.5*alpha);
            glVertex2f(parent_->radius()*3.5, 0);
            glVertex2f(parent_->radius()*3.5,  0);
        glColor4f(1.0, 1.0, 0.2, 0);
            glVertex2f(parent_->radius()*8.0, 0);
    glEnd();
}

void H2OMG::fire() const {
    float time = timer::totalTime();
    if (time - timer_ > 0.1f) {
        timer_ = time;
        float angleRad = parent_->rotation()*M_PI / 180;
        Vector2f faceDirection(std::cos(angleRad), std::sin(angleRad));
        particles::spawn(particles::pAmmoH2OMG, parent_->location() + faceDirection*parent_->radius(), faceDirection, parent_->velocity(), Color3f(), parent_->getOwner());
        sound::playSound(sound::BlubPop, parent_->location());
    }
}

float H2OMG::maxDistance() const {
    return FLT_MAX;
}

float H2OMG::minDistance() const {
    return 0.f;
}

float H2OMG::maxAngle() const {
    return 20.f;
}
