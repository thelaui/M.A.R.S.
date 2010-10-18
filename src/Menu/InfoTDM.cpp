/* InfoTDM.cpp

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

# include "Menu/InfoTDM.hpp"

# include "Interface/UiWindow.hpp"
# include "Interface/Button.hpp"
# include "Menu/menus.hpp"
# include "Interface/TextBox.hpp"
# include "Interface/Checkbox.hpp"
# include "Interface/Line.hpp"
# include "Media/text.hpp"
# include "System/settings.hpp"
# include "Locales/locales.hpp"

UiWindow* InfoTDM::instance_(NULL);
bool InfoTDM::kClose_(false);
TextBox* InfoTDM::infoBox_(NULL);

UiWindow* InfoTDM::get() {
    if (instance_ == NULL) {
        instance_ = new InfoTDM(320, 300);
        instance_->addWidget(new Button(locales::getLocale(locales::Close), &kClose_, Vector2f(240,270), 70, 20));
        instance_->addWidget(new Label(new sf::String("Team-DeathMatch"), TEXT_ALIGN_LEFT, Vector2f(10,10), 20.f));
        instance_->addWidget(new Label(locales::getLocale(locales::Info), TEXT_ALIGN_RIGHT, Vector2f(310,18), 12.f));
        instance_->addWidget(new Line(Vector2f(10, 35), Vector2f(310, 35)));
        instance_->addWidget(new Label(locales::getLocale(locales::ShortDescriptionTDM), TEXT_ALIGN_LEFT, Vector2f(10,50), 12.f, Color3f(1.f, 0.7f, 0.9f)));
        infoBox_ = new TextBox(locales::getLocale(locales::Ok), Vector2f(10, 80), 300, 300);
        instance_->addWidget(infoBox_);
        instance_->addWidget(new Checkbox(locales::getLocale(locales::ShowAgainButton), &settings::C_showInfoTDM, Vector2f(10,270), 170));
    }
    return instance_;
}

void InfoTDM::checkWidgets() {
    if (kClose_) {
        kClose_ = false;
        menus::hideWindow();
        settings::save();
    }
}

void InfoTDM::onShow() {
    infoBox_->setText(locales::getLocale(locales::InfoTDM));
}
