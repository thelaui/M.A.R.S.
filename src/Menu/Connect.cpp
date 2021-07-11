/* Connect.cpp

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

# include "Menu/Connect.hpp"

# include "Interface/UiWindow.hpp"
# include "Interface/Button.hpp"
# include "Menu/menus.hpp"
# include "Interface/TextBox.hpp"
# include "Interface/Line.hpp"
# include "Media/text.hpp"
# include "Locales/locales.hpp"

UiWindow* Connect::instance_(NULL);
bool Connect::kClose_(false);

UiWindow* Connect::get() {
    if (instance_ == NULL) {
        instance_ = new Connect(320, 180);
		  instance_->addWidget(new Button(_("Close"), NULL, &kClose_, Vector2f(220,150), 90, 20));
		  instance_->addWidget(new Label(_("Start Network Game"), TEXT_ALIGN_LEFT, Vector2f(10,10), 20.f, Color3f(1.f, 0.5f, 0.9f), false));
        instance_->addWidget(new Label(new sf::String("TODO"), TEXT_ALIGN_RIGHT, Vector2f(310,18), 12.f, Color3f(1.f, 0.5f, 0.9f), false));
        instance_->addWidget(new Line(Vector2f(10, 35), Vector2f(310, 35)));
		  instance_->addWidget(new TextBox(
										  _("M.A.R.S. gaming over the world wide web or a local area network is a "
											 "planned feature, but not implemented yet.\n\n"
											 "Stay tuned!"),
										  Vector2f(10, 50), 300, 90));
    }
    return instance_;
}

void Connect::checkWidgets() {
    if (kClose_) {
        kClose_ = false;
        menus::hideWindow();
    }
}

void Connect::reset() {
    if (instance_)
        delete instance_;
    instance_ = NULL;
}





