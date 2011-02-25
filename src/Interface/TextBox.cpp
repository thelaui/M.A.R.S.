/* TextBox.cpp

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

# include "Interface/TextBox.hpp"

# include "Media/text.hpp"

# include <SFML/OpenGL.hpp>

TextBox::TextBox(sf::String* text, Vector2f const& topLeft, int width, int height, Color3f const& color):
    UiElement(topLeft, width, height),
    color_(color),
    slider_(NULL),
    position_(0),
    scrollSpeed_(0.f) {

        sf::String wholeText = *text;
        sf::String word;
        sf::String line;
        int lastSpace(0);

        // search for "\n" and replace them with '\n'
        for (unsigned int i=0; i<wholeText.GetSize()-1; ++i) {
            if (wholeText[i] == '\\' && wholeText[i+1] == 'n') {
                wholeText[i]  = ' ';
                wholeText[++i]= '\n';
            }
        }

        // remove doubled spaces
        for (unsigned int i=0; i<wholeText.GetSize()-1; ++i)
            if (wholeText[i] == ' ' && wholeText[i+1] == ' ')
                wholeText.Erase(i--, 1);

        // break lines
        for (unsigned int i=0; i<wholeText.GetSize(); ++i) {
            if (wholeText[i] == '\n') {
                line = "";
                word = "";
            }
            else if (wholeText[i] != ' ') {
                word += wholeText[i];
                sf::String tmp(line + word);
                if (text::getCharacterPos(tmp, tmp.GetSize(), 12.f, TEXT_ALIGN_LEFT) > width_-10) {
                    if (lastSpace == 0) {
                        wholeText.Insert(i-1, '\n');
                        line = "";
                        word = wholeText[i];
                        ++i;
                    }
                    else {
                        wholeText[lastSpace] = '\n';
                        line = word;
                        lastSpace = 0;
                    }
                }
            }
            else {
                lastSpace = i;
                line += word + " ";
                word = "";
            }
        }

        // create single labels
        line = "";
        int top(0);
        for (unsigned int i=0; i<wholeText.GetSize(); ++i) {
            if (wholeText[i] == '\n') {
                texts_.push_back(new sf::String(line));
                line = "";
            }
            else {
                 line += wholeText[i];
            }
        }
        if (line != "") {
            texts_.push_back(new sf::String(line));
        }

    if (texts_.size()*15.f > height_) {
        slider_ = new VerticalSlider(&position_, 0, texts_.size()*15.f-height, Vector2f(width_-12, 0), height_);
        slider_->setParent(this);
    }

}

TextBox::~TextBox() {
    for (std::vector<sf::String*>::iterator it=texts_.begin(); it!=texts_.end(); ++it)
        delete *it;
    if (slider_)
        delete slider_;
}

void TextBox::mouseMoved(Vector2f const& position) {
    UiElement::mouseMoved(position);
    if (slider_)
        slider_->mouseMoved(position);
}

void TextBox::mouseWheelMoved(Vector2f const& position, int delta) {
    if (hovered_ && slider_) {
        if (delta > 0) scrollSpeed_ = -200.f;
        else           scrollSpeed_ =  200.f;
    }
}

void TextBox::mouseLeft(bool down) {
    if (slider_)
        slider_->mouseLeft(down);
}

void TextBox::draw () const {
    //mouswheel scroll
    if (slider_) {
        if (scrollSpeed_ > 0.f) {
            scrollSpeed_ -= timer::frameTime()*400.f;
            if (scrollSpeed_ <= 0.f) scrollSpeed_ = 0.f;
        }
        else if (scrollSpeed_ < 0.f) {
            scrollSpeed_ += timer::frameTime()*400.f;
            if (scrollSpeed_ >= 0.f) scrollSpeed_ = 0.f;
        }

        position_ += scrollSpeed_*timer::frameTime();

        if (position_ < 0) position_ = 0;
        else if (position_ > texts_.size()*15.f-height_) position_ = texts_.size()*15.f-height_;
    }

    Vector2f origin(getTopLeft());
    int top(-position_);

    /*glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
        glColor3f(0.3,0.1,0.2);
        glVertex2f(origin.x_, origin.y_);
        glVertex2f(origin.x_, height_ + origin.y_);
        glVertex2f(width() + origin.x_, height_ + origin.y_);
        glVertex2f(width() + origin.x_, origin.y_);
    glEnd();*/

    for (std::vector<sf::String*>::const_iterator it=texts_.begin(); it!=texts_.end(); ++it) {
        float alpha(1.f);
        if (top > height_-15.f)
            alpha = (height_-top)/15.f;
        else if (top < 0 && top > -15)
            alpha = 1 + top/15.f;
        else if (top <= -15)
            alpha = 0;

        if (alpha > 0)
            text::drawScreenText(*(*it), origin+Vector2f(0, top), 12.f, TEXT_ALIGN_LEFT, color_, alpha);
        top += 15.f;
    }

    if (slider_)
        slider_->draw();

    text::drawFooText();
}




