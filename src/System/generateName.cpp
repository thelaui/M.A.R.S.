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
                    if ((*it).toAnsiString()[0] == '[') {
                        if (newList.size() > 0) {
                            botNames_.push_back(newList);
                            newList.clear();
                        }
                    }
                    else {
                        std::stringstream strengthStream(std::string((*it).toAnsiString(), (*it).getSize()-3));
                        int strength;
                        strengthStream >> strength;
                        (*it).erase((*it).getSize()-3, 3);
                        while ((*it)[(*it).getSize()-1] == ' ' || (*it)[(*it).getSize()-1] == '\t')
                            (*it).erase((*it).getSize()-1);
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
                if (static_cast<int>(keyCode) >= 0 && static_cast<int>(keyCode) <= 25) result = static_cast<char>(keyCode+65);

                // numbers
                if (static_cast<int>(keyCode) >= 26 && static_cast<int>(keyCode) <= 35) result = static_cast<char>(keyCode+22);

					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::Add) result =         *_("Add");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::BackSlash) result =   *_("Back Slash");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::Comma) result =       *_("Comma");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::Dash) result =        *_("Dash");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::Delete) result =      *_("Delete");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::Divide) result =      *_("Divide");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::Down) result =        *_("Down");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::End) result =         *_("End");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::Equal) result =       *_("Equal");
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
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::Home) result =        *_("Home");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::Insert) result =      *_("Insert");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::LAlt) result =        *_("Left Alt");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::LBracket) result =    *_("Left Bracket");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::LControl) result =    *_("Left Control");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::Left) result =        *_("Left");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::LShift) result =      *_("Left Shift");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::LSystem) result =     *_("Left Super");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::Menu) result =        *_("Menu");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::Multiply) result =    *_("Multiply");
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
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::PageDown) result =    *_("Page Down");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::PageUp) result =      *_("Page Up");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::Pause) result =       *_("Pause");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::Period) result =      *_("Period");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::Quote) result =       *_("Quote");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::RAlt) result =        *_("Right Alt");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::RBracket) result =    *_("Right Bracket");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::RControl) result =    *_("Right Control");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::Return) result =      *_("Return");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::Right) result =       *_("Right");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::RShift) result =      *_("Right Shift");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::RSystem) result =     *_("Right Super");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::SemiColon) result =   *_("Semi Colon");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::Slash) result =       *_("Slash");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::Space) result =       *_("Space");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::Subtract) result =    *_("Subtract");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::Tilde) result =       *_("Tilde");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::Tab) result =         *_("Tab");
					 /** TRANSLATORS: A key on your keyboard */
					 else if (keyCode == sf::Keyboard::Up) result =          *_("Up");
                break;
            }

            case Key::kJoyButton: {
                unsigned int keyCode(key.code_.joyButton_);
                std::stringstream tmp;
                tmp << key.joyID_;
					 result = *_("Joystick") + tmp.str() + " - ";
					 /** TRANSLATORS: A joystick button */
					 if (keyCode == 0) result +=             *_("A Button");
					 /** TRANSLATORS: A joystick button */
					 else if (keyCode == 1) result +=        *_("B Button");
					 /** TRANSLATORS: A joystick button */
					 else if (keyCode == 2) result +=        *_("X Button");
					 /** TRANSLATORS: A joystick button */
					 else if (keyCode == 3) result +=        *_("Y Button");
					 /** TRANSLATORS: A joystick button */
					 else if (keyCode == 4) result +=        *_("Left Bumper");
					 /** TRANSLATORS: A joystick button */
					 else if (keyCode == 5) result +=        *_("Right Bumper");
					 /** TRANSLATORS: A joystick button */
					 else if (keyCode == 6) result +=        *_("Start Button");
					 /** TRANSLATORS: A joystick button */
					 else if (keyCode == 10) result +=       *_("Back Button");
                else {
                    std::stringstream tmptmp;
                    tmptmp <<keyCode;
						  /** TRANSLATORS: A joystick button */
						  result += *_("Button") +" " + tmptmp.str();
                }
                break;
            }

            case Key::kJoyAxis: {
                Key::AxisType keyCode(key.code_.joyAxis_);
                std::stringstream tmp;
                tmp << key.joyID_;
					 result = *_("Joystick") + tmp.str() + " - ";
					 /** TRANSLATORS: A joystick axis type */
					 if (keyCode == Key::aLT) result +=             *_("Left Trigger");
					 /** TRANSLATORS: A joystick axis type */
					 else if (keyCode == Key::aRT) result +=        *_("Right Trigger");
					 /** TRANSLATORS: A joystick axis type */
					 else if (keyCode == Key::aARup) result +=      *_("Stick2 Up");
					 /** TRANSLATORS: A joystick axis type */
					 else if (keyCode == Key::aARdown) result +=    *_("Stick2 Down");
					 /** TRANSLATORS: A joystick axis type */
					 else if (keyCode == Key::aARleft) result +=    *_("Stick2 Left");
					 /** TRANSLATORS: A joystick axis type */
					 else if (keyCode == Key::aARright) result +=   *_("Stick2 Right");
					 /** TRANSLATORS: A joystick axis type */
					 else if (keyCode == Key::aALup) result +=      *_("Stick1 Up");
					 /** TRANSLATORS: A joystick axis type */
					 else if (keyCode == Key::aALdown) result +=    *_("Stick1 Down");
					 /** TRANSLATORS: A joystick axis type */
					 else if (keyCode == Key::aALleft) result +=    *_("Stick1 Left");
					 /** TRANSLATORS: A joystick axis type */
					 else if (keyCode == Key::aALright) result +=   *_("Stick1 Right");
					 /** TRANSLATORS: A joystick axis type */
					 else if (keyCode == Key::aPOVup) result +=     *_("Direction Up");
					 /** TRANSLATORS: A joystick axis type */
					 else if (keyCode == Key::aPOVdown) result +=   *_("Direction Down");
					 /** TRANSLATORS: A joystick axis type */
					 else if (keyCode == Key::aPOVleft) result +=   *_("Direction Left");
					 /** TRANSLATORS: A joystick axis type */
					 else if (keyCode == Key::aPOVright) result +=  *_("Direction Right");
					 /** TRANSLATORS: A joystick axis type */
                break;
            }
        }
        return result;
    }
}
