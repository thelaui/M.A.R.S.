/* generateName.cpp

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

# include <cstdlib>
# include <algorithm>
# include <list>
# include <fstream>
# include <sstream>
# include <iostream>

namespace generateName {

    namespace {

        std::vector<std::list<std::string> > botNames_;
        std::list<std::string> gameNames_;
        std::vector<std::string> shipNames_;
        bool initialized_(false);

        void loadBotNames () {
            std::ifstream inStream("botnames.txt");
            char line[1024];

            if (inStream.good()) {
                while (inStream.good()) {
                    std::list<std::string> newList;
                    while(inStream.getline(line, 1024)) {
                        std::string inputLine (line);

                        while (inputLine[0] == ' ')
                            inputLine.erase(inputLine.begin());

                        // start new list
                        if (inputLine[0] == '[') break;

                        // ignore comments, empty lines and blank space after names
                        if(inputLine.size() == 2 || (inputLine.size() > 2 && (inputLine[0] != '/' && inputLine[1] != '/'))) {
                            while (inputLine[inputLine.size()-2] == ' ')
                                inputLine.erase(inputLine.size()-2);
                            if(inputLine.size() >= 1) {
                                inputLine.erase(inputLine.size()-1);
                                newList.push_back(inputLine);
                            }
                        }
                    }
                    if (newList.size() > 0) botNames_.push_back(newList);
                }
            }
            else {
                std::cout << "Could not find botnames.txt! Reverting to some boring default names...\n";
                std::list<std::string> defaultnames;
                defaultnames.push_back("Ernst [BOT]");
                defaultnames.push_back("Holger [BOT]");
                defaultnames.push_back("Jimmy [BOT]");
                defaultnames.push_back("Arnold [BOT]");
                defaultnames.push_back("Ute [BOT]");
                defaultnames.push_back("John [BOT]");
                defaultnames.push_back("Matt [BOT]");
                defaultnames.push_back("Gudrun [BOT]");
                defaultnames.push_back("Ringo [BOT]");
                defaultnames.push_back("Elvis [BOT]");
                defaultnames.push_back("Bertold [BOT]");
                defaultnames.push_back("Karl [BOT]");
                defaultnames.push_back("Joe [BOT]");
                defaultnames.push_back("Tom [BOT]");
                defaultnames.push_back("Hilde [BOT]");
                defaultnames.push_back("Herbert [BOT]");
                defaultnames.push_back("Lars [BOT]");
                defaultnames.push_back("Jeremy [BOT]");
                defaultnames.push_back("Thomas [BOT]");
                defaultnames.push_back("Jenny [BOT]");
                defaultnames.push_back("James [BOT]");
                defaultnames.push_back("Erwin [BOT]");
                defaultnames.push_back("Thompson [BOT]");
                defaultnames.push_back("August [BOT]");
                defaultnames.push_back("Thorben [BOT]");
                defaultnames.push_back("Tony [BOT]");
                botNames_.push_back(defaultnames);
            }
        }

        void loadShipNames () {
            std::ifstream inStream("shipnames.txt");
            char line[1024];

            if (inStream.good()) {
                while (inStream.good()) {
                    std::vector<std::string> newVector;
                    while(inStream.getline(line, 1024)) {
                        std::string inputLine (line);

                        while (inputLine[0] == ' ')
                            inputLine.erase(inputLine.begin());
                        while (inputLine[inputLine.size()-2] == ' ')
                            inputLine.erase(inputLine.size()-2);

                        // ignore comments, empty lines and blank space after names
                        if(inputLine.size() == 2 || (inputLine.size() > 2 && (inputLine[0] != '/' && inputLine[1] != '/'))) {
                            while (inputLine[inputLine.size()-2] == ' ')
                                inputLine.erase(inputLine.size()-2);
                            if(inputLine.size() >= 1) {
                                inputLine.erase(inputLine.size());
                                newVector.push_back(inputLine);
                            }
                        }
                    }
                    if (newVector.size() > 0) shipNames_.assign(newVector.begin(), newVector.end());
                }
            }
            else {
                std::cout << "Could not find shipnames.txt! Using boring numbers instead...\n";
            }
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
            std::vector<std::string> temp;
            for (unsigned int i=0; i<botNames_.size(); ++i) {
                for (std::list<std::string>::iterator it = botNames_[i].begin(); it != botNames_[i].end(); ++it) temp.push_back(*it);
                std::random_shuffle(temp.begin(), temp.end());
                botNames_[i].clear();
                for (std::vector<std::string>::iterator it = temp.begin(); it != temp.end(); ++it) botNames_[i].push_back(*it);
                temp.clear();
            }

            for (std::list<std::string>::iterator it = gameNames_.begin(); it != gameNames_.end(); ++it) temp.push_back(*it);
            std::random_shuffle(temp.begin(), temp.end());
            gameNames_.clear();
            for (std::vector<std::string>::iterator it = temp.begin(); it != temp.end(); ++it) gameNames_.push_back(*it);

            initialized_ = true;
        }
    }

    std::string bot(int randomNumber) {
        if (!initialized_) init_();
        int group = randomNumber%botNames_.size();
        botNames_[group].push_front(botNames_[group].back());
        botNames_[group].pop_back();
        return *botNames_[group].begin();
    }

    std::string game() {
        if (!initialized_) init_();
        gameNames_.push_front(gameNames_.back());
        gameNames_.pop_back();
        return *gameNames_.begin();
    }

    std::vector<std::string> const& shipNames() {
        return shipNames_;
    }
}
