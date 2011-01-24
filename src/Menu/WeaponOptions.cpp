/* ShaderError.cpp

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

# include "Menu/WeaponOptions.hpp"

# include "Interface/Line.hpp"
# include "Interface/UiWindow.hpp"
# include "Interface/Button.hpp"
# include "System/window.hpp"
# include "Menu/menus.hpp"
# include "Locales/locales.hpp"

UiWindow* WeaponOptions::instance_(NULL);
bool WeaponOptions::kOk_(false);

UiWindow* WeaponOptions::get() {
    if (instance_ == NULL) {
        instance_ = new WeaponOptions(350, 300);
        instance_->addWidget(new Button(locales::getLocale(locales::Ok),     NULL, &kOk_, Vector2f(270,270), 70, 20));
        instance_->addWidget(new Label(locales::getLocale(locales::WeaponOptions), TEXT_ALIGN_LEFT, Vector2f(10,10), 20.f, Color3f(1.f, 0.5f, 0.9f), false));
        instance_->addWidget(new Line(Vector2f(10, 35), Vector2f(310, 35)));

        instance_->addWidget(new Label(locales::getLocale(locales::Weapons), TEXT_ALIGN_LEFT, Vector2f(10, 50), 12.f, Color3f(1.f, 0.5f, 0.9f), false));
        instance_->addWidget(new Label(locales::getLocale(locales::Specials), TEXT_ALIGN_LEFT, Vector2f(160, 50), 12.f, Color3f(1.f, 0.5f, 0.9f), false));


    }
    return instance_;
}

void WeaponOptions::checkWidgets() {
    if (kOk_) {
        kOk_ = false;
        menus::hideWindow();
    }
}

void WeaponOptions::reset() {
    if (instance_)
        delete instance_;
    instance_ = NULL;
}





