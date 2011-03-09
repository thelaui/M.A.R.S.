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

# include "Specials/Blast.hpp"

# include "SpaceObjects/Ship.hpp"
# include "SpaceObjects/ships.hpp"
# include "Players/Player.hpp"
# include "System/timer.hpp"
# include "SpaceObjects/physics.hpp"
# include "Menu/menus.hpp"
# include "Games/games.hpp"
# include "Teams/Team.hpp"

# include <SFML/Graphics.hpp>

void Blast::draw(float alpha) const {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // draw glow
    parent_->getOwner()->team()->color().brightened().gl4f(0.8f*alpha);

    const int posX = 0;
    const int posY = 0;

    float scale(4 + std::sin(timer::totalTime()*6)*0.3f);

    glBegin(GL_QUADS);
        glTexCoord2f( posX*0.25f,    posY*0.25f);    glVertex2f(-parent_->radius()*scale,-parent_->radius()*scale);
        glTexCoord2f( posX*0.25f,   (posY+1)*0.25f); glVertex2f(-parent_->radius()*scale, parent_->radius()*scale);
        glTexCoord2f((posX+1)*0.25f,(posY+1)*0.25f); glVertex2f( parent_->radius()*scale, parent_->radius()*scale);
        glTexCoord2f((posX+1)*0.25f, posY*0.25f);    glVertex2f( parent_->radius()*scale,-parent_->radius()*scale);
    glEnd();

    // draw effect
    if (timer_ > 0.f) {
        if (parent_->getLife() <= 0.f)
            timer_ = 0.f;

        float alpha(0.f);
        if(timer_ > 0.4f)
            alpha = std::pow(0.5f-timer_,2)*100.f;
        else
            alpha = -2.5f*(0.5f-timer_)+1.25f;

        float scale(1.f-timer_*2.f);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 1.0f, 1.0f, alpha);

        const int posX = 0;
        const int posY = 1;

        glBegin(GL_QUADS);
            glTexCoord2f( posX*0.25f,    posY*0.25f);    glVertex2f(-radius_*scale,-radius_*scale);
            glTexCoord2f( posX*0.25f,   (posY+1)*0.25f); glVertex2f(-radius_*scale, radius_*scale);
            glTexCoord2f((posX+1)*0.25f,(posY+1)*0.25f); glVertex2f( radius_*scale, radius_*scale);
            glTexCoord2f((posX+1)*0.25f, posY*0.25f);    glVertex2f( radius_*scale,-radius_*scale);
        glEnd();

        if (!menus::visible() || games::type() == games::gMenu)
            timer_ -= timer::frameTime();
    }
}

void Blast::activate() const {
    if (parent_->fragStars_ > 0) {
        radius_ = radius();
        parent_->fragStars_ = 0;
        physics::causeShockWave(parent_->getOwner(), parent_->location(), 1000.f, radius_*0.8f, 0.f);
        timer_ = 0.5f;
    }
}

float Blast::radius() const {
    return (parent_->fragStars_ > 0 ? parent_->fragStars_*150.f+150.f : 0.f);
}


