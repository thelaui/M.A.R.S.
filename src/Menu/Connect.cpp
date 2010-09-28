/* Connect.cpp

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

UiWindow* Connect::instance_(NULL);
bool Connect::kClose_(false);

UiWindow* Connect::get() {
    if (instance_ == NULL) {
        instance_ = new Connect(320, 180);
        instance_->addWidget(new Button("Close", &kClose_, Vector2f(240,150), 70, 20));
        instance_->addWidget(new Label("Network Gaming", TEXT_ALIGN_LEFT, Vector2f(10,10), 20.f));
        instance_->addWidget(new Label("TODO", TEXT_ALIGN_RIGHT, Vector2f(310,18), 12.f));
        instance_->addWidget(new Line(Vector2f(10, 35), Vector2f(310, 35)));
        instance_->addWidget(new TextBox("M.A.R.S. gaming over the world wide web or a local area network is a planned feature, but not implemented yet. \n\nStay tuned!", Vector2f(10, 50), 300, 300));
    }
    return instance_;
}

void Connect::checkWidgets() {
    if (kClose_) {
        kClose_ = false;
        menus::hideWindow();
    }
}







