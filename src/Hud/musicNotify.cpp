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
/*
# include <taglib/taglib.h>
# include <taglib/fileref.h>
# include <taglib/tag.h>
*/

namespace musicNotify {
    namespace {
        float timer_(0.f);

        sf::String title_;
        sf::String artist_;
        sf::String album_;

        const sf::String by_("Artist: ");
        const sf::String from_("Album: ");
    }

    void draw() {
        if (timer_ > 0.f) {

            float alpha(1.f);
            if (timer_ < 1.f)
                alpha = timer_;
            else if (timer_ < 0.f)
                alpha = 0.f;

            const float artistLength(text::getCharacterPos(artist_, artist_.GetSize(), 12.f, TEXT_ALIGN_LEFT));
            const float albumLength(text::getCharacterPos(album_, album_.GetSize(), 12.f, TEXT_ALIGN_LEFT));
            const float byLength(text::getCharacterPos(by_, by_.GetSize(), 12.f, TEXT_ALIGN_LEFT));
            const float fromLength(text::getCharacterPos(from_, from_.GetSize(), 12.f, TEXT_ALIGN_LEFT));
            const float spaceLength(text::getCharacterPos(" ", 1, 12.f, TEXT_ALIGN_LEFT));


            float byLineLenght(0.f);
            if (artist_ != "" && album_ != "") byLineLenght = artistLength + albumLength + byLength + fromLength + spaceLength;
            else if (artist_ != "")            byLineLenght = artistLength + byLength;
            else if (album_ != "")             byLineLenght = albumLength  + fromLength ;

            const float titleLenght(text::getCharacterPos(title_, title_.GetSize(), 20.f, TEXT_ALIGN_LEFT));

            int   height((byLineLenght == 0.f) ? 38 : 50);
            int   width(std::max(titleLenght, byLineLenght) + 10);

            int mirror(locales::getCurrentLocale().LTR_ ? 1 : -1);

            width *= -mirror;

            Vector2f position(window::getViewPort().x_*0.5f + mirror*(window::getViewPort().x_*0.5f -20), 20);

            // draw background
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glColor4f(0.0,0.0,0.0, alpha/1.25f);
            glBegin(GL_QUADS);
                glVertex2f(position.x_,position.y_+height);
                glVertex2f(position.x_,position.y_);
                glVertex2f(position.x_+width,position.y_);
                glVertex2f(position.x_+width,position.y_+height);
            glEnd();

            // draw border
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glLineWidth(1.f);

            glColor4f(1.0,0.4,0.8,alpha);
            glBegin(GL_LINE_LOOP);
                glVertex2f(position.x_,position.y_+height);
                glVertex2f(position.x_,position.y_);
                glVertex2f(position.x_+width,position.y_);
                glVertex2f(position.x_+width,position.y_+height);
            glEnd();

            // draw text
            text::drawScreenText(title_, position + Vector2f(-5*mirror, 5), 20.f, TEXT_ALIGN_RIGHT, Color3f(1.f, 0.5f, 0.9f)*alpha);

            if (artist_ != "" && album_ != "") {
                text::drawScreenText(by_, position + Vector2f(mirror*( -artistLength - spaceLength - albumLength - fromLength - 5), 28), 12.f, TEXT_ALIGN_RIGHT, Color3f(0.7f, 0.7f, 0.7f)*alpha);
                text::drawScreenText(artist_, position + Vector2f(mirror*( -spaceLength - albumLength - fromLength -5), 28), 12.f, TEXT_ALIGN_RIGHT, Color3f(1.f, 0.5f, 0.9f)*alpha);
                text::drawScreenText(" ", position + Vector2f(mirror*( -albumLength - fromLength -5), 28), 12.f, TEXT_ALIGN_RIGHT, Color3f(0.7f, 0.7f, 0.7f)*alpha);
                text::drawScreenText(from_, position + Vector2f(mirror*( -albumLength -5), 28), 12.f, TEXT_ALIGN_RIGHT, Color3f(0.7f, 0.7f, 0.7f)*alpha);
                text::drawScreenText(album_, position + Vector2f(mirror*(-5), 28), 12.f, TEXT_ALIGN_RIGHT, Color3f(1.f, 0.5f, 0.9f)*alpha);
            }
            else if (artist_ != "") {
                text::drawScreenText(by_, position + Vector2f(mirror*( -artistLength - 5), 28), 12.f, TEXT_ALIGN_RIGHT, Color3f(0.7f, 0.7f, 0.7f)*alpha);
                text::drawScreenText(artist_, position + Vector2f(mirror*(-5), 28), 12.f, TEXT_ALIGN_RIGHT, Color3f(1.f, 0.5f, 0.9f)*alpha);
            }
            else if (album_ != "") {
                text::drawScreenText(from_, position + Vector2f(mirror*( -albumLength -5), 28), 12.f, TEXT_ALIGN_RIGHT, Color3f(0.7f, 0.7f, 0.7f)*alpha);
                text::drawScreenText(album_, position + Vector2f(mirror*(-5), 28), 12.f, TEXT_ALIGN_RIGHT, Color3f(1.f, 0.5f, 0.9f)*alpha);
            }
        }
    }

    void update() {
         if (timer_ > 0.f)
            timer_ -= timer::frameTime();
    }

    void show(std::string const& fileName) {
       /* TagLib::FileRef ref(fileName.c_str());
        TagLib::Tag *tag(ref.tag());


        sf::String title(tag->title().toWString());

        if (title != "") {
            title_ = title;
            artist_ = tag->artist().toWString();
            album_  = tag->album().toWString();
            timer_ = 4.f;
        }*/
    }
}


