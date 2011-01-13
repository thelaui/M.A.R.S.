/* Label.cpp

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

# include "Interface/Label.hpp"

# include "Media/text.hpp"

# include <SFML/OpenGL.hpp>

Label::Label (sf::String* text, int textAlign, Vector2f const& topLeft, float fontSize, Color3f color, bool interactive):
    UiElement(topLeft, 10, 10),
    text_(text),
    textAlign_(textAlign),
    fontSize_(fontSize),
    color_(color),
    interactive_(interactive) {}

void Label::mouseMoved(Vector2f const& position) {
    hovered_ = parent_->isHovered();
}

void Label::draw() const {
    UiElement::draw();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    Vector2f position(topLeft_ + parent_->getTopLeft());
    if (interactive_) {
        if (parent_->isPressed())
            position += Vector2f(1, 1);
        float highlight(std::max(hoveredFadeTime_, focusedFadeTime_));
        Color3f color(color_*(1-highlight) + highlight*(Color3f(1.f, 0.6f, 0.8f)*(1-hoveredFadeTime_) + Color3f(1, 1, 1)*hoveredFadeTime_));
        text::drawScreenText(*text_, position, fontSize_, textAlign_, color);
    }
    else
        text::drawScreenText(*text_, position, fontSize_, textAlign_, color_);
}

void Label::setFocus(UiElement* toBeFocused, bool isPrevious) {
    focused_ = true;
}

void Label::clearFocus() {
    focused_ = false;
}
