/* Label.cpp

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

# include "Interface/Label.hpp"

# include "Media/text.hpp"

# include <SFML/OpenGL.hpp>

Label::Label (sf::String* text, int textAlign, Vector2f const& topLeft, float fontSize, Color3f color):
    UiElement(topLeft, 10, 10),
    text_(text),
    textAlign_(textAlign),
    fontSize_(fontSize),
    color_(color) {}

void Label::draw() const {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    if (parent_->isPressed() && parent_->isHovered()) {
        text::drawScreenText(*text_, topLeft_ + parent_->getTopLeft() + Vector2f(1,1), font::Ubuntu, fontSize_, textAlign_, Color3f(1*color_.r(),0.8*color_.g(),0.9*color_.b()));
    }
    else if (parent_->isHovered()) {
        text::drawScreenText(*text_, topLeft_ + parent_->getTopLeft(), font::Ubuntu, fontSize_, textAlign_, Color3f(1*color_.r(),0.8*color_.g(),0.9*color_.b()));
    }
    else {
        text::drawScreenText(*text_, topLeft_ + parent_->getTopLeft(), font::Ubuntu, fontSize_, textAlign_, Color3f(0.7*color_.r(),0.7*color_.g(),0.7*color_.b()));
    }
}

