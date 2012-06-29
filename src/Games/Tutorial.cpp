/* Tutorial.cpp

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

# include "Games/Tutorial.hpp"

# include "Teams/TutTeam.hpp"
# include "Teams/DMTeam.hpp"
# include "System/settings.hpp"
# include "Media/music.hpp"
# include "Items/items.hpp"
# include "DecoObjects/decoObjects.hpp"
# include "Menu/menus.hpp"
# include "Hud/hud.hpp"
# include "Players/Player.hpp"
# include "Players/players.hpp"
# include "Teams/teams.hpp"
# include "SpaceObjects/spaceObjects.hpp"
# include "SpaceObjects/Home.hpp"
# include "Menu/OptionsMenu.hpp"
# include "Menu/TutorialWindow.hpp"
# include "Menu/EnterName.hpp"
# include "Locales/locales.hpp"
# include "defines.hpp"

Tutorial::Tutorial():
    Game(games::gTutorial),
    state_(-1),
    savedState_(0),
    evilHome_(NULL),
    evilPlayer1_(NULL),
    evilPlayer2_(NULL),
    friendPlayer_(NULL),
    timer_(0.f),
    deadTimer_(0.f),
    dead_(false),
    life_(false),
    fuel_(false),
    name_(false) {

    settings::C_EnabledWeapons  = weapons::wAFK47 | weapons::wShotgun | weapons::wROFLE;
    settings::C_EnabledSpecials = specials::sFreeze | specials::sHeal | specials::sBlast;

    settings::C_iDumb = 0;

    music::play(settings::C_dataPath + "audio/tutorial.ogg");

    players::addPlayer (teams::addTeam(new DMTeam(settings::C_playerITeamColor)), controllers::cPlayer1);
    settings::C_playerIIteamL = false;
    settings::C_playerIIteamR = false;
    settings::C_playerIteamL = false;
    settings::C_playerIteamR = true;
    teams::assignHomes(spaceObjects::addHome(Vector2f(SPACE_X_RESOLUTION+20, 450.f), 100, settings::C_playerITeamColor, 100, 15000));
    players::createShips();

    evilHome_ = spaceObjects::addHome(Vector2f(-40.f, 550.f), 100, settings::C_playerITeamColor.inverted(), 100, 15000);

    spaceObjects::addPlanet(Vector2f(680.f, 300.f), 150.f);
    spaceObjects::addPlanet(Vector2f(280.f, 650.f), 80.f);
}

void Tutorial::update() {
    Game::update();

    if (!dead_ && players::getPlayerI()->ship()->getLife() == 0.f) {
        dead_ = true;
        savedState_ = state_;
        deadTimer_ = timer::totalTime();
        state_ = 50;
    }
    else if (!life_ && players::getPlayerI()->ship()->getLife() < 30.f && players::getPlayerI()->ship()->getLife() != 0.f) {
        life_ = true;
        savedState_ = state_;
        state_ = 51;
    }
    else if (!fuel_ && players::getPlayerI()->ship()->getFuel() < 30.f && players::getPlayerI()->ship()->getLife() != 0.f) {
        fuel_ = true;
        savedState_ = state_;
        state_ = 52;
    }
    else if (!name_ && evilPlayer1_ && evilPlayer1_->ship()->getLife() < 50.f) {
        name_ = true;
        savedState_ = state_;
        state_ = 53;
    }

    switch (state_) {
        case -1:
            if (!menus::visible()) {
                if (settings::C_playerIName == "PlayerI")
                    menus::showWindow(EnterName::get());
                ++state_;
            } break;
        case 0:
            if (!menus::visible()) {
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut01), locales::getLocale(locales::TutText01), 1, false, true));
                ++state_;
            } break;
        case 1:
            if (!menus::visible()) {
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut02), locales::getLocale(locales::TutText02), 2, false, true));
                ++state_;
            } break;
        case 2:
            if (!menus::visible()) {
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut03), locales::getLocale(locales::TutText03), 3, false, false));
                ++state_;
            } break;
        case 3:
            zones::addTutorialZone(Vector2f(200.f, 200.f), 200.f);
            ++state_;
            break;
        case 4:
            if (zones::updateTutZones()){
                zones::addTutorialZone(Vector2f(1105.f, 175.f), 175.f);
                ++state_;
            } break;
        case 5:
            if (zones::updateTutZones()){
                zones::addTutorialZone(Vector2f(150.f, 650.f), 150.f);
                ++state_;
            } break;
        case 6:
            if (zones::updateTutZones()){
                zones::addTutorialZone(Vector2f(850.f, 675.f), 125.f);
                ++state_;
            } break;
        case 7:
            if (zones::updateTutZones()){
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut04), locales::getLocale(locales::TutText04), 4, false, false));
                zones::addTutorialZone(Vector2f(SPACE_X_RESOLUTION+20, 450.f), 190.f);
                ++state_;
            } break;
        case 8:
            if (players::getPlayerI()->ship()->docked_){
                zones::clear();
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut05), locales::getLocale(locales::TutText05), 5, false, true));
                ++state_;
            } break;
        case 9:
            if (!menus::visible()) {
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut06), locales::getLocale(locales::TutText06), 6, false, false));
                ++state_;
            } break;
        case 10:
            zones::addTutorialZone(Vector2f(680.f, 80.f));
            zones::addTutorialZone(Vector2f(510.f, 100.f));
            zones::addTutorialZone(Vector2f(350.f, 140.f));
            zones::addTutorialZone(Vector2f(220.f, 230.f));
            zones::addTutorialZone(Vector2f(260.f, 380.f));
            zones::addTutorialZone(Vector2f(360.f, 510.f));
            zones::addTutorialZone(Vector2f(500.f, 620.f));
            zones::addTutorialZone(Vector2f(680.f, 680.f));
            zones::addTutorialZone(Vector2f(880.f, 650.f));
            zones::addTutorialZone(Vector2f(1030.f, 600.f));
            ++state_;
            break;
        case 11:
            if (zones::updateTutZones()){
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut07), locales::getLocale(locales::TutText07), 7, false, true));
                ++state_;
            } break;
        case 12:
            if (!menus::visible()) {
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut08), locales::getLocale(locales::TutText08), 8, false, false));
                zones::addTutorialZone(Vector2f(SPACE_X_RESOLUTION+20, 450.f), 190.f);
                ++state_;
            } break;
        case 13:
            if (players::getPlayerI()->ship()->docked_){
                zones::clear();
                zones::createRaster(4, 3);
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut09), locales::getLocale(locales::TutText09), 9, false, true));
                ++state_;
            } break;
        case 14:
            if (!menus::visible()) {
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut10), locales::getLocale(locales::TutText10), 10, false, false));
                ++state_;
            } break;
        case 15:
            if (!players::getPlayerI()->ship()->docked_) {
                timer_ = timer::totalTime();
                ++state_;
            } break;
        case 16:
            if (timer::totalTime() > timer_ + 1.f) {
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut11), locales::getLocale(locales::TutText11), 11, false, false));
                Team* evilTeam = teams::addTeam(new TutTeam( settings::C_playerITeamColor.inverted()));
                players::addPlayer(evilTeam, controllers::cBot);
                evilTeam->setHome(evilHome_);
                std::vector<Player*> evilPlayer(evilTeam->members());
                evilPlayer1_ = evilPlayer[0];
                evilHome_->createShips(evilPlayer);
                hud::init();
                ++state_;
            } break;
        case 17:
            if (evilPlayer1_->ship()->getLife() == 0.f) {
                players::getPlayerI()->ship()->fragStars_ = 1;
                timer_ = timer::totalTime();
                ++state_;
            } break;
        case 18:
            if (timer::totalTime() > timer_ + 1.f) {
                evilPlayer1_->ship()->location_ = Vector2f(3000, 3000);
                evilPlayer1_->ship()->respawnTimer_ = FLT_MAX;
                zones::addTutorialZone(Vector2f(SPACE_X_RESOLUTION+20, 450.f), 190.f);
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut12), locales::getLocale(locales::TutText12), 12, false, true));
                ++state_;
            } break;

         case 19:
            if (players::getPlayerI()->ship()->docked_) {
                zones::clear();
                zones::createRaster(4, 3);
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut21), locales::getLocale(locales::TutText21), 13, false, true));
                ++state_;
            } break;
        case 20:
            if (!menus::visible()) {
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut22), locales::getLocale(locales::TutText22), 14, false, false));
                ++state_;
            } break;
        case 21:
            if (!players::getPlayerI()->ship()->docked_) {
                timer_ = timer::totalTime();
                ++state_;
            } break;
       // select freezer
       case 22:
            if (timer::totalTime() > timer_ + 1.f) {
                if (players::getPlayerI()->ship()->currentSpecial_->getType() == specials::sFreeze) {
                    menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut24), locales::getLocale(locales::TutText24), 15, false, true));
                    state_ += 2;
                }
                else {
                    menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut23), locales::getLocale(locales::TutText23), 0, true, false));
                    zones::addTutorialZone(Vector2f(SPACE_X_RESOLUTION+20, 450.f), 190.f);
                    ++state_;
                }
            } break;
        case 23:
            if (players::getPlayerI()->ship()->docked_) {
                zones::clear();
                zones::createRaster(4, 3);
                timer_ = timer::totalTime();
                state_ -= 2;
            } break;
        // correctly selected
        case 24:
            if (!menus::visible()) {
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut25), locales::getLocale(locales::TutText25), 16, false, false));
                evilPlayer1_->ship()->respawnTimer_ = 0.f;
                players::getPlayerI()->ship()->fragStars_ = 3;
                timer_ = timer::totalTime();
                ++state_;
            } break;
        case 25:
            if (evilPlayer1_->ship()->frozen_ > 0) {
                timer_ = timer::totalTime();
                state_ +=2;
            }
            else if (evilPlayer1_->ship()->frozen_ <= 0 && players::getPlayerI()->ship()->fragStars_ == 0) {
                timer_ = timer::totalTime();
                ++state_;
            } break;
        case 26:
            if (timer::totalTime() > timer_ + 1.f) {
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut26), locales::getLocale(locales::TutText26), 0, true, false));
                players::getPlayerI()->ship()->fragStars_ = 3;
                timer_ = timer::totalTime();
                --state_;
            } break;
        case 27:
            if (timer::totalTime() > timer_ + 7.f) {
                if (evilPlayer1_->ship()->getLife() > 0)
                    evilPlayer1_->ship()->explode();
                evilPlayer1_->ship()->respawnTimer_ = FLT_MAX;
                timer_ = timer::totalTime();
                ++state_;
            } break;
         case 28:
            if (timer::totalTime() > timer_ + 1.5f) {
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut27), locales::getLocale(locales::TutText27), 17, true, false));
                zones::addTutorialZone(Vector2f(SPACE_X_RESOLUTION+20, 450.f), 190.f);
                ++state_;
            } break;
        case 29:
            if (players::getPlayerI()->ship()->docked_ && !menus::visible()) {
                zones::clear();
                zones::createRaster(4, 3);
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut28), locales::getLocale(locales::TutText28), 18, true, true));
                evilPlayer1_->ship()->respawnTimer_ = 0.f;
                players::addPlayer(evilPlayer1_->team(), controllers::cBot);
                std::vector<Player*> evilPlayer(evilPlayer1_->team()->members().begin()+1, evilPlayer1_->team()->members().end());
                evilPlayer2_ = evilPlayer[0];
                evilHome_->createShips(evilPlayer);
                hud::init();
                ++state_;
            } break;
        case 30:
            if (!menus::visible()) {
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut13), locales::getLocale(locales::TutText13), 19, false, false));
                players::addPlayer(players::getPlayerI()->team(), controllers::cBot);
                std::vector<Player*> friends(players::getPlayerI()->team()->members().begin()+1, players::getPlayerI()->team()->members().end());
                friendPlayer_ = friends[0];
                players::getPlayerI()->team()->home()->createShips(friends);
                hud::init();
                timer_ = timer::totalTime();
                teams::resetTeamPoints();
                players::resetPlayerPoints();
                ++state_;
            } break;
        case 31:
            if (players::getPlayerI()->team()->points() >= 5) {
                timer_ = timer::totalTime();
                ++state_;
            } break;

        case 32:
            if (timer::totalTime() > timer_ + 1.f) {
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut14), locales::getLocale(locales::TutText14), 20, false, false));
                ++state_;
            } break;
        case 33:
            if (!menus::visible()) {
                menus::showWindow(OptionsMenu::get());
                ++state_;
            } break;
        case 34:
            if (!menus::visible()) {
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut15), locales::getLocale(locales::TutText15), 21, false, true));
                ++state_;
            } break;
        case 35:
            if (!menus::visible()) {
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut16), locales::getLocale(locales::TutText16), 22, false, false));
                ++state_;
            } break;


        case 50:
            if (!menus::visible() && timer::totalTime() > deadTimer_ + 1.f) {
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut17), locales::getLocale(locales::TutText17), 0, true, false));
                state_ = 99;
            } break;
        case 51:
            if (!menus::visible()) {
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut18), locales::getLocale(locales::TutText18), 0, true, false));
                state_ = 99;
            } break;
        case 52:
            if (!menus::visible()) {
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut19), locales::getLocale(locales::TutText19), 0, true, false));
                state_ = 99;
            } break;
        case 53:
            if (!menus::visible()) {
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut20), locales::getLocale(locales::TutText20), 0, true, false));
                state_ = 99;
            } break;


        case 99:
            if (!menus::visible()) {
                state_ = savedState_;
            } break;
        default:;
    }
}

void Tutorial::draw() const {
    zones::draw();
    Game::draw();
}

void Tutorial::restart() {
    Game::restart();

    state_ = -1;
    savedState_ = 0;
    deadTimer_ = 0.f;
    timer_ = 0.f;

    dead_ = false;
    life_ = false;
    fuel_ = false;
    name_ = false;
    players::clear();
    teams::clear();
    controllers::clear();
    evilPlayer1_ = NULL;
    evilPlayer2_ = NULL;

    players::addPlayer(teams::addTeam(new DMTeam(settings::C_playerITeamColor)), controllers::cPlayer1);
    teams::assignHomes(spaceObjects::addHome(Vector2f(SPACE_X_RESOLUTION+20, 450.f), 100, settings::C_playerIColor, 100, 15000));
    players::createShips();

    evilHome_ = spaceObjects::addHome(Vector2f(-40.f, 550.f), 100, Color3f(0.5f, 0.f, 0.5f), 100, 15000);

    spaceObjects::addPlanet(Vector2f(680.f, 300.f), 150.f);
    spaceObjects::addPlanet(Vector2f(280.f, 650.f), 80.f);
}










