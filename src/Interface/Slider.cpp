/* Slider.cpp

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

# include "Interface/Slider.hpp"

# include "System/settings.hpp"
# include "System/window.hpp"
# include "Media/text.hpp"
# include "Media/texture.hpp"
# include "Locales/locales.hpp"
# include "Interface/toolTip.hpp"

# include <SFML/OpenGL.hpp>
# include <sstream>

Slider::Slider (sf::String* text, sf::String* toolTip, int* value, int minValue, int maxValue, Vector2f const& topLeft, int width, int labelWidth, bool showValue, std::vector<sf::String> const& sliderNames):
    UiElement(topLeft, width, 20),
    value_(value),
    minValue_(minValue),
    maxValue_(maxValue),
    labelWidth_(labelWidth),
    showValue_(showValue),
    sliderNames_(sliderNames),
    toolTip_(toolTip) {

    label_ = new Label(text, TEXT_ALIGN_LEFT, Vector2f(0,0));
    label_->setParent(this);
}

Slider::~Slider () {
    delete label_;
}

void Slider::mouseLeft(bool down) {
    UiElement::mouseLeft(hovered_ && down);
    const sf::Input& input(window::getInput());
    mouseMoved(window::PixelToCoord(Vector2f(input.GetMouseX(), input.GetMouseY())));
}

void Slider::mouseMoved(Vector2f const& position) {
    UiElement::mouseMoved(position);
    int mirror(locales::getCurrentLocale().LTR_ ? 1 : -1);

    if (pressed_ && focused_) {
        *value_ = (position.x_+(0.5f*(width() - (labelWidth_+10)*mirror))/(maxValue_ - minValue_)-getTopLeft().x_-(labelWidth_+5)*mirror)*(maxValue_ - minValue_)/(width() - (labelWidth_+10)*mirror) + minValue_;
        if (*value_ < minValue_) *value_ = minValue_;
        if (*value_ > maxValue_) *value_ = maxValue_;
    }
    label_->mouseMoved(position);

    if (hovered_ && toolTip_)
        toolTip::show(toolTip_);
}

void Slider::keyEvent(bool down, sf::Key::Code keyCode) {
    if (down) {
        if (locales::getCurrentLocale().LTR_) {
            if (keyCode == sf::Key::Left && *value_ > minValue_)
                --(*value_);
            else if (keyCode == sf::Key::Right && *value_ < maxValue_)
                ++(*value_);
        }
        else {
            if (keyCode == sf::Key::Left && *value_ < maxValue_)
                ++(*value_);
            else if (keyCode == sf::Key::Right && *value_ > minValue_)
                --(*value_);
        }
    }
}

void Slider::draw() const {
    UiElement::draw();

    Vector2f origin = getTopLeft();

    int mirror(locales::getCurrentLocale().LTR_ ? 1 : -1);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // draw line
    // Hover effect
    Color3f((Color3f(1, 1, 1)*(1-hoveredFadeTime_) + hoveredFadeTime_*Color3f(1.f, 0.8f, 0.9f))*(0.7f+focusedFadeTime_*0.3f)).gl4f(0.7f);

    glLineWidth(2);
    glBegin(GL_LINES);
        glVertex2f(origin.x_ + labelWidth_*mirror, 10 + origin.y_);
        glVertex2f(width() + origin.x_, 10 + origin.y_);
    glEnd();

    // Hover effect
    Vector2f sliderPosition(((width()-(labelWidth_+10)*mirror)*(*value_- minValue_)/(maxValue_ - minValue_)) + origin.x_+(labelWidth_+5)*mirror, 10 + origin.y_);

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
        glTexCoord2f(x*0.25f+0.25f, y*0.25f);       glVertex2f(sliderPosition.x_+8.f, sliderPosition.y_+8.f);
        glTexCoord2f(x*0.25f, y*0.25f);             glVertex2f(sliderPosition.x_+8.f, sliderPosition.y_-8.f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    text::drawFooText();

    if (showValue_) {
        std::stringstream sstr;
        if(!sliderNames_.empty()) {
            if (sliderNames_.size() <= *value_-minValue_) {
                sstr << *value_;
                text::drawScreenText(sf::String(sstr.str()), origin + Vector2f((labelWidth_-10)*mirror,1), 12.f, TEXT_ALIGN_RIGHT, Color3f(0.7, 0.7, 0.7));
            }
            else
                text::drawScreenText(sliderNames_[*value_-minValue_], origin + Vector2f((labelWidth_-10)*mirror,1), 12.f, TEXT_ALIGN_RIGHT, Color3f(0.7, 0.7, 0.7));
        }
        else {
            sstr << *value_;
            text::drawScreenText(sf::String(sstr.str()), origin + Vector2f((labelWidth_-10)*mirror,1), 12.f, TEXT_ALIGN_RIGHT, Color3f(0.7, 0.7, 0.7));
        }

    }

    // draw Label
    label_->draw();
}

void Slider::setFocus (UiElement* toBeFocused, bool isPrevious) {
    UiElement::setFocus(this, isPrevious);
    label_->setFocus(this, isPrevious);
}

void Slider::clearFocus() {
    UiElement::clearFocus();
    label_->clearFocus();
}
