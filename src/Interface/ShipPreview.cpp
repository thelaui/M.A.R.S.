/* ShipPreview.cpp

Copyright (c) 2010 by Felix Lauer und Simon Schneegans

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

# include "Interface/ShipPreview.hpp"

# include "System/settings.hpp"
# include "System/timer.hpp"
# include "Media/texture.hpp"

# include <SFML/OpenGL.hpp>

ShipPreview::ShipPreview (Color3f* value, int* graphic, Vector2f const& topLeft):
    UiElement(topLeft, 20, 20),
    color_(value),
    graphic_(graphic) {}

void ShipPreview::draw() const {
    Vector2f origin = parent_->getTopLeft() + topLeft_ + Vector2f(10.f, 10.f);

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(origin.x_, origin.y_, 0.f);

    glEnable(GL_TEXTURE_2D);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // draw glow
    glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Ships));
    Color3f bg = *color_;
    if (bg.v() < 0.5f) bg.v(0.5f);
    bg.gl4f(0.6f);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(-16.f*3.2f,-16.f*3.2f);
        glTexCoord2f(0, 0.125f); glVertex2f(-16.f*3.2f, 16.f*3.2f);
        glTexCoord2f(0.125f, 0.125f); glVertex2f( 16.f*3.2f, 16.f*3.2f);
        glTexCoord2f(0.125f, 0); glVertex2f( 16.f*3.2f,-16.f*3.2f);
    glEnd();

    // draw ship
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glRotatef(timer::totalTime()*160, 0.f, 0.f, 1.f);

    float x, y;

    x = static_cast<float>((*graphic_)%4)*0.25f + 0.125f;
    y = static_cast<float>(std::floor((*graphic_)*0.25f))*0.125f;

    glColor3f(1.f, 1.f, 1.f);
    glBegin(GL_QUADS);
        glTexCoord2f(x, y+0.125f);          glVertex2f(-16.f, -16.f);
        glTexCoord2f(x+0.125f, y+0.125f);   glVertex2f(-16.f,  16.f);
        glTexCoord2f(x+0.125f, y);          glVertex2f( 16.f,  16.f);
        glTexCoord2f(x, y);                 glVertex2f( 16.f, -16.f);
    glEnd();

    x -= 0.125f;

    color_->gl3f();
    glBegin(GL_QUADS);
        glTexCoord2f(x, y+0.125f);          glVertex2f(-16.f, -16.f);
        glTexCoord2f(x+0.125f, y+0.125f);   glVertex2f(-16.f,  16.f);
        glTexCoord2f(x+0.125f, y);          glVertex2f( 16.f,  16.f);
        glTexCoord2f(x, y);                 glVertex2f( 16.f, -16.f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
    glPopAttrib();
}

