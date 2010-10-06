/* NewGameConfirm.cpp

Copyright (c) 2010 by Felix Lauer und Simon Schneegans

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

# include "Menu/NewGameConfirm.hpp"

# include "Interface/UiWindow.hpp"
# include "Interface/Button.hpp"
# include "System/window.hpp"
# include "Menu/menus.hpp"
# include "Games/games.hpp"
# include "Media/text.hpp"
# include "Locales/locales.hpp"

UiWindow* NewGameConfirm::instance_(NULL);
bool NewGameConfirm::kOk_(false);
bool NewGameConfirm::kCancel_(false);

UiWindow* NewGameConfirm::get() {
    if (instance_ == NULL) {
        instance_ = new NewGameConfirm(280, 80);
        instance_->addWidget(new Label(locales::getLocale(locales::QuitCurrentGameText), TEXT_ALIGN_LEFT, Vector2f(10, 8)));
        instance_->addWidget(new Button(locales::getLocale(locales::Ok), &kOk_, Vector2f(200,50), 70, 20));
        instance_->addWidget(new Button(locales::getLocale(locales::Cancel), &kCancel_, Vector2f(120,50), 70, 20));
    }
    return instance_;
}

void NewGameConfirm::checkWidgets() {
    if (kOk_) {
        kOk_ = false;
        menus::hideWindow();
        menus::hideWindow();
        games::restart();
    }
    else if (kCancel_) {
        kCancel_ = false;
        menus::hideWindow();
    }
}


