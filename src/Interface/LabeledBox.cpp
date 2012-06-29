/* LabeledBox.cpp

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

# include "Interface/LabeledBox.hpp"

# include "Media/text.hpp"
# include "Interface/Label.hpp"

# include <SFML/OpenGL.hpp>

LabeledBox::LabeledBox (sf::String* text, Vector2f const& topLeft, int width, int height):
    UiElement(topLeft, width, height),
    label_(NULL) {

    if (text) {
        label_ = new Label(text, TEXT_ALIGN_LEFT, Vector2f(5, 5), 12.f, Color3f(1.f, 0.5f, 0.9f), false);
        label_->setParent(this);
    }
}

LabeledBox::~LabeledBox () {
    if (label_)
        delete label_;
}

void LabeledBox::draw() const {
    UiElement::draw();

    Vector2f origin = getTopLeft();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
        // dark background
        if (isTopMost())   glColor4f(1.f,0.5,0.8, hoveredFadeTime_*0.05f);
        else               glColor4f(1.0f,1.0f,1.0f,0.f);
        glVertex2f(origin.x_, origin.y_);
        if (isTopMost())   glColor4f(1.f,0.5,0.8, 0.f);
        else               glColor4f(1.0f,1.0f,1.0f,0.f);
        glVertex2f(origin.x_, height_*0.7f + origin.y_);
        glVertex2f(width()*0.7f + origin.x_, height_*0.7f + origin.y_);
        glVertex2f(width()*0.7f + origin.x_, origin.y_);
    glEnd();

    glLineWidth(2.f);
    glBegin(GL_LINE_LOOP);
        // dark background
        if (isTopMost())   glColor4f(1.f,0.5,0.8, hoveredFadeTime_*0.15f+0.45f);
        else               glColor4f(1.0f,1.0f,1.0f,0.1f);
        glVertex2f(origin.x_, origin.y_);
        if (isTopMost())   glColor4f(1.f,0.5,0.8, 0.f);
        else               glColor4f(1.0f,1.0f,1.0f,0.f);
        glVertex2f(origin.x_, height_*0.7f + origin.y_);
        glVertex2f(width()*0.7f + origin.x_, height_*0.7f + origin.y_);
        glVertex2f(width()*0.7f + origin.x_, origin.y_);
    glEnd();

    if (label_)
        label_->draw();
}


