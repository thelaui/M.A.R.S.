/* Blast.cpp

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

# include "Specials/specials.hpp"

# include "SpaceObjects/Ship.hpp"
# include "SpaceObjects/ships.hpp"
# include "Players/Player.hpp"
# include "System/timer.hpp"
# include "SpaceObjects/physics.hpp"

# include <SFML/Graphics.hpp>

void Blast::draw() const {
    if (timer_ > 0.f) {
        float alpha(0.f);
        if(timer_ > 0.4f)
            alpha = std::pow(0.5f-timer_,2)*100.f;
        else
            alpha = -2.5f*(0.5f-timer_)+1.25f;

        float scale(1.f-timer_);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 1.0f, 1.0f, alpha);

        const int posX = 3;
        const int posY = 2;

        glBegin(GL_QUADS);
            glTexCoord2f( posX*0.25f,    posY*0.25f);    glVertex2f(-radius_*scale,-radius_*scale);
            glTexCoord2f( posX*0.25f,   (posY+1)*0.25f); glVertex2f(-radius_*scale, radius_*scale);
            glTexCoord2f((posX+1)*0.25f,(posY+1)*0.25f); glVertex2f( radius_*scale, radius_*scale);
            glTexCoord2f((posX+1)*0.25f, posY*0.25f);    glVertex2f( radius_*scale,-radius_*scale);
        glEnd();

        timer_ -= timer::frameTime();
    }
}

void Blast::activate() const {
    if (parent_->fragStars_ > 0) {
        radius_ = parent_->fragStars_*100.f;
        parent_->fragStars_ = 0;
        physics::causeShockWave(parent_->owner_, parent_->location_, radius_*2.f, radius_*1.5f, 10.f);
        timer_ = 0.5f;
    }
}

void Blast::next() {
    parent_->currentSpecial_ = new Heal(parent_);
    delete this;
}

void Blast::previous() {
    parent_->currentSpecial_ = new Freezer(parent_);
    delete this;
}



