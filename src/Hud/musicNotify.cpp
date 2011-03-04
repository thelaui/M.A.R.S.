/* musicNotify.cpp

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

# include "Hud/musicNotify.hpp"

# include "System/window.hpp"
# include "System/timer.hpp"
# include "Media/text.hpp"
# include "Locales/locales.hpp"

# include <SFML/OpenGL.hpp>

# include <taglib/taglib.h>
# include <taglib/fileref.h>
# include <taglib/tag.h>

namespace musicNotify {
    namespace {
        float timer_(0.f);

        sf::String title_;
        sf::String artist_;
        sf::String album_;

        sf::String* by_;
        sf::String* from_;

    }

    void draw() {
        if (timer_ > 0.f) {

            float alpha(1.f);
            if (timer_ > 4.f)
                alpha = 5 - timer_;
            else if (timer_ < 1.f)
                alpha = timer_;
            else if (timer_ < 0.f)
                alpha = 0.f;

            const float artistLength(text::getCharacterPos(artist_, artist_.GetSize(), 12.f, TEXT_ALIGN_LEFT));
            const float albumLength(text::getCharacterPos(album_, album_.GetSize(), 12.f, TEXT_ALIGN_LEFT));
            const float fromLength(text::getCharacterPos(*from_, from_->GetSize(), 12.f, TEXT_ALIGN_LEFT));
            const float byLength(text::getCharacterPos(*by_, by_->GetSize(), 12.f, TEXT_ALIGN_LEFT));
            const float spaceLength(text::getCharacterPos(" ", 1, 12.f, TEXT_ALIGN_LEFT));



            float byLineLenght(0.f);
            if (artist_ != "" && album_ != "") byLineLenght = artistLength + albumLength + byLength + fromLength + spaceLength;
            else if (artist_ != "")            byLineLenght = artistLength + byLength;
            else if (album_ != "")             byLineLenght = albumLength  + fromLength ;

            const float titleLenght(text::getCharacterPos(title_, title_.GetSize(), 20.f, TEXT_ALIGN_LEFT));

            const float cornerRadius(8.f);



            int   height((byLineLenght == 0.f) ? 38 : 50);
            int   width(std::max(titleLenght, byLineLenght) + 10);

            int mirror(locales::getCurrentLocale().LTR_ ? 1 : -1);

            width *= -mirror;

            Vector2f position(window::getViewPort().x_*0.5f + mirror*(window::getViewPort().x_*0.5f -20), 20);

            // draw background
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glColor4f(0.0,0.0,0.0, alpha/1.25f);
            glBegin(GL_QUADS);
                glVertex2f(position.x_,position.y_+height+cornerRadius);
                glVertex2f(position.x_,position.y_-cornerRadius);
                glVertex2f(position.x_+width,position.y_-cornerRadius);
                glVertex2f(position.x_+width,position.y_+height+cornerRadius);

                glVertex2f(position.x_+width,position.y_);
                glVertex2f(position.x_+width-cornerRadius*mirror,position.y_);
                glVertex2f(position.x_+width-cornerRadius*mirror,position.y_+height);
                glVertex2f(position.x_+width,position.y_+height);

                glVertex2f(position.x_,position.y_);
                glVertex2f(position.x_+cornerRadius*mirror,position.y_);
                glVertex2f(position.x_+cornerRadius*mirror,position.y_+height);
                glVertex2f(position.x_,position.y_+height);
            glEnd();

            glBegin(GL_TRIANGLE_FAN);
                glVertex2f(position.x_, position.y_);

                for (int i=0; i<=360; i+=30) {
                    Vector2f cornerPosition;
                    if (i == 90) {
                        glVertex2f(position.x_, position.y_-cornerRadius);
                        glEnd();
                        glBegin(GL_TRIANGLE_FAN);
                        glVertex2f(position.x_+width, position.y_);
                    }
                    else if (i == 180) {
                        glVertex2f(position.x_+width-cornerRadius*mirror, position.y_);
                        glEnd();
                        glBegin(GL_TRIANGLE_FAN);
                        glVertex2f(position.x_+width, position.y_+height);
                    }
                    else if (i == 270) {
                        glVertex2f(position.x_+width, position.y_+height+cornerRadius);
                        glEnd();
                        glBegin(GL_TRIANGLE_FAN);
                        glVertex2f(position.x_, position.y_+height);
                    }
                    else if (i == 360) {
                        glVertex2f(position.x_+cornerRadius*mirror, position.y_+height);
                    }


                    if (i < 90)         cornerPosition = position;
                    else if (i < 180)   cornerPosition = position + Vector2f(width, 0.f);
                    else if (i < 270)   cornerPosition = position + Vector2f(width, height);
                    else                cornerPosition = position + Vector2f(0.f, height);

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
                    if (i == 0)         glVertex2f(position.x_+cornerRadius*mirror, position.y_+height);
                    else if (i == 90)   glVertex2f(position.x_, position.y_-cornerRadius);
                    else if (i == 180)  glVertex2f(position.x_+width-cornerRadius*mirror, position.y_);
                    else if (i == 270)  glVertex2f(position.x_+width, position.y_+height+cornerRadius);


                    if (i < 90)         cornerPosition = position;
                    else if (i < 180)   cornerPosition = position + Vector2f(width, 0.f);
                    else if (i < 270)   cornerPosition = position + Vector2f(width, height);
                    else                cornerPosition = position + Vector2f(0.f, height);

                    float rad = i*M_PI/180;
                    glVertex2f(cornerPosition.x_ + std::cos(rad)*cornerRadius*mirror, cornerPosition.y_-std::sin(rad)*cornerRadius);
                }
            glEnd();

            // draw text
            text::drawScreenText(title_, position + Vector2f(-5*mirror, 5), 20.f, TEXT_ALIGN_RIGHT, Color3f(1.f, 0.5f, 0.9f)*alpha);

            if (artist_ != "" && album_ != "") {
                text::drawScreenText(*by_, position + Vector2f(mirror*( -artistLength - spaceLength - albumLength - fromLength - 5), 28), 12.f, TEXT_ALIGN_RIGHT, Color3f(0.7f, 0.7f, 0.7f)*alpha);
                text::drawScreenText(artist_, position + Vector2f(mirror*( -spaceLength - albumLength - fromLength -5), 28), 12.f, TEXT_ALIGN_RIGHT, Color3f(1.f, 0.5f, 0.9f)*alpha);
                text::drawScreenText(" ", position + Vector2f(mirror*( -albumLength - fromLength -5), 28), 12.f, TEXT_ALIGN_RIGHT, Color3f(0.7f, 0.7f, 0.7f)*alpha);
                text::drawScreenText(*from_, position + Vector2f(mirror*( -albumLength -5), 28), 12.f, TEXT_ALIGN_RIGHT, Color3f(0.7f, 0.7f, 0.7f)*alpha);
                text::drawScreenText(album_, position + Vector2f(mirror*(-5), 28), 12.f, TEXT_ALIGN_RIGHT, Color3f(1.f, 0.5f, 0.9f)*alpha);
            }
            else if (artist_ != "") {
                text::drawScreenText(*by_, position + Vector2f(mirror*( -artistLength - 5), 28), 12.f, TEXT_ALIGN_RIGHT, Color3f(0.7f, 0.7f, 0.7f)*alpha);
                text::drawScreenText(artist_, position + Vector2f(mirror*(-5), 28), 12.f, TEXT_ALIGN_RIGHT, Color3f(1.f, 0.5f, 0.9f)*alpha);
            }
            else if (album_ != "") {
                text::drawScreenText(*from_, position + Vector2f(mirror*( -albumLength -5), 28), 12.f, TEXT_ALIGN_RIGHT, Color3f(0.7f, 0.7f, 0.7f)*alpha);
                text::drawScreenText(album_, position + Vector2f(mirror*(-5), 28), 12.f, TEXT_ALIGN_RIGHT, Color3f(1.f, 0.5f, 0.9f)*alpha);
            }
        }
    }

    void update() {
         if (timer_ > 0.f)
            timer_ -= timer::realFrameTime();
    }

    void show(std::string const& fileName) {
        by_ = locales::getLocale(locales::Artist);
        from_ = locales::getLocale(locales::Album);

        TagLib::FileRef ref((settings::C_dataPath + "/audio/music/" + fileName).c_str());

        TagLib::Tag *tag(ref.tag());

        sf::String title(tag->title().toWString());

        if (title != "") title_ = title;
        else             title_ = fileName;
        artist_ = tag->artist().toWString();
        album_  = tag->album().toWString();

        if (timer_ <= 0)
            timer_ = 5.f;
        else if (timer_ < 4)
            timer_ = 4.f;
    }
}


