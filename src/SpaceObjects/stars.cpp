/* stars.cpp

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

# include "SpaceObjects/stars.hpp"

# include "Media/texture.hpp"
# include "System/settings.hpp"
# include "System/window.hpp"

# include <SFML/System.hpp>

namespace stars {

    void draw() {
        window::setPixelView();

        glDisable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Background));
        glBegin(GL_QUADS);

        Vector2f const& viewport = window::getViewPort();

        glColor3f(1.f, 1.0f, 1.0f);
        glTexCoord2f(0, 0); glVertex2f(0, 0);
        glTexCoord2f(0, static_cast<float>(viewport.y_)/900.f); glVertex2f(0, viewport.y_);
        glTexCoord2f(static_cast<float>(viewport.x_)/1440.f, static_cast<float>(viewport.y_)/900.f); glVertex2f(viewport.x_, viewport.y_);
        glTexCoord2f(static_cast<float>(viewport.x_)/1440.f, 0); glVertex2f(viewport.x_, 0);

        glEnd();

        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        glEnable(GL_BLEND);

        window::setSpaceView();
    }
}
