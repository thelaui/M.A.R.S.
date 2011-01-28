
/* FloatingTrail.cpp

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

# include "TrailEffects/PersistantTrail.hpp"

# include "System/timer.hpp"
# include "Media/texture.hpp"
# include "SpaceObjects/SpaceObject.hpp"

# include <SFML/OpenGL.hpp>

PersistantTrail::PersistantTrail(SpaceObject* target, float timeStep, float duration, float width, Color3f const& color):
    Trail(target),
    points_(),
    alpha_(1.f),
    timer_(0.f),
    duration_(duration),
    timeStep_(timeStep),
    width_(width),
    color_(color) {
        points_.push_back(target_->location());
}

void PersistantTrail::update() {
    timer_ -= timer::frameTime();
    if (target_) {
        if (timer_ < 0.f) {
            timer_ = timeStep_;
            points_.push_back(target_->location());
        }
    }
    else {
        alpha_ -= timer::frameTime()/duration_;
    }
}

void PersistantTrail::draw() const {
    if (points_.size() > 1) {
        const int posX = 1;
        const int posY = 1;

        Vector2f toNext;

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Particles));
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glBegin(GL_QUAD_STRIP);

            toNext = (points_[0] - points_[1]).normalize()*width_;

            if (target_) {
                color_.gl4f(0);
                glTexCoord2f((posX + points_.size()%2)*0.125f,     posY*0.125f);
                    glVertex2f(target_->location().x_ + toNext.y_, target_->location().y_ - toNext.x_);
                glTexCoord2f((posX + points_.size()%2)*0.125f, (posY+1)*0.125f);
                    glVertex2f(target_->location().x_ - toNext.y_, target_->location().y_ + toNext.x_);
            }

            for (int i=points_.size()-1; i>=0; --i) {
                if (!target_ && i==points_.size()-1)
                    color_.gl4f(0);
                else if (i>points_.size()*0.5)
                    color_.gl4f(alpha_);
                else
                    color_.gl4f((i*2.f/points_.size())*alpha_);

                if (i > 1)
                    toNext = (points_[i-1] - points_[i]).normalize()*width_;

                glTexCoord2f((posX + 0.5)*0.125f,     posY*0.125f);
                    glVertex2f(points_[i].x_ + toNext.y_, points_[i].y_ - toNext.x_);
                glTexCoord2f((posX + 0.5)*0.125f, (posY+1)*0.125f);
                    glVertex2f(points_[i].x_ - toNext.y_, points_[i].y_ + toNext.x_);
            }



        glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

bool PersistantTrail::isDead() const {
    return alpha_ <= 0;
}
