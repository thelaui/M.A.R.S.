/* trailEffects.cpp

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

# include "TrailEffects/trailEffects.hpp"

# include "System/timer.hpp"
# include "SpaceObjects/balls.hpp"
# include "Media/texture.hpp"

# include <SFML/OpenGL.hpp>
# include <vector>

namespace trailEffects {
    namespace {
        std::vector<Vector2f> points_(20);
        int frontIndex_(0);
        float timer_(0.f);
    }

    void update() {
        if (balls::getBall()) {
            timer_ += timer::frameTime();
            if (timer_ > 0.05f) {
                timer_ = 0.f;
                points_[frontIndex_++] = balls::getBall()->location();
                frontIndex_ %= points_.size();
            }
        }
    }

    void draw() {
        if (balls::getBall()) {
            const int posX = 1;
            const int posY = 1;
            int index(frontIndex_), lastIndex(frontIndex_);
            Vector2f toLast;

            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Particles));
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);

            glBegin(GL_QUAD_STRIP);

                for (int i=0; i<points_.size(); ++i) {
                    index = (i+frontIndex_)%points_.size();
                    if (index != lastIndex) {
                        glColor4f(1.f, 1.0f, 1.0f, static_cast<float>(i)/points_.size());
                        toLast = (points_[lastIndex] - points_[index])*0.5f;

                        glTexCoord2f((posX + index%2)*0.125f + std::pow(-1, index) * 0.01f,     posY*0.125f); glVertex2f(points_[lastIndex].x_ + toLast.y_, points_[lastIndex].y_ - toLast.x_);
                        glTexCoord2f((posX + index%2)*0.125f + std::pow(-1, index) * 0.01f, (posY+1)*0.125f); glVertex2f(points_[lastIndex].x_ - toLast.y_, points_[lastIndex].y_ + toLast.x_);
                    }
                    lastIndex = index;
                }
                glTexCoord2f((posX + index%2)*0.125f,     posY*0.125f); glVertex2f(points_[index].x_ + toLast.y_, points_[index].y_ - toLast.x_);
                glTexCoord2f((posX + index%2)*0.125f, (posY+1)*0.125f); glVertex2f(points_[index].x_ - toLast.y_, points_[index].y_ + toLast.x_);

                ++index;

                glTexCoord2f((posX + index%2)*0.125f,     posY*0.125f); glVertex2f(balls::getBall()->location().x_ + toLast.y_, balls::getBall()->location().y_ - toLast.x_);
                glTexCoord2f((posX + index%2)*0.125f, (posY+1)*0.125f); glVertex2f(balls::getBall()->location().x_ - toLast.y_, balls::getBall()->location().y_ + toLast.x_);

            glEnd();

            glDisable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    //int  count();

    //void clear();
}


