/* Button.cpp

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

# include "Interface/Button.hpp"

# include "System/settings.hpp"
# include "Media/sound.hpp"
# include "Media/text.hpp"

# include <SFML/OpenGL.hpp>

Button::Button (sf::String* text, bool* key, Vector2f const& topLeft, int width, int height):
    UiElement(topLeft, width, height),
    key_(key) {

    label_ = new Label(text, TEXT_ALIGN_CENTER, Vector2f(width/2, 2));
    label_->setParent(this);
}

Button::~Button () {
    delete label_;
}

void Button::mouseLeft(bool down) {
    UiElement::mouseLeft(down);
    if (!pressed_ && hovered_) {
        *key_ = true;
        hovered_ = false;
        sound::playSound(sound::Click);
    }
}

void Button::draw () const {
    Vector2f origin = parent_->getTopLeft() + topLeft_;
    // draw Button

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
        glColor4f(0.0,0.0,0.0,0.8);
        glVertex2f(origin.x_, origin.y_);
        glVertex2f(width_ + origin.x_, origin.y_);
        glVertex2f(width_ + origin.x_, height_ + origin.y_);
        glVertex2f(origin.x_, height_ + origin.y_);

    if (pressed_ && hovered_) {
            glColor4f(1.0,1.0,1.0,0.02);
            glVertex2f(origin.x_, height_/2 + origin.y_);
            glVertex2f(width_ + origin.x_, height_/2 + origin.y_);
            glColor4f(1.0,1.0,1.0,0.2);
            glVertex2f(width_ + origin.x_, height_ + origin.y_);
            glVertex2f(origin.x_, height_ + origin.y_);

            glColor4f(0.5,0.25,0.4,0.0);
            glVertex2f(origin.x_,origin.y_);
            glVertex2f(origin.x_+width_,origin.y_);
            glColor4f(0.5,0.25,0.4,0.4);
            glVertex2f(origin.x_+width_,origin.y_+height_);
            glVertex2f(origin.x_,origin.y_+height_);
        glEnd();
    }
    else if (hovered_) {
            glColor4f(1.0,1.0,1.0,0.2);
            glVertex2f(origin.x_, origin.y_);
            glVertex2f(width_ + origin.x_, origin.y_);
            glColor4f(1.0,1.0,1.0,0.02);
            glVertex2f(width_ + origin.x_, height_/2 + origin.y_);
            glVertex2f(origin.x_, height_/2 + origin.y_);

            glColor4f(0.5,0.25,0.4,0.0);
            glVertex2f(origin.x_,origin.y_);
            glVertex2f(origin.x_+width_,origin.y_);
            glColor4f(0.5,0.25,0.4,0.4);
            glVertex2f(origin.x_+width_,origin.y_+height_);
            glVertex2f(origin.x_,origin.y_+height_);
        glEnd();
    }

    else {
            glColor4f(1.0,1.0,1.0,0.2);
            glVertex2f(origin.x_, origin.y_);
            glVertex2f(width_ + origin.x_, origin.y_);
            glColor4f(1.0,1.0,1.0,0.02);
            glVertex2f(width_ + origin.x_, height_/2 + origin.y_);
            glVertex2f(origin.x_, height_/2 + origin.y_);
        glEnd();
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    if (hovered_) glLineWidth(1.5f);
    else           glLineWidth(0.5f);

    glBegin(GL_LINE_LOOP);
        glColor4f(1.0,0.4,0.8,0.4);
        glVertex2f(origin.x_,origin.y_+height_);
        glVertex2f(origin.x_,origin.y_);
        glVertex2f(origin.x_+width_,origin.y_);
        glVertex2f(origin.x_+width_,origin.y_+height_);
    glEnd();



    // draw Label
    label_->draw();
}
