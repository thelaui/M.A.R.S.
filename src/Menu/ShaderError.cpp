/* ShaderError.cpp

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

# include "Menu/ShaderError.hpp"

# include "Media/text.hpp"
# include "Interface/UiWindow.hpp"
# include "Interface/Button.hpp"
# include "System/window.hpp"
# include "Menu/menus.hpp"
# include "Games/games.hpp"
# include "Locales/locales.hpp"

UiWindow* ShaderError::instance_(NULL);
bool ShaderError::kOk_(false);

UiWindow* ShaderError::get() {
    if (instance_ == NULL) {
        instance_ = new ShaderError(350, 80);
		  instance_->addWidget(new Button(_("OK"),     NULL, &kOk_, Vector2f(250,50), 90, 20));
		  instance_->addWidget(new Label(_("Sadly, shaders are not supported by your hardware!"), TEXT_ALIGN_LEFT, Vector2f(10, 8)));
    }
    return instance_;
}

void ShaderError::checkWidgets() {
    if (kOk_) {
        kOk_ = false;
        menus::hideWindow();
    }
}

void ShaderError::reset() {
    if (instance_)
        delete instance_;
    instance_ = NULL;
}




