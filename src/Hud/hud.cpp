/* hud.cpp

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

# include "Hud/hud.hpp"

# include "System/window.hpp"
# include "SpaceObjects/ships.hpp"
# include "SpaceObjects/Home.hpp"
# include "Games/games.hpp"
# include "Menu/menus.hpp"
# include "Hud/Logo.hpp"
# include "Hud/LeftLife.hpp"
# include "Hud/RightLife.hpp"
# include "Hud/GameStats.hpp"
# include "Hud/GamePoints.hpp"
# include "Hud/TabStats.hpp"
# include "Hud/Message.hpp"
# include "DecoObjects/decoObjects.hpp"
# include "Hud/CountDown.hpp"
# include "Particles/particles.hpp"
# include "Media/text.hpp"
# include "Hud/musicNotify.hpp"

# include <sstream>

namespace hud {

    namespace {
        Logo* logo_ = new Logo();
        LeftLife* leftLife_ = new LeftLife();
        RightLife* rightLife_ = new RightLife();
        GameStats* gameStats_ = new GameStats();
        GamePoints* gamePoints_ = new GamePoints();
        TabStats* tabStats_ = new TabStats();
        CountDown* countDown_ = new CountDown();

        std::list<Message*> messages_;
    }

    void update() {
        gamePoints_->update();
        tabStats_->update();

        if (games::type() == games::gMenu)   logo_->update();

        if (games::type() != games::gMenu && window::isKeyDown(settings::C_statisticsKey)) {
            tabStats_->display(true);
            gamePoints_->display();
        }
        else tabStats_->display(false);

        std::list<Message*>::iterator it = messages_.begin();
        while (it != messages_.end()) {
            (*it)->update();
            if ((*it)->isDead()) {
                delete *it;
                it = messages_.erase(it);
            }
            else ++it;
        }
        musicNotify::update();

    }

    void draw() {
        particles::drawNumbers();

        if (games::type() == games::gMenu)
            logo_->draw();
        else if (games::elapsedTime() < 6.f && games::type() != games::gTutorial)
            countDown_->draw();

        gameStats_->draw();

        if (games::type() != games::gMenu)
            decoObjects::drawNames();

        if ((!menus::visible() && games::active()) || games::type() == games::gTutorial) {
            if(games::type() == games::gSpaceBall || games::type() == games::gCannonKeep) {
                std::vector<Home*> const& homes = spaceObjects::getHomes();
                for (std::vector<Home*>::const_iterator it = homes.begin(); it != homes.end(); ++it)
                    (*it)->drawLife();
                gamePoints_->draw();
            }
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            rightLife_->draw();
            leftLife_ ->draw();
        }
        tabStats_->draw();
    }

    void drawMessages() {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        text::drawFooText();
        for (std::list<Message*>::iterator it=messages_.begin(); it!=messages_.end(); ++it)
            (*it)->draw();
        musicNotify::draw();
    }

    void displayPoints() {
        gamePoints_->display();
    }

    void displayStats(bool show) {
        gamePoints_->display();
        tabStats_->display(show);
    }

    void init() {
        tabStats_->refresh();
    }

    bool statsVisible() {
        return tabStats_->visible();
    }

    void displayMessage(sf::String const& message, Color3f const& color) {
        messages_.push_back(new Message(message, color));
    }
}

