/* TextBox.cpp

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


TextBox::TextBox(std::string const& text, Vector2f const& topLeft, int width, int height):
    UiElement(topLeft, width, height) {

    std::string word;
    std::string line;

    for (int i=0; i<text.length(); ++i) {
        if (text[i] == '\n') {
            addLine(line);
            line = "";
            word = "";
        }
        else if (text[i] != ' ') {
            word.push_back(text[i]);
        }
        else {
            std::string tmp = line + word;
            if (text::getCharacterPos(tmp, tmp.length(), font::HandelGotDLig, 12.f, TEXT_ALIGN_LEFT) < width_) {
                line.append(word + " ");
                word = "";
            }
            else {
                addLine(line);
                line = word + " ";
                word = "";
            }
        }
    }
    line.append(word + " ");
    if (line.length() > 0)
        addLine(line);
}

TextBox::~TextBox() {
    for (std::vector<Label*>::const_iterator it = lines_.begin(); it != lines_.end(); ++it)
        delete *it;
}

void TextBox::draw () const {
    for (std::vector<Label*>::const_iterator it = lines_.begin(); it != lines_.end(); ++it)
        (*it)->draw();
}

void TextBox::addLine(std::string text) {
    Label* newLabel = new Label(text, TEXT_ALIGN_LEFT, Vector2f(0,lines_.size()*15.f));
    newLabel->setParent(this);
    lines_.push_back(newLabel);
}





