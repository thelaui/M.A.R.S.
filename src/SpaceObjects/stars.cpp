/* stars.cpp

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

# include "SpaceObjects/stars.hpp"

# include "Media/texture.hpp"
# include "System/settings.hpp"
# include "System/window.hpp"
# include "Particles/particles.hpp"
# include "Particles/Star.hpp"
# include "defines.hpp"

# include <SFML/System.hpp>

namespace stars {

    namespace {
        int tex_;
        float top_, left_;
        float spawnTimer_(0.f);
    }

    void init() {
        tex_ = sf::Randomizer::Random(0, 1);
        top_ = sf::Randomizer::Random(0.f, 1.f);
        left_= sf::Randomizer::Random(0.f, 1.f);
        Star::init();
    }

    void draw() {
        glDisable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);

        if (settings::C_StarsHigh)
            switch (tex_) {
                case 0: glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Stars1_large)); break;
                case 1: glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Stars2_large)); break;
            }
        else
            switch (tex_) {
                case 0: glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Stars1_medium)); break;
                case 1: glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Stars2_medium)); break;
            }

        Vector2f const& viewport = window::getViewPort();
        const float res          = settings::C_StarsHigh ? 1.f/2048.f : 1.f/1024.f;

        glBegin(GL_QUADS);
            glColor3f(1.f, 1.0f, 1.0f);
            glTexCoord2f(left_,                   top_);                   glVertex2f(0, 0);
            glTexCoord2f(left_,                   top_ + res*viewport.y_); glVertex2f(0, viewport.y_);
            glTexCoord2f(left_ + res*viewport.x_, top_ + res*viewport.y_); glVertex2f(viewport.x_, viewport.y_);
            glTexCoord2f(left_ + res*viewport.x_, top_);                   glVertex2f(viewport.x_, 0);
        glEnd();

        glEnable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
