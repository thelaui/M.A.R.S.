/* Button.cpp

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

# include "Interface/Button.hpp"

# include "System/settings.hpp"
# include "Media/sound.hpp"
# include "Media/text.hpp"
# include "Menu/menus.hpp"
# include "Interface/toolTip.hpp"

# include <SFML/OpenGL.hpp>

Button::Button (sf::String* text, sf::String* toolTip, bool* key, Vector2f const& topLeft, int width, int height, int align, sf::Font* font):
    UiElement(topLeft, width, height),
    key_(key),
    toolTip_(toolTip) {

    switch (align) {
        case TEXT_ALIGN_LEFT:  label_ = new Label(text, align, Vector2f(4, 2), 12.f, Color3f(0.75f,0.75f,0.75f), true, font);          break;
        case TEXT_ALIGN_RIGHT: label_ = new Label(text, align, Vector2f(width-4, 2), 12.f, Color3f(0.75f,0.75f,0.75f), true, font);    break;
        default:               label_ = new Label(text, align, Vector2f(width*0.5f, 2), 12.f, Color3f(0.75f,0.75f,0.75f), true, font);
    }

    label_->setParent(this);
}

Button::~Button () {
    delete label_;
}

void Button::mouseMoved(Vector2f const& position) {
    UiElement::mouseMoved(position);
    label_->mouseMoved(position);
    if (hovered_ && toolTip_)
        toolTip::show(toolTip_);
}

void Button::mouseLeft(bool down) {
    UiElement::mouseLeft(down);
    if (!pressed_ && hovered_ && focused_) {
        *key_ = true;
        hovered_ = false;
        toolTip::show(NULL);
        sound::playSound(sound::Click);
    }
}

void Button::keyEvent(bool down, Key const& key) {
    if (key.navi_ == Key::nConfirm) {
        pressed_ = down;
        if (!pressed_) {
            *key_ = true;
            hovered_ = false;
            toolTip::show(NULL);
            sound::playSound(sound::Click);
        }
    }
}

void Button::draw () const {
    UiElement::draw();

    Vector2f origin = getTopLeft();

    // draw Button
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
        // dark background
        if (isTopMost())   glColor4f(0.3*focusedFadeTime_,0.1*focusedFadeTime_,0.2*focusedFadeTime_,0.8);
        else               glColor4f(0.0,0.0,0.0,0.8);
        glVertex2f(origin.x_, origin.y_);
        glVertex2f(origin.x_, height_ + origin.y_);
        glColor4f(0.0,0.0,0.0,0.8);
        glVertex2f(width()*0.5f + origin.x_, height_ + origin.y_);
        glVertex2f(width()*0.5f + origin.x_, origin.y_);
        glVertex2f(width()*0.5f + origin.x_, height_ + origin.y_);
        glVertex2f(width()*0.5f + origin.x_, origin.y_);
        if (isTopMost())   glColor4f(0.3*focusedFadeTime_,0.1*focusedFadeTime_,0.2*focusedFadeTime_,0.8);
        else               glColor4f(0.0,0.0,0.0,0.8);
        glVertex2f(width() + origin.x_, origin.y_);
        glVertex2f(width() + origin.x_, height_ + origin.y_);

        // glossy bottom
        glColor4f(1.0,1.0,1.0,0.0);
        glVertex2f(origin.x_, height_*0.7f + origin.y_);
        glVertex2f(width() + origin.x_, height_*0.7f + origin.y_);
        glColor4f(1.0,1.0,1.0,0.06);
        glVertex2f(width() + origin.x_, height_ + origin.y_);
        glVertex2f(origin.x_, height_ + origin.y_);

    if (pressed_ && (hovered_ || focused_)) {
        // bottom glow
        glColor4f(0.5,0.25,0.4,0.0);
        glVertex2f(origin.x_,origin.y_);
        glVertex2f(origin.x_+width(),origin.y_);
        glColor4f(0.5,0.25,0.4,0.4);
        glVertex2f(origin.x_+width(),origin.y_+height_);
        glVertex2f(origin.x_,origin.y_+height_);
    }
    else if (hovered_) {
        glColor4f(0.5,0.25,0.4,0.0);
        glVertex2f(origin.x_,height_*0.5f + origin.y_);
        glVertex2f(origin.x_+width(),height_*0.5f + origin.y_);
        glColor4f(0.5,0.25,0.4,0.4);
        glVertex2f(origin.x_+width(),origin.y_+height_);
        glVertex2f(origin.x_,origin.y_+height_);

        // glossy top
        glColor4f(1.0,1.0,1.0,0.2);
        glVertex2f(origin.x_, origin.y_);
        glVertex2f(width() + origin.x_, origin.y_);
        glColor4f(1.0,1.0,1.0,0.05);
        glVertex2f(width() + origin.x_, height_*0.5f + origin.y_);
        glVertex2f(origin.x_, height_*0.5f + origin.y_);
    }
    else {
        // glossy top
        glColor4f(1.0,1.0,1.0,0.2);
        glVertex2f(origin.x_, origin.y_);
        glVertex2f(width() + origin.x_, origin.y_);
        glColor4f(1.0,1.0,1.0,0.05);
        glVertex2f(width() + origin.x_, height_*0.5f + origin.y_);
        glVertex2f(origin.x_, height_*0.5f + origin.y_);
    }
    glEnd();


    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glLineWidth(1.f);

    glColor4f(1.0,0.4,0.8,0.3f+hoveredFadeTime_*0.7f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(origin.x_,origin.y_+height_);
        glVertex2f(origin.x_,origin.y_);
        glVertex2f(origin.x_+width(),origin.y_);
        glVertex2f(origin.x_+width(),origin.y_+height_);
    glEnd();

    // draw Label
    label_->draw();
}

void Button::setFocus (UiElement* toBeFocused, bool isPrevious) {
    UiElement::setFocus(this, isPrevious);
    label_->setFocus(this, isPrevious);
}

void Button::clearFocus() {
    UiElement::clearFocus();
    label_->clearFocus();
}
