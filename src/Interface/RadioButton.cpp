/* RadioButton.cpp

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

# include "Interface/RadioButton.hpp"

# include "System/settings.hpp"
# include "Media/sound.hpp"
# include "Media/texture.hpp"
# include "Media/text.hpp"

# include <SFML/OpenGL.hpp>

RadioButton::RadioButton (sf::String* text, bool* value, Vector2f const& topLeft, int width,bool offSwitchable):
    UiElement(topLeft, width, 20),
    value_(value),
    offSwitchable_(offSwitchable) {

    label_ = new Label(text, false, Vector2f(15,0));
    label_->setParent(this);
}

RadioButton::~RadioButton () {
    delete label_;
}

void RadioButton::mouseMoved(Vector2f const& position) {
    UiElement::mouseMoved(position);
    label_->mouseMoved(position);
}

void RadioButton::mouseLeft(bool down) {
    UiElement::mouseLeft(down);
    if (!pressed_ && hovered_ && focused_) {
        if (*value_ && offSwitchable_)
            *value_ = false;
        else {
            dynamic_cast<RadioGroup*>(parent_)->allOff();
            *value_ = true;
        }
        sound::playSound(sound::Check);
    }
}

void RadioButton::keyEvent(bool down, sf::Key::Code keyCode) {
    if (keyCode == sf::Key::Return || keyCode == sf::Key::Space) {
        pressed_ = down;
        if (!pressed_) {
            if (*value_ && offSwitchable_)
                *value_ = false;
            else {
                dynamic_cast<RadioGroup*>(parent_)->allOff();
                *value_ = true;
            }
            sound::playSound(sound::Check);
        }
    }
}

void RadioButton::draw() const {
    Vector2f origin = parent_->getTopLeft() + topLeft_;

    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Widgets));

    int x(0), y(0);

    if (hovered_ && pressed_) {
        if (*value_) {
            x = 2;
            y = 0;
        }
        else {
            x = 2;
            y = 1;
        }
    }
    else if (hovered_) {
        if (*value_) {
            x = 1;
            y = 0;
        }
        else {
            x = 1;
            y = 1;
        }
    }
    else {
        if (*value_) {
            x = 0;
            y = 0;
        }
        else {
            x = 0;
            y = 1;
        }
    }

    glColor3f(1.f, 1.f, 1.f);
    glBegin(GL_QUADS);
        glTexCoord2f(x*0.25f, y*0.25f+0.25f);       glVertex2f(origin.x_-4.f, origin.y_-0.f);
        glTexCoord2f(x*0.25f+0.25f, y*0.25f+0.25f); glVertex2f(origin.x_-4.f, origin.y_+16.f);
        glTexCoord2f(x*0.25f+0.25f, y*0.25f);       glVertex2f(origin.x_+12.f, origin.y_+16.f);
        glTexCoord2f(x*0.25f, y*0.25f);             glVertex2f(origin.x_+12.f, origin.y_-0.f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    // draw Label
    text::drawFooText();
    label_->draw();
}

void RadioButton::setFocus (UiElement* toBeFocused, bool isPrevious) {
    UiElement::setFocus(this, isPrevious);
    label_->setFocus(this, isPrevious);
}

void RadioButton::clearFocus() {
    UiElement::clearFocus();
    label_->clearFocus();
}
