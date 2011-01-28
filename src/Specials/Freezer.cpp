/* Freezer.cpp

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

# include "Specials/Freezer.hpp"

# include "SpaceObjects/Ship.hpp"
# include "SpaceObjects/ships.hpp"
# include "SpaceObjects/balls.hpp"
# include "Players/Player.hpp"
# include "System/timer.hpp"
# include "DecoObjects/decoObjects.hpp"
# include "Menu/menus.hpp"
# include "Games/games.hpp"
# include "Teams/Team.hpp"

# include <SFML/Graphics.hpp>
# include <vector>

void Freezer::draw() const {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // draw glow
    Color3f tmp = parent_->getOwner()->team()->color();
    if (tmp.v() < 0.5f) tmp.v(0.5f);
    if (tmp.s() < 0.5f) tmp.s(0.5f);
    tmp.gl4f(0.8f);

    const int posX = 2;
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

        float scale(1.f-timer_);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 1.0f, 1.0f, alpha);

        const int posX = 2;
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

void Freezer::activate() const {
    if (parent_->fragStars_ > 0) {
        radius_ = parent_->fragStars_*50.f+50.f;
        std::vector<Ship*> const& ships = ships::getShips();
        for (std::vector<Ship*>::const_iterator it=ships.begin(); it!=ships.end(); ++it) {
            if ((*it)!=parent_ && (*it)->visible_) {
                float distance(((*it)->location()-parent_->location()).length());
                if (distance <= radius_) {
                    (*it)->setDamageSource(parent_->getOwner());
                    (*it)->velocity_=Vector2f();
                    (*it)->mass_=9999999999.f;
                    if ((*it)->frozen_ <= 0)
                        decoObjects::addShipIce(*it);
                    (*it)->frozen_=40;
                }
            }
        }
        Ball* ball = balls::getBall();

        if(ball && ball->visible_) {
            float distance((ball->location()-parent_->location()).length());
                if (distance <= radius_) {
                    ball->frozen_=40;
                    ball->velocity_=Vector2f();
                    ball->mass_=9999999999.f;
                    decoObjects::addBallIce(ball);
            }
        }

        for (std::list<AmmoRocket*>::iterator it=AmmoRocket::activeParticles_.begin(); it!=AmmoRocket::activeParticles_.end(); ++it) {
            float distance(((*it)->location()-parent_->location()).length());
            if (distance <= radius_) {
                (*it)->frozen_=40;
                (*it)->velocity_=Vector2f();
                (*it)->mass_=9999999999.f;
                decoObjects::addRocketIce(*it);
            }
        }

        timer_ = 0.5f;
        parent_->fragStars_ = 0;
    }
}


