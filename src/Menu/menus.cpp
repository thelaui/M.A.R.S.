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
# include "Menu/InfoCK.hpp"
# include "Menu/InfoSB.hpp"
# include "Menu/InfoDM.hpp"
# include "Menu/InfoTDM.hpp"
# include "Menu/InfoHide.hpp"
# include "Menu/MainMenu.hpp"
# include "Menu/PauseMenu.hpp"
# include "Menu/ExitConfirm.hpp"
# include "Interface/toolTip.hpp"
# include "Tutorial/TutWindow01.hpp"
# include "Tutorial/TutWindow02.hpp"
# include "Tutorial/TutWindow03.hpp"
# include "Tutorial/TutWindow04.hpp"
# include "Tutorial/TutWindow05.hpp"
# include "Tutorial/TutWindow06.hpp"
# include "Tutorial/TutWindow07.hpp"
# include "Tutorial/TutWindow08.hpp"
# include "Tutorial/TutWindow09.hpp"
# include "Tutorial/TutWindow10.hpp"
# include "Tutorial/TutWindow11.hpp"
# include "Tutorial/TutWindow12.hpp"
# include "Tutorial/TutWindow13.hpp"
# include "Tutorial/TutWindow14.hpp"
# include "Tutorial/TutWindow15.hpp"
# include "Tutorial/TutWindow16.hpp"
# include "Tutorial/TutWindow17.hpp"
# include "Tutorial/TutWindow18.hpp"
# include "Tutorial/TutWindow19.hpp"
# include "Tutorial/TutWindow20.hpp"
# include "defines.hpp"

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
        if (!visible())
            showWindow(PauseMenu::get());
    }

    void draw() {
        if (visible() && !hidden_) {
            for (std::vector<UiWindow*>::iterator it = windowStack_.begin(); it != windowStack_.end(); ++it)
                (*it)->draw();

            Vector2f viewPort = window::getViewPort();
            text::drawScreenText(sf::String("M.A.R.S. " + sf::String(VERSION_MAJOR) + "." + sf::String(VERSION_MINOR) + "." + sf::String(VERSION_PATCH)
                                            + " - " + sf::String(__DATE__)), Vector2f(viewPort.x_-4.f, viewPort.y_-14.f) , 11.f, TEXT_ALIGN_RIGHT, Color3f(0.8, 0.8, 0.8));

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

    void mouseLeft(bool down) {
        if (visible()) {
            windowStack_.back()->mouseLeft(down);
            if (!down) windowStack_.back()->checkWidgets();
        }
    }

    void keyEvent(bool down, sf::Key::Code keyCode) {
        if (visible()) {
            if (keyboardFixTarget_) {
                keyboardFixTarget_->keyEvent(down, keyCode);
                windowStack_.back()->checkWidgets();
            }
            else if (down && keyCode == sf::Key::Escape) {
                if (hidden_) {
                    hidden_ = false;
                    window::showCursor(true);
                }
                else if (windowStack_.back() == MainMenu::get())
                    showWindow(ExitConfirm::get());
                else
                    hideWindow();
            }
            else if (down && ((keyCode == sf::Key::Tab && (window::getInput().IsKeyDown(sf::Key::LControl) || window::getInput().IsKeyDown(sf::Key::RControl)))
             || (keyCode == sf::Key::Tab && (window::getInput().IsKeyDown(sf::Key::LShift) || window::getInput().IsKeyDown(sf::Key::RShift)))
             || (keyCode == sf::Key::Up))) {
                windowStack_.back()->tabPrevious();
            }
            else if (down && (keyCode == sf::Key::Tab || keyCode == sf::Key::Down)) {
                windowStack_.back()->tabNext();
            }
            else {
                windowStack_.back()->keyEvent(down, keyCode);
                windowStack_.back()->checkWidgets();
            }
        }
        else if (down && keyCode == sf::Key::Escape)
            showPause();
    }

    void textEntered(int keyCode) {
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
        Connect::reset();
        InfoCK::reset();
        InfoDM::reset();
        InfoHide::reset();
        InfoSB::reset();
        InfoTDM::reset();
        TutWindow01::reset();
        TutWindow02::reset();
        TutWindow03::reset();
        TutWindow04::reset();
        TutWindow05::reset();
        TutWindow06::reset();
        TutWindow07::reset();
        TutWindow08::reset();
        TutWindow09::reset();
        TutWindow10::reset();
        TutWindow11::reset();
        TutWindow12::reset();
        TutWindow13::reset();
        TutWindow14::reset();
        TutWindow15::reset();
        TutWindow16::reset();
        TutWindow17::reset();
        TutWindow18::reset();
        TutWindow19::reset();
        TutWindow20::reset();
    }

    bool visible() {
        return !windowStack_.empty();
    }
}
