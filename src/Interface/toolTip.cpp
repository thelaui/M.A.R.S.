/* toolTip.cpp

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

# include "Interface/toolTip.hpp"

# include "Media/text.hpp"
# include "System/timer.hpp"
# include "System/window.hpp"
# include "Locales/locales.hpp"

# include <vector>
# include <SFML/OpenGL.hpp>

# define WAITING   0
# define FADE_IN   1
# define VISIBLE   2
# define FADE_OUT  3
# define INVISIBLE 4

namespace toolTip {

    namespace {
        sf::String*             currentLocale_(NULL);
        std::vector<sf::String> lines_;
        int                     width_(300);
        float                   timer_(-5.f);
        Vector2f                position_;
        short                   state_(INVISIBLE);

        void create() {
            sf::String wholeText = *currentLocale_;
            sf::String word;
            sf::String line;
            int lastSpace(0);

            lines_.clear();

            if (wholeText.getSize() > 0) {

                // search for "\n" and replace them with '\n'
                for (unsigned int i=0; i<wholeText.getSize()-1; ++i) {
                    if (wholeText[i] == '\\' && wholeText[i+1] == 'n') {
                        wholeText[i]  = ' ';
                        wholeText[++i]= '\n';
                    }
                }

                // remove doubled spaces
                for (unsigned int i=0; i<wholeText.getSize()-1; ++i)
                    if (wholeText[i] == ' ' && wholeText[i+1] == ' ')
                        wholeText.erase(i--, 1);

                // break lines
                for (unsigned int i=0; i<wholeText.getSize(); ++i) {
                    if (wholeText[i] == '\n') {
                        line = "";
                        word = "";
                    }
                    else if (wholeText[i] != ' ') {
                        word += wholeText[i];
                        sf::String tmp(line + word);
                        if (text::getCharacterPos(tmp, tmp.getSize(), 12.f, TEXT_ALIGN_LEFT) > width_) {
                            if (lastSpace == 0) {
                                wholeText.insert(i-1, '\n');
                                line = "";
                                word = wholeText[i];
                                ++i;
                            }
                            else {
                                wholeText[lastSpace] = '\n';
                                line = word;
                                lastSpace = 0;
                            }
                        }
                    }
                    else {
                        lastSpace = i;
                        line += word + " ";
                        word = "";
                    }
                }

                // create single labels
                line = "";
                int top(0);
                for (unsigned int i=0; i<wholeText.getSize(); ++i) {
                    if (wholeText[i] == '\n') {
                        lines_.push_back(line);
                        line = "";
                    }
                    else {
                         line += wholeText[i];
                    }
                }
                if (line != "") {
                    lines_.push_back(line);
                }
            }
        }

        void drawToolTip(float alpha) {
            int   height(lines_.size()*15+10);
            int   width(10);

            for (std::vector<sf::String>::iterator it = lines_.begin(); it!=lines_.end(); ++it) {
                int tmp(text::getCharacterPos(*it, it->getSize(), 12.f, TEXT_ALIGN_LEFT) + 10);
                if (tmp > width)
                    width = tmp;
            }

            int mirror(locales::getCurrentLocale().LTR_ ? -1 : 1);

            const float cornerRadius(3.f);

            width *= -mirror;


            // draw background
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glColor4f(0.0,0.0,0.0, alpha/1.25f);
            glBegin(GL_QUADS);
                glVertex2f(position_.x_,position_.y_+height+cornerRadius);
                glVertex2f(position_.x_,position_.y_-cornerRadius);
                glVertex2f(position_.x_+width,position_.y_-cornerRadius);
                glVertex2f(position_.x_+width,position_.y_+height+cornerRadius);

                glVertex2f(position_.x_+width,position_.y_);
                glVertex2f(position_.x_+width-cornerRadius*mirror,position_.y_);
                glVertex2f(position_.x_+width-cornerRadius*mirror,position_.y_+height);
                glVertex2f(position_.x_+width,position_.y_+height);

                glVertex2f(position_.x_,position_.y_);
                glVertex2f(position_.x_+cornerRadius*mirror,position_.y_);
                glVertex2f(position_.x_+cornerRadius*mirror,position_.y_+height);
                glVertex2f(position_.x_,position_.y_+height);
            glEnd();

            glBegin(GL_TRIANGLE_FAN);
                glVertex2f(position_.x_, position_.y_);

                for (int i=0; i<=360; i+=30) {
                    Vector2f cornerPosition;
                    if (i == 90) {
                        glVertex2f(position_.x_, position_.y_-cornerRadius);
                        glEnd();
                        glBegin(GL_TRIANGLE_FAN);
                        glVertex2f(position_.x_+width, position_.y_);
                    }
                    else if (i == 180) {
                        glVertex2f(position_.x_+width-cornerRadius*mirror, position_.y_);
                        glEnd();
                        glBegin(GL_TRIANGLE_FAN);
                        glVertex2f(position_.x_+width, position_.y_+height);
                    }
                    else if (i == 270) {
                        glVertex2f(position_.x_+width, position_.y_+height+cornerRadius);
                        glEnd();
                        glBegin(GL_TRIANGLE_FAN);
                        glVertex2f(position_.x_, position_.y_+height);
                    }
                    else if (i == 360) {
                        glVertex2f(position_.x_+cornerRadius*mirror, position_.y_+height);
                    }


                    if (i < 90)         cornerPosition = position_;
                    else if (i < 180)   cornerPosition = position_ + Vector2f(width, 0.f);
                    else if (i < 270)   cornerPosition = position_ + Vector2f(width, height);
                    else                cornerPosition = position_ + Vector2f(0.f, height);

                    float rad = i*M_PI/180;
                    glVertex2f(cornerPosition.x_ + std::cos(rad)*cornerRadius*mirror, cornerPosition.y_-std::sin(rad)*cornerRadius);
                }
            glEnd();

            // draw border
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glLineWidth(1.f);

            glColor4f(1.0,0.4,0.8,alpha);
            glBegin(GL_LINE_LOOP);

                for (int i=0; i<360; i+=30) {
                    Vector2f cornerPosition;
                    if (i == 0)         glVertex2f(position_.x_+cornerRadius*mirror, position_.y_+height);
                    else if (i == 90)   glVertex2f(position_.x_, position_.y_-cornerRadius);
                    else if (i == 180)  glVertex2f(position_.x_+width-cornerRadius*mirror, position_.y_);
                    else if (i == 270)  glVertex2f(position_.x_+width, position_.y_+height+cornerRadius);


                    if (i < 90)         cornerPosition = position_;
                    else if (i < 180)   cornerPosition = position_ + Vector2f(width, 0.f);
                    else if (i < 270)   cornerPosition = position_ + Vector2f(width, height);
                    else                cornerPosition = position_ + Vector2f(0.f, height);

                    float rad = i*M_PI/180;
                    glVertex2f(cornerPosition.x_ + std::cos(rad)*cornerRadius*mirror, cornerPosition.y_-std::sin(rad)*cornerRadius);
                }
            glEnd();

            // draw text
            int top(5);
            for (std::vector<sf::String>::iterator it = lines_.begin(); it!=lines_.end(); ++it) {
                text::drawScreenText(*it, position_ + Vector2f(-5*mirror, top), 12.f, TEXT_ALIGN_LEFT, Color3f(0.7f, 0.7f, 0.7f)*alpha);
                top += 15;
            }
        }
    }

    void draw() {
        if (currentLocale_ && lines_.size() > 0) {
            float alpha(0.f);
            float time(timer::realFrameTime()*1.3f);

            switch (state_) {
                case WAITING:
                    timer_ += time;
                    if (timer_ > 1.f) {
                        timer_ = 0.f;
                        state_ = FADE_IN;
                    }
                    break;
                case FADE_IN:
                    timer_ += time*5.f;
                    alpha = timer_;
                    if (timer_ > 1.f) {
                        state_ = VISIBLE;
                    }
                    break;
                case VISIBLE:
                    alpha = 1.f;
                    break;
                case FADE_OUT:
                    timer_ -= time*5;
                    alpha = timer_;
                    if (timer_ < 0.f) {
                        state_ = INVISIBLE;
                    }
                    break;
                default:;
            }

            if (settings::C_showToolTips && alpha > 0)
                drawToolTip(alpha);
        }
    }

    void mouseMoved(Vector2f const& position) {
        if (state_ == INVISIBLE || state_ == WAITING) {
            position_ = position + Vector2f(0, 25);
            state_ = INVISIBLE;
        }
        else if ((position - position_ - Vector2f(0, 25)).lengthSquare() > 60) {
            if (state_ != FADE_OUT) {
                state_ = FADE_OUT;
                timer_ = 1.f;
            }
            if (state_ == WAITING)
                state_ = INVISIBLE;
        }
    }

    void show(sf::String* text) {
        if (text != currentLocale_ && state_ != FADE_OUT) {
            currentLocale_ = text;
            if (currentLocale_) {
                create();
                if (state_ == FADE_OUT) {
                    state_ = VISIBLE;
                }
            }
        }
        if (currentLocale_) {
            if (state_ == INVISIBLE) {
                state_ = WAITING;
                timer_ = 0.f;
            }
        }
        else {
            state_ = INVISIBLE;
        }
    }

}


