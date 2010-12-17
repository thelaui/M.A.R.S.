/* ColorPicker.cpp

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

# include "Interface/ColorPicker.hpp"

# include "System/settings.hpp"
# include "System/window.hpp"
# include "Media/text.hpp"
# include "Media/texture.hpp"
# include "Locales/locales.hpp"
# include "Media/sound.hpp"
# include "Menu/menus.hpp"
# include "Interface/ColorPickerWindow.hpp"
# include "Interface/Button.hpp"

# include <SFML/OpenGL.hpp>
# include <iostream>

ColorPicker::ColorPicker (sf::String* text, Color3f* value, Vector2f const& topLeft, int width, int labelWidth):
    UiElement(topLeft, width, 16),
    colorWindow_(NULL),
    labelWidth_(labelWidth),
    currentValue_(value),
    opened_(false) {

    label_ = new Label(text, TEXT_ALIGN_LEFT, Vector2f(0,0));
    label_->setParent(this);

    colorWindow_ = new ColorPickerWindow(this, currentValue_);
}

ColorPicker::~ColorPicker () {
    delete label_;
    delete colorWindow_;
}

void ColorPicker::mouseLeft(bool down) {
    UiElement::mouseLeft(down);
    if (!pressed_ && hovered_) {
        sound::playSound(sound::Click);
        menus::showWindow(colorWindow_);
    }
}

void ColorPicker::draw() const {
    Vector2f origin = parent_->getTopLeft() + topLeft_;

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
        // dark background
        currentValue_->gl3f();
        glVertex2f(labelWidth_+origin.x_, origin.y_);
        glVertex2f(width_ + origin.x_, origin.y_);
        glVertex2f(width_ + origin.x_, height_ + origin.y_);
        glVertex2f(labelWidth_+origin.x_, height_ + origin.y_);

        // glossy bottom
        glColor4f(1.0,1.0,1.0,0.0);
        glVertex2f(labelWidth_+origin.x_, height_*0.7f + origin.y_);
        glVertex2f(width_ + origin.x_, height_*0.7f + origin.y_);
        glColor4f(1.0,1.0,1.0,0.06);
        glVertex2f(width_ + origin.x_, height_ + origin.y_);
        glVertex2f(labelWidth_+origin.x_, height_ + origin.y_);

    if (pressed_ && hovered_) {
        // bottom glow
        glColor4f(0.5,0.25,0.4,0.0);
        glVertex2f(labelWidth_+origin.x_,origin.y_);
        glVertex2f(origin.x_+width_,origin.y_);
        glColor4f(0.5,0.25,0.4,0.4);
        glVertex2f(origin.x_+width_,origin.y_+height_);
        glVertex2f(labelWidth_+origin.x_,origin.y_+height_);
    }
    else if (hovered_) {
        glColor4f(0.5,0.25,0.4,0.0);
        glVertex2f(labelWidth_+origin.x_,height_*0.5f + origin.y_);
        glVertex2f(origin.x_+width_,height_*0.5f + origin.y_);
        glColor4f(0.5,0.25,0.4,0.4);
        glVertex2f(origin.x_+width_,origin.y_+height_);
        glVertex2f(labelWidth_+origin.x_,origin.y_+height_);

        // glossy top
        glColor4f(1.0,1.0,1.0,0.2);
        glVertex2f(labelWidth_+origin.x_, origin.y_);
        glVertex2f(width_ + origin.x_, origin.y_);
        glColor4f(1.0,1.0,1.0,0.05);
        glVertex2f(width_ + origin.x_, height_*0.5f + origin.y_);
        glVertex2f(labelWidth_+origin.x_, height_*0.5f + origin.y_);
    }
    else {
        // glossy top
        glColor4f(1.0,1.0,1.0,0.2);
        glVertex2f(labelWidth_+origin.x_, origin.y_);
        glVertex2f(width_ + origin.x_, origin.y_);
        glColor4f(1.0,1.0,1.0,0.05);
        glVertex2f(width_ + origin.x_, height_*0.5f + origin.y_);
        glVertex2f(labelWidth_+origin.x_, height_*0.5f + origin.y_);
    }
    glEnd();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glLineWidth(1.f);
    if (hovered_)  glColor4f(1.0,0.4,0.8,0.9);
    else           glColor4f(1.0,0.4,0.8,0.4);

    glBegin(GL_LINE_LOOP);
        glVertex2f(labelWidth_+origin.x_,origin.y_+height_);
        glVertex2f(labelWidth_+origin.x_,origin.y_);
        glVertex2f(origin.x_+width_,origin.y_);
        glVertex2f(origin.x_+width_,origin.y_+height_);
    glEnd();

    text::drawFooText();
    // draw Label
    label_->draw();
}



