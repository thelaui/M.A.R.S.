/* controllers.cpp

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

# include "Controllers/DefBot.hpp"
# include "Controllers/MidBot.hpp"
# include "Controllers/AggroBot.hpp"
# include "Controllers/CKBot.hpp"
# include "Controllers/DMBot.hpp"
# include "Controllers/KeyController.hpp"
# include "System/settings.hpp"
# include "System/window.hpp"

# include <SFML/Window.hpp>

namespace controllers {
    namespace {
        KeyController* keyControllers1_(NULL);
        KeyController* keyControllers2_(NULL);
        std::vector<BotController*> botControllers_;

        sf::Input const& input_ = window::getMainWindow()->GetInput();
    }

    void update() {
        if (keyControllers1_ != NULL && (settings::C_playerIteamL | settings::C_playerIteamR))
            keyControllers1_->update(input_);
        if (keyControllers2_ != NULL && (settings::C_playerIIteamL | settings::C_playerIIteamR))
            keyControllers2_->update(input_);

        for (std::vector<BotController*>::iterator it = botControllers_.begin(); it != botControllers_.end(); ++it)
            (*it)->update();
    }

    void singleKeyEvent(sf::Key::Code keyCode) {
        if (keyControllers1_ != NULL && (settings::C_playerIteamL | settings::C_playerIteamR))
            keyControllers1_->update(keyCode);
        if (keyControllers2_ != NULL && (settings::C_playerIIteamL | settings::C_playerIIteamR))
            keyControllers2_->update(keyCode);
    }

    void addController(ControlType type, Player* slave) {
        switch (type) {
            case cDefBot:     botControllers_.push_back(new DefBot(slave)); break;
            case cMidBot:     botControllers_.push_back(new MidBot(slave)); break;
            case cAggroBot:     botControllers_.push_back(new AggroBot(slave)); break;
            case cCKBot:     botControllers_.push_back(new CKBot(slave)); break;
            case cDMBot:     botControllers_.push_back(new DMBot(slave)); break;
            case cPlayer1: keyControllers1_ = new KeyController(type, slave);  break;
            case cPlayer2: keyControllers2_ = new KeyController(type, slave);  break;
        }
    }

    void resetBots() {
        for (std::vector<BotController*>::iterator it = botControllers_.begin(); it != botControllers_.end(); ++it)
            (*it)->reset();
    }

    void clear() {
        delete keyControllers1_;
        delete keyControllers2_;
        keyControllers1_ = NULL;
        keyControllers2_ = NULL;

        for (std::vector<BotController*>::iterator it = botControllers_.begin(); it != botControllers_.end(); ++it)
            delete *it;
        botControllers_.clear();
    }
}


