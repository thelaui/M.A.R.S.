/* GameStats.cpp

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

# include "Hud/GameStats.hpp"

# include "System/settings.hpp"
# include "Media/text.hpp"
# include "System/timer.hpp"
# include "Particles/particles.hpp"
# include "Media/texture.hpp"

# include <SFML/OpenGL.hpp>
# include <sstream>

void GameStats::draw() const {
    float top(20.f);

    if (settings::C_showFPS)            top += 20.f;
    if (settings::C_showParticleCount)  top += 20.f;
    if (settings::C_showLatency)        top += 20.f;

    if (top > 20.f) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBegin(GL_QUADS);
            // bg
            glColor4f(0.0,0.0,0.0,0.45);
            glVertex2f(5.f,15.f);
            glVertex2f(105.f,15.f);
            glVertex2f(105.f,5.f+top);
            glVertex2f(5.f,5.f+top);

            glColor4f(1.0,1.0,1.0,0.2);
            glVertex2f(5.f,15.f);
            glVertex2f(105.f,15.f);
            glColor4f(1.0,1.0,1.0,0.02);
            glVertex2f(105.f,5.f+top/4);
            glVertex2f(5.f,5.f+top/3);
        glEnd();

        glLineWidth(1.f);
        glColor4f(1.0,1.0,1.0,0.3);
        glBegin(GL_LINE_STRIP);
            glVertex2f(5.f,5.f+top);
            glVertex2f(5.f,15.f);
            glVertex2f(105.f,15.f);
            glColor4f(1.0,1.0,1.0,0.1);
            glVertex2f(105.f,15.f);
            glVertex2f(105.f,5.f+top);
            glVertex2f(5.f,5.f+top);
        glEnd();

        glEnable(GL_TEXTURE_2D);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::TopHandle));
        glBegin(GL_QUADS);
            glColor3f(1.0,1.0,1.0);
            glTexCoord2i(0, 0); glVertex2f(20.f, 0.f);
            glTexCoord2i(0, 1); glVertex2f(20.f, 24.f);
            glTexCoord2i(1, 1); glVertex2f(28.f, 24.f);
            glTexCoord2i(1, 0); glVertex2f(28.f, 0.f);

            glTexCoord2i(0, 0); glVertex2f(82.f, 0.f);
            glTexCoord2i(0, 1); glVertex2f(82.f, 24.f);
            glTexCoord2i(1, 1); glVertex2f(90.f, 24.f);
            glTexCoord2i(1, 0); glVertex2f(90.f, 0.f);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        top = 20.f;

        if (settings::C_showFPS) {
            std::stringstream sstr;
            sstr << "FPS: " << static_cast<int>(timer::fps() + 0.5f);
            text::drawScreenText(sstr.str(), Vector2f(10.f,top), font::Ubuntu, 12.f, TEXT_ALIGN_LEFT, Color3f(0.5f, 0.5f, 0.5f));
            top += 20.f;
        }
        if (settings::C_showParticleCount) {
            if (updateTimer_ > 0.f)
                updateTimer_ -= timer::frameTime();
            else {
                particleCount_ = particles::count();
                updateTimer_ = 0.5f;
            }
            std::stringstream sstr;
            sstr << "Particles: " << particleCount_;
            text::drawScreenText(sstr.str(), Vector2f(10.f,top), font::Ubuntu, 12.f, TEXT_ALIGN_LEFT, Color3f(0.5f, 0.5f, 0.5f));
            top += 20.f;
        }
        if (settings::C_showLatency) {
            std::stringstream sstr;
            sstr << "Latency: " << particles::count();
            text::drawScreenText(sstr.str(), Vector2f(10.f,top), font::Ubuntu, 12.f, TEXT_ALIGN_LEFT, Color3f(0.5f, 0.5f, 0.5f));
            top += 20.f;
        }
    }
}
