/* ShipPreview.cpp

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
    glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Glow));
    color_->gl4f(0.6f);
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2f(-16.f*3.2f,-16.f*3.2f);
        glTexCoord2i(0, 1); glVertex2f(-16.f*3.2f, 16.f*3.2f);
        glTexCoord2i(1, 1); glVertex2f( 16.f*3.2f, 16.f*3.2f);
        glTexCoord2i(1, 0); glVertex2f( 16.f*3.2f,-16.f*3.2f);
    glEnd();

    // draw ship
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glRotatef(timer::totalTime()*160, 0.f, 0.f, 1.f);

    switch (*graphic_) {
        case 1: glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Exhaust1)); break;
        case 2: glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Exhaust2)); break;
        case 3: glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Exhaust3)); break;
        case 4: glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Exhaust4)); break;
        case 5: glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Exhaust5)); break;
        case 6: glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Exhaust6)); break;
        case 7: glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Exhaust7)); break;
        case 8: glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Exhaust8)); break;
        case 9: glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Exhaust9)); break;
        case 10: glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Exhaust10)); break;
        case 11: glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Exhaust11)); break;
        default:;
    }
    glColor3f(1.f, 1.f, 1.f);
    glBegin(GL_QUADS);
        glTexCoord2i(0, 1); glVertex2f(-16.f, -16.f);
        glTexCoord2i(1, 1); glVertex2f(-16.f,  16.f);
        glTexCoord2i(1, 0); glVertex2f( 16.f,  16.f);
        glTexCoord2i(0, 0); glVertex2f( 16.f, -16.f);
    glEnd();

    switch (*graphic_) {
        case 1: glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::ShipPlayerLayer1)); break;
        case 2: glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::ShipPlayerLayer2)); break;
        case 3: glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::ShipPlayerLayer3)); break;
        case 4: glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::ShipPlayerLayer4)); break;
        case 5: glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::ShipPlayerLayer5)); break;
        case 6: glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::ShipPlayerLayer6)); break;
        case 7: glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::ShipPlayerLayer7)); break;
        case 8: glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::ShipPlayerLayer8)); break;
        case 9: glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::ShipPlayerLayer9)); break;
        case 10: glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::ShipPlayerLayer10)); break;
        case 11: glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::ShipPlayerLayer11)); break;
        default:;
    }
    color_->gl3f();
    glBegin(GL_QUADS);
        glTexCoord2i(0, 1); glVertex2f(-16.f, -16.f);
        glTexCoord2i(1, 1); glVertex2f(-16.f,  16.f);
        glTexCoord2i(1, 0); glVertex2f( 16.f,  16.f);
        glTexCoord2i(0, 0); glVertex2f( 16.f, -16.f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
    glPopAttrib();
}

