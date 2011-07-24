/* generateName.cpp

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

# include "System/generateName.hpp"

# include "Media/file.hpp"
# include "Locales/locales.hpp"
# include "System/settings.hpp"

# include <cstdlib>
# include <algorithm>
# include <list>
# include <iostream>
# include <ctime>
# include <sstream>

namespace generateName {

    namespace {
        std::vector<std::list<std::pair<sf::String, int> > > botNames_;
        std::list<sf::String> gameNames_;
        std::vector<sf::String> shipNames_;
        bool initialized_(false);

        void loadBotNames () {
            std::vector<sf::String> lines;
            if (file::load(settings::C_dataPath + "botnames.txt", lines)) {
                std::list<std::pair<sf::String, int> > newList;
                for (std::vector<sf::String>::iterator it = lines.begin(); it != lines.end(); ++it) {
                    if ((*it).ToAnsiString()[0] == '[') {
                        if (newList.size() > 0) {
                            botNames_.push_back(newList);
                            newList.clear();
                        }
                    }
                    else {
                        std::stringstream strengthStream(std::string((*it).ToAnsiString(), (*it).GetSize()-3));
                        int strength;
                        strengthStream >> strength;
                        (*it).Erase((*it).GetSize()-3, 3);
                        while ((*it)[(*it).GetSize()-1] == ' ' || (*it)[(*it).GetSize()-1] == '\t')
                            (*it).Erase((*it).GetSize()-1);
                        newList.push_back(std::make_pair(*it, strength));
                    }
                }
            }
            if (botNames_.size() == 0) {
                std::cout << "Failed to open botnames.txt! Reverting to some boring default names...\n";
                std::list<std::pair<sf::String, int> > defaultnames;
                defaultnames.push_back(std::make_pair("Ernst", 90));
                defaultnames.push_back(std::make_pair("Holger", 50));
                defaultnames.push_back(std::make_pair("Jimmy", 78));
                defaultnames.push_back(std::make_pair("Arnold", 100));
                defaultnames.push_back(std::make_pair("Ute", 92));
                defaultnames.push_back(std::make_pair("John", 94));
                defaultnames.push_back(std::make_pair("Matt", 20));
                defaultnames.push_back(std::make_pair("Gudrun", 67));
                defaultnames.push_back(std::make_pair("Ringo", 82));
                defaultnames.push_back(std::make_pair("Elvis", 98));
                defaultnames.push_back(std::make_pair("Bertold", 100));
                defaultnames.push_back(std::make_pair("Karl", 87));
                defaultnames.push_back(std::make_pair("Joe", 41));
                defaultnames.push_back(std::make_pair("Tom", 95));
                defaultnames.push_back(std::make_pair("Hilde", 84));
                defaultnames.push_back(std::make_pair("Herbert", 85));
                defaultnames.push_back(std::make_pair("Lars", 99));
                defaultnames.push_back(std::make_pair("Jeremy", 36));
                defaultnames.push_back(std::make_pair("Thomas", 57));
                defaultnames.push_back(std::make_pair("Jenny", 99));
                defaultnames.push_back(std::make_pair("James", 66));
                defaultnames.push_back(std::make_pair("Erwin", 92));
                defaultnames.push_back(std::make_pair("Thompson", 90));
                defaultnames.push_back(std::make_pair("August", 30));
                defaultnames.push_back(std::make_pair("Thorben", 77));
                defaultnames.push_back(std::make_pair("Tony", 80));
                botNames_.push_back(defaultnames);
            }
        }

        void loadShipNames () {
            if (!file::load(settings::C_dataPath + "shipnames.txt", shipNames_))
                std::cout << "No Botnames found! Using boring numbers instead...\n";
        }


        void init_() {
            // init rand()
            srand(time(NULL));

            // load list of bot names from file
            loadBotNames();

            // load list of ship names from file
            loadShipNames();

            // init game names
            gameNames_.push_back("Retarded Shooter");
            gameNames_.push_back("Random Shooter");
            gameNames_.push_back("Ridiculous Shooter");
            gameNames_.push_back("Rapid Shooter");
            gameNames_.push_back("Rough Shooter");
            gameNames_.push_back("Rigged Shooter");
            gameNames_.push_back("Rude Shooter");
            gameNames_.push_back("Retro-Shooter");

            // shuffle both lists
            std::vector<std::pair<sf::String, int> > temp;
            for (unsigned int i=0; i<botNames_.size(); ++i) {
                for (std::list<std::pair<sf::String, int> >::iterator it = botNames_[i].begin(); it != botNames_[i].end(); ++it) temp.push_back(*it);
                std::random_shuffle(temp.begin(), temp.end());
                botNames_[i].clear();
                for (std::vector<std::pair<sf::String, int> >::iterator it = temp.begin(); it != temp.end(); ++it) botNames_[i].push_back(*it);
                temp.clear();
            }

            std::vector<sf::String> temp2;
            for (std::list<sf::String>::iterator it = gameNames_.begin(); it != gameNames_.end(); ++it) temp2.push_back(*it);
            std::random_shuffle(temp2.begin(), temp2.end());
            gameNames_.clear();
            for (std::vector<sf::String>::iterator it = temp2.begin(); it != temp2.end(); ++it) gameNames_.push_back(*it);

            initialized_ = true;
        }
    }

    std::pair<sf::String, int> const& bot(int randomNumber) {
        if (!initialized_) init_();
        int group = randomNumber%botNames_.size();
        botNames_[group].push_front(botNames_[group].back());
        botNames_[group].pop_back();
        return *botNames_[group].begin();
    }

    sf::String const& game() {
        if (!initialized_) init_();
        gameNames_.push_front(gameNames_.back());
        gameNames_.pop_back();
        return *gameNames_.begin();
    }

    std::vector<sf::String> const& shipNames() {
        return shipNames_;
    }

    sf::String const key(Key const& key) {
        sf::String result("Unknown Key");
        switch (key.type_) {
            case Key::kKeyBoard: {
                sf::Keyboard::Key keyCode(key.code_.keyBoard_);
                // "normal" character
                if (static_cast<int>(keyCode) >= 97 && static_cast<int>(keyCode) <= 122) result = static_cast<char>(keyCode-32);

                // numbers
                if (static_cast<int>(keyCode) >= 48 && static_cast<int>(keyCode) <= 57) result = static_cast<char>(keyCode);

                else if (keyCode == sf::Keyboard::Add) result =         *locales::getLocale(locales::Add);
                else if (keyCode == sf::Keyboard::BackSlash) result =   *locales::getLocale(locales::BackSlash);
                else if (keyCode == sf::Keyboard::Comma) result =       *locales::getLocale(locales::Comma);
                else if (keyCode == sf::Keyboard::Dash) result =        *locales::getLocale(locales::Dash);
                else if (keyCode == sf::Keyboard::Delete) result =      *locales::getLocale(locales::Delete);
                else if (keyCode == sf::Keyboard::Divide) result =      *locales::getLocale(locales::Divide);
                else if (keyCode == sf::Keyboard::Down) result =        *locales::getLocale(locales::Down);
                else if (keyCode == sf::Keyboard::End) result =         *locales::getLocale(locales::End);
                else if (keyCode == sf::Keyboard::Equal) result =       *locales::getLocale(locales::Equal);
                else if (keyCode == sf::Keyboard::F1) result =          "F1";
                else if (keyCode == sf::Keyboard::F2) result =          "F2";
                else if (keyCode == sf::Keyboard::F3) result =          "F3";
                else if (keyCode == sf::Keyboard::F4) result =          "F4";
                else if (keyCode == sf::Keyboard::F5) result =          "F5";
                else if (keyCode == sf::Keyboard::F6) result =          "F6";
                else if (keyCode == sf::Keyboard::F7) result =          "F7";
                else if (keyCode == sf::Keyboard::F8) result =          "F8";
                else if (keyCode == sf::Keyboard::F9) result =          "F9";
                else if (keyCode == sf::Keyboard::F10) result =         "F10";
                else if (keyCode == sf::Keyboard::F11) result =         "F11";
                else if (keyCode == sf::Keyboard::F12) result =         "F12";
                else if (keyCode == sf::Keyboard::Home) result =        *locales::getLocale(locales::Home);
                else if (keyCode == sf::Keyboard::Insert) result =      *locales::getLocale(locales::Insert);
                else if (keyCode == sf::Keyboard::LAlt) result =        *locales::getLocale(locales::LeftAlt);
                else if (keyCode == sf::Keyboard::LBracket) result =    *locales::getLocale(locales::LeftBracket);
                else if (keyCode == sf::Keyboard::LControl) result =    *locales::getLocale(locales::LeftControl);
                else if (keyCode == sf::Keyboard::Left) result =        *locales::getLocale(locales::Left);
                else if (keyCode == sf::Keyboard::LShift) result =      *locales::getLocale(locales::LeftShift);
                else if (keyCode == sf::Keyboard::LSystem) result =     *locales::getLocale(locales::LeftSuper);
                else if (keyCode == sf::Keyboard::Menu) result =        *locales::getLocale(locales::Menu);
                else if (keyCode == sf::Keyboard::Multiply) result =    *locales::getLocale(locales::Multiply);
                else if (keyCode == sf::Keyboard::Numpad0) result =     "Num 0";
                else if (keyCode == sf::Keyboard::Numpad1) result =     "Num 1";
                else if (keyCode == sf::Keyboard::Numpad2) result =     "Num 2";
                else if (keyCode == sf::Keyboard::Numpad3) result =     "Num 3";
                else if (keyCode == sf::Keyboard::Numpad4) result =     "Num 4";
                else if (keyCode == sf::Keyboard::Numpad5) result =     "Num 5";
                else if (keyCode == sf::Keyboard::Numpad6) result =     "Num 6";
                else if (keyCode == sf::Keyboard::Numpad7) result =     "Num 7";
                else if (keyCode == sf::Keyboard::Numpad8) result =     "Num 8";
                else if (keyCode == sf::Keyboard::Numpad9) result =     "Num 9";
                else if (keyCode == sf::Keyboard::PageDown) result =    *locales::getLocale(locales::PageDown);
                else if (keyCode == sf::Keyboard::PageUp) result =      *locales::getLocale(locales::PageUp);
                else if (keyCode == sf::Keyboard::Pause) result =       *locales::getLocale(locales::Pause);
                else if (keyCode == sf::Keyboard::Period) result =      *locales::getLocale(locales::Period);
                else if (keyCode == sf::Keyboard::Quote) result =       *locales::getLocale(locales::Quote);
                else if (keyCode == sf::Keyboard::RAlt) result =        *locales::getLocale(locales::RightAlt);
                else if (keyCode == sf::Keyboard::RBracket) result =    *locales::getLocale(locales::RightBracket);
                else if (keyCode == sf::Keyboard::RControl) result =    *locales::getLocale(locales::RightControl);
                else if (keyCode == sf::Keyboard::Return) result =      *locales::getLocale(locales::Return);
                else if (keyCode == sf::Keyboard::Right) result =       *locales::getLocale(locales::Right);
                else if (keyCode == sf::Keyboard::RShift) result =      *locales::getLocale(locales::RightShift);
                else if (keyCode == sf::Keyboard::RSystem) result =     *locales::getLocale(locales::RightSuper);
                else if (keyCode == sf::Keyboard::SemiColon) result =   *locales::getLocale(locales::SemiColon);
                else if (keyCode == sf::Keyboard::Slash) result =       *locales::getLocale(locales::Slash);
                else if (keyCode == sf::Keyboard::Space) result =       *locales::getLocale(locales::Space);
                else if (keyCode == sf::Keyboard::Subtract) result =    *locales::getLocale(locales::Subtract);
                else if (keyCode == sf::Keyboard::Tilde) result =       *locales::getLocale(locales::Tilde);
                else if (keyCode == sf::Keyboard::Tab) result =         *locales::getLocale(locales::Tab);
                else if (keyCode == sf::Keyboard::Up) result =          *locales::getLocale(locales::Up);

                break;
            }

            case Key::kJoyButton: {
                unsigned int keyCode(key.code_.joyButton_);
                std::stringstream tmp;
                tmp << key.joyID_;
                result = *locales::getLocale(locales::JoyStick) + tmp.str() + " - ";
                if (keyCode == 0) result +=             *locales::getLocale(locales::JoyButtonA);
                else if (keyCode == 1) result +=        *locales::getLocale(locales::JoyButtonB);
                else if (keyCode == 2) result +=        *locales::getLocale(locales::JoyButtonX);
                else if (keyCode == 3) result +=        *locales::getLocale(locales::JoyButtonY);
                else if (keyCode == 4) result +=        *locales::getLocale(locales::JoyButtonLB);
                else if (keyCode == 5) result +=        *locales::getLocale(locales::JoyButtonRB);
                else if (keyCode == 6) result +=        *locales::getLocale(locales::JoyButtonStart);
                else if (keyCode == 10) result +=       *locales::getLocale(locales::JoyButtonBack);
                else {
                    std::stringstream tmptmp;
                    tmptmp <<keyCode;
                    result += *locales::getLocale(locales::JoyButton) +" " + tmptmp.str();
                }
                break;
            }

            case Key::kJoyAxis: {
                Key::AxisType keyCode(key.code_.joyAxis_);
                std::stringstream tmp;
                tmp << key.joyID_;
                result = *locales::getLocale(locales::JoyStick) + tmp.str() + " - ";
                if (keyCode == Key::aLT) result +=             *locales::getLocale(locales::JoyAxisLT);
                else if (keyCode == Key::aRT) result +=        *locales::getLocale(locales::JoyAxisRT);
                else if (keyCode == Key::aARup) result +=      *locales::getLocale(locales::JoyA2up);
                else if (keyCode == Key::aARdown) result +=    *locales::getLocale(locales::JoyA2down);
                else if (keyCode == Key::aARleft) result +=    *locales::getLocale(locales::JoyA2left);
                else if (keyCode == Key::aARright) result +=   *locales::getLocale(locales::JoyA2right);
                else if (keyCode == Key::aALup) result +=      *locales::getLocale(locales::JoyA1up);
                else if (keyCode == Key::aALdown) result +=    *locales::getLocale(locales::JoyA1down);
                else if (keyCode == Key::aALleft) result +=    *locales::getLocale(locales::JoyA1left);
                else if (keyCode == Key::aALright) result +=   *locales::getLocale(locales::JoyA1right);
                else if (keyCode == Key::aPOVup) result +=     *locales::getLocale(locales::JoyPOVup);
                else if (keyCode == Key::aPOVdown) result +=   *locales::getLocale(locales::JoyPOVdown);
                else if (keyCode == Key::aPOVleft) result +=   *locales::getLocale(locales::JoyPOVleft);
                else if (keyCode == Key::aPOVright) result +=  *locales::getLocale(locales::JoyPOVright);
                break;
            }
        }
        return result;
    }
}
