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
    UiElement(topLeft, width, height) {

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
                if (text::getCharacterPos(tmp, tmp.GetSize(), 12.f, TEXT_ALIGN_LEFT) > width_) {
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
                Label* newLabel = new Label(texts_.back(), TEXT_ALIGN_LEFT, Vector2f(0.f, top), 12.f, color);
                newLabel->setParent(this);
                lines_.push_back(newLabel);
                top += 15;
                line = "";
            }
            else {
                 line += wholeText[i];
            }
        }
        if (line != "") {
            texts_.push_back(new sf::String(line));
            Label* newLabel = new Label(texts_.back(), TEXT_ALIGN_LEFT, Vector2f(0.f, top), 12.f, color);
            newLabel->setParent(this);
            lines_.push_back(newLabel);
        }


    }

TextBox::~TextBox() {
    for (std::vector<Label*>::iterator it=lines_.begin(); it!=lines_.end(); ++it)
        delete *it;
    for (std::vector<sf::String*>::iterator it=texts_.begin(); it!=texts_.end(); ++it)
        delete *it;
}

void TextBox::draw () const {
    for (std::vector<Label*>::const_iterator it=lines_.begin(); it!=lines_.end(); ++it)
        (*it)->draw();
}




