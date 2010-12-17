/* ShipPreview.cpp

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

# include "Interface/ShipPreview.hpp"

# include "System/settings.hpp"
# include "System/timer.hpp"
# include "Media/texture.hpp"

# include <SFML/OpenGL.hpp>

ShipPreview::ShipPreview (Color3f* color, Color3f* teamColor, int* graphic, Vector2f const& topLeft):
    UiElement(topLeft, 20, 20),
    color_(color),
    teamColor_(teamColor),
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
    Color3f bg = *teamColor_;
    if (bg.v() < 0.4f) bg.v(0.4f);
    if (bg.s() < 0.5f) bg.s(0.5f);
    bg.gl4f(0.6f);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0.75f); glVertex2f(-16.f*3.2f,-16.f*3.2f);
        glTexCoord2f(0, 0.875f); glVertex2f(-16.f*3.2f, 16.f*3.2f);
        glTexCoord2f(0.125f, 0.875f); glVertex2f( 16.f*3.2f, 16.f*3.2f);
        glTexCoord2f(0.125f, 0.75f); glVertex2f( 16.f*3.2f,-16.f*3.2f);
    glEnd();

    // draw ship
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glRotatef(timer::totalTime()*100, 0.f, 0.f, 1.f);

    float x, y;

    x = static_cast<float>(*graphic_%8)*0.125f;
    y = static_cast<float>(std::floor(*graphic_*0.125f))*0.375f;

    glColor3f(1.f, 1.f, 1.f);
    glBegin(GL_QUADS);
        glTexCoord2f(x, y+0.125f);          glVertex2f(-32.f, -32.f);
        glTexCoord2f(x+0.125f, y+0.125f);   glVertex2f(-32.f,  32.f);
        glTexCoord2f(x+0.125f, y);          glVertex2f( 32.f,  32.f);
        glTexCoord2f(x, y);                 glVertex2f( 32.f, -32.f);
    glEnd();

    y += 0.125f;

    teamColor_->gl3f();
    glBegin(GL_QUADS);
        glTexCoord2f(x, y+0.125f);          glVertex2f(-32.f, -32.f);
        glTexCoord2f(x+0.125f, y+0.125f);   glVertex2f(-32.f,  32.f);
        glTexCoord2f(x+0.125f, y);          glVertex2f( 32.f,  32.f);
        glTexCoord2f(x, y);                 glVertex2f( 32.f, -32.f);
    glEnd();

    y += 0.125f;

    color_->gl3f();
    glBegin(GL_QUADS);
        glTexCoord2f(x, y+0.125f);          glVertex2f(-32.f, -32.f);
        glTexCoord2f(x+0.125f, y+0.125f);   glVertex2f(-32.f,  32.f);
        glTexCoord2f(x+0.125f, y);          glVertex2f( 32.f,  32.f);
        glTexCoord2f(x, y);                 glVertex2f( 32.f, -32.f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
    glPopAttrib();
}

