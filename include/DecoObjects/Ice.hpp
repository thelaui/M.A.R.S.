/* Ice.hpp

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

# ifndef ICE_HPP_INCLUDED
# define ICE_HPP_INCLUDED

# include "DecoObjects/DecoObject.hpp"

# include "DecoObjects/decoObjects.hpp"
# include "SpaceObjects/Ball.hpp"
# include "SpaceObjects/Ship.hpp"


/// An ice block drawn over frozen SpaceObjects.


template <typename Object>
class Ice: public DecoObject {
    public:
        Ice(Object* object): object_(object) {}

        void draw() const {
            if (object_->visible_) {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Ice));
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                glPushMatrix();
                glLoadIdentity();
                glTranslatef(object_->location_.x_, object_->location_.y_, 0.f);
                glRotatef(object_->rotation_, 0.f, 0.f, 1.f);
                if (object_->frozen_ > 0) {
                    int posX;
                    int posY;

                    if (object_->frozen_ >= 30) {
                        posX = 0;
                        posY = 0;
                    }
                    else if (object_->frozen_ >= 20) {
                        posX = 1;
                        posY = 0;
                    }
                    else if (object_->frozen_ >= 10) {
                        posX = 0;
                        posY = 1;
                    }
                    else if (object_->frozen_ >= 0) {
                        posX = 1;
                        posY = 1;
                    }

                    else {
                        decoObjects::removeIce(this);
                        return;
                    }

                    glColor3f(1.f, 1.f, 1.f);
                    glBegin(GL_QUADS);
                        glTexCoord2f( posX*0.5f,    posY*0.5f);    glVertex2f(-object_->radius_*1.5f,-object_->radius_*1.5f);
                        glTexCoord2f( posX*0.5f,   (posY+1)*0.5f); glVertex2f(-object_->radius_*1.5f, object_->radius_*1.5f);
                        glTexCoord2f((posX+1)*0.5f,(posY+1)*0.5f); glVertex2f( object_->radius_*1.5f, object_->radius_*1.5f);
                        glTexCoord2f((posX+1)*0.5f, posY*0.5f);    glVertex2f( object_->radius_*1.5f,-object_->radius_*1.5f);
                    glEnd();

                }

                glPopMatrix();

                glDisable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
        }

    private:
        Object* object_;
};

# endif // ICE_HPP_INCLUDED



