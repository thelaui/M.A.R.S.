/* TabStats.cpp

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

# include "Hud/TabStats.hpp"

# include "System/settings.hpp"
# include "Media/text.hpp"
# include "System/timer.hpp"
# include "Media/texture.hpp"
# include "Menu/menus.hpp"
# include "Players/Player.hpp"
# include "System/window.hpp"
# include "Games/games.hpp"
# include "SpaceObjects/ships.hpp"
# include "Locales/locales.hpp"
# include "Teams/teams.hpp"

# include <SFML/OpenGL.hpp>
# include <sstream>
# include <iomanip>

// helper function
void inline writeScoreAtCol(int value, int col, Vector2f topLeft, int mirror, Color3f drawColor) {
    std::stringstream sstr;
    sstr << value;
    text::drawScreenText(sf::String(sstr.str()), topLeft+Vector2f((170+70*col + 1)*mirror,1), 12.f, TEXT_ALIGN_CENTER, Color3f(0.f, 0.f, 0.f));
    text::drawScreenText(sf::String(sstr.str()), topLeft+Vector2f((170+70*col)*mirror,0), 12.f, TEXT_ALIGN_CENTER, drawColor);
}

TabStats::TabStats():
    visible_(false),
    refresh_(false),
    sumPoints_(0) {}

void TabStats::update() {
    if (visible_ || refresh_) {
        // check for necessityof a map-update
        int currentPoints(0);
        for (std::vector<Team*>::const_iterator it = teams::getAllTeams().begin(); it != teams::getAllTeams().end(); ++it)
            currentPoints += (*it)->points();

        if (currentPoints != sumPoints_ || refresh_) {
            refresh_ = false;
            sumPoints_ = currentPoints;
            // create Map
            teamMap_ = std::multimap<Team*, std::multiset<Player*, playerPtrCmp>, teamPtrCmp >();
            std::vector<Team*>const& teams = teams::getAllTeams();
            for (std::vector<Team*>::const_iterator it = teams.begin(); it != teams.end(); ++it)
                teamMap_.insert(std::make_pair(*it, std::multiset<Player*, playerPtrCmp>((*it)->members().begin(), (*it)->members().end())));
        }
    }
}

void TabStats::draw() const {
    if (visible_ && !menus::visible()) {
        int mirror(locales::getCurrentLocale().LTR_ ? 1 : -1);
        //get height of list
        int height;
        if (games::type() == games::gDeathMatch)
            height = ships::getShips().size()*12 + teamMap_.size()*2 + 85;
        else
            height = ships::getShips().size()*12 + teamMap_.size()*20 + 85;

        // Compute the width:
        // Points, Frags, TeamKills, Suicides
        int nbColumns = 5;
        // CannonShots
        if (games::type() == games::gCannonKeep)
            nbColumns++;
        // Padding, Name, [BOT], Padding, Columns, Padding
        int width = (3 + 77 + 20 + 70 + (nbColumns*70) + 5)*mirror;

        // draw background
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        Vector2f topLeft;
        topLeft = (window::getViewPort() - Vector2f(width, height))/2.f;

        glBegin(GL_QUADS);
            // bg
            glColor4f(0.0,0.0,0.0,0.85);
            glVertex2f(topLeft.x_-10*mirror,topLeft.y_-10);
            glVertex2f(topLeft.x_+width+10*mirror,topLeft.y_-10);
            glVertex2f(topLeft.x_+width+10*mirror,topLeft.y_+height+10);
            glVertex2f(topLeft.x_-10*mirror,topLeft.y_+height+10);

            glColor4f(1.0,1.0,1.0,0.2);
            glVertex2f(topLeft.x_-10*mirror,topLeft.y_-10);
            glVertex2f(topLeft.x_+width+10*mirror,topLeft.y_-10);
            glColor4f(1.0,1.0,1.0,0.02);
            glVertex2f(topLeft.x_+width+10*mirror,topLeft.y_+height/4);
            glVertex2f(topLeft.x_-10*mirror,topLeft.y_+height/3);
        glEnd();

        glLineWidth(2.f);
        glBegin(GL_LINE_LOOP);
            glColor4f(0.5f, 0.5f, 0.5f, 0.8f);
            glVertex2f(topLeft.x_-10*mirror,topLeft.y_+height+10);
            glVertex2f(topLeft.x_-10*mirror,topLeft.y_-10);
            glVertex2f(topLeft.x_+width+10*mirror,topLeft.y_-10);
            glVertex2f(topLeft.x_+width+10*mirror,topLeft.y_+height+10);
        glEnd();

        glColor3f(1.f, 1.f, 1.f);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Interface));

        glBegin(GL_QUADS);
            // topleft
        glTexCoord2f(0, 0);                   glVertex2i(topLeft.x_-60*mirror, topLeft.y_-60);
        glTexCoord2f(0, 96.f/512.f);          glVertex2i(topLeft.x_-60*mirror, topLeft.y_+36);
        glTexCoord2f(96.f/512.f, 96.f/512.f); glVertex2i(topLeft.x_+36*mirror, topLeft.y_+36);
        glTexCoord2f(96.f/512.f, 0);          glVertex2i(topLeft.x_+36*mirror, topLeft.y_-60);
        // topRight
        glTexCoord2f(96.f/512.f, 0);          glVertex2i(topLeft.x_-36*mirror+width, topLeft.y_-60);
        glTexCoord2f(96.f/512.f, 96.f/512.f); glVertex2i(topLeft.x_-36*mirror+width, topLeft.y_+36);
        glTexCoord2f(96.f/256.f, 96.f/512.f); glVertex2i(topLeft.x_+60*mirror+width, topLeft.y_+36);
        glTexCoord2f(96.f/256.f, 0);          glVertex2i(topLeft.x_+60*mirror+width, topLeft.y_-60);
        // bottomleft
        glTexCoord2f(0, 96.f/512.f);          glVertex2i(topLeft.x_-60*mirror, topLeft.y_-36+height);
        glTexCoord2f(0, 96.f/256.f);          glVertex2i(topLeft.x_-60*mirror, topLeft.y_+60+height);
        glTexCoord2f(96.f/512.f, 96.f/256.f); glVertex2i(topLeft.x_+36*mirror, topLeft.y_+60+height);
        glTexCoord2f(96.f/512.f, 96.f/512.f); glVertex2i(topLeft.x_+36*mirror, topLeft.y_-36+height);
        // bottomRight
        glTexCoord2f(96.f/512.f, 96.f/512.f); glVertex2i(topLeft.x_-36*mirror+width, topLeft.y_-36+height);
        glTexCoord2f(96.f/512.f, 96.f/256.f); glVertex2i(topLeft.x_-36*mirror+width, topLeft.y_+60+height);
        glTexCoord2f(96.f/256.f, 96.f/256.f); glVertex2i(topLeft.x_+60*mirror+width, topLeft.y_+60+height);
        glTexCoord2f(96.f/256.f, 96.f/512.f); glVertex2i(topLeft.x_+60*mirror+width, topLeft.y_-36+height);

        // top
        glTexCoord2f(193.f/512.f, 0);          glVertex2i(topLeft.x_+36*mirror, topLeft.y_-60);
        glTexCoord2f(193.f/512.f, 96.f/512.f); glVertex2i(topLeft.x_+36*mirror, topLeft.y_+36);
        glTexCoord2f(199.f/512.f, 96.f/512.f); glVertex2i(topLeft.x_-36*mirror+width, topLeft.y_+36);
        glTexCoord2f(199.f/512.f, 0);          glVertex2i(topLeft.x_-36*mirror+width, topLeft.y_-60);
        // right
        glTexCoord2f(96.f/512.f, 193.f/512.f); glVertex2i(topLeft.x_-36*mirror+width, topLeft.y_+36);
        glTexCoord2f(96.f/512.f, 199.f/512.f); glVertex2i(topLeft.x_-36*mirror+width, topLeft.y_-36+height);
        glTexCoord2f(96.f/256.f, 199.f/512.f); glVertex2i(topLeft.x_+60*mirror+width, topLeft.y_-36+height);
        glTexCoord2f(96.f/256.f, 193.f/512.f); glVertex2i(topLeft.x_+60*mirror+width, topLeft.y_+36);
        // left
        glTexCoord2f(0, 193.f/512.f);          glVertex2i(topLeft.x_-60*mirror, topLeft.y_+36);
        glTexCoord2f(0, 199.f/512.f);          glVertex2i(topLeft.x_-60*mirror, topLeft.y_-36+height);
        glTexCoord2f(96.f/512.f, 199.f/512.f); glVertex2i(topLeft.x_+36*mirror, topLeft.y_-36+height);
        glTexCoord2f(96.f/512.f, 193.f/512.f); glVertex2i(topLeft.x_+36*mirror, topLeft.y_+36);
        // bottom
        glTexCoord2f(193.f/512.f, 96.f/512.f); glVertex2i(topLeft.x_+36*mirror, topLeft.y_-36+height);
        glTexCoord2f(193.f/512.f, 96.f/256.f); glVertex2i(topLeft.x_+36*mirror, topLeft.y_+60+height);
        glTexCoord2f(199.f/512.f, 96.f/256.f); glVertex2i(topLeft.x_-36*mirror+width, topLeft.y_+60+height);
        glTexCoord2f(199.f/512.f, 96.f/512.f); glVertex2i(topLeft.x_-36*mirror+width, topLeft.y_-36+height);
        glEnd();


        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);

        text::drawFooText();
        text::drawScreenText(*locales::getLocale(locales::Statistics), topLeft + Vector2f(10*mirror, 10), 20.f, TEXT_ALIGN_LEFT, Color3f(1.f, 0.5f, 0.8f));
        std::stringstream sstr;
        int seconds = games::elapsedTime();
        sstr << std::setfill('0') << std::setw(2) << (seconds-seconds%60)/60 << " : " << std::setw(2) << seconds%60;
        text::drawScreenText(sf::String(sstr.str()), topLeft + Vector2f(width-10*mirror, 18), 12.f, TEXT_ALIGN_RIGHT, Color3f(1.f, 0.5f, 0.8f));

        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glLineWidth(2.f);

        glBegin(GL_LINES);
            glColor4f(1.f, 0.5f, 0.8f, 1.0f);
            glVertex2f(topLeft.x_+10*mirror, topLeft.y_+35);
            glColor4f(1.f, 0.5f, 0.8f, 0.0f);
            glVertex2f(topLeft.x_+width-10*mirror, topLeft.y_+35);
        glEnd();

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        width -= 20*mirror;
        height = 12;
        topLeft += Vector2f(10.f*mirror, 60.f);

        // Index of the column we are writing
        int col = 0;

        text::drawScreenText(*locales::getLocale(locales::Points), topLeft+Vector2f((170+70*col++)*mirror,0), 12.f, TEXT_ALIGN_CENTER, Color3f(0.7f, 0.7f, 0.7f));
        text::drawScreenText(*locales::getLocale(locales::Frags), topLeft + Vector2f((170+70*col++)*mirror, 0), 12.f, TEXT_ALIGN_CENTER, Color3f(0.7f, 0.7f, 0.7f));
        if (games::type() == games::gCannonKeep)
            text::drawScreenText(*locales::getLocale(locales::CannonShots), topLeft + Vector2f((170+70*col++)*mirror, 0), 12.f, TEXT_ALIGN_CENTER, Color3f(0.7f, 0.7f, 0.7f));
        text::drawScreenText(*locales::getLocale(locales::TeamKills), topLeft + Vector2f((170+70*col++)*mirror, 0), 12.f, TEXT_ALIGN_CENTER, Color3f(0.7f, 0.7f, 0.7f));
        text::drawScreenText(*locales::getLocale(locales::Suicides), topLeft + Vector2f((170+70*col++)*mirror, 0), 12.f, TEXT_ALIGN_CENTER, Color3f(0.7f, 0.7f, 0.7f));
        text::drawScreenText(*locales::getLocale(locales::Deaths), topLeft + Vector2f((170+70*col++)*mirror, 0), 12.f, TEXT_ALIGN_CENTER, Color3f(0.7f, 0.7f, 0.7f));

        topLeft.y_ += 15;

        for (std::multimap<Team*, std::multiset<Player*, playerPtrCmp>, teamPtrCmp >::const_iterator it = teamMap_.begin(); it != teamMap_.end(); ++it) {
            int totalPoints(0), totalFrags(0), totalCannonShots(0), totalSuicides(0), totalTeamKills(0), totalDeaths(0);
            Color3f teamColor = it->first->color();
            teamColor.v(1.f);
            teamColor.s(0.5f);
            std::multiset<Player*, playerPtrCmp>const& members = it->second;
            for (std::multiset<Player*, playerPtrCmp>::iterator currentPlayer = members.begin(); currentPlayer != members.end(); ++currentPlayer) {
                if ((*currentPlayer)->controlType_ == controllers::cPlayer1 || (*currentPlayer)->controlType_ == controllers::cPlayer2)
                    teamColor.gl4f(0.4 + std::sin(timer::totalTime()*150.f)*0.1f);
                else
                    teamColor.gl4f(0.2);

                glBegin(GL_QUADS);
                    glVertex2f(topLeft.x_,topLeft.y_+1);
                    glVertex2f(topLeft.x_+width,topLeft.y_+1);
                    glVertex2f(topLeft.x_+width,topLeft.y_+height+1);
                    glVertex2f(topLeft.x_,topLeft.y_+height+1);
                glEnd();

                Color3f drawColor((*currentPlayer)->color());
                drawColor.v(1.f);
                drawColor.s(0.8f);
                // draw name, shadowed
                text::drawScreenText((*currentPlayer)->name(), topLeft + Vector2f(3*mirror, 1), 12.f, TEXT_ALIGN_LEFT, Color3f(0.f, 0.f, 0.f));
                text::drawScreenText((*currentPlayer)->name(), topLeft + Vector2f(2*mirror, 0), 12.f, TEXT_ALIGN_LEFT, drawColor);
                // draw [BOT]
                if ((*currentPlayer)->controlType_ != controllers::cPlayer1 && (*currentPlayer)->controlType_ != controllers::cPlayer2) {
                    text::drawScreenText(sf::String("[BOT]"), topLeft+Vector2f(81*mirror,1), 12.f, TEXT_ALIGN_LEFT, Color3f(0.f, 0.f, 0.f));
                    text::drawScreenText(sf::String("[BOT]"), topLeft+Vector2f(80*mirror,0), 12.f, TEXT_ALIGN_LEFT, drawColor);
                }
                col = 0;
                // draw points
                int value = (*currentPlayer)->points_;
                if (value > 0)      drawColor = Color3f(0.3,1,0.3);
                else if (value < 0) drawColor = Color3f(1,0.3,0.3);
                else                drawColor = Color3f(1,1,0.3);
                writeScoreAtCol(value, col++, topLeft, mirror, drawColor);
                totalPoints += value;
                // draw frags
                value = (*currentPlayer)->frags_;
                if (value > 0)      drawColor = Color3f(0.3,1,0.3);
                else                drawColor = Color3f(1,1,0.3);
                writeScoreAtCol(value, col++, topLeft, mirror, drawColor);
                totalFrags += value;
                // draw cannonShots
                if (games::type() == games::gCannonKeep) {
                    value = (*currentPlayer)->cannonShots_;
                    if (value > 0)      drawColor = Color3f(0.3,1,0.3);
                    else                drawColor = Color3f(1,1,0.3);
                    writeScoreAtCol(value, col++, topLeft, mirror, drawColor);
                    totalCannonShots += value;
                }
                // draw teamKills
                value = (*currentPlayer)->teamKills_;
                if (value > 0)      drawColor = Color3f(1,0.3,0.3);
                else                drawColor = Color3f(0.3,1,0.3);
                writeScoreAtCol(value, col++, topLeft, mirror, drawColor);
                totalTeamKills += value;
                // draw suicides
                value = (*currentPlayer)->suicides_;
                if (value > 0)      drawColor = Color3f(1,0.3,0.3);
                else                drawColor = Color3f(0.3,1,0.3);
                writeScoreAtCol(value, col++, topLeft, mirror, drawColor);
                totalSuicides += value;
                // draw deaths
                value = (*currentPlayer)->deaths_;
                if (value > 0)      drawColor = Color3f(1,0.3,0.3);
                else                drawColor = Color3f(0.3,1,0.3);
                writeScoreAtCol(value, col++, topLeft, mirror, drawColor);
                totalDeaths += value;

                topLeft.y_ += 12;
            }
            if (games::type() != games::gDeathMatch) {
                topLeft.y_ += 2;
                col = 0;

                teamColor.gl4f(0.2f);
                glBegin(GL_QUADS);
                    glVertex2f(topLeft.x_,topLeft.y_+1);
                    glVertex2f(topLeft.x_+width,topLeft.y_+1);
                    glVertex2f(topLeft.x_+width,topLeft.y_+height+1);
                    glVertex2f(topLeft.x_,topLeft.y_+height+1);
                glEnd();

                Color3f drawColor(teamColor);
                text::drawScreenText(*locales::getLocale(locales::Total), topLeft + Vector2f(2*mirror, 0), 12.f, TEXT_ALIGN_LEFT, drawColor);

                if (totalPoints > 0)      drawColor = Color3f(0.3,1,0.3);
                else if (totalPoints < 0) drawColor = Color3f(1,0.3,0.3);
                else                              drawColor = Color3f(1,1,0.3);
                writeScoreAtCol(totalPoints, col++, topLeft, mirror, drawColor);

                if (totalFrags > 0)     drawColor = Color3f(0.3,1,0.3);
                else                    drawColor = Color3f(1,1,0.3);
                writeScoreAtCol(totalFrags, col++, topLeft, mirror, drawColor);

                if (games::type() == games::gCannonKeep) {
                    if (totalCannonShots > 0)     drawColor = Color3f(0.3,1,0.3);
                    else                          drawColor = Color3f(1,1,0.3);
                    writeScoreAtCol(totalCannonShots, col++, topLeft, mirror, drawColor);
                }

                if (totalTeamKills > 0) drawColor = Color3f(1,0.3,0.3);
                else                    drawColor = Color3f(0.3,1,0.3);
                writeScoreAtCol(totalTeamKills, col++, topLeft, mirror, drawColor);

                if (totalSuicides > 0)  drawColor = Color3f(1,0.3,0.3);
                else                    drawColor = Color3f(0.3,1,0.3);
                writeScoreAtCol(totalSuicides, col++, topLeft, mirror, drawColor);

                if (totalDeaths > 0)  drawColor = Color3f(1,0.3,0.3);
                else                    drawColor = Color3f(0.3,1,0.3);
                writeScoreAtCol(totalDeaths, col++, topLeft, mirror, drawColor);

                topLeft.y_ += 18;
            }
            else
                topLeft.y_ += 2;
        }
        text::drawFooText();
    }
}

void TabStats::display(bool show) {
    visible_ = show;
}

void TabStats::refresh() {
    refresh_ = true;
}

bool TabStats::visible() const {
    return visible_;
}
