/* Evil.cpp

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

# include "DecoObjects/Evil.hpp"

# include "Media/texture.hpp"
# include "System/timer.hpp"
# include "defines.hpp"

# include <SFML/OpenGL.hpp>

void Evil::draw() const {
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::CannonSocket));
    glColor3f(1,1,1);
    glTranslatef(SPACE_X_RESOLUTION*0.5f+210.f, -100.f, 0.f);
    glRotatef(std::sin(timer::totalTime())*5.f, 0.f, 0.f, 1.f);

    glBegin(GL_QUADS);
        glTexCoord2f(0.f, 0.5f); glVertex2f(-50.f, 90.f);
        glTexCoord2f(0.f, 0.95f); glVertex2f(-50.f, 180.f);
        glTexCoord2f(0.5f, 0.95f); glVertex2f( 50.f, 180.f);
        glTexCoord2f(0.5f, 0.5f); glVertex2f( 50.f, 90.f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}






