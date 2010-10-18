/* keyNames.cpp

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


# include "System/keyNames.hpp"

# include "Locales/locales.hpp"

namespace keyNames {
    sf::String const key(sf::Key::Code keyCode) {
        sf::String key;
        if (static_cast<int>(keyCode) > 32 && static_cast<int>(keyCode) < 126) key = static_cast<char>(keyCode);

        else if (keyCode == sf::Key::Add) key =         *locales::getLocale(locales::Add);
        else if (keyCode == sf::Key::BackSlash) key =   *locales::getLocale(locales::BackSlash);
        else if (keyCode == sf::Key::Comma) key =       *locales::getLocale(locales::Comma);
        else if (keyCode == sf::Key::Dash) key =        *locales::getLocale(locales::Dash);
        else if (keyCode == sf::Key::Delete) key =      *locales::getLocale(locales::Delete);
        else if (keyCode == sf::Key::Divide) key =      *locales::getLocale(locales::Divide);
        else if (keyCode == sf::Key::Down) key =        *locales::getLocale(locales::Down);
        else if (keyCode == sf::Key::End) key =         *locales::getLocale(locales::End);
        else if (keyCode == sf::Key::Equal) key =       *locales::getLocale(locales::Equal);
        else if (keyCode == sf::Key::F1) key =          "F1";
        else if (keyCode == sf::Key::F2) key =          "F2";
        else if (keyCode == sf::Key::F3) key =          "F3";
        else if (keyCode == sf::Key::F4) key =          "F4";
        else if (keyCode == sf::Key::F5) key =          "F5";
        else if (keyCode == sf::Key::F6) key =          "F6";
        else if (keyCode == sf::Key::F7) key =          "F7";
        else if (keyCode == sf::Key::F8) key =          "F8";
        else if (keyCode == sf::Key::F9) key =          "F9";
        else if (keyCode == sf::Key::F10) key =         "F10";
        else if (keyCode == sf::Key::F11) key =         "F11";
        else if (keyCode == sf::Key::F12) key =         "F12";
        else if (keyCode == sf::Key::Home) key =        *locales::getLocale(locales::Home);
        else if (keyCode == sf::Key::Insert) key =      *locales::getLocale(locales::Insert);
        else if (keyCode == sf::Key::LAlt) key =        *locales::getLocale(locales::LeftAlt);
        else if (keyCode == sf::Key::LBracket) key =    *locales::getLocale(locales::LeftBracket);
        else if (keyCode == sf::Key::LControl) key =    *locales::getLocale(locales::LeftControl);
        else if (keyCode == sf::Key::Left) key =        *locales::getLocale(locales::Left);
        else if (keyCode == sf::Key::LShift) key =      *locales::getLocale(locales::LeftShift);
        else if (keyCode == sf::Key::LSystem) key =     *locales::getLocale(locales::LeftSuper);
        else if (keyCode == sf::Key::Menu) key =        *locales::getLocale(locales::Menu);
        else if (keyCode == sf::Key::Multiply) key =    *locales::getLocale(locales::Multiply);
        else if (keyCode == sf::Key::Numpad0) key =     "Num 0";
        else if (keyCode == sf::Key::Numpad1) key =     "Num 1";
        else if (keyCode == sf::Key::Numpad2) key =     "Num 2";
        else if (keyCode == sf::Key::Numpad3) key =     "Num 3";
        else if (keyCode == sf::Key::Numpad4) key =     "Num 4";
        else if (keyCode == sf::Key::Numpad5) key =     "Num 5";
        else if (keyCode == sf::Key::Numpad6) key =     "Num 6";
        else if (keyCode == sf::Key::Numpad7) key =     "Num 7";
        else if (keyCode == sf::Key::Numpad8) key =     "Num 8";
        else if (keyCode == sf::Key::Numpad9) key =     "Num 9";
        else if (keyCode == sf::Key::PageDown) key =    *locales::getLocale(locales::PageDown);
        else if (keyCode == sf::Key::PageUp) key =      *locales::getLocale(locales::PageUp);
        else if (keyCode == sf::Key::Pause) key =       *locales::getLocale(locales::Pause);
        else if (keyCode == sf::Key::Period) key =      *locales::getLocale(locales::Period);
        else if (keyCode == sf::Key::Quote) key =       *locales::getLocale(locales::Quote);
        else if (keyCode == sf::Key::RAlt) key =        *locales::getLocale(locales::RightAlt);
        else if (keyCode == sf::Key::RBracket) key =    *locales::getLocale(locales::RightBracket);
        else if (keyCode == sf::Key::RControl) key =    *locales::getLocale(locales::RightControl);
        else if (keyCode == sf::Key::Return) key =      *locales::getLocale(locales::Return);
        else if (keyCode == sf::Key::Right) key =       *locales::getLocale(locales::Right);
        else if (keyCode == sf::Key::RShift) key =      *locales::getLocale(locales::RightShift);
        else if (keyCode == sf::Key::RSystem) key =     *locales::getLocale(locales::RightSuper);
        else if (keyCode == sf::Key::SemiColon) key =   *locales::getLocale(locales::SemiColon);
        else if (keyCode == sf::Key::Slash) key =       *locales::getLocale(locales::Slash);
        else if (keyCode == sf::Key::Space) key =       *locales::getLocale(locales::Space);
        else if (keyCode == sf::Key::Subtract) key =    *locales::getLocale(locales::Subtract);
        else if (keyCode == sf::Key::Tilde) key =       *locales::getLocale(locales::Tilde);
        else if (keyCode == sf::Key::Tab) key =         *locales::getLocale(locales::Tab);
        else if (keyCode == sf::Key::Up) key =          *locales::getLocale(locales::Up);
        return key;
    }
}




