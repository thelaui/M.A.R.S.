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


TextBox::TextBox(sf::String* text, Vector2f const& topLeft, int width, int height, Color3f const& color):
    UiElement(topLeft, width, height),
    label_(NULL) {

    text_ = *text;
    sf::String word;
    sf::String line;
    int lastSpace(0);

    // search for "\n" and replace them with '\n'
    for (unsigned int i=0; i<text_.GetSize()-1; ++i) {
        if (text_[i] == '\\' && text_[i+1] == 'n') {
            text_[i]  = ' ';
            text_[++i]= '\n';
        }
    }

    // remove doubled spaces
    for (unsigned int i=0; i<text_.GetSize()-1; ++i)
        if (text_[i] == ' ' && text_[i+1] == ' ')
            text_.Erase(i--, 1);

    // break lines
    for (unsigned int i=0; i<text_.GetSize(); ++i) {
        if (text_[i] == '\n') {
            line = "";
            word = "";
        }
        else if (text_[i] != ' ') {
            word += text_[i];
        }
        else {
            sf::String tmp = line + word;
            if (text::getCharacterPos(tmp, tmp.GetSize(), font::Ubuntu, 12.f, TEXT_ALIGN_LEFT) < width_) {
                lastSpace = i;
                line += word + " ";
                word = "";
            }
            else {
                text_[lastSpace] = '\n';
                line = word + " ";
                word = "";
            }
        }
    }

    label_ = new Label(&text_, TEXT_ALIGN_LEFT, Vector2f(0.f, 0.f), 12.f, color);
    label_->setParent(this);

    }

TextBox::~TextBox() {
    delete label_;
}

void TextBox::draw () const {
    label_->draw();
}




