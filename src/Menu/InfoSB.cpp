/* InfoSB.cpp

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

# include "Menu/InfoSB.hpp"

# include "Interface/UiWindow.hpp"
# include "Interface/Button.hpp"
# include "Menu/menus.hpp"
# include "Interface/TextBox.hpp"
# include "Interface/Checkbox.hpp"
# include "Interface/Line.hpp"
# include "Media/text.hpp"
# include "System/settings.hpp"
# include "Locales/locales.hpp"

UiWindow* InfoSB::instance_(NULL);
bool InfoSB::kClose_(false);

UiWindow* InfoSB::get() {
    if (instance_ == NULL) {
        instance_ = new InfoSB(320, 300);
		  instance_->addWidget(new Button(_("Close"), NULL, &kClose_, Vector2f(220,270), 90, 20));
        instance_->addWidget(new Label(new sf::String("SpaceBall"), TEXT_ALIGN_LEFT, Vector2f(10,10), 20.f, Color3f(1.f, 0.5f, 0.9f), false));
		  instance_->addWidget(new Label(_("Info"), TEXT_ALIGN_RIGHT, Vector2f(310,18), 12.f, Color3f(1.f, 0.5f, 0.9f), false));
        instance_->addWidget(new Line(Vector2f(10, 35), Vector2f(310, 35)));
		  instance_->addWidget(new TextBox(_("Kick or shoot the ball to your enemy's planet!"), Vector2f(10,40), 300, 30, Color3f(1.f, 0.7f, 0.9f)));
		  instance_->addWidget(new TextBox(
										  _("In year 3547, civilizations all over the galaxy have settled their own "
											 "planets, living in peace and harmony with its environment. \n"
											 "But outside the contemplative habitats, the GREAT WAR is raging. \n"
											 "As a famous fighter on your way to never ending honor and prosperity, you "
											 "have to protect your planet from the oncoming doom of your jealous \n"
											 "neighbours! Fight the battle, a true hero as you are was born for: "
											 "SPACEBALL. \n"
											 "Protect your planet from the sphere of death and defeat your enemies by "
											 "letting it crush into their ridiculous globe."),
										  Vector2f(10, 80), 300, 160));
		  instance_->addWidget(new Checkbox(_("Show this info again."), NULL, &settings::C_showInfoSB, Vector2f(10,270), 170));
    }
    return instance_;
}

void InfoSB::checkWidgets() {
    if (kClose_) {
        kClose_ = false;
        menus::hideWindow();
        settings::save();
    }
}

void InfoSB::onShow() {
    settings::C_showInfoSB = false;
}

void InfoSB::reset() {
    if (instance_)
        delete instance_;
    instance_ = NULL;
}
