/* RightLife.cpp

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

# include "Hud/RightLife.hpp"

# include "Players/players.hpp"
# include "Players/Player.hpp"
# include "System/settings.hpp"
# include "Media/text.hpp"
# include "System/window.hpp"
# include "Players/Team.hpp"

# include <sstream>

void RightLife::draw() const {

    Player const* player(NULL);

    if ((settings::C_playerIteamL && settings::C_playerIIteamL)
        | (settings::C_playerIteamR && settings::C_playerIIteamR))  player = players::getPlayerI();
    else if (settings::C_playerIteamL && settings::C_playerIIteamR) player = players::getPlayerII();
    else if (settings::C_playerIIteamL && settings::C_playerIteamR) player = players::getPlayerI();
    else if (settings::C_playerIteamR)                              player = players::getPlayerI();
    else if (settings::C_playerIIteamR)                             player = players::getPlayerII();

    if (player) {
        Color3f color = player->color();
        if (color.v() < 0.5f) color.v(0.5f);
        Vector2f const& port = window::getViewPort();

        std::stringstream sstr;
        int first(players::getFirstPoints());
        if (player->team()->points() == first) {
            int second(players::getSecondPoints());
            sstr << player->name() << " (" << player->team()->points() << "/ +" << first-second << ")";
        }
        else
            sstr << player->name() << " (" << player->team()->points() << "/ -" << first-player->team()->points() << ")";

        text::drawScreenText(sstr.str(), Vector2f(port.x_-10.f,port.y_-75.f), font::HandelGotDLig, 18.f, TEXT_ALIGN_RIGHT, color);

        float life = player->ship()->getLife();
        float fuel = player->ship()->getFuel();

        // player life bar
        glBegin(GL_QUADS);
            Color3f lifeColor(1.f, 0.f, 0.f);
            lifeColor.h(lifeColor.h() + life);
            lifeColor.gl4f(0.9f);
            glVertex2i(port.x_ - 15, port.y_-32);
            glVertex2i(port.x_ - 15, port.y_-47);
            glVertex2i(port.x_ - 15 - life*1.64f, port.y_-47);
            glVertex2i(port.x_ - 15 - life*1.64f, port.y_-32);
        glEnd();

        // draw caps
        if (life > 0) {
            if (life == 100.f) {
                glBegin(GL_QUADS);
                    glVertex2i(port.x_ - 179, port.y_-47);
                    glVertex2i(port.x_ - 184, port.y_-43);
                    glVertex2i(port.x_ - 184, port.y_-37);
                    glVertex2i(port.x_ - 179, port.y_-32);
                glEnd();
            }

            glBegin(GL_QUADS);
                glVertex2i(port.x_ - 15, port.y_-47);
                glVertex2i(port.x_ - 10, port.y_-43);
                glVertex2i(port.x_ - 10, port.y_-37);
                glVertex2i(port.x_ - 15, port.y_-32);
            glEnd();
        }

        // player fuel bar
        glBegin(GL_QUADS);
            glColor4f(1.f,1.f,0.f,0.9f);
            glVertex2i(port.x_ - 15, port.y_-8);
            glVertex2i(port.x_ - 15, port.y_-23);
            glVertex2i(port.x_ - 15 - fuel*1.64f, port.y_-23);
            glVertex2i(port.x_ - 15 - fuel*1.64f, port.y_-8);
        glEnd();

        // draw caps
        if (fuel > 0) {
            if (fuel == 100.f) {
                glBegin(GL_QUADS);
                    glVertex2i(port.x_ - 179, port.y_-23);
                    glVertex2i(port.x_ - 184, port.y_-18);
                    glVertex2i(port.x_ - 184, port.y_-13);
                    glVertex2i(port.x_ - 179, port.y_-8);
                glEnd();
            }

            glBegin(GL_QUADS);
                glVertex2i(port.x_ - 15, port.y_-23);
                glVertex2i(port.x_ - 10, port.y_-18);
                glVertex2i(port.x_ - 10, port.y_-13);
                glVertex2i(port.x_ - 15, port.y_-8);
            glEnd();
        }

        glEnable(GL_TEXTURE_2D);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::EnergyBars));
        glBegin(GL_QUADS);
            glColor3f(1.0,1.0,1.0);
            glTexCoord2i(0, 0); glVertex2f(port.x_, port.y_-64);
            glTexCoord2i(0, 1); glVertex2f(port.x_, port.y_);
            glTexCoord2i(1, 1); glVertex2f(port.x_-192, port.y_);
            glTexCoord2i(1, 0); glVertex2f(port.x_-192, port.y_-64);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
