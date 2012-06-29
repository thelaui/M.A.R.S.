/* EndMenu.cpp

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

# include "Menu/EndMenu.hpp"

# include "Interface/UiWindow.hpp"
# include "Interface/Button.hpp"
# include "Menu/menus.hpp"
# include "Menu/ToMainConfirm.hpp"
# include "Games/games.hpp"
# include "Menu/OptionsMenu.hpp"
# include "Menu/InfoHide.hpp"
# include "System/settings.hpp"
# include "Locales/locales.hpp"

UiWindow* EndMenu::instance_(NULL);
bool EndMenu::kNew_(false);
bool EndMenu::kOptions_(false);
bool EndMenu::kToMainMenu_(false);
bool EndMenu::kHide_(false);

UiWindow* EndMenu::get() {
    if (instance_ == NULL) {
        instance_ = new EndMenu(180, 130);
        instance_->addWidget(new Button(locales::getLocale(locales::RestartGame),     NULL, &kNew_,         Vector2f(10,10), 160, 20));
        instance_->addWidget(new Button(locales::getLocale(locales::Options),         NULL, &kOptions_,     Vector2f(10,40), 160, 20));
        instance_->addWidget(new Button(locales::getLocale(locales::HideMenu),        NULL, &kHide_,     Vector2f(10,70), 160, 20));
        instance_->addWidget(new Button(locales::getLocale(locales::QuitCurrentGame), NULL, &kToMainMenu_,  Vector2f(10,100), 160, 20));
    }
    return instance_;
}

void EndMenu::checkWidgets() {
    if (kToMainMenu_) {
        kToMainMenu_ = false;
        menus::showWindow(ToMainConfirm::get());
    }
    else if (kOptions_) {
        kOptions_ = false;
        menus::showWindow(OptionsMenu::get());
    }
    else if (kNew_) {
        kNew_ = false;
        menus::hideWindow();
        games::restart();
    }
    else if (kHide_) {
        kHide_ = false;
        if (settings::C_showInfoHide)
            menus::showWindow(InfoHide::get());
        else
            menus::hideMenu();
    }
}

void EndMenu::reset() {
    if (instance_)
        delete instance_;
    instance_ = NULL;
}


