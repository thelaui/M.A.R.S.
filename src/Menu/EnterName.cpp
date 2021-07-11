/* EnterName.cpp

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

# include "Menu/EnterName.hpp"

# include "Interface/UiWindow.hpp"
# include "Interface/Button.hpp"
# include "Interface/Checkbox.hpp"
# include "Menu/menus.hpp"
# include "Interface/TextEdit.hpp"
# include "Interface/TextBox.hpp"
# include "Interface/Line.hpp"
# include "System/generateName.hpp"
# include "System/settings.hpp"
# include "Media/text.hpp"
# include "Locales/locales.hpp"

UiWindow* EnterName::instance_(NULL);
bool EnterName::kOk_(false);

UiWindow* EnterName::get() {
    if (instance_ == NULL) {
        instance_ = new EnterName(320, 160);
        instance_->addWidget(new TextEdit(NULL, &settings::C_playerIName, "PlayerI", Vector2f(10,85), 300, 0, TEXT_EDIT, 12));
		  instance_->addWidget(new Button(_("OK"), NULL, &kOk_, Vector2f(220,130), 90, 20));
		  /** TRANSLATORS: %s is the player's name.*/
		  instance_->addWidget(new Label(locales::format_string(gettext("Hey, %s!"), reinterpret_cast<const char*>(settings::C_playerIName.toUtf8().c_str())), TEXT_ALIGN_LEFT, Vector2f(10,10), 20.f, Color3f(1.f, 0.5f, 0.9f), false));
		  instance_->addWidget(new Label(_("Info"), TEXT_ALIGN_RIGHT, Vector2f(310,18), 12.f, Color3f(1.f, 0.5f, 0.9f), false));
        instance_->addWidget(new Line(Vector2f(10, 35), Vector2f(310, 35)));
		  instance_->addWidget(new TextBox(_("That's a bad name... Feel free to enter a better one!"), Vector2f(10, 40), 300, 30));
    }
    return instance_;
}

void EnterName::checkWidgets() {
    if (kOk_) {
        kOk_ = false;
        menus::hideWindow();
		  locales::load();
		  locales::setCurrentLocale();
        menus::reload();
        settings::save();
    }
}

void EnterName::onShow() {
    settings::C_showInfoHide = false;
}

void EnterName::reset() {
    if (instance_)
        delete instance_;
    instance_ = NULL;
}

