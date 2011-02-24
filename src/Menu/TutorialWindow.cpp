/* TutorialWindow.cpp

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

# include "Menu/TutorialWindow.hpp"

# include "Interface/UiWindow.hpp"
# include "Interface/Button.hpp"
# include "Menu/menus.hpp"
# include "Media/text.hpp"
# include "Interface/TextBox.hpp"
# include "Interface/Line.hpp"
# include "Locales/locales.hpp"

# include <sstream>

UiWindow* TutorialWindow::instance_(NULL);
bool TutorialWindow::kOk_(false);
sf::String TutorialWindow::index_("1/1");

UiWindow* TutorialWindow::get(sf::String* title, sf::String* text, int index, bool info, bool next) {
    reset();
    if (instance_ == NULL) {
        instance_ = new TutorialWindow(400, 200);
        instance_->addWidget(new Button(locales::getLocale(next ? locales::Next : locales::Ok), NULL, &kOk_, Vector2f(320,170), 70, 20));
        instance_->addWidget(new Label(title, TEXT_ALIGN_LEFT, Vector2f(10,10), 20.f));
        if (info) {
            instance_->addWidget(new Label(locales::getLocale(locales::Info), TEXT_ALIGN_RIGHT, Vector2f(380,18), 12.f, Color3f(1.f, 0.5f, 0.9f), false));
        }
        else {
            std::stringstream sstr;
            sstr << index << "/22";
            index_ = sf::String(sstr.str());
            instance_->addWidget(new Label(&index_, TEXT_ALIGN_RIGHT, Vector2f(380,18), 12.f, Color3f(1.f, 0.5f, 0.9f), false));
        }
        instance_->addWidget(new Line(Vector2f(10, 35), Vector2f(340, 35)));
        instance_->addWidget(new TextBox(text, Vector2f(10, 50), 380, 110));
    }
    return instance_;
}

void TutorialWindow::checkWidgets() {
    if (kOk_) {
        kOk_ = false;
        menus::hideWindow();
    }
}

void TutorialWindow::reset() {
    if (instance_)
        delete instance_;
    instance_ = NULL;
}






