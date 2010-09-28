/* InfoDM.cpp

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

# include "Menu/InfoDM.hpp"

# include "Interface/UiWindow.hpp"
# include "Interface/Button.hpp"
# include "Menu/menus.hpp"
# include "Interface/TextBox.hpp"
# include "Interface/Line.hpp"
# include "Media/text.hpp"

UiWindow* InfoDM::instance_(NULL);
bool InfoDM::kClose_(false);

UiWindow* InfoDM::get() {
    if (instance_ == NULL) {
        instance_ = new InfoDM(320, 400);
        instance_->addWidget(new Button("Close", &kClose_, Vector2f(240,370), 70, 20));
        instance_->addWidget(new Label("DeathMatch", TEXT_ALIGN_LEFT, Vector2f(10,10), 20.f));
        instance_->addWidget(new Label("Rules and Infos", TEXT_ALIGN_RIGHT, Vector2f(310,18), 12.f));
        instance_->addWidget(new Line(Vector2f(10, 35), Vector2f(310, 35)));
        instance_->addWidget(new TextBox("As a result of the GREAT WAR you have lost everything: your home, your family and \nyour self-esteem. \nYour last chance to get out of the miserable life you are stuck in, is to become a gladiator in the glorious \nDEATHMATCH-arena. \nResist the other fighters and earn points by destroying the nutshells, they call ships. \nPrepare yourself! \nDeath and Glory awaits you. \n \nUse the keys you configured in options menu for controlling your ship. \nTo change your weapon, land on your planet, press \nFIRE and then navigate through the weapons by pressing LEFT or RIGHT. \nUse TAB to show statistics about the current game.", Vector2f(10, 50), 300, 300));
    }
    return instance_;
}

void InfoDM::checkWidgets() {
    if (kClose_) {
        kClose_ = false;
        menus::hideWindow();
    }
}






