/* InfoDM.cpp

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

# include "Menu/InfoGIP.hpp"

# include "Interface/UiWindow.hpp"
# include "Interface/Button.hpp"
# include "Menu/menus.hpp"
# include "Interface/TextBox.hpp"
# include "Interface/Checkbox.hpp"
# include "Interface/Line.hpp"
# include "Media/text.hpp"
# include "System/settings.hpp"
# include "Locales/locales.hpp"

UiWindow* InfoGIP::instance_(NULL);
bool InfoGIP::kClose_(false);

UiWindow* InfoGIP::get() {
    if (instance_ == NULL) {
        instance_ = new InfoGIP(320, 300);
		  instance_->addWidget(new Button(_("Close"), NULL, &kClose_, Vector2f(220,270), 90, 20));
        instance_->addWidget(new Label(new sf::String("Grave-Itation Pit"), TEXT_ALIGN_LEFT, Vector2f(10,10), 20.f, Color3f(1.f, 0.5f, 0.9f), false));
		  instance_->addWidget(new Label(_("Info"), TEXT_ALIGN_RIGHT, Vector2f(310,18), 12.f, Color3f(1.f, 0.5f, 0.9f), false));
        instance_->addWidget(new Line(Vector2f(10, 35), Vector2f(310, 35)));
		  instance_->addWidget(new TextBox(_("Destroy es many enemies as possible!"), Vector2f(10,40), 300, 30, Color3f(1.f, 0.7f, 0.9f)));
		  instance_->addWidget(new TextBox(
										  _("As the GREAT WAR reduces your resources drastically, scientists were engaged "
											 "to find a way of efficiently destroying your enemy by limiting the given input. "
											 "They did a lot of research and eventually created a new weapon with an incredible property: "
											 "It kills instantly, but launched projectiles are really affine to any gravitation "
											 "and they're quite difficult to handle. Therefore they called it: INSTA-GRAVE!\n\n"
											 "Unfortunately someone stole the technology, so your enemies can use this evil weapon, too. "
											 "Now it is your turn: Defeat those thieves in the great GRAVE-ITATION-PIT to regain the upper hand.\n\n"
											 "Win this battle, and the victory of the GREAT WAR may be yours."),
										  Vector2f(10, 80), 300, 160));
		  instance_->addWidget(new Checkbox(_("Show this info again."), NULL, &settings::C_showInfoDM, Vector2f(10,270), 170));
    }
    return instance_;
}

void InfoGIP::checkWidgets() {
    if (kClose_) {
        kClose_ = false;
        menus::hideWindow();
        settings::save();
    }
}

void InfoGIP::onShow() {
    settings::C_showInfoDM = false;
}

void InfoGIP::reset() {
    if (instance_)
        delete instance_;
    instance_ = NULL;
}

