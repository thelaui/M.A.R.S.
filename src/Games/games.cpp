/* games.cpp

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

# include "Games/games.hpp"

# include "Games/Game.hpp"
# include "Games/CannonKeep.hpp"
# include "Games/DeathMatch.hpp"
# include "Games/MenuGame.hpp"
# include "Games/SpaceBall.hpp"
# include "Games/TeamDeathMatch.hpp"
# include "Games/Tutorial.hpp"
# include "System/settings.hpp"
# include "System/timer.hpp"
# include "Menu/menus.hpp"
# include "Hud/hud.hpp"
# include "System/window.hpp"

# include <SFML/OpenGL.hpp>

namespace games {

    namespace {
        Game* currentGame_(0);
        bool restart_(false);
        bool newGame_(true);
        GameType newGameType_(gNoGame);
        bool fadeOut_(false);
        bool fadeIn_(false);
        float fadeFactor_(1.f);

        void fade() {
            if (fadeIn_) {
                fadeFactor_ -= timer::frameTime()*2.f;
                if (fadeFactor_ <= 0.f) {
                    fadeFactor_ = 0.f;
                    fadeIn_ = false;
                }
            }
            else if (fadeOut_) {
                fadeFactor_ += timer::frameTime()*3.f;
                if (fadeFactor_ >= 1.f) {
                    fadeFactor_ = 1.f;
                    fadeOut_ = false;
                }
            }

            Vector2f viewPort = window::getViewPort();

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBegin(GL_QUADS);
                glColor4f(0.0,0.0,0.0,fadeFactor_);
                glVertex2f(0.f, 0.f);
                glVertex2f(0.f, viewPort.y_);
                glVertex2f(viewPort.x_, viewPort.y_);
                glVertex2f(viewPort.x_, 0.f);
            glEnd();
        }

    }

    void update() {
        if (!fadeIn_ && !fadeOut_) currentGame_->update();
        if (restart_ && !fadeOut_) restart();
        if (newGame_ && !fadeOut_) start(newGameType_);
    }

    void draw() {
        window::startDrawSpace();

        currentGame_->draw();

        window::startDrawHUD();

        hud::draw();
        menus::draw();

        if (fadeIn_ || fadeOut_) fade();
    }

    void start(GameType const& type) {
        if (!newGame_) {
            newGameType_ = type;
            newGame_ = true;
            fadeOut_ = true;
        }
        else {
            if (currentGame_) {
                delete currentGame_;
                currentGame_ = 0;
            }

            switch (type) {
                case gMenu:              currentGame_ = new MenuGame();       break;
                case gSpaceBall:         currentGame_ = new SpaceBall();      break;
                case gDeathMatch:        currentGame_ = new DeathMatch();     break;
                case gTeamDeathMatch:    currentGame_ = new TeamDeathMatch(); break;
                case gCannonKeep:        currentGame_ = new CannonKeep();     break;
                case gTutorial:          currentGame_ = new Tutorial();       break;
                default: break;
            }

            newGame_ = false;
            fadeIn_  = true;
        }
    }

    void restart() {
        if (!restart_) {
            restart_ = true;
            fadeOut_ = true;
        }
        else {
            currentGame_->restart();
            restart_ = false;
            fadeIn_  = true;
        }
    }

    bool active() {
        return (!fadeIn_ && !fadeOut_);
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
