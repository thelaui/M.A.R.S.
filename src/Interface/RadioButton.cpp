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

void RadioButton::mouseLeft(bool down) {
    UiElement::mouseLeft(down);
    if (!pressed_ && hovered_) {
        if (*value_ && offSwitchable_)
            *value_ = false;
        else {
            dynamic_cast<RadioGroup*>(parent_)->allOff();
            *value_ = true;
        }
        sound::playSound(sound::Check);
    }
}

void RadioButton::draw() const {
    Vector2f origin = parent_->getTopLeft() + topLeft_;


    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (hovered_ && pressed_) {
        if (*value_) {
            glColor4f(1,1,1,0.6);
            glPointSize(6);
        }
        else {
            glColor4f(1,1,1,0.5);
            glPointSize(3);
        }
    }
    else if (hovered_) {
        if (*value_) {
            glColor4f(1,1,1,1);
            glPointSize(8);
        }
        else {
            glColor4f(1,1,1,0.7);
            glPointSize(5);
        }
    }
    else {
        if (*value_) {
            glColor4f(1,1,1,0.8);
            glPointSize(8);
        }
        else {
            glColor4f(1,1,1,0.3);
            glPointSize(4);
        }
    }

    glBegin(GL_POINTS);
        glVertex2f(origin.x_+5, origin.y_+8);
    glEnd();



    // draw Label
    label_->draw();
}

