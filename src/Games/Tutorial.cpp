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

Tutorial::Tutorial():
    Game(games::gTutorial),
    state_(-1),
    savedState_(0),
    evilHome_(NULL),
    evilPlayer1_(NULL),
    evilPlayer2_(NULL),
    timer_(0.f),
    deadTimer_(0.f),
    dead_(false),
    life_(false),
    fuel_(false),
    name_(false) {

    settings::C_EnabledWeapons  = weapons::wAFK47 | weapons::wShotgun | weapons::wROFLE;
    settings::C_EnabledSpecials = specials::sFreeze | specials::sHeal | specials::sBlast;

    settings::C_iDumb = 0;

    music::playGameMusic();

    players::addPlayer (teams::addTeam(new TutTeam(settings::C_playerITeamColor)), controllers::cPlayer1);
    settings::C_playerIIteamL = false;
    settings::C_playerIIteamR = false;
    settings::C_playerIteamL = false;
    settings::C_playerIteamR = true;
    teams::assignHomes(spaceObjects::addHome(Vector2f(1300.f, 450.f), settings::C_playerITeamColor));
    players::createShips();

    evilHome_ = spaceObjects::addHome(Vector2f(-40.f, 550.f), Color3f(0.5f, 0.f, 0.5f));

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
    else if (!life_ && players::getPlayerI()->ship()->getLife() < 50.f && players::getPlayerI()->ship()->getLife() != 0.f) {
        life_ = true;
        savedState_ = state_;
        state_ = 51;
    }
    else if (!fuel_ && players::getPlayerI()->ship()->getFuel() < 50.f && players::getPlayerI()->ship()->getLife() != 0.f) {
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
                zones::addTutorialZone(Vector2f(1300.f, 450.f), 190.f);
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
                zones::addTutorialZone(Vector2f(1300.f, 450.f), 190.f);
                ++state_;
            } break;
        case 13:
            if (players::getPlayerI()->ship()->docked_){
                zones::clear();
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
                zones::createRaster(4, 3);
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut11), locales::getLocale(locales::TutText11), 11, false, false));
                Team* evilTeam = teams::addTeam(new TutTeam(Color3f(0.5f, 0.f, 0.5f)));
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
                timer_ = timer::totalTime();
                ++state_;
            } break;
        case 18:
            if (timer::totalTime() > timer_ + 1.f) {
                evilPlayer1_->ship()->location_ = Vector2f(3000, 3000);
                evilPlayer1_->ship()->respawnTimer_ = FLT_MAX;
                zones::addTutorialZone(Vector2f(1300.f, 450.f), 190.f);
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut12), locales::getLocale(locales::TutText12), 12, false, true));
                ++state_;
            } break;

         case 19:
            if (players::getPlayerI()->ship()->docked_) {
                zones::clear();
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


        case 22:
            if (timer::totalTime() > timer_ + 5.f) {
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut13), locales::getLocale(locales::TutText13), 15, false, false));
                ++state_;
            } break;
        case 23:
            if (!menus::visible()) {
                timer_ = timer::totalTime();
                ++state_;
            } break;
        case 24:
            if (timer::totalTime() > timer_ + 1.f) {
                players::addPlayer(evilPlayer1_->team(), controllers::cBot);
                std::vector<Player*> evilPlayer(evilPlayer1_->team()->members().begin()+1, evilPlayer1_->team()->members().end());
                evilPlayer2_ = evilPlayer[0];
                evilHome_->createShips(evilPlayer);
                hud::init();
                ++state_;
            } break;
        case 25:
            if (evilPlayer2_->ship()->getLife() == 0.f) {
                timer_ = timer::totalTime();
                ++state_;
            } break;



        case 26:
            if (timer::totalTime() > timer_ + 1.f) {
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut14), locales::getLocale(locales::TutText14), 16, false, false));
                ++state_;
            } break;
        case 27:
            if (!menus::visible()) {
                menus::showWindow(OptionsMenu::get());
                ++state_;
            } break;
        case 28:
            if (!menus::visible()) {
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut15), locales::getLocale(locales::TutText15), 17, false, true));
                ++state_;
            } break;
        case 29:
            if (!menus::visible()) {
                menus::showWindow(TutorialWindow::get(locales::getLocale(locales::Tut16), locales::getLocale(locales::TutText16), 18, false, false));
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


    players::addPlayer(teams::addTeam(new TutTeam(settings::C_playerITeamColor)), controllers::cPlayer1);
    teams::assignHomes(spaceObjects::addHome(Vector2f(1300.f, 450.f), settings::C_playerIColor));
    players::createShips();

    evilHome_ = spaceObjects::addHome(Vector2f(-40.f, 550.f), Color3f(0.5f, 0.f, 0.5f));

    spaceObjects::addPlanet(Vector2f(680.f, 300.f), 150.f);
    spaceObjects::addPlanet(Vector2f(280.f, 650.f), 80.f);
}










