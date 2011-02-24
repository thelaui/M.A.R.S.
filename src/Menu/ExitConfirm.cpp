/* ExitConfirm.cpp

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

# include "Menu/ExitConfirm.hpp"

# include "Interface/UiWindow.hpp"
# include "Interface/Button.hpp"
# include "System/window.hpp"
# include "Menu/menus.hpp"
# include "Media/text.hpp"
# include "Locales/locales.hpp"
# include "System/settings.hpp"

UiWindow* ExitConfirm::instance_(NULL);
bool ExitConfirm::kOk_(false);
bool ExitConfirm::kCancel_(false);

UiWindow* ExitConfirm::get() {
    if (instance_ == NULL) {
        instance_ = new ExitConfirm(280, 80);
        instance_->addWidget(new Button(locales::getLocale(locales::Ok), NULL, &kOk_, Vector2f(200,50), 70, 20));
        instance_->addWidget(new Button(locales::getLocale(locales::Cancel), NULL, &kCancel_, Vector2f(120,50), 70, 20));
        instance_->addWidget(new Label(locales::getLocale(locales::QuitText), TEXT_ALIGN_LEFT, Vector2f(10, 8), 12.f, Color3f(0.65f, 0.65f, 0.65f), false));
    }
    return instance_;
}

void ExitConfirm::checkWidgets() {
    if (kOk_) {
        kOk_ = false;
        settings::save();
        window::close();
    }
    else if (kCancel_) {
        kCancel_ = false;
        menus::hideWindow();
    }
}

void ExitConfirm::reset() {
    if (instance_)
        delete instance_;
    instance_ = NULL;
}


