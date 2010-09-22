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
# include "Hud/hud.hpp"
# include "System/window.hpp"

void RightLife::draw() const {

    Player const* player(NULL);

    if ((settings::C_playerIteamL && settings::C_playerIIteamL)
        | (settings::C_playerIteamR && settings::C_playerIIteamR))  player = players::getPlayerI();
    else if (settings::C_playerIteamL && settings::C_playerIIteamR) player = players::getPlayerII();
    else if (settings::C_playerIIteamL && settings::C_playerIteamR) player = players::getPlayerI();
    else if (settings::C_playerIteamR)                              player = players::getPlayerI();
    else if (settings::C_playerIIteamR)                             player = players::getPlayerII();

    if (player) {
        Color3f const& color = player->color();
        Vector2f const& port = window::getViewPort();

        hud::drawScreenText(player->name(), Vector2f(port.x_-10.f,port.y_-85.f), font::HandelGotDLig, 20.f, TEXT_ALIGN_RIGHT, color);

        float life = player->ship()->getLife();
        float fuel = player->ship()->getFuel();

        // player life bar
        glBegin(GL_QUADS);
            Color3f lifeColor(1.f, 0.f, 0.f);
            lifeColor.h(lifeColor.h() + life);
            lifeColor.gl4f(0.9f);
            glVertex2i(port.x_ - 16, port.y_-37);
            glVertex2i(port.x_ - 16, port.y_-54);
            glVertex2i(port.x_ - 16 - life*1.92f, port.y_-54);
            glVertex2i(port.x_ - 16 - life*1.92f, port.y_-37);
        glEnd();

        // draw caps
        if (life > 0) {
            if (life == 100.f) {
                glBegin(GL_QUADS);
                    glVertex2i(port.x_ - 208, port.y_-54);
                    glVertex2i(port.x_ - 214, port.y_-49);
                    glVertex2i(port.x_ - 214, port.y_-42);
                    glVertex2i(port.x_ - 208, port.y_-37);
                glEnd();
            }

            glBegin(GL_QUADS);
                glVertex2i(port.x_ - 16, port.y_-54);
                glVertex2i(port.x_ - 10, port.y_-49);
                glVertex2i(port.x_ - 10, port.y_-42);
                glVertex2i(port.x_ - 16, port.y_-37);
            glEnd();
        }

        // player fuel bar
        glBegin(GL_QUADS);
            glColor4f(1.f,1.f,0.f,0.9f);
            glVertex2i(port.x_ - 16, port.y_-10);
            glVertex2i(port.x_ - 16, port.y_-27);
            glVertex2i(port.x_ - 16 - fuel*1.92f, port.y_-27);
            glVertex2i(port.x_ - 16 - fuel*1.92f, port.y_-10);
        glEnd();

        // draw caps
        if (fuel > 0) {
            if (fuel == 100.f) {
                glBegin(GL_QUADS);
                    glVertex2i(port.x_ - 208, port.y_-27);
                    glVertex2i(port.x_ - 214, port.y_-22);
                    glVertex2i(port.x_ - 214, port.y_-15);
                    glVertex2i(port.x_ - 208, port.y_-10);
                glEnd();
            }

            glBegin(GL_QUADS);
                glVertex2i(port.x_ - 16, port.y_-27);
                glVertex2i(port.x_ - 10, port.y_-22);
                glVertex2i(port.x_ - 10, port.y_-15);
                glVertex2i(port.x_ - 16, port.y_-10);
            glEnd();
        }

        glEnable(GL_TEXTURE_2D);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::EnergyBars));
        glBegin(GL_QUADS);
            glColor3f(1.0,1.0,1.0);
            glTexCoord2i(0, 0); glVertex2f(port.x_, port.y_-64);
            glTexCoord2i(0, 1); glVertex2f(port.x_, port.y_);
            glTexCoord2i(1, 1); glVertex2f(port.x_-224, port.y_);
            glTexCoord2i(1, 0); glVertex2f(port.x_-224, port.y_-64);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
