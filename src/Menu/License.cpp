/* License.cpp

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

# include "Menu/License.hpp"

# include "Interface/UiWindow.hpp"
# include "Interface/Button.hpp"
# include "Menu/menus.hpp"
# include "Interface/TextBox.hpp"
# include "Interface/Line.hpp"
# include "System/generateName.hpp"
# include "Media/text.hpp"
# include "Locales/locales.hpp"

UiWindow* License::instance_(NULL);
bool License::kClose_(false);

UiWindow* License::get() {
    if (instance_ == NULL) {
        instance_ = new License(400, 300);
        instance_->addWidget(new Button(locales::getLocale(locales::Close), &kClose_, Vector2f(320,270), 70, 20));
        instance_->addWidget(new Label(locales::getLocale(locales::License), TEXT_ALIGN_LEFT, Vector2f(10,10), 20.f));
        instance_->addWidget(new Label(new sf::String("GNU v3"), TEXT_ALIGN_RIGHT, Vector2f(390,18), 12.f));
        instance_->addWidget(new Line(Vector2f(10, 35), Vector2f(390, 35)));
        instance_->addWidget(new TextBox(locales::getLocale(locales::LicenseText), Vector2f(10, 50), 380, 100));
    }
    return instance_;
}

void License::checkWidgets() {
    if (kClose_) {
        kClose_ = false;
        menus::hideWindow();
    }
}






