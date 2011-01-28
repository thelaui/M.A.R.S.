/* Heal.cpp

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

# include "Specials/Heal.hpp"

# include "SpaceObjects/Ship.hpp"
# include "SpaceObjects/ships.hpp"
# include "Players/Player.hpp"
# include "System/timer.hpp"
# include "Menu/menus.hpp"
# include "Games/games.hpp"
# include "Teams/Team.hpp"

# include <SFML/Graphics.hpp>
# include <vector>

void Heal::draw() const {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // draw glow
    Color3f tmp = parent_->getOwner()->team()->color();
    if (tmp.v() < 0.5f) tmp.v(0.5f);
    if (tmp.s() < 0.5f) tmp.s(0.5f);

    float alpha(0.6 + std::sin(timer::totalTime()*6)*0.1f);

    tmp.gl4f(alpha);

    const int posX = 1;
    const int posY = 0;

    glBegin(GL_QUADS);
        glTexCoord2f( posX*0.25f,    posY*0.25f);    glVertex2f(-parent_->radius()*4,-parent_->radius()*4);
        glTexCoord2f( posX*0.25f,   (posY+1)*0.25f); glVertex2f(-parent_->radius()*4, parent_->radius()*4);
        glTexCoord2f((posX+1)*0.25f,(posY+1)*0.25f); glVertex2f( parent_->radius()*4, parent_->radius()*4);
        glTexCoord2f((posX+1)*0.25f, posY*0.25f);    glVertex2f( parent_->radius()*4,-parent_->radius()*4);
    glEnd();

    if (timer_ > 0.f) {
        float alpha(0.f);
        if(timer_ > 0.4f)
            alpha = std::pow(0.5f-timer_,2)*100.f;
        else
            alpha = -2.5f*(0.5f-timer_)+1.25f;

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 1.0f, 1.0f, alpha);

        const int posX = 1;
        const int posY = 1;

        glBegin(GL_QUADS);
            glTexCoord2f( posX*0.25f,    posY*0.25f);    glVertex2f(-radius_,-radius_);
            glTexCoord2f( posX*0.25f,   (posY+1)*0.25f); glVertex2f(-radius_, radius_);
            glTexCoord2f((posX+1)*0.25f,(posY+1)*0.25f); glVertex2f( radius_, radius_);
            glTexCoord2f((posX+1)*0.25f, posY*0.25f);    glVertex2f( radius_,-radius_);
        glEnd();

        if (!menus::visible() || games::type() == games::gMenu)
            timer_ -= timer::frameTime();
    }
}

void Heal::activate() const {
    if (parent_->fragStars_ > 0) {
        radius_ = parent_->fragStars_*50.f +50.f;
        std::vector<Ship*> const& ships = ships::getShips();
        for (std::vector<Ship*>::const_iterator it=ships.begin(); it!=ships.end(); ++it) {
            if ((*it)!=parent_) {
                float distance(((*it)->location()-parent_->location()).length());
                if ((*it)->getLife() > 0 && parent_->getOwner()->team() == (*it)->getOwner()->team() && distance <= radius_) {
                    (*it)->heal(parent_->getOwner(), ((radius_/distance)-0.8f)*parent_->fragStars_*10);
                    (*it)->refuel(parent_->getOwner(), ((radius_/distance)-0.8f)*parent_->fragStars_*10);
                }
            }
            else {
                parent_->heal(parent_->getOwner(), parent_->fragStars_*10);
                parent_->refuel(parent_->getOwner(), parent_->fragStars_*10);
            }
        }
        timer_ = 0.5f;
        parent_->fragStars_ = 0;
    }
}



