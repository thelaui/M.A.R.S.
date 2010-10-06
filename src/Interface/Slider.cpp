/* Slider.cpp

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

# include "Interface/Slider.hpp"

# include "System/settings.hpp"
# include "System/window.hpp"
# include "Media/text.hpp"

# include <SFML/OpenGL.hpp>
# include <string>
# include <sstream>

Slider::Slider (std::string* text, int* value, int minValue, int maxValue, Vector2f const& topLeft, int width, int labelWidth, bool showValue, std::vector<std::string> const& sliderNames):
    UiElement(topLeft, width, 20),
    value_(value),
    minValue_(minValue),
    maxValue_(maxValue),
    labelWidth_(labelWidth),
    showValue_(showValue),
    sliderNames_(sliderNames) {

    label_ = new Label(text, TEXT_ALIGN_LEFT, Vector2f(0,0));
    label_->setParent(this);
}

Slider::~Slider () {
    delete label_;
}

void Slider::mouseLeft(bool down) {
    UiElement::mouseLeft(down);
    const sf::Input& input(window::getMainWindow()->GetInput());
    mouseMoved(Vector2f(input.GetMouseX(), input.GetMouseY()));
}

void Slider::mouseMoved(Vector2f const& position) {
    UiElement::mouseMoved(position);
    if (pressed_ && hovered_) {
        *value_ = (position.x_+(0.5f*(width_ - (labelWidth_+10)))/(maxValue_ - minValue_)-parent_->getTopLeft().x_-topLeft_.x_-(labelWidth_+5))*(maxValue_ - minValue_)/(width_ - (labelWidth_+10)) + minValue_;
        if (*value_ < minValue_) *value_ = minValue_;
        if (*value_ > maxValue_) *value_ = maxValue_;
    }
}

void Slider::draw() const {
    Vector2f origin = parent_->getTopLeft() + topLeft_;


    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // draw line
    // Hover effect
    if (hovered_)
        glColor4f(1,1,1,0.5);
    else
        glColor4f(1,1,1,0.3);

    glLineWidth(2);
    glBegin(GL_LINES);
        glVertex2f(origin.x_ + labelWidth_, 10 + origin.y_);
        glVertex2f(width_ + origin.x_, 10 + origin.y_);
    glEnd();

    // Hover effect
    Vector2f sliderPosition(((width_-(labelWidth_+10))*(*value_- minValue_)/(maxValue_ - minValue_)) + origin.x_+(labelWidth_+5), 10 + origin.y_);

    if (hovered_ && pressed_) {
        glColor4f(1,1,1,0.6);
        glPointSize(8);
    }
    else if (hovered_){
        glColor4f(1,1,1,1);
        glPointSize(10);
    }
    else {
        glColor4f(1,1,1,0.8);
        glPointSize(10);
    }

    glBegin(GL_POINTS);
        glVertex2f(sliderPosition.x_, sliderPosition.y_);
    glEnd();



    if (showValue_) {
        std::stringstream sstr;
        if(!sliderNames_.empty()) {
            if (sliderNames_.size() < *value_)
                sstr << *value_;
            else
                sstr << sliderNames_[*value_-1];
        }
        else {
            sstr << *value_;
        }
        text::drawScreenText(sstr.str(), origin + Vector2f(labelWidth_-10,1), font::HandelGotDLig, 12.f, TEXT_ALIGN_RIGHT, Color3f(0.7, 0.7, 0.7));
    }

    // draw Label
    label_->draw();
}
