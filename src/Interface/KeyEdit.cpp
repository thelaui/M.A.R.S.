/* KeyEdit.cpp

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

# include "Interface/KeyEdit.hpp"

# include "System/settings.hpp"
# include "Media/text.hpp"
# include "Locales/locales.hpp"
# include "System/generateName.hpp"

# include <SFML/OpenGL.hpp>

KeyEdit::KeyEdit (sf::String* text, sf::Key::Code* value, Vector2f const& topLeft, int width):
    UiElement(topLeft, width, 20),
    value_(value) {

    label_ = new Label(text, TEXT_ALIGN_LEFT, Vector2f(0,0));
    label_->setParent(this);
}

KeyEdit::~KeyEdit () {
    delete label_;
}


void KeyEdit::buttonPressed(sf::Key::Code keyCode) {
    if (hovered_ && keyCode != 0 && keyCode != 128) {
        *value_ = keyCode;
    }
}

void KeyEdit::draw() const {
    Vector2f origin = parent_->getTopLeft() + topLeft_;
    // draw Button

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Hover effect
    if (hovered_)
        glColor4f(1,0.7,0.9,0.4);
    else
        glColor4f(1,1,1,0.1);

    glBegin(GL_QUADS);
        glVertex2f(origin.x_+185, origin.y_+2);
        glVertex2f(width_ + origin.x_, origin.y_+2);
        glColor4f(1,1,1,0.2);
        glVertex2f(width_ + origin.x_, height_ + origin.y_-2);
        glVertex2f(origin.x_+185, height_ + origin.y_-2);
    glEnd();


    if (pressed_ && hovered_)
        text::drawScreenText(*locales::getLocale(locales::KeyEditHover), origin + Vector2f((width_+185)/2,1), font::Ubuntu, 12.f, TEXT_ALIGN_CENTER, Color3f(0.7, 0.7, 0.7));
    else
        text::drawScreenText(generateName::key(*value_), origin + Vector2f((width_+185)/2,1), font::Ubuntu, 12.f, TEXT_ALIGN_CENTER, Color3f(0.7, 0.7, 0.7));

    //draw Label
    label_->draw();
}

