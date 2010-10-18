/* mainMenu.cpp

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

# include "Menu/MainMenu.hpp"

# include "Interface/UiWindow.hpp"
# include "Interface/Button.hpp"
# include "Menu/menus.hpp"
# include "Menu/ExitConfirm.hpp"
# include "Menu/OptionsMenu.hpp"
# include "Menu/NewGameMenu.hpp"
# include "Menu/About.hpp"
# include "Menu/Connect.hpp"
# include "Menu/InfoSB.hpp"
# include "System/settings.hpp"
# include "Locales/locales.hpp"
# include "Games/games.hpp"

UiWindow* MainMenu::instance_(NULL);
bool MainMenu::kStartLocal_(false);
bool MainMenu::kStartTut_(false);
bool MainMenu::kStartMulti_(false);
bool MainMenu::kJoinMulti_(false);
bool MainMenu::kOptions_(false);
bool MainMenu::kAbout_(false);
bool MainMenu::kExit_(false);

UiWindow* MainMenu::get() {
    if (instance_ == NULL) {
        instance_ = new MainMenu(180, 220, Vector2f(0.f, 50.f));
        instance_->addWidget(new Button(locales::getLocale(locales::StartLocalGame),   &kStartLocal_,  Vector2f(10,10),  160, 20));
        instance_->addWidget(new Button(locales::getLocale(locales::StartTutorial),    &kStartTut_,    Vector2f(10,40),  160, 20));
        instance_->addWidget(new Button(locales::getLocale(locales::StartNetworkGame), &kStartMulti_,  Vector2f(10,70),  160, 20));
        instance_->addWidget(new Button(locales::getLocale(locales::JoinNetworkGame),  &kJoinMulti_,   Vector2f(10,100), 160, 20));
        instance_->addWidget(new Button(locales::getLocale(locales::Options),          &kOptions_,     Vector2f(10,130), 160, 20));
        instance_->addWidget(new Button(locales::getLocale(locales::About),            &kAbout_,       Vector2f(10,160), 160, 20));
        instance_->addWidget(new Button(locales::getLocale(locales::Quit),             &kExit_,        Vector2f(10,190), 160, 20));
    }
    return instance_;
}

void MainMenu::checkWidgets() {
    if (kExit_) {
        kExit_ = false;
        menus::showWindow(ExitConfirm::get());
    }
    else if (kAbout_) {
        kAbout_ = false;
        menus::showWindow(About::get());
    }
    else if (kOptions_) {
        kOptions_ = false;
        menus::showWindow(OptionsMenu::get());
    }
    else if (kStartLocal_) {
        kStartLocal_ = false;
        menus::showWindow(NewGameMenu::get());
        if(settings::C_showInfoSB)
            menus::showWindow(InfoSB::get());
    }
    else if (kStartTut_) {
        kStartTut_ = false;
        menus::hideWindow();
        games::start(games::gTutorial);
    }
    else if (kJoinMulti_) {
        kJoinMulti_ = false;
        menus::showWindow(Connect::get());
    }
    else if (kStartMulti_) {
        kStartMulti_ = false;
        menus::showWindow(Connect::get());
    }
}

