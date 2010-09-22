/* InfoTDM.cpp

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
# include "Interface/Line.hpp"

UiWindow* InfoTDM::instance_(NULL);
bool InfoTDM::kClose_(false);

UiWindow* InfoTDM::get() {
    if (instance_ == NULL) {
        instance_ = new InfoTDM(320, 400);
        instance_->addWidget(new Button("Close", &kClose_, Vector2f(240,370), 70, 20));
        instance_->addWidget(new Label("Team-DeathMatch", TEXT_ALIGN_LEFT, Vector2f(10,10), 20.f));
        instance_->addWidget(new Label("Rules and Infos", TEXT_ALIGN_RIGHT, Vector2f(310,18), 12.f));
        instance_->addWidget(new Line(Vector2f(10, 35), Vector2f(310, 35)));
        instance_->addWidget(new TextBox("The negotiations have failed!\nBoth of you and your neighbours had to record enormous losses in conclusion of the abiding GREAT WAR.\nAs a last attempt of gaining the upper hand, you are ordered to decimate your enemy's resources and to protect your own one's. The only way to do so is an epic TEAMDEATHMATCH.\nProtect your comrades and destroy your enemies!\nTeach them a lesson, they will never forget.\n\nUse the keys you configured in options menu for controlling your ship.\nTo change your weapon, land on your planet, press\nFIRE and then navigate through the weapons by pressing LEFT or RIGHT.\nUse TAB to show statistics about the current game.", Vector2f(10, 50), 300, 300));
    }
    return instance_;
}

void InfoTDM::checkWidgets() {
    if (kClose_) {
        kClose_ = false;
        menus::hideWindow();
    }
}






