/* FireWall.cpp

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

# include "Specials/FireWall.hpp"

# include "SpaceObjects/Ship.hpp"
# include "SpaceObjects/ships.hpp"
# include "Players/Player.hpp"
# include "System/timer.hpp"
# include "Particles/particles.hpp"
# include "Menu/menus.hpp"
# include "Games/games.hpp"
# include "Teams/Team.hpp"

# include <SFML/Graphics.hpp>

void FireWall::draw(float alpha) const {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // draw glow
    Color3f tmp = parent_->getOwner()->team()->color();
    if (tmp.v() < 0.5f) tmp.v(0.5f);
    if (tmp.s() < 0.5f) tmp.s(0.5f);
    tmp.gl4f(0.8f*alpha);

    const int posX = 3;
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
        if (!menus::visible() || games::type() == games::gMenu)
            timer_ -= timer::frameTime();

        if (burnTimer_ - timer_ > 0.04f) {
            burnTimer_ = timer_;
            Vector2f dir(std::cos(timer::totalTime()*3), std::sin(timer::totalTime()*3));
            for (int i=0; i<20; ++i) {
                particles::spawn(particles::pAmmoBurner, parent_->location_+Vector2f( dir.x_,  dir.y_)*parent_->radius()*1.25f, Vector2f( dir.x_,  dir.y_), parent_->velocity(), Color3f(), parent_->getOwner());
                particles::spawn(particles::pAmmoBurner, parent_->location_+Vector2f(-dir.x_, -dir.y_)*parent_->radius()*1.25f, Vector2f(-dir.x_, -dir.y_), parent_->velocity(), Color3f(), parent_->getOwner());
                particles::spawn(particles::pAmmoBurner, parent_->location_+Vector2f( dir.y_, -dir.x_)*parent_->radius()*1.25f, Vector2f( dir.y_, -dir.x_), parent_->velocity(), Color3f(), parent_->getOwner());
                particles::spawn(particles::pAmmoBurner, parent_->location_+Vector2f(-dir.y_,  dir.x_)*parent_->radius()*1.25f, Vector2f(-dir.y_,  dir.x_), parent_->velocity(), Color3f(), parent_->getOwner());
            }
        }
    }
}

void FireWall::activate() const {
    if (parent_->fragStars_ > 0) {
        timer_ = parent_->fragStars_*1.5f;
        burnTimer_ = timer_;
        parent_->fragStars_ = 0;

        particles::spawnMultiple(50,         particles::pDust,     parent_->location_);
        particles::spawnMultiple(20,         particles::pExplode,  parent_->location_);
    }
}

float FireWall::radius() const {
    return (parent_->fragStars_ > 0 ? 250.f : 0.f);
}


