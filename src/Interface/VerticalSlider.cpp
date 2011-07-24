/* VerticalSlider.cpp

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

# include "Interface/VerticalSlider.hpp"

# include "System/settings.hpp"
# include "System/window.hpp"
# include "Media/text.hpp"
# include "Media/texture.hpp"
# include "Locales/locales.hpp"
# include "Interface/toolTip.hpp"

# include <SFML/OpenGL.hpp>
# include <sstream>

VerticalSlider::VerticalSlider (float* value, float minValue, float maxValue, Vector2f const& topLeft, int height):
    UiElement(topLeft, 20, height),
    value_(value),
    minValue_(minValue),
    maxValue_(maxValue) {
}

void VerticalSlider::mouseLeft(bool down) {
    UiElement::mouseLeft(hovered_ && down);
    mouseMoved(window::PixelToCoord(window::getMousePosition()));
}

void VerticalSlider::mouseMoved(Vector2f const& position) {
    UiElement::mouseMoved(position);

    if (pressed_ && focused_) {
        *value_ = (position.y_+(0.5f*(height_ - 10))/(maxValue_ - minValue_)-getTopLeft().y_-5)*(maxValue_ - minValue_)/(height_ - 10) + minValue_;
        if (*value_ < minValue_) *value_ = minValue_;
        if (*value_ > maxValue_) *value_ = maxValue_;
    }
}

void VerticalSlider::keyEvent(bool down, Key const& key) {
    if (down) {
        if (key.navi_ == Key::nLeft && *value_ > minValue_)
            --(*value_);
        else if (key.navi_ == Key::nRight && *value_ < maxValue_)
            ++(*value_);
    }
}

void VerticalSlider::draw() const {
    UiElement::draw();

    Vector2f origin = getTopLeft();

    int mirror(locales::getCurrentLocale().LTR_ ? 1 : -1);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // draw line
    // Hover effect
    Color3f((Color3f(1, 1, 1)*(1-hoveredFadeTime_) + hoveredFadeTime_*Color3f(1.f, 0.8f, 0.9f))*(0.7f+focusedFadeTime_*0.3f)).gl4f(0.7f);

    glLineWidth(2);
    glBegin(GL_LINES);
        glVertex2f(10*mirror + origin.x_, origin.y_);
        glVertex2f(10*mirror + origin.x_, height_ + origin.y_);
    glEnd();

    // Hover effect
    Vector2f sliderPosition(10*mirror + origin.x_, (height_-10)*(*value_- minValue_)/(maxValue_ - minValue_) + origin.y_+5);

    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Widgets));

    int x(0), y(0);

    if (hovered_ && pressed_) {
        x = 3;
        y = 2;
    }
    else if (hovered_){
        x = 3;
        y = 1;
    }
    else {
        x = 3;
        y = 0;
    }

    glColor3f(1.f, 1.f, 1.f);
    glBegin(GL_QUADS);
        glTexCoord2f(x*0.25f, y*0.25f+0.25f);       glVertex2f(sliderPosition.x_-8.f,  sliderPosition.y_-8.f);
        glTexCoord2f(x*0.25f+0.25f, y*0.25f+0.25f); glVertex2f(sliderPosition.x_-8.f,  sliderPosition.y_+8.f);
        glTexCoord2f(x*0.25f+0.25f, y*0.25f);       glVertex2f(sliderPosition.x_+8.f,  sliderPosition.y_+8.f);
        glTexCoord2f(x*0.25f, y*0.25f);             glVertex2f(sliderPosition.x_+8.f,  sliderPosition.y_-8.f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

