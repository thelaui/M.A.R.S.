/* ChooseLanguage.cpp

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

# include "Menu/ChooseLanguage.hpp"

# include "Interface/UiWindow.hpp"
# include "Interface/Button.hpp"
# include "Menu/menus.hpp"
# include "Interface/TextBox.hpp"
# include "Interface/Line.hpp"
# include "Media/text.hpp"
# include "Locales/locales.hpp"
# include "System/settings.hpp"

UiWindow* ChooseLanguage::instance_(NULL);
bool ChooseLanguage::kCancel_(false);
std::map<sf::String, bool*> ChooseLanguage::languageKeyMap_;

UiWindow* ChooseLanguage::get() {
    if (instance_ == NULL) {
        std::vector<sf::String> languages = locales::getLanguages();

        instance_ = new ChooseLanguage(220, 100 + 30*languages.size());

        int top(50);
        for (std::vector<sf::String>::iterator it = languages.begin(); it != languages.end(); ++it) {
            bool* key = new bool(false);
            languageKeyMap_.insert(std::make_pair(*it, key));
            instance_->addWidget(new Button(new sf::String(*it), key, Vector2f(10, top), 200, 20));
            top += 30;
        }

        instance_->addWidget(new Button(locales::getLocale(locales::Cancel), &kCancel_, Vector2f(140,top+20), 70, 20));
        instance_->addWidget(new Label(new sf::String("Select Language"), TEXT_ALIGN_LEFT, Vector2f(10,10), 20.f));
        instance_->addWidget(new Line(Vector2f(10, 35), Vector2f(210, 35)));
    }
    return instance_;
}

void ChooseLanguage::checkWidgets() {
    if (kCancel_) {
        kCancel_ = false;
        settings::save();
        menus::hideWindow();
    }
    for (std::map<sf::String, bool*>::iterator it = languageKeyMap_.begin(); it != languageKeyMap_.end(); ++it)
        if (*(it->second)) {
            *(it->second) = false;
            settings::C_language = it->first;
            locales::load();
            settings::save();
            menus::hideWindow();
            break;
        }
}

void ChooseLanguage::reset() {
    if (instance_)
        delete instance_;
    instance_ = NULL;
}






