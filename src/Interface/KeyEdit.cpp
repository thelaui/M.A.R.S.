/* KeyEdit.cpp

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

# include "Interface/KeyEdit.hpp"

# include "System/settings.hpp"
# include "Media/text.hpp"
# include "Locales/locales.hpp"
# include "System/generateName.hpp"
# include "Media/sound.hpp"
# include "Menu/menus.hpp"

# include <SFML/OpenGL.hpp>

KeyEdit::KeyEdit (sf::String* text, sf::Key::Code* value, Vector2f const& topLeft, int width):
    UiElement(topLeft, width, 20),
    value_(value) {

    label_ = new Label(text, TEXT_ALIGN_LEFT, Vector2f(0,0));
    label_->setParent(this);
}

KeyEdit::~KeyEdit () {
    delete label_;
}

void KeyEdit::mouseMoved(Vector2f const& position) {
    UiElement::mouseMoved(position);
    label_->mouseMoved(position);
}

void KeyEdit::mouseLeft(bool down) {
    if (down && hovered_) {
        menus::clearFocus();
        setFocus(this);
        pressed_ = true;
        menus::fixKeyboardOn(this);
        sound::playSound(sound::Click);
    }
}

void KeyEdit::keyEvent(bool down, sf::Key::Code keyCode) {
    if (pressed_) {
        if (down && keyCode != sf::Key::Escape) {
            *value_ = keyCode;
            pressed_ = false;
            menus::unFixKeyboard();
        }
        else if (down && (keyCode == sf::Key::Escape)) {
            menus::unFixKeyboard();
            pressed_ = false;
        }
    }
    else if (down && (keyCode == sf::Key::Return || keyCode == sf::Key::Space)) {
        menus::fixKeyboardOn(this);
        pressed_ = true;
    }
}

void KeyEdit::draw() const {
    UiElement::draw();
    Vector2f origin = parent_->getTopLeft() + topLeft_;

    // draw Button
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
        if (isTopMost())   glColor4f(0.3*focusedFadeTime_,0.1*focusedFadeTime_,0.2*focusedFadeTime_,0.8);
        else               glColor4f(0.0,0.0,0.0,0.8);
        glVertex2f(origin.x_+185, origin.y_+2);
        glVertex2f(width_ + origin.x_, origin.y_+2);
        glVertex2f(width_ + origin.x_, height_ + origin.y_-2);
        glVertex2f(origin.x_+185, height_ + origin.y_-2);

        // glossy bottom
        glColor4f(1.0,1.0,1.0,0.0);
        glVertex2f(origin.x_+185, origin.y_+2);
        glVertex2f(width_ + origin.x_, origin.y_+2);
        if (pressed_)   glColor4f(1.0,1.0,1.0,0.1);
        else            glColor4f(1.0,1.0,1.0,0.06);
        glVertex2f(width_ + origin.x_, height_ + origin.y_-2);
        glVertex2f(origin.x_+185, height_ + origin.y_-2);

        if (!pressed_) {
            // glossy top
            glColor4f(1.0,1.0,1.0,0.2);
            glVertex2f(origin.x_+185, origin.y_+2);
            glVertex2f(width_ + origin.x_, origin.y_+2);
            glColor4f(1.0,1.0,1.0,0.05);
            glVertex2f(width_ + origin.x_, height_*0.5f + origin.y_);
            glVertex2f(origin.x_+185, height_*0.5f + origin.y_);
        }
    glEnd();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glLineWidth(1.f);

    glColor4f(1.0,0.4,0.8,0.3f+hoveredFadeTime_*0.7f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(origin.x_+185, origin.y_+2);
        glVertex2f(width_ + origin.x_, origin.y_+2);
        glVertex2f(width_ + origin.x_, height_ + origin.y_-2);
        glVertex2f(origin.x_+185, height_ + origin.y_-2);
    glEnd();

    if (pressed_)
        text::drawScreenText("...", origin + Vector2f((width_+185)/2,1)+Vector2f(1, 1), font::Ubuntu, 12.f, TEXT_ALIGN_CENTER, Color3f(0.7, 0.7, 0.7));
    else
        text::drawScreenText(generateName::key(*value_), origin + Vector2f((width_+185)/2,1), font::Ubuntu, 12.f, TEXT_ALIGN_CENTER, Color3f(0.5, 0.5, 0.5));

    //draw Label
    label_->draw();
}

void KeyEdit::setFocus (UiElement* toBeFocused) {
    UiElement::setFocus(this);
    label_->setFocus(this);
}

void KeyEdit::clearFocus() {
    UiElement::clearFocus();
    pressed_ = false;
    menus::unFixKeyboard();
    label_->clearFocus();
}
