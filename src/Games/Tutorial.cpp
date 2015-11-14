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
# include "System/generateName.hpp"
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
					 menus::showWindow(TutorialWindow::get(
												 _("Welcome, Space Rookie!"),
												 locales::format_string(
													 /** TRANSLATORS: %s is the player's name.*/
													 gettext("We are glad to recruit a rising hero like you, %s, to "
														"join us in the GREAT WAR!\n"
														"We figured out you needed some practice with your new ship.\n"
														"Let the training begin..."),
													 reinterpret_cast<const char*>(settings::C_playerIName.toUtf8().c_str())),
												 1, false, true));
                ++state_;
            } break;
        case 1:
            if (!menus::visible()) {
					 menus::showWindow(TutorialWindow::get(
												 _("Basic Movement..."),
												 locales::format_string(
													 /** TRANSLATORS: all %s are hotkeys on the keyboard.*/
													 gettext("Controlling your ship is the most important task to master.\n"
														"All movement is governed by the laws of gravity. To decelerate you "
														"have to turn and accelerate in the opposite direction. Control your "
														"ship, using the keys %s, %s and "
														"%s!"),
													 reinterpret_cast<const char*>(generateName::key(settings::C_playerIup).toUtf8().c_str()),
													 reinterpret_cast<const char*>(generateName::key(settings::C_playerIleft).toUtf8().c_str()),
													 reinterpret_cast<const char*>(generateName::key(settings::C_playerIright).toUtf8().c_str())),
												 2, false, true));
                ++state_;
            } break;
        case 2:
            if (!menus::visible()) {
					 menus::showWindow(TutorialWindow::get(
												 _("Warming Up!"),
												 _("Reach the pink zone!\nTry not to crash your brand new ship."),
												 3, false, false));
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
					 menus::showWindow(TutorialWindow::get(
												 locales::format_string(
													 /** TRANSLATORS: %s is the player's name.*/
													 gettext("Well Done, %s!"),
													 reinterpret_cast<const char*>(settings::C_playerIName.toUtf8().c_str())),
												 _("Now head back to your home planet and try to land your ship by slowly "
													"floating back while approaching your planet."),
												 4, false, false));
                zones::addTutorialZone(Vector2f(SPACE_X_RESOLUTION+20, 450.f), 190.f);
                ++state_;
            } break;
        case 8:
            if (players::getPlayerI()->ship()->docked_){
                zones::clear();
					 menus::showWindow(TutorialWindow::get(
												 _("Good Work!"),
												 _("Proper landing is not a piece of cake, is it?\n"
													"You should learn it by heart."),
												 5, false, true));
                ++state_;
            } break;
        case 9:
            if (!menus::visible()) {
					 menus::showWindow(TutorialWindow::get(
												 _("Something More Difficult..."),
												 _("Try to follow the path marked up by the pink zones!\n"
													"We will contact you again, after you have mastered this.\n"
													"Now go for it!"),
												 6, false, false));
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
					 menus::showWindow(TutorialWindow::get(
												 _("Awesome!"),
												 _("You are the best rookie we have ever seen.\n"
													"Now prepare yourself for some explosive stuff."),
												 7, false, true));
                ++state_;
            } break;
        case 12:
            if (!menus::visible()) {
					 menus::showWindow(TutorialWindow::get(
												 _("Shooting..."),
												 locales::format_string(
													 /** TRANSLATORS: %s is a hotkey on the keyboard.*/
													 gettext("To defeat your enemies, you have to know how to shoot them up!\n"
														"For that purpose press %s. \n"
														"Have some fun shooting around and then land your ship to continue the "
														"tutorial."),
													 reinterpret_cast<const char*>(generateName::key(settings::C_playerIfire).toUtf8().c_str())),
												 8, false, false));
                zones::addTutorialZone(Vector2f(SPACE_X_RESOLUTION+20, 450.f), 190.f);
                ++state_;
            } break;
        case 13:
            if (players::getPlayerI()->ship()->docked_){
                zones::clear();
                zones::createRaster(4, 3);
					 menus::showWindow(TutorialWindow::get(
												 locales::format_string(
													 /** TRANSLATORS: %s is the player's name.*/
													 gettext("Good Landing, %s!"),
													 reinterpret_cast<const char*>(settings::C_playerIName.toUtf8().c_str())),
												 _("You turn out to be a great pilot.\n"
													"Now how about some more weapons?"),
												 9, false, true));
                ++state_;
            } break;
        case 14:
            if (!menus::visible()) {
					 menus::showWindow(TutorialWindow::get(
												 _("The Weapons..."),
												 locales::format_string(
													 /** TRANSLATORS: all %s are hotkeys on the keyboard.*/
													 gettext("We know you've been waiting for this. When docked you can switch "
														"through some incredibly evil weapons. Press %s to "
														"open your arsenal.\n"
														"Then use %s and %s to choose a "
														"weapon! \n"
														"Take-off, when you're ready!"),
													 reinterpret_cast<const char*>(generateName::key(settings::C_playerIfire).toUtf8().c_str()),
													 reinterpret_cast<const char*>(generateName::key(settings::C_playerIleft).toUtf8().c_str()),
													 reinterpret_cast<const char*>(generateName::key(settings::C_playerIright).toUtf8().c_str())),
												 10, false, false));
                ++state_;
            } break;
        case 15:
            if (!players::getPlayerI()->ship()->docked_) {
                timer_ = timer::totalTime();
                ++state_;
            } break;
        case 16:
            if (timer::totalTime() > timer_ + 1.f) {
					 menus::showWindow(TutorialWindow::get(
												 _("Fight!"),
												 _("Now let's see how you make it against a rookie from another planet. "
													"Seems like he is trying to improve his flying abilities.\n"
													"Beat him!"),
												 11, false, false));
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
					 menus::showWindow(TutorialWindow::get(
												 _("Impressive!"),
												 _("You did it! As you may have noticed, a little star appeared over "
													"your ship. The more enemies you kill, the more of them will show. "
													"But they are not only a frag indicator! \n"
													"Land again and we will show you what else stars are good for."),
												 12, false, true));
                ++state_;
            } break;

         case 19:
            if (players::getPlayerI()->ship()->docked_) {
                zones::clear();
                zones::createRaster(4, 3);
					 menus::showWindow(TutorialWindow::get(
												 _("Special Abilities..."),
												 _("As promised, we will tell you the stars' meaning. "
													"They are a resource for your ship's impressively powerful extra abilities. "
													"For example you can heal you including nearby friends, blast away or "
													"freeze ships within a certain distance! \n"
													"The more stars you have, the larger range and strength of the ability grow."),
												 13, false, true));
                ++state_;
            } break;
        case 20:
            if (!menus::visible()) {
					 menus::showWindow(TutorialWindow::get(
												 _("Selection..."),
												 locales::format_string(
													 /** TRANSLATORS: all %s are hotkeys on the keyboard.*/
													 gettext("To switch through the abilities, you just have to press %s. "
														"To choose one, you may cycle through them using %s and "
														"%s!\nFor this lesson, select the \"Freezer\"! "
														"Take off, when you got it!"),
													 reinterpret_cast<const char*>(generateName::key(settings::C_playerISpecialKey).toUtf8().c_str()),
													 reinterpret_cast<const char*>(generateName::key(settings::C_playerIleft).toUtf8().c_str()),
													 reinterpret_cast<const char*>(generateName::key(settings::C_playerIright).toUtf8().c_str())),
												 14, false, false));
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
						  menus::showWindow(TutorialWindow::get(
													  _("The Freezer..."),
													  locales::format_string(
														  /** TRANSLATORS: %s is a hotkey on the keyboard.*/
														  gettext("This is a powerful special ability which freezes nearby ships. \n"
															 "You have to fly close to your enemy and press %s "
															 "to freeze him. Watch him loosing life slowly... until he dies!"),
														  reinterpret_cast<const char*>(generateName::key(settings::C_playerISpecialKey).toUtf8().c_str())),
													  15, false, true));
                    state_ += 2;
                }
                else {
						  menus::showWindow(TutorialWindow::get(
													  locales::format_string(
														  /** TRANSLATORS: %s is the player's name.*/
														  gettext("Stop, %s!"),
														  reinterpret_cast<const char*>(settings::C_playerIName.toUtf8().c_str())),
													  _("We told you to select the Freezer, but you have chosen another ability!\n"
														 "Please land again and take the Freezer!"),
													  0, true, false));
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
					 menus::showWindow(TutorialWindow::get(
												 _("Practice!"),
												 _("Now try to freeze your enemy! \n"
													"For this tutorial we will give you some FragStars. But remember: "
													"In real life you have to smash opponents to get those stars!"),
												 16, false, false));
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
					 menus::showWindow(TutorialWindow::get(
												 locales::format_string(
													 /** TRANSLATORS: %s is the player's name.*/
													 gettext("You Missed Him, %s!"),
													 reinterpret_cast<const char*>(settings::C_playerIName.toUtf8().c_str())),
												 _("Try again. we will refill your FragStars."),
												 0, true, false));
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
					 menus::showWindow(TutorialWindow::get(
												 _("You Did It!"),
												 _("Now head back to your planet to select another special ability!"),
												 17, true, false));
                zones::addTutorialZone(Vector2f(SPACE_X_RESOLUTION+20, 450.f), 190.f);
                ++state_;
            } break;
        case 29:
            if (players::getPlayerI()->ship()->docked_ && !menus::visible()) {
                zones::clear();
                zones::createRaster(4, 3);
					 menus::showWindow(TutorialWindow::get(
												 locales::format_string(
													 /** TRANSLATORS: %s is the player's name.*/
													 gettext("Beware, %s!"),
													 reinterpret_cast<const char*>(settings::C_playerIName.toUtf8().c_str())),
												 _("There is no time for further testing!\n"
													"Our enemies have sent two of their real warriors for vengeance!"),
												 18, true, true));
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
					 menus::showWindow(TutorialWindow::get(
												 _("Beat Them!"),
												 locales::format_string(
													 /** TRANSLATORS: %s is the player's name.*/
													 gettext("Prove your skills! Score at least five points by killing them! "
														"Your current score is displayed next to your name!\n"
														"We will send in an experienced warrior assisting you, %s!"),
													 reinterpret_cast<const char*>(settings::C_playerIName.toUtf8().c_str())),
												 19, false, false));
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
					 menus::showWindow(TutorialWindow::get(
												 _("Unbelievable!"),
												 _("You managed all our tasks successfully. Now what to do?\n"
													"Let's have a look at the options menu. There you can adjust a lot of... "
													"well... options.\n"
													"For example you may choose your ship style and color or change your name..."),
												 20, false, false));
                ++state_;
            } break;
        case 33:
            if (!menus::visible()) {
                menus::showWindow(OptionsMenu::get());
                ++state_;
            } break;
        case 34:
            if (!menus::visible()) {
					 menus::showWindow(TutorialWindow::get(
												 _("Who's Best?"),
												 locales::format_string(
													 /** TRANSLATORS: %s is a hotkey on the keyboard.*/
													 gettext("You can display a list of frags, points and so on via %s.\n"
														"Furthermore the numbers next to your name illustrate your frag counter "
														"and your distance to the best one."),
													 reinterpret_cast<const char*>(generateName::key(settings::C_statisticsKey).toUtf8().c_str())),
												 21, false, true));
                ++state_;
            } break;
        case 35:
				if (!menus::visible()) {
					 menus::showWindow(TutorialWindow::get(
												 locales::format_string(
													 /** TRANSLATORS: %s is the player's name.*/
													 gettext("Congratulations, %s!"),
													 reinterpret_cast<const char*>(settings::C_playerIName.toUtf8().c_str())),
												 _("You finished this tutorial. You are now ready to fight in the GREAT WAR. "
													"If you want to continue practising, just wait for the enemies to re-spawn. "
													"You can end your training with <Esc>.\n\n"
													"Have a great time with M.A.R.S.!"),
												 22, false, false));
                ++state_;
            } break;


        case 50:
            if (!menus::visible() && timer::totalTime() > deadTimer_ + 1.f) {
					 menus::showWindow(TutorialWindow::get(
												 _("You Failed!"),
												 locales::format_string(
													 /** TRANSLATORS: %s is the player's name.*/
													 gettext("Awesome, %s! You destroyed your brand new ship. "
														"All right, we will get you a new one. Just wait some seconds..."),
													 reinterpret_cast<const char*>(settings::C_playerIName.toUtf8().c_str())),
												 0, true, false));
                state_ = 99;
            } break;
        case 51:
            if (!menus::visible()) {
					 menus::showWindow(TutorialWindow::get(
												 _("Did You Notice?"),
												 _("Your life is draining! Keep an eye on the upper life bar. "
													"You better land if it's too empty."),
												 0, true, false));
                state_ = 99;
            } break;
        case 52:
            if (!menus::visible()) {
					 menus::showWindow(TutorialWindow::get(
												 _("Did You Notice?"),
												 _("Your fuel is draining! Keep an eye on the yellow fuel bar. "
													"You better land if it's too empty."),
												 0, true, false));
                state_ = 99;
            } break;
        case 53:
            if (!menus::visible()) {
					 menus::showWindow(TutorialWindow::get(
												 _("You Nearly Got Him!"),
												 _("Watch your opponent's name. Its colour indicates his life points. "
													"The more it turns red, the weaker your enemy is."),
												 0, true, false));
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










