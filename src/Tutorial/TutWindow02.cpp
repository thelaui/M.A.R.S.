/* TutWindow02.cpp

Copyright (c) 2010 by Felix Lauer und Simon Schneegans

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

# include "Tutorial/TutWindow02.hpp"

# include "Interface/UiWindow.hpp"
# include "Interface/Button.hpp"
# include "Menu/menus.hpp"
# include "Media/text.hpp"
# include "Interface/TextBox.hpp"
# include "Interface/Line.hpp"
# include "Locales/locales.hpp"
# include "System/keyNames.hpp"
# include "System/settings.hpp"

UiWindow* TutWindow02::instance_(NULL);
bool TutWindow02::kOk_(false);
sf::String TutWindow02::text_("");

UiWindow* TutWindow02::get() {
    if (instance_ == NULL) {
        text_ = *locales::getLocale(locales::TutText02) + "\n<" + keyNames::key(settings::C_playerIup) + "> , <"
                + keyNames::key(settings::C_playerIleft) + "> , <" + keyNames::key(settings::C_playerIright)+">.";
        instance_ = new TutWindow02(350, 150);
        instance_->addWidget(new Button(locales::getLocale(locales::Next), &kOk_, Vector2f(270,120), 70, 20));
        instance_->addWidget(new Label(locales::getLocale(locales::Tut02), TEXT_ALIGN_LEFT, Vector2f(10,10), 20.f));
        instance_->addWidget(new Line(Vector2f(10, 35), Vector2f(340, 35)));
        instance_->addWidget(new TextBox(&text_, Vector2f(10, 50), 330, 60));
    }
    return instance_;
}

void TutWindow02::checkWidgets() {
    if (kOk_) {
        kOk_ = false;
        menus::hideWindow();
    }
}








