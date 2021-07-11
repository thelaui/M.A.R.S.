/*menus.cpp

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
this program.  If not, see <http://www.gnu.org/licenses/>.*/

# include "Menu/menus.hpp"

# include "Media/text.hpp"
# include "System/window.hpp"
# include "Menu/ChooseLanguage.hpp"
# include "Menu/ShaderError.hpp"
# include "Menu/About.hpp"
# include "Menu/Connect.hpp"
# include "Menu/EnterName.hpp"
# include "Menu/InfoCK.hpp"
# include "Menu/InfoGIP.hpp"
# include "Menu/InfoSB.hpp"
# include "Menu/InfoDM.hpp"
# include "Menu/InfoTDM.hpp"
# include "Menu/InfoHide.hpp"
# include "Menu/MainMenu.hpp"
# include "Menu/NewGameConfirm.hpp"
# include "Menu/NewGameMenu.hpp"
# include "Menu/OptionsMenu.hpp"
# include "Menu/PauseMenu.hpp"
# include "Menu/ToMainConfirm.hpp"
# include "Menu/TutorialWindow.hpp"
# include "Menu/WeaponOptions.hpp"
# include "Menu/EndMenu.hpp"
# include "Menu/ExitConfirm.hpp"
# include "Interface/toolTip.hpp"
# include "defines.hpp"
# include "Games/games.hpp"

namespace menus {

    namespace {
        std::vector<UiWindow*> windowStack_;
        bool hidden_(false);
        UiElement* keyboardFixTarget_(NULL);
    }

    void showMain() {
        windowStack_.clear();
        showWindow(MainMenu::get());
    }

    void showPause() {
        if (!visible()) {
            if (games::ended())
                showWindow(EndMenu::get());
            else
                showWindow(PauseMenu::get());
        }
    }

    void draw() {
        if (visible() && !hidden_) {
            for (std::vector<UiWindow*>::iterator it = windowStack_.begin(); it != windowStack_.end(); ++it)
                (*it)->draw();

            Vector2f viewPort = window::getViewPort();
            text::drawScreenText(sf::String("M.A.R.S. ") + sf::String(VERSION_MAJOR) + "." + sf::String(VERSION_MINOR) + "." + sf::String(VERSION_PATCH),
                                            Vector2f(viewPort.x_-4.f, viewPort.y_-14.f) , 11.f, TEXT_ALIGN_RIGHT, Color3f(0.8, 0.8, 0.8));

            toolTip::draw();
        }
    }

    void mouseMoved(Vector2f const& position) {
        if (visible()) {
            toolTip::mouseMoved(position);
            windowStack_.back()->mouseMoved(position);
            windowStack_.back()->checkWidgets();
        }
    }

    void mouseWheelMoved(Vector2f const& position, int delta) {
        if (visible())
            windowStack_.back()->mouseWheelMoved(position, delta);
    }

    void mouseLeft(bool down) {
        if (visible()) {
            windowStack_.back()->mouseLeft(down);
            if (!down) windowStack_.back()->checkWidgets();
        }
    }

    void keyEvent(bool down, Key const& key) {
        if (down && key == settings::C_screenShotKey && !menus::keyboardFixed())
            window::screenShot();
        else if (down && key == settings::C_audioNextKey && !menus::keyboardFixed())
            music::next();
        else if (down && key == settings::C_audioPreviousKey && !menus::keyboardFixed())
            music::previous();
        if (visible()) {
            if (keyboardFixTarget_) {
                keyboardFixTarget_->keyEvent(down, key);
                windowStack_.back()->checkWidgets();
            }
            else if (down && key.navi_ == Key::nAbort) {
                if (hidden_) {
                    hidden_ = false;
                    window::showCursor(true);
                }
                else if (windowStack_.back() == MainMenu::get())
                    showWindow(ExitConfirm::get());
                else
                    hideWindow();
            }
            else if (down && key.navi_ == Key::nUp && !hidden_) {
                windowStack_.back()->tabPrevious();
            }
            else if (down && key.navi_ == Key::nDown && !hidden_) {
                windowStack_.back()->tabNext();
            }
            else if (!hidden_) {
                windowStack_.back()->keyEvent(down, key);
                windowStack_.back()->checkWidgets();
            }
        }
        else if (down && key.navi_ == Key::nAbort)
            showPause();
    }

    void textEntered(sf::Uint32 keyCode) {
        if (visible())
            windowStack_.back()->textEntered(keyCode);
    }

    void showWindow(UiWindow* toBeShown) {
        window::showCursor(true);
        toBeShown->onShow();
        toBeShown->setTopMost(true);
        if (visible()) windowStack_.back()->setTopMost(false);
        windowStack_.push_back(toBeShown);
    }

    void hideWindow() {
        windowStack_.pop_back();
        if (!visible()) window::showCursor(false);
        else windowStack_.back()->setTopMost(true);
    }

    void hideMenu() {
        window::showCursor(false);
        hidden_ = true;
    }

    void clearFocus() {
        if (visible()) windowStack_.back()->clearFocus();
    }

    void fixKeyboardOn(UiElement* target) {
        keyboardFixTarget_ = target;
    }

    void unFixKeyboard() {
        keyboardFixTarget_ = NULL;
    }

    bool keyboardFixed() {
        return keyboardFixTarget_;
    }

	 void reload() {
		  About::reset();
		  ChooseLanguage::reset();
		  Connect::reset();
		  EndMenu::reset();
		  EnterName::reset();
		  ExitConfirm::reset();
		  InfoCK::reset();
		  InfoDM::reset();
		  InfoGIP::reset();
		  InfoHide::reset();
		  InfoSB::reset();
		  InfoTDM::reset();
		  MainMenu::reset();
		  NewGameConfirm::reset();
		  NewGameMenu::reset();
		  OptionsMenu::reset();
		  PauseMenu::reset();
		  ShaderError::reset();
		  ToMainConfirm::reset();
		  TutorialWindow::reset();
		  WeaponOptions::reset();
	 }

    bool visible() {
        return !windowStack_.empty();
    }
}
