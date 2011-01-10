/* CountDown.cpp

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

# include "Hud/CountDown.hpp"

# include "Media/texture.hpp"
# include "System/window.hpp"
# include "Games/games.hpp"
# include "System/timer.hpp"
# include "Media/sound.hpp"

# include <SFML/OpenGL.hpp>

void CountDown::draw() const {
    glEnable(GL_TEXTURE_2D);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float time = games::elapsedTime();
    if (time < 1.f)
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::CountDown4));
    else if (time < 2.f) {
        if (time - timer::frameTime() <= 1.f) sound::playSound(sound::Countdown);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::CountDown3));
    }
    else if (time < 3.f) {
        if (time - timer::frameTime() <= 2.f) sound::playSound(sound::Countdown);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::CountDown2));
    }
    else if (time < 4.f) {
        if (time - timer::frameTime() <= 3.f) sound::playSound(sound::Countdown);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::CountDown1));
    }
    else {
        if (time - timer::frameTime() <= 4.f) sound::playSound(sound::Start);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::CountDown0));
    }

    float mid(window::getViewPort().x_/2.f);
    float top(0.f);
    if (time > 5.f) top = 800.f*std::pow(time - 5.25f, 2) - 50.f;

    if (time-timer::frameTime() < 5.f && time >= 5.f)
        sound::playSound(sound::Swish);

    glBegin(GL_QUADS);
        glColor3f(1.0,1.0,1.0);
        glTexCoord2f(0, 0); glVertex2f(mid-256, -50-top);
        glTexCoord2f(0, 1); glVertex2f(mid-256, 206-top);
        glTexCoord2f(1, 1); glVertex2f(mid+256, 206-top);
        glTexCoord2f(1, 0); glVertex2f(mid+256, -50-top);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

