/* Trail.cpp

Copyright (c) 2010 by Felix Lauer and Simon Schneegans

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

# include "TrailEffects/Trail.hpp"

# include "System/timer.hpp"
# include "Media/texture.hpp"
# include "SpaceObjects/SpaceObject.hpp"

# include <SFML/OpenGL.hpp>

Trail::Trail(SpaceObject* target, int resolution, float length, float width, Color3f const& color):
    target_(target),
    points_(resolution < 2 ? 2 : resolution),
    frontIndex_(1),
    length_(1),
    timer_(0.f),
    timeStep_(length/resolution),
    width_(width),
    color_(color) {
        points_[0] = target_->location();
}

void Trail::update() {
    timer_ -= timer::frameTime();
    if (timer_ < 0.f) {
        timer_ = timeStep_;
        if (target_) {
            points_[frontIndex_] = target_->location();
            ++frontIndex_;
            frontIndex_ %= points_.size();
            if (length_ < points_.size())
                ++length_;
        }
        else {
            --length_;
        }
    }
}

void Trail::draw() const {
    if (length_ > 1) {
        const int posX = 1;
        const int posY = 1;

        Vector2f toNext((points_[(frontIndex_-2)%points_.size()] - points_[(frontIndex_-1)%points_.size()]).normalize()*width_);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Particles));
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glBegin(GL_QUAD_STRIP);


            if (target_) {
                color_.gl4f(0.5f*static_cast<float>(length_)/points_.size());
                glTexCoord2f((posX + frontIndex_%2)*0.125f,     posY*0.125f);
                    glVertex2f(target_->location().x_ + toNext.y_, target_->location().y_ - toNext.x_);
                glTexCoord2f((posX + frontIndex_%2)*0.125f, (posY+1)*0.125f);
                    glVertex2f(target_->location().x_ - toNext.y_, target_->location().y_ + toNext.x_);
            }
            for (int i=0; i<length_; ++i) {
               // glColor3f(static_cast<float>(i)/points_.size(), static_cast<float>(length_ - i - 1)/points_.size(), 0.f);
                color_.gl4f(0.5f*static_cast<float>(length_ - i - 1)/points_.size());

                int index    ((frontIndex_-i-1)%points_.size());
                int nextIndex((frontIndex_-i-2)%points_.size());

                if (i != length_-1)
                    toNext = (points_[nextIndex] - points_[index]).normalize()*width_;

                glTexCoord2f((posX + index%2)*0.125f + std::pow(-1, index) * 0.01f,     posY*0.125f);
                    glVertex2f(points_[index].x_ + toNext.y_, points_[index].y_ - toNext.x_);
                glTexCoord2f((posX + index%2)*0.125f + std::pow(-1, index) * 0.01f, (posY+1)*0.125f);
                    glVertex2f(points_[index].x_ - toNext.y_, points_[index].y_ + toNext.x_);
            }

        glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Trail::detach() {
    target_ = NULL;
}

bool Trail::isDead() const {
    return length_ <= 0;
}

SpaceObject* Trail::target() const {
    return target_;
}

