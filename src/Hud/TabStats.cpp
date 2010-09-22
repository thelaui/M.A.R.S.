/* TabStats.cpp

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

# include "Hud/TabStats.hpp"

# include "System/settings.hpp"
# include "Hud/hud.hpp"
# include "System/timer.hpp"
# include "Media/texture.hpp"
# include "Menu/menus.hpp"
# include "Players/Player.hpp"
# include "System/window.hpp"
# include "Games/games.hpp"
# include "SpaceObjects/ships.hpp"

# include <SFML/OpenGL.hpp>
# include <sstream>
# include <iomanip>

TabStats::TabStats():
    visible_(false),
    refresh_(false),
    totalPoints_(0) {}

void TabStats::update() {
    if (visible_ || refresh_) {
        // check for necessityof a map-update
        int currentPoints(0);
        for (std::vector<Team*>::const_iterator it = players::getAllTeams().begin(); it != players::getAllTeams().end(); ++it)
            currentPoints += (*it)->points_;

        if (currentPoints != totalPoints_ || refresh_) {
            refresh_ = false;
            totalPoints_ = currentPoints;
            // create Map
            teamMap_ = std::multimap<Team*, std::multiset<Player*, playerPtrCmp>, teamPtrCmp >();
            std::vector<Team*>const& teams = players::getAllTeams();
            for (std::vector<Team*>::const_iterator it = teams.begin(); it != teams.end(); ++it)
                teamMap_.insert(std::make_pair(*it, std::multiset<Player*, playerPtrCmp>((*it)->members().begin(), (*it)->members().end())));
        }
    }
}

void TabStats::draw() const {
    if (visible_ && !menus::visible()) {

        window::setPixelView();
        //get height of list
        int height;
        if (games::type() == games::gDeathMatch)
            height = ships::getShips().size()*12 + teamMap_.size()*2 + 85;
        else
            height = ships::getShips().size()*12 + teamMap_.size()*20 + 85;
        int width = 460;
        // draw background
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        Vector2f topLeft;
        topLeft = (window::getViewPort() - Vector2f(width, height))/2.f;

        glBegin(GL_QUADS);
            // bg
            glColor4f(0.0,0.0,0.0,0.85);
            glVertex2f(topLeft.x_-10,topLeft.y_-10);
            glVertex2f(topLeft.x_+width+10,topLeft.y_-10);
            glVertex2f(topLeft.x_+width+10,topLeft.y_+height+10);
            glVertex2f(topLeft.x_-10,topLeft.y_+height+10);

            glColor4f(1.0,1.0,1.0,0.2);
            glVertex2f(topLeft.x_-10,topLeft.y_-10);
            glVertex2f(topLeft.x_+width+10,topLeft.y_-10);
            glColor4f(1.0,1.0,1.0,0.02);
            glVertex2f(topLeft.x_+width+10,topLeft.y_+height/4);
            glVertex2f(topLeft.x_-10,topLeft.y_+height/3);
        glEnd();

        glLineWidth(2.f);
        glBegin(GL_LINE_LOOP);
            glColor4f(0.5f, 0.5f, 0.5f, 0.8f);
            glVertex2f(topLeft.x_-10,topLeft.y_+height+10);
            glVertex2f(topLeft.x_-10,topLeft.y_-10);
            glVertex2f(topLeft.x_+width+10,topLeft.y_-10);
            glVertex2f(topLeft.x_+width+10,topLeft.y_+height+10);
        glEnd();

        glColor3f(1.f, 1.f, 1.f);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::WindowOn));

        glBegin(GL_QUADS);
            // topleft
            glTexCoord2f(0, 0);                   glVertex2i(topLeft.x_-60, topLeft.y_-60);
            glTexCoord2f(0, 96.f/200.f);          glVertex2i(topLeft.x_-60, topLeft.y_+36);
            glTexCoord2f(96.f/200.f, 96.f/200.f); glVertex2i(topLeft.x_+36, topLeft.y_+36);
            glTexCoord2f(96.f/200.f, 0);          glVertex2i(topLeft.x_+36, topLeft.y_-60);
            // topRight
            glTexCoord2f(96.f/200.f, 0);          glVertex2i(topLeft.x_-36+width, topLeft.y_-60);
            glTexCoord2f(96.f/200.f, 96.f/200.f); glVertex2i(topLeft.x_-36+width, topLeft.y_+36);
            glTexCoord2f(96.f/100.f, 96.f/200.f); glVertex2i(topLeft.x_+60+width, topLeft.y_+36);
            glTexCoord2f(96.f/100.f, 0);          glVertex2i(topLeft.x_+60+width, topLeft.y_-60);
            // bottomleft
            glTexCoord2f(0, 96.f/200.f);          glVertex2i(topLeft.x_-60, topLeft.y_-36+height);
            glTexCoord2f(0, 96.f/100.f);          glVertex2i(topLeft.x_-60, topLeft.y_+60+height);
            glTexCoord2f(96.f/200.f, 96.f/100.f); glVertex2i(topLeft.x_+36, topLeft.y_+60+height);
            glTexCoord2f(96.f/200.f, 96.f/200.f); glVertex2i(topLeft.x_+36, topLeft.y_-36+height);
            // bottomRight
            glTexCoord2f(96.f/200.f, 96.f/200.f); glVertex2i(topLeft.x_-36+width, topLeft.y_-36+height);
            glTexCoord2f(96.f/200.f, 96.f/100.f); glVertex2i(topLeft.x_-36+width, topLeft.y_+60+height);
            glTexCoord2f(96.f/100.f, 96.f/100.f); glVertex2i(topLeft.x_+60+width, topLeft.y_+60+height);
            glTexCoord2f(96.f/100.f, 96.f/200.f); glVertex2i(topLeft.x_+60+width, topLeft.y_-36+height);

            // top
            glTexCoord2f(96.f/100.f, 0);          glVertex2i(topLeft.x_+36, topLeft.y_-60);
            glTexCoord2f(96.f/100.f, 96.f/200.f); glVertex2i(topLeft.x_+36, topLeft.y_+36);
            glTexCoord2f(1.f, 96.f/200.f);        glVertex2i(topLeft.x_-36+width, topLeft.y_+36);
            glTexCoord2f(1.f, 0);                 glVertex2i(topLeft.x_-36+width, topLeft.y_-60);
            // right
            glTexCoord2f(96.f/200.f, 96.f/100.f); glVertex2i(topLeft.x_-36+width, topLeft.y_+36);
            glTexCoord2f(96.f/200.f, 1.f);        glVertex2i(topLeft.x_-36+width, topLeft.y_-36+height);
            glTexCoord2f(96.f/100.f, 1.f);        glVertex2i(topLeft.x_+60+width, topLeft.y_-36+height);
            glTexCoord2f(96.f/100.f, 96.f/100.f); glVertex2i(topLeft.x_+60+width, topLeft.y_+36);
            // left
            glTexCoord2f(0, 96.f/100.f);          glVertex2i(topLeft.x_-60, topLeft.y_+36);
            glTexCoord2f(0, 1.f);                 glVertex2i(topLeft.x_-60, topLeft.y_-36+height);
            glTexCoord2f(96.f/200.f, 1.f);        glVertex2i(topLeft.x_+36, topLeft.y_-36+height);
            glTexCoord2f(96.f/200.f, 96.f/100.f); glVertex2i(topLeft.x_+36, topLeft.y_+36);
            // bottom
            glTexCoord2f(96.f/100.f, 96.f/200.f); glVertex2i(topLeft.x_+36, topLeft.y_-36+height);
            glTexCoord2f(96.f/100.f, 96.f/100.f); glVertex2i(topLeft.x_+36, topLeft.y_+60+height);
            glTexCoord2f(1.f, 96.f/100.f);        glVertex2i(topLeft.x_-36+width, topLeft.y_+60+height);
            glTexCoord2f(1.f, 96.f/200.f);        glVertex2i(topLeft.x_-36+width, topLeft.y_-36+height);
        glEnd();


        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);

        hud::drawScreenText("foo", Vector2f(-10000, -10000), font::HandelGotDLig, 1.f);
        hud::drawScreenText("Statistics", topLeft + Vector2f(10, 10), font::HandelGotDLig, 20.f, TEXT_ALIGN_LEFT, Color3f(0.7f, 0.7f, 0.7f));
        std::stringstream sstr;
        int seconds = games::elapsedTime();
        sstr << std::setfill('0') << std::setw(2) << (seconds-seconds%60)/60 << " : " << std::setw(2) << seconds%60;
        hud::drawScreenText(sstr.str(), topLeft + Vector2f(width-10, 18), font::FreeSans, 12.f, TEXT_ALIGN_RIGHT, Color3f(0.7f, 0.7f, 0.7f));

        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glLineWidth(2.f);

        glBegin(GL_LINES);
            glColor4f(1.f, 0.5f, 0.8f, 1.0f);
            glVertex2f(topLeft.x_+10, topLeft.y_+35);
            glColor4f(1.f, 0.5f, 0.8f, 0.0f);
            glVertex2f(topLeft.x_+width-10, topLeft.y_+35);
        glEnd();

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        width -= 20;
        height = 12;
        topLeft += Vector2f(10.f, 60.f);

        hud::drawScreenText("Points", topLeft+Vector2f(170,0), font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER);
        hud::drawScreenText("Frags", topLeft + Vector2f(225, 0), font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER);
        hud::drawScreenText("TeamKills", topLeft + Vector2f(280, 0), font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER);
        hud::drawScreenText("Suicides", topLeft + Vector2f(335, 0), font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER);
        hud::drawScreenText("Deaths", topLeft + Vector2f(390, 0), font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER);

        topLeft.y_ += 15;

        for (std::multimap<Team*, std::multiset<Player*, playerPtrCmp>, teamPtrCmp >::const_iterator it = teamMap_.begin(); it != teamMap_.end(); ++it) {
            int totalFrags(0), totalSuicides(0), totalTeamKills(0), totalDeaths(0);
            Color3f teamColor = it->first->color();
            std::multiset<Player*, playerPtrCmp>const& members = it->second;
            for (std::multiset<Player*, playerPtrCmp>::iterator currentPlayer = members.begin(); currentPlayer != members.end(); ++currentPlayer) {
                if ((*currentPlayer)->controlType_ == controllers::cPlayer1 || (*currentPlayer)->controlType_ == controllers::cPlayer2)
                    teamColor.gl4f(0.5);
                else
                    teamColor.gl4f(0.2);

                glBegin(GL_QUADS);
                    glVertex2f(topLeft.x_,topLeft.y_+1);
                    glVertex2f(topLeft.x_+width,topLeft.y_+1);
                    glVertex2f(topLeft.x_+width,topLeft.y_+height+1);
                    glVertex2f(topLeft.x_,topLeft.y_+height+1);
                glEnd();

                Color3f drawColor((*currentPlayer)->color());
                // draw name
                hud::drawScreenText((*currentPlayer)->name_, topLeft + Vector2f(2, 0), font::HandelGotDLig, 12.f, TEXT_ALIGN_LEFT, drawColor);
                // draw [BOT]
                if ((*currentPlayer)->controlType_ != controllers::cPlayer1 && (*currentPlayer)->controlType_ != controllers::cPlayer2)
                    hud::drawScreenText("[BOT]", topLeft+Vector2f(80,0), font::HandelGotDLig, 12.f, TEXT_ALIGN_LEFT, drawColor);
                // draw points
                int value = (*currentPlayer)->frags_ - (*currentPlayer)->suicides_ - (*currentPlayer)->teamKills_;
                if (value > 0)      drawColor = Color3f(0.3,1,0.3);
                else if (value < 0) drawColor = Color3f(1,0.3,0.3);
                else                drawColor = Color3f(1,1,0.3);
                std::stringstream sstr;
                sstr << value;
                hud::drawScreenText(sstr.str(), topLeft+Vector2f(170,0), font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER, drawColor);
                // draw frags
                value = (*currentPlayer)->frags_;
                if (value > 0)      drawColor = Color3f(0.3,1,0.3);
                else                drawColor = Color3f(1,1,0.3);
                totalFrags += value;
                sstr.str("");
                sstr << value;
                hud::drawScreenText(sstr.str(), topLeft + Vector2f(225, 0), font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER, drawColor);
                // draw teamKills
                value = (*currentPlayer)->teamKills_;
                if (value > 0)      drawColor = Color3f(1,0.3,0.3);
                else                drawColor = Color3f(0.3,1,0.3);
                totalTeamKills += value;
                sstr.str("");
                sstr << value;
                hud::drawScreenText(sstr.str(), topLeft + Vector2f(280, 0), font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER, drawColor);
                // draw suicides
                value = (*currentPlayer)->suicides_;
                if (value > 0)      drawColor = Color3f(1,0.3,0.3);
                else                drawColor = Color3f(0.3,1,0.3);
                totalSuicides += value;
                sstr.str("");
                sstr << value;
                hud::drawScreenText(sstr.str(), topLeft + Vector2f(335, 0), font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER, drawColor);
                // draw deaths
                value = (*currentPlayer)->deaths_;
                if (value > 0)      drawColor = Color3f(1,0.3,0.3);
                else                drawColor = Color3f(0.3,1,0.3);
                totalDeaths += value;
                sstr.str("");
                sstr << value;
                hud::drawScreenText(sstr.str(), topLeft + Vector2f(390, 0), font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER, drawColor);

                topLeft.y_ += 12;
            }
            if (games::type() != games::gDeathMatch) {
                topLeft.y_ += 2;

                teamColor.gl4f(0.2f);
                glBegin(GL_QUADS);
                    glVertex2f(topLeft.x_,topLeft.y_+1);
                    glVertex2f(topLeft.x_+width,topLeft.y_+1);
                    glVertex2f(topLeft.x_+width,topLeft.y_+height+1);
                    glVertex2f(topLeft.x_,topLeft.y_+height+1);
                glEnd();

                Color3f drawColor(teamColor);
                hud::drawScreenText("Total:", topLeft + Vector2f(2, 0), font::HandelGotDLig, 12.f, TEXT_ALIGN_LEFT, drawColor);

                if (totalFrags-totalSuicides-totalTeamKills > 0)      drawColor = Color3f(0.3,1,0.3);
                else if (totalFrags-totalSuicides-totalTeamKills < 0) drawColor = Color3f(1,0.3,0.3);
                else                                                  drawColor = Color3f(1,1,0.3);
                std::stringstream sstr;
                sstr << totalFrags-totalSuicides-totalTeamKills;
                hud::drawScreenText(sstr.str(), topLeft + Vector2f(170, 0), font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER, drawColor);

                if (totalFrags > 0)     drawColor = Color3f(0.3,1,0.3);
                else                    drawColor = Color3f(1,1,0.3);
                sstr.str("");
                sstr << totalFrags;
                hud::drawScreenText(sstr.str(), topLeft + Vector2f(225, 0), font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER, drawColor);

                if (totalTeamKills > 0) drawColor = Color3f(1,0.3,0.3);
                else                    drawColor = Color3f(0.3,1,0.3);
                sstr.str("");
                sstr << totalTeamKills;
                hud::drawScreenText(sstr.str(), topLeft + Vector2f(280, 0), font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER, drawColor);

                if (totalSuicides > 0)  drawColor = Color3f(1,0.3,0.3);
                else                    drawColor = Color3f(0.3,1,0.3);
                sstr.str("");
                sstr << totalSuicides;
                hud::drawScreenText(sstr.str(), topLeft + Vector2f(335, 0), font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER, drawColor);

                if (totalDeaths > 0)  drawColor = Color3f(1,0.3,0.3);
                else                    drawColor = Color3f(0.3,1,0.3);
                sstr.str("");
                sstr << totalDeaths;
                hud::drawScreenText(sstr.str(), topLeft + Vector2f(390, 0), font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER, drawColor);

                topLeft.y_ += 18;
            }
            else
                topLeft.y_ += 2;
        }
        hud::drawScreenText("foo", Vector2f(-10000, -10000), font::HandelGotDLig, 1.f);
    }
}

void TabStats::display(bool show) {
    visible_ = show;
}

void TabStats::refresh() {
    refresh_ = true;
}
