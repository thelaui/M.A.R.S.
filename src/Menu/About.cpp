/* About.cpp

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

# include "Menu/About.hpp"

# include "Interface/UiWindow.hpp"
# include "Interface/Button.hpp"
# include "Menu/menus.hpp"
# include "Interface/TextBox.hpp"
# include "Interface/Line.hpp"
# include "System/generateName.hpp"
# include "Menu/License.hpp"
# include "Media/text.hpp"
# include "Locales/locales.hpp"

UiWindow* About::instance_(NULL);
bool About::kClose_(false);
bool About::kLisence_(false);
std::string About::marsName_("");

UiWindow* About::get() {
    if (instance_ == NULL) {
        instance_ = new About(320, 210);
        instance_->addWidget(new Button(locales::getLocale(locales::Close), &kClose_, Vector2f(240,180), 70, 20));
        instance_->addWidget(new Button(locales::getLocale(locales::License), &kLisence_, Vector2f(10,180), 70, 20));
        instance_->addWidget(new Label(new std::string("M.A.R.S."), TEXT_ALIGN_LEFT, Vector2f(10,10), 20.f));
        instance_->addWidget(new Label(&marsName_, TEXT_ALIGN_RIGHT, Vector2f(310,18), 12.f));
        instance_->addWidget(new Line(Vector2f(10, 35), Vector2f(310, 35)));
        instance_->addWidget(new TextBox(locales::getLocale(locales::AboutText), Vector2f(10, 50), 300, 150));
    }
    return instance_;
}

void About::checkWidgets() {
    if (kClose_) {
        kClose_ = false;
        menus::hideWindow();
    }
    if (kLisence_) {
        kLisence_ = false;
        menus::showWindow(License::get());
    }
}

void About::onShow() {
    marsName_ = "A " + generateName::game();
}





