/* GamePoints.cpp

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

# include "Hud/GamePoints.hpp"

# include "Media/text.hpp"
# include "Media/texture.hpp"
# include "System/window.hpp"
# include "Players/players.hpp"
# include "Teams/Team.hpp"
# include "Particles/particles.hpp"
# include "Media/sound.hpp"
# include "System/timer.hpp"
# include "Teams/teams.hpp"

# include <SFML/OpenGL.hpp>
# include <sstream>

void GamePoints::draw() const {

    if (timer_ > 0.f) {
        Vector2f viewPort = window::getViewPort();
        float midPoint = viewPort.x_/2;
        float top;
        if (timer_ < 1.1578f) {
            if (timer_+timer::realFrameTime() >= 0.7f && timer_ < 0.7f) sound::playSound(sound::Swish);
            top = -75.f/0.64*std::pow((timer_-0.8f), 2) +15.f;
        }
        else if (timer_ < 8.06f)
            top = 0.f;
        else {
            top = -6.12f*std::pow((timer_-6.5f), 2) +15.f;
            particles::spawnTimed(20, particles::pDust, Vector2f(SPACE_X_RESOLUTION*0.5f, 820));
        }

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBegin(GL_QUADS);
            // bg
            glColor4f(0.0,0.0,0.0,0.45);
            glVertex2f(midPoint-40.f, viewPort.y_-20.f-top);
            glVertex2f(midPoint+40.f, viewPort.y_-20.f-top);
            glVertex2f(midPoint+40.f, viewPort.y_-65.f-top);
            glVertex2f(midPoint-40.f, viewPort.y_-65.f-top);

            glColor4f(1.0,1.0,1.0,0.2);
            glVertex2f(midPoint-40.f, viewPort.y_-65.f-top);
            glVertex2f(midPoint+40.f, viewPort.y_-65.f-top);
            glColor4f(1.0,1.0,1.0,0.02);
            glVertex2f(midPoint+40.f, viewPort.y_-20.f-60/2-top);
            glVertex2f(midPoint-40.f, viewPort.y_-20.f-60/4-top);
        glEnd();

        glLineWidth(1.f);
        glColor4f(1.0,1.0,1.0,0.3);
        glBegin(GL_LINE_STRIP);
            glVertex2f(midPoint-40.f, viewPort.y_-20.f-top);
            glVertex2f(midPoint-40.f, viewPort.y_-65.f-top);
            glVertex2f(midPoint+40.f, viewPort.y_-65.f-top);

            glColor4f(1.0,1.0,1.0,0.1);
            glVertex2f(midPoint+40.f, viewPort.y_-65.f-top);
            glVertex2f(midPoint+40.f, viewPort.y_-20.f-top);
            glVertex2f(midPoint-40.f, viewPort.y_-20.f-top);
        glEnd();

        glEnable(GL_TEXTURE_2D);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Interface));
        glBegin(GL_QUADS);
            glColor3f(1.0,1.0,1.0);
            glTexCoord2f(0.78125f, 0.f);           glVertex2f(midPoint-31.f, viewPort.y_-35.f-top);
            glTexCoord2f(0.78125f, 0.09765625f);   glVertex2f(midPoint-31.f, viewPort.y_-top+15.f);
            glTexCoord2f(0.8203125f, 0.09765625f); glVertex2f(midPoint-11.f, viewPort.y_-top+15.f);
            glTexCoord2f(0.8203125f, 0.f);         glVertex2f(midPoint-11.f, viewPort.y_-35.f-top);

            glTexCoord2f(0.78125f, 0.f);           glVertex2f(midPoint+11.f, viewPort.y_-35.f-top);
            glTexCoord2f(0.78125f, 0.09765625f);   glVertex2f(midPoint+11.f, viewPort.y_-top+15.f);
            glTexCoord2f(0.8203125f, 0.09765625f); glVertex2f(midPoint+31.f, viewPort.y_-top+15.f);
            glTexCoord2f(0.8203125f, 0.f);         glVertex2f(midPoint+31.f, viewPort.y_-35.f-top);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        std::stringstream sstr;
        sstr << teams::getTeamL()->victories() << " : " << teams::getTeamR()->victories();
        text::drawFooText();
        text::drawScreenText(sf::String(sstr.str()), Vector2f(midPoint, viewPort.y_-60.f-top), 20.f, TEXT_ALIGN_CENTER, Color3f(1.f,0.5f,0.8f));
    }
}

void GamePoints::update() {
    if (timer_ > 0.f)
        timer_ -= timer::realFrameTime();
}

void GamePoints::display(bool show) {
    if (show) {
        if (timer_ <= 0.f) {
            sound::playSound(sound::Score);
            timer_ = 10.f;
        }
        else if (timer_ > 1.1578f && timer_ < 8.06f)
            timer_ = 8.f;
    }
    else
        timer_ = 0.f;
}
