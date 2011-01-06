/* TextEdit.cpp

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

# include "Interface/TextEdit.hpp"

# include "System/settings.hpp"
# include "System/window.hpp"
# include "Media/text.hpp"
# include "Locales/locales.hpp"

# include <SFML/OpenGL.hpp>
# include <iostream>

TextEdit::TextEdit (sf::String* text, sf::String* value, Vector2f const& topLeft, int width, int type, int maxLength):
    UiElement(topLeft, width, 20),
    value_(value),
    maxLength_(maxLength),
    cursorPos_(0),
    cursorTimer_(0),
    type_(type) {

    if (type == 1) maxLength_ = 5;
    else if (type == 2) maxLength_ = 15;

    label_ = new Label(text, TEXT_ALIGN_LEFT, Vector2f(0,0));
    label_->setParent(this);
}

TextEdit::~TextEdit () {
    delete label_;
}

void TextEdit::mouseMoved(Vector2f const& position) {
    UiElement::mouseMoved(position);
    label_->mouseMoved(position);
}

void TextEdit::keyEvent(bool down, sf::Key::Code keyCode) {
    if (hovered_ && down) {
        // backspace
        if (keyCode == sf::Key::Back && cursorPos_ > 0) {
            value_->Erase(cursorPos_-1, 1);
            --cursorPos_;
            cursorTimer_ = 0;
        }
        // delete
        else if (keyCode == sf::Key::Delete && cursorPos_ < value_->GetSize()) {
            value_->Erase(cursorPos_, 1);
        }
        // move cursor
        else if (keyCode == sf::Key::Left && cursorPos_ > 0) {
            --cursorPos_;
            cursorTimer_ = 0;
        }
        else if (keyCode == sf::Key::Right && cursorPos_ < value_->GetSize()) {
            ++cursorPos_;
            cursorTimer_ = 0;
        }
    }
}

void TextEdit::textEntered(int keyCode) {
    if (hovered_) {
        if (type_ == TEXT_EDIT) {
            if (keyCode > 32 && keyCode < 126 && value_->GetSize() < maxLength_) {
                value_->Insert(cursorPos_, static_cast<char>(keyCode));
                ++cursorPos_;
                cursorTimer_ = 0;
            }
        }
        else if (type_ == IP_EDIT) {
             if (((keyCode > 47 && keyCode < 58) || keyCode == 46) && value_->GetSize() < maxLength_) {
                value_->Insert(cursorPos_, static_cast<char>(keyCode));
                ++cursorPos_;
                cursorTimer_ = 0;
            }
        }
        else if (type_ == PORT_EDIT) {
             if ((keyCode > 47 && keyCode < 58) && value_->GetSize() < maxLength_) {
                value_->Insert(cursorPos_, static_cast<char>(keyCode));
                ++cursorPos_;
                cursorTimer_ = 0;
            }
        }
    }
}

void TextEdit::draw() const {
    if (++cursorTimer_ > 50) cursorTimer_ = 0;

    Vector2f origin = parent_->getTopLeft() + topLeft_;


    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Hover effect
    if (hovered_)
        glColor4f(1,0.7,0.9,0.4);
    else
        glColor4f(1,1,1,0.1);

    glBegin(GL_QUADS);
        glVertex2f(origin.x_+185, origin.y_+2);
        glVertex2f(width_ + origin.x_, origin.y_+2);
        glColor4f(1,1,1,0.2);
        glVertex2f(width_ + origin.x_, height_ + origin.y_-2);
        glVertex2f(origin.x_+185, height_ + origin.y_-2);
    glEnd();

    glColor4f(1,1,1,0.7);

    if (pressed_ && hovered_)
        text::drawScreenText(*locales::getLocale(locales::TextEditHover), origin + Vector2f((width_+185)/2,1), font::Ubuntu, 12.f, TEXT_ALIGN_CENTER, Color3f(0.7, 0.7, 0.7));
    else
        text::drawScreenText(*value_, origin + Vector2f((width_+185)/2,1), font::Ubuntu, 12.f, TEXT_ALIGN_CENTER, Color3f(0.7, 0.7, 0.7));

    // draw cursor
    if (hovered_ && cursorTimer_ < 30) {
        int pos = text::getCharacterPos(*value_, cursorPos_, font::Ubuntu, 12.f, TEXT_ALIGN_CENTER);
        glColor4f(1,0.8,0.9,0.5);
        glLineWidth(0.5f);
        glBegin(GL_LINES);
            glVertex2f(origin.x_ + pos +(width_+185)/2, origin.y_+3);
            glVertex2f(origin.x_ + pos +(width_+185)/2, origin.y_+height_-3);
        glEnd();
    }



    // draw Label
    label_->draw();
}

void TextEdit::setFocus (UiElement* toBeFocused) {
    UiElement::setFocus(this);
    label_->setFocus(this);
}

void TextEdit::clearFocus() {
    UiElement::clearFocus();
    label_->clearFocus();
}
