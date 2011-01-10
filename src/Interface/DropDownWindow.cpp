/* DropDownWindow.cpp

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

# include "Interface/DropDownWindow.hpp"

# include "Interface/ComboBox.hpp"
# include "Interface/Button.hpp"
# include "Media/text.hpp"
# include "Media/texture.hpp"
# include "Menu/menus.hpp"

# include <SFML/OpenGL.hpp>

DropDownWindow::DropDownWindow (int width, ComboBox* parent, std::vector<sf::String> elements):
    UiWindow(width, elements.size()*24 + 20),
    parent_(parent),
    elements_(elements.size()) {

    int top(10);
    for (int i=0; i<elements_.size(); ++i) {
        elements_[i] = std::make_pair(elements[i], false);
        Button* newOne=new Button(&(elements_[i].first), &(elements_[i].second), Vector2f(10, top), width_-20, 20);
        newOne->setParent(this);
        addWidget(newOne);
        top += 24 ;
    }
}

void DropDownWindow::checkWidgets() {
    for (std::vector<std::pair<sf::String, bool> >::iterator it=elements_.begin(); it!=elements_.end(); ++it)
        if (it->second) {
            it->second = false;
            *(parent_->currentValue_) = it->first;
            menus::hideWindow();
        }
}
