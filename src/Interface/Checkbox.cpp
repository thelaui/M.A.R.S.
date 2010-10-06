/* Checkbox.cpp

Copyright (c) 2010 by Felix Lauer und Simon Schneegans

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

# include "Interface/Checkbox.hpp"

# include "System/settings.hpp"
# include "Media/sound.hpp"
# include "Media/text.hpp"

# include <SFML/OpenGL.hpp>

Checkbox::Checkbox (std::string* text, bool* value, Vector2f const& topLeft, int width):
    UiElement(topLeft, width, 20),
    value_(value) {

    label_ = new Label(text, TEXT_ALIGN_LEFT, Vector2f(15,0));
    label_->setParent(this);
}

Checkbox::~Checkbox () {
    delete label_;
}

void Checkbox::mouseLeft(bool down) {
    UiElement::mouseLeft(down);
    if (!pressed_ && hovered_) {
        *value_ = !*value_;
        sound::playSound(sound::Check);
    }
}

void Checkbox::draw() const {
    Vector2f origin = parent_->getTopLeft();
    bool pointStyle;


    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (hovered_ && pressed_) {
        if (*value_) {
            pointStyle = false;
            glColor4f(1,1,1,0.6);
            glLineWidth(1);
        }
        else {
            pointStyle = true;
            glColor4f(1,1,1,0.5);
            glPointSize(3);
        }
    }
    else if (hovered_) {
        if (*value_) {
            pointStyle = false;
            glColor4f(1,1,1,1);
            glLineWidth(3);
        }
        else {
            pointStyle = true;
            glColor4f(1,1,1,0.7);
            glPointSize(5);
        }
    }
    else {
        if (*value_) {
            pointStyle = false;
            glColor4f(1,1,1,0.8);
            glLineWidth(2);
        }
        else {
            pointStyle = true;
            glColor4f(1,1,1,0.3);
            glPointSize(5);
        }
    }

    if (pointStyle) {
        glBegin(GL_POINTS);
            glVertex2f(origin.x_+topLeft_.x_+5, origin.y_+topLeft_.y_+8);
        glEnd();
    }
    else {
        glBegin(GL_LINES);
            glVertex2f(origin.x_+topLeft_.x_+9, origin.y_+topLeft_.y_+12);
            glVertex2f(origin.x_+topLeft_.x_+1, origin.y_+topLeft_.y_+4);
            glVertex2f(origin.x_+topLeft_.x_+9, origin.y_+topLeft_.y_+4);
            glVertex2f(origin.x_+topLeft_.x_+1, origin.y_+topLeft_.y_+12);
        glEnd();
    }



    // draw Label
    label_->draw();
}

