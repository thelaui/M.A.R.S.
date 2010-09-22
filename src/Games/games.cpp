/* games.cpp

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

# include "Games/games.hpp"

# include "Games/Game.hpp"
# include "Games/CannonKeep.hpp"
# include "Games/DeathMatch.hpp"
# include "Games/MenuGame.hpp"
# include "Games/SpaceBall.hpp"
# include "Games/TeamDeathMatch.hpp"
# include "Players/players.hpp"
# include "System/settings.hpp"
# include "Hud/hud.hpp"
# include "System/window.hpp"

# include <SFML/OpenGL.hpp>

namespace games {
    namespace {
        Game* currentGame_(0);
        bool newGame_(false);
    }

    void update() {
        glClear(GL_COLOR_BUFFER_BIT);
        currentGame_->update();
        if (newGame_) restart();
    }

    void draw() {
        currentGame_->draw();
    }

    void start(GameType const& type) {
        switch (type) {
            case gMenu:              currentGame_ = new MenuGame();       break;
            case gSpaceBall:         currentGame_ = new SpaceBall();      break;
            case gDeathMatch:        currentGame_ = new DeathMatch();     break;
            case gTeamDeathMatch:    currentGame_ = new TeamDeathMatch(); break;
            case gCannonKeep:        currentGame_ = new CannonKeep();     break;
            default: end();
        }
        hud::refreshTabStats();
    }

    void restart() {
        if (!newGame_)
            newGame_ = true;
        else {
            currentGame_->restart();
            newGame_ = false;
        }
    }

    void end() {
        if (currentGame_) {
            delete currentGame_;
            currentGame_ = 0;
        }
    }

    GameType type() {
        if (currentGame_)
            return currentGame_->type();
        else
            return gNoGame;
    }

    float elapsedTime() {
        if (currentGame_)
            return currentGame_->elapsedTime();
        else
            return 0.f;
    }
}
