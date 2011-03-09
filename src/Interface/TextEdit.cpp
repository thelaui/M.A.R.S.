/* TextEdit.cpp

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

# include "Interface/TextEdit.hpp"

# include "System/settings.hpp"
# include "System/window.hpp"
# include "Media/text.hpp"
# include "Locales/locales.hpp"
# include "Menu/menus.hpp"
# include "Media/sound.hpp"

# include <SFML/OpenGL.hpp>
# include <iostream>

TextEdit::TextEdit (sf::String* text, sf::String* value, sf::String fallBack, Vector2f const& topLeft, int width, int labelWidth, int type, int maxLength):
    UiElement(topLeft, width, 20),
    value_(value),
    fallBack_(fallBack),
    label_(NULL),
    maxLength_(maxLength),
    cursorPos_(value->GetSize()),
    cursorTimer_(0),
    type_(type),
    labelWidth_(labelWidth) {

    if (type == 1) maxLength_ = 5;
    else if (type == 2) maxLength_ = 15;

    if (text) {
        label_ = new Label(text, TEXT_ALIGN_LEFT, Vector2f(0,0));
        label_->setParent(this);
    }
}

TextEdit::~TextEdit () {
    if (label_)
        delete label_;
}

void TextEdit::mouseMoved(Vector2f const& position) {
    UiElement::mouseMoved(position);
    if (label_)
        label_->mouseMoved(position);
    if (pressed_ && window::getInput().IsMouseButtonDown(sf::Mouse::Left)) {
        cursorPos_ = 0;
        cursorTimer_ = 0;
        int mirror(locales::getCurrentLocale().LTR_ ? 1 : -1);
        while (text::getCharacterPos(*value_, cursorPos_, 12.f, TEXT_ALIGN_CENTER) + 2*mirror + getTopLeft().x_ +(width()+labelWidth_*mirror)/2 < window::getInput().GetMouseX() - 3 && cursorPos_ < value_->GetSize())
            ++ cursorPos_;
    }
}

void TextEdit::mouseLeft(bool down) {
    if (down && hovered_) {
        menus::clearFocus();
        setFocus(this, false);
        pressed_ = true;
        menus::fixKeyboardOn(this);
        sound::playSound(sound::Click);
    }
    if (pressed_) {
        cursorPos_ = 0;
        cursorTimer_ = 0;
        int mirror(locales::getCurrentLocale().LTR_ ? 1 : -1);
        while (text::getCharacterPos(*value_, cursorPos_, 12.f, TEXT_ALIGN_CENTER) + 2*mirror + getTopLeft().x_ +(width()+labelWidth_*mirror)/2 < window::getInput().GetMouseX() - 3 && cursorPos_ < value_->GetSize())
            ++ cursorPos_;
    }
}

void TextEdit::keyEvent(bool down, Key const& key) {
    if (key.type_ == Key::kKeyBoard) {
        if (pressed_) {
            if (down) {
                // backspace
                if (key.code_.keyBoard_ == sf::Key::Back && cursorPos_ > 0) {
                    value_->Erase(cursorPos_-1, 1);
                    --cursorPos_;
                    cursorTimer_ = 0;
                }
                // delete
                else if (key.code_.keyBoard_ == sf::Key::Delete && cursorPos_ < value_->GetSize()) {
                    value_->Erase(cursorPos_, 1);
                }
                // move cursor
                else if (key.code_.keyBoard_ == sf::Key::Left && cursorPos_ > 0) {
                    --cursorPos_;
                    cursorTimer_ = 0;
                }
                else if (key.code_.keyBoard_ == sf::Key::Right && cursorPos_ < value_->GetSize()) {
                    ++cursorPos_;
                    cursorTimer_ = 0;
                }
                else if (key.navi_ == Key::nAbort || key.code_.keyBoard_ == sf::Key::Up || key.code_.keyBoard_ == sf::Key::Down || key.navi_ == Key::nConfirm) {
                    if (*value_ == "")
                        *value_ = fallBack_;
                    menus::unFixKeyboard();
                    pressed_ = false;
                }
            }
        }
        else if (down && (key.navi_ == Key::nConfirm)) {
            menus::fixKeyboardOn(this);
            pressed_ = true;
        }
    }
}

void TextEdit::textEntered(sf::Uint32 keyCode) {
    if (pressed_) {
        if (type_ == TEXT_EDIT) {
            if (value_->GetSize() < maxLength_ && keyCode != 8 && keyCode != 13 && keyCode != 32 && keyCode != 127) {
                value_->Insert(cursorPos_, keyCode);
                ++cursorPos_;
                cursorTimer_ = 0;
            }
        }
        else if (type_ == IP_EDIT) {
             if (((keyCode > 47 && keyCode < 58) || keyCode == 46) && value_->GetSize() < maxLength_) {
                value_->Insert(cursorPos_, keyCode);
                ++cursorPos_;
                cursorTimer_ = 0;
            }
        }
        else if (type_ == PORT_EDIT) {
             if (value_->GetSize() < maxLength_) {
                value_->Insert(cursorPos_, keyCode);
                ++cursorPos_;
                cursorTimer_ = 0;
            }
        }
    }
}

void TextEdit::draw() const {
    UiElement::draw();

    int mirror(locales::getCurrentLocale().LTR_ ? 1 : -1);

    if (++cursorTimer_ > 50) cursorTimer_ = 0;

    Vector2f origin = getTopLeft();


    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // draw Button
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
        if (isTopMost())   glColor4f(0.3*focusedFadeTime_,0.1*focusedFadeTime_,0.2*focusedFadeTime_,0.8);
        else               glColor4f(0.0,0.0,0.0,0.8);
        glVertex2f(origin.x_+labelWidth_*mirror, origin.y_+2);
        glVertex2f(width() + origin.x_, origin.y_+2);
        glVertex2f(width() + origin.x_, height_ + origin.y_-2);
        glVertex2f(origin.x_+labelWidth_*mirror, height_ + origin.y_-2);

        // glossy bottom
        glColor4f(1.0,1.0,1.0,0.0);
        glVertex2f(origin.x_+labelWidth_*mirror, origin.y_+2);
        glVertex2f(width() + origin.x_, origin.y_+2);
        if (pressed_)   glColor4f(1.0,1.0,1.0,0.1);
        else            glColor4f(1.0,1.0,1.0,0.06);
        glVertex2f(width() + origin.x_, height_ + origin.y_-2);
        glVertex2f(origin.x_+labelWidth_*mirror, height_ + origin.y_-2);

        if (!pressed_) {
            // glossy top
            glColor4f(1.0,1.0,1.0,0.2);
            glVertex2f(origin.x_+labelWidth_*mirror, origin.y_+2);
            glVertex2f(width() + origin.x_, origin.y_+2);
            glColor4f(1.0,1.0,1.0,0.05);
            glVertex2f(width() + origin.x_, height_*0.5f + origin.y_);
            glVertex2f(origin.x_+labelWidth_*mirror, height_*0.5f + origin.y_);
        }
    glEnd();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glLineWidth(1.f);

    glColor4f(1.0,0.4,0.8,0.3f+hoveredFadeTime_*0.7f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(origin.x_+labelWidth_*mirror, origin.y_+2);
        glVertex2f(width() + origin.x_, origin.y_+2);
        glVertex2f(width() + origin.x_, height_ + origin.y_-2);
        glVertex2f(origin.x_+labelWidth_*mirror, height_ + origin.y_-2);
    glEnd();

    float highlight(std::max(hoveredFadeTime_, focusedFadeTime_));
    Color3f color(Color3f(0.7f, 0.7f, 0.7f)*(1-highlight) + highlight*(Color3f(1.f, 0.6f, 0.8f)*(1-hoveredFadeTime_) + Color3f(1, 1, 1)*hoveredFadeTime_));

    if (pressed_)
        text::drawScreenText(*value_, origin + Vector2f((width()+labelWidth_*mirror)/2,1) + Vector2f(1,1), 12.f, TEXT_ALIGN_CENTER, color);
    else
        text::drawScreenText(*value_, origin + Vector2f((width()+labelWidth_*mirror)/2,1), 12.f, TEXT_ALIGN_CENTER, color);

    // draw cursor
    if (pressed_ && cursorTimer_ < 30) {
        int pos = text::getCharacterPos(*value_, cursorPos_, 12.f, TEXT_ALIGN_CENTER) + 2;
        glColor4f(1,0.8,0.9,0.5);
        glLineWidth(0.5f);
        glBegin(GL_LINES);
            glVertex2f(origin.x_ + pos +(width()+labelWidth_*mirror)/2, origin.y_+3);
            glVertex2f(origin.x_ + pos +(width()+labelWidth_*mirror)/2, origin.y_+height_-3);
        glEnd();
    }

    // draw Label
    if (label_)
        label_->draw();
}

void TextEdit::setFocus (UiElement* toBeFocused, bool isPrevious) {
    UiElement::setFocus(this, isPrevious);
    if (label_)
        label_->setFocus(this, isPrevious);
}

void TextEdit::clearFocus() {
    UiElement::clearFocus();
    pressed_ = false;
    if (*value_ == "")
        *value_ = fallBack_;
    menus::unFixKeyboard();
    if (label_)
        label_->clearFocus();
}
