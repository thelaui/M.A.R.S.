/* ToMainConfirm.cpp

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

# include "Menu/ToMainConfirm.hpp"

# include "Media/text.hpp"
# include "Interface/UiWindow.hpp"
# include "Interface/Button.hpp"
# include "System/window.hpp"
# include "Menu/menus.hpp"
# include "Games/games.hpp"
# include "Locales/locales.hpp"

UiWindow* ToMainConfirm::instance_(NULL);
bool ToMainConfirm::kOk_(false);
bool ToMainConfirm::kCancel_(false);

UiWindow* ToMainConfirm::get() {
    if (instance_ == NULL) {
        instance_ = new ToMainConfirm(280, 80);
        instance_->addWidget(new Button(locales::getLocale(locales::Ok),     NULL, &kOk_, Vector2f(180,50), 90, 20));
        instance_->addWidget(new Button(locales::getLocale(locales::Cancel), NULL, &kCancel_, Vector2f(80,50), 90, 20));
        instance_->addWidget(new Label(locales::getLocale(locales::QuitCurrentGameText), TEXT_ALIGN_LEFT, Vector2f(10, 8), 12.f, Color3f(0.7f, 0.7f, 0.7f), false));
    }
    return instance_;
}

void ToMainConfirm::checkWidgets() {
    if (kOk_) {
        kOk_ = false;
        menus::hideWindow();
        menus::hideWindow();
        games::start(games::gMenu);
        music::fadeOut();
    }
    else if (kCancel_) {
        kCancel_ = false;
        menus::hideWindow();
    }
}

void ToMainConfirm::reset() {
    if (instance_)
        delete instance_;
    instance_ = NULL;
}



