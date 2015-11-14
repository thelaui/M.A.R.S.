/* OptionsMenu.cpp

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

# include "Menu/OptionsMenu.hpp"

# include "Media/text.hpp"
# include "Interface/UiWindow.hpp"
# include "Interface/Button.hpp"
# include "Interface/Slider.hpp"
# include "Interface/Checkbox.hpp"
# include "Interface/Tab.hpp"
# include "Interface/TabList.hpp"
# include "Interface/KeyEdit.hpp"
# include "Interface/TextEdit.hpp"
# include "Interface/ShipPreview.hpp"
# include "Interface/Line.hpp"
# include "Interface/RadioGroup.hpp"
# include "Interface/RadioButton.hpp"
# include "Interface/ComboBox.hpp"
# include "Interface/LanguageButton.hpp"
# include "Interface/ColorPicker.hpp"
# include "Interface/LabeledBox.hpp"
# include "System/window.hpp"
# include "System/settings.hpp"
# include "Menu/menus.hpp"
# include "System/generateName.hpp"
# include "Media/music.hpp"
# include "Particles/Star.hpp"
# include "Media/sound.hpp"
# include "Locales/locales.hpp"
# include "Menu/ChooseLanguage.hpp"
# include "Menu/ShaderError.hpp"
# include "Menu/About.hpp"
# include "Menu/Connect.hpp"
# include "Menu/InfoCK.hpp"
# include "Menu/InfoSB.hpp"
# include "Menu/InfoDM.hpp"
# include "Menu/InfoTDM.hpp"
# include "Menu/InfoHide.hpp"
# include "Shaders/postFX.hpp"
# include "defines.hpp"

# include <SFML/Window.hpp>
# include <sstream>

UiWindow* OptionsMenu::instance_(NULL);
bool OptionsMenu::kOk_(false);
bool OptionsMenu::fullscreen_(false);
bool OptionsMenu::vsync_(false);
bool OptionsMenu::shaders_(false);
int OptionsMenu::starfield_(0);
sf::String OptionsMenu::resolution_("");
sf::String OptionsMenu::colorDepth_("");
sf::String OptionsMenu::format_("");
int  OptionsMenu::soundVolume_(0);
int  OptionsMenu::musicVolume_(0);
int  OptionsMenu::announcerVolume_(0);

UiWindow* OptionsMenu::get() {
    if (instance_ == NULL) {
        instance_ = new OptionsMenu(600, 390);

		  instance_->addWidget(new Button(_("OK"), NULL, &kOk_, Vector2f(500,360), 90, 20));
		  instance_->addWidget(new Label(_("Options"), TEXT_ALIGN_LEFT, Vector2f(10,10), 20.f, Color3f(1.f, 0.5f, 0.9f), false));

        TabList* tabList  = new TabList(Vector2f(10,55), 580, 270);
		  Tab* tabInterface = new Tab(_("Interface"), 90);
		  Tab* tabGameplay  = new Tab(_("Gameplay"), 90);
		  Tab* tabGraphics  = new Tab(_("Graphics"), 80);
		  Tab* tabAudio     = new Tab(_("Audio"), 80);
        Tab* tabPlayer1   = new Tab(&settings::C_playerIName, 100);
        Tab* tabPlayer2   = new Tab(&settings::C_playerIIName, 100);

		  tabInterface->addWidget(new LabeledBox(_("General Settings"), Vector2f(10, 30), 560, 110));
		  tabInterface->addWidget(new LanguageButton(_("Language"), Vector2f(20,60), 540, 240));
        std::vector<sf::String> fileFormats;
            fileFormats.push_back("BITMAP (*.bmp)");
            fileFormats.push_back("GIF (*.gif)");
            fileFormats.push_back("JPEG (*.jpg)");
            fileFormats.push_back("PNG(*.png)");
            fileFormats.push_back("TARGA (*.tga)");
		  tabInterface->addWidget(new ComboBox(
											  _("File Format for Screenshots"),
											  _("Sets the format being used when screenshots are saved."),
											  &format_, fileFormats, Vector2f(20,80), 540, 240));
		  tabInterface->addWidget(new KeyEdit(
											  _("Hot Key for Screenshots"),
											  locales::format_string(
												  /** TRANSLATORS: %s is a file path, representing /path/to/mars_config/screenshots.*/
												  _("When this key is pressed, a screenshot is saved to %s"),
												  (settings::C_configPath + std::string("screenshots/")).c_str()),
											  &settings::C_screenShotKey, Vector2f(20,100), 540, 240));
		  tabInterface->addWidget(new KeyEdit(
											  _("Hot Key for Statistics"),
											  _("When this key is pressed while playing a game, a window pops up displaying statistics for the current game."),
											  &settings::C_statisticsKey, Vector2f(20,120), 540, 240));
		  tabInterface->addWidget(new LabeledBox(_("Show Game Information"), Vector2f(10, 160), 560, 90));
		  tabInterface->addWidget(new Checkbox(
											  _("Frames per Second"),
											  _("Displays the amount of Frames calculated per Second.\n"
												 "If this is always very low (<20), you should adjust some settings in graphics tab!"),
											  &settings::C_showFPS, Vector2f(20,190), 140));
		  tabInterface->addWidget(new Checkbox(
											  _("Particle Count"),
											  _("Displays the amount of active particles."),
											  &settings::C_showParticleCount, Vector2f(20,210), 140));
		  tabInterface->addWidget(new Checkbox(
											  _("Display Tool Tips"),
											  _("Dismisses tool tips like this one."),
											  &settings::C_showToolTips, Vector2f(20,230), 140));

		  tabGraphics->addWidget(new Checkbox(
											 _("Full screen"),
											 _("Maximizes the window, making it cover the entire screen."),
											 &fullscreen_, Vector2f(20,60), 150));
		  tabGraphics->addWidget(new LabeledBox(_("Window Settings"), Vector2f(10, 30), 560, 110));
		  tabGraphics->addWidget(new Checkbox(
											 _("V-Sync"),
											 _("Makes the game update according to the refresh rate of your monitor.\n"
												"Enabling this option typically makes the game run more smoothly."),
											 &vsync_, Vector2f(20,80), 150));
		  tabGraphics->addWidget(new Checkbox(
											 _("Shader Effects"),
											 _("Enables cool effects like heat haze, but reduces game performance."),
											 &shaders_, Vector2f(20,100), 150));
        std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
        std::vector<sf::String> resolutions;
        std::vector<sf::String> colorDepths;
        for (std::vector<sf::VideoMode>::iterator it = modes.begin(); it != modes.end(); ++it) {
            if (it->width >= 800 && it->bitsPerPixel >= 8) {
                std::stringstream res, depth;
                res << it->width << " x " << it->height;
                depth << it->bitsPerPixel;
                sf::String resString(res.str()), depthString(depth.str());

                bool newDepth(true), newRes(true);
                for (std::vector<sf::String>::iterator it = resolutions.begin(); it != resolutions.end(); ++it)
                    if (*it == resString) newRes = false;
                for (std::vector<sf::String>::iterator it = colorDepths.begin(); it != colorDepths.end(); ++it)
                    if (*it == depthString) newDepth = false;
                if (newRes)
                    resolutions.push_back(res.str());
                if (newDepth)
                    colorDepths.push_back(depth.str());
            }
        }
        std::vector<sf::String> off;
		  off.push_back(*_("Off"));
		  tabGraphics->addWidget(new ComboBox(
											 _("Resolution"),
											 _("Sets the size of the window.\nAffects both full screen and windowed mode."),
											 &resolution_, resolutions, Vector2f(20,120), 540, 240));
		  tabGraphics->addWidget(new LabeledBox(_("Game Settings"), Vector2f(10, 170), 560, 90));
		  tabGraphics->addWidget(new Checkbox(
											 _("High Star Field Resolution"),
											 _("With high resolution displays, this option makes the star field loop later.\n"
												"Disabling may increase game performance slightly."),
											 &settings::C_StarsHigh, Vector2f(20,200), 150));
		  tabGraphics->addWidget(new Slider(
											 _("Moving Stars"),
											 _("Draws some moving stars on the background. Eye candy, but reduces game performance."),
											 &starfield_, 0, 20000, 1000, Vector2f(20,220), 540, 240, true, off));
		  tabGraphics->addWidget(new Slider(
											 _("Particle Count"),
											 _("Adjusts the global particle count.\nLower values will increase performance significantly."),
											 &settings::C_globalParticleCount, 1, 300, 15, Vector2f(20,240), 540, 240));
		  tabGraphics->addWidget(new Slider(
											 _("Particle Lifetime"),
											 _("Adjusts the global particle life time.\nLower values will increase performance significantly."),
											 &settings::C_globalParticleLifeTime, 1, 300, 15, Vector2f(20,260), 540, 240));

		  tabAudio->addWidget(new LabeledBox(_("Volume Settings"), Vector2f(10, 30), 560, 90));
		  tabAudio->addWidget(new Slider(
										 _("Music"),
										 _("Adjusts volume of background music."),
										 &musicVolume_, 0, 100, 5, Vector2f(20,60), 540, 240, true, off));
		  tabAudio->addWidget(new Slider(
										 _("Sound"),
										 _("Adjusts volume of sound effects."),
										 &soundVolume_, 0, 100, 5, Vector2f(20,80), 540, 240, true, off));
		  tabAudio->addWidget(new Slider(
										 _("Announcer"),
										 _("Adjusts volume of announcers voice."),
										 &announcerVolume_, 0, 100, 5, Vector2f(20,100), 540, 240, true, off));
		  tabAudio->addWidget(new LabeledBox(_("Playback Settings"), Vector2f(10, 140), 560, 60));
		  tabAudio->addWidget(new Checkbox(
										 _("Play Tracks in Random Order"),
										 _("Enables music play back in a random sequence."),
										 &settings::C_audioRandom, Vector2f(20,170), 150));
		  tabAudio->addWidget(new KeyEdit(
										 _("Play Next Track"),
										 _("Pressing this key switches to the next music track."),
										 &settings::C_audioNextKey, Vector2f(20,190), 540, 240));
		  tabAudio->addWidget(new KeyEdit(
										 _("Play Previous Track"),
										 _("Pressing this key switches to the previous music track."),
										 &settings::C_audioPreviousKey, Vector2f(20,210), 540, 240));

		  tabGameplay->addWidget(new LabeledBox(_("Special Effects"), Vector2f(10, 30), 560, 90));
		  tabGameplay->addWidget(new Slider(
											 _("Explosions for Slow Motion"),
											 _("If that many ships explode simultaneously, the slow motion effect kicks in."),
											 &settings::C_slowMoKickIn, 0, 10, 1, Vector2f(20,60), 540, 240, true, off));
		  tabGameplay->addWidget(new Slider(
											 _("Game Speed"),
											 _("Adjusts the game speed in percent."),
											 &settings::C_gameSpeed, 50, 200, 5, Vector2f(20,80), 540, 240, true));
		  tabGameplay->addWidget(new LabeledBox(_("Show Debug Information"), Vector2f(10, 140), 560, 90));
		  tabGameplay->addWidget(new Checkbox(
											 _("Bots Orientation"),
											 _("Displays the job of each bot."),
											 &settings::C_drawBotJobs, Vector2f(20,170), 140));
		  tabGameplay->addWidget(new Checkbox(
											 _("Zones"),
											 _("Displays tactical zones, which are important for the artificial intelligence."),
											 &settings::C_drawZones, Vector2f(20,190), 140));
		  tabGameplay->addWidget(new Checkbox(
											 _("AI-Paths"),
											 _("Displays paths calculated by bots."),
											 &settings::C_drawAIPath, Vector2f(20,210), 140));

		  tabPlayer1->addWidget(new TextEdit(_("Name"), &settings::C_playerIName, "PlayerI", Vector2f(20,30), 540, 240, TEXT_EDIT, 12));
		  tabPlayer1->addWidget(new KeyEdit(_("Accelerate"), NULL, &settings::C_playerIup, Vector2f(20,50), 540, 240));
		  tabPlayer1->addWidget(new KeyEdit(_("Turn Left"), NULL, &settings::C_playerIleft, Vector2f(20,70), 540, 240));
		  tabPlayer1->addWidget(new KeyEdit(_("Turn Right"), NULL, &settings::C_playerIright, Vector2f(20,90), 540, 240));
		  tabPlayer1->addWidget(new KeyEdit(_("Weapon"), NULL, &settings::C_playerIfire, Vector2f(20,110), 540, 240));
		  tabPlayer1->addWidget(new KeyEdit(_("Special Ability"), NULL, &settings::C_playerISpecialKey, Vector2f(20,130), 540, 240));
		  tabPlayer1->addWidget(new LabeledBox(_("Ship Settings"), Vector2f(10, 170), 560, 90));
        tabPlayer1->addWidget(new ShipPreview(&settings::C_playerIColor, &settings::C_playerITeamColor, &settings::C_playerIShip, Vector2f(510,210)));
		  tabPlayer1->addWidget(new Slider(_("Ship"), NULL, &settings::C_playerIShip, 0, SHIP_GRAPHICS_COUNT, 1, Vector2f(20,200), 410, 240, true, generateName::shipNames()));
		  tabPlayer1->addWidget(new ColorPicker(_("Player Color"), &settings::C_playerIColor, Vector2f(20,220), 410, 240));
		  tabPlayer1->addWidget(new ColorPicker(_("Team Color"), &settings::C_playerITeamColor, Vector2f(20,240), 410, 240));

		  tabPlayer2->addWidget(new TextEdit(_("Name"), &settings::C_playerIIName, "PlayerII", Vector2f(20,30), 540, 240, TEXT_EDIT, 12));
		  tabPlayer2->addWidget(new KeyEdit(_("Accelerate"), NULL, &settings::C_playerIIup, Vector2f(20,50), 540, 240));
		  tabPlayer2->addWidget(new KeyEdit(_("Turn Left"), NULL, &settings::C_playerIIleft, Vector2f(20,70), 540, 240));
		  tabPlayer2->addWidget(new KeyEdit(_("Turn Right"), NULL, &settings::C_playerIIright, Vector2f(20,90), 540, 240));
		  tabPlayer2->addWidget(new KeyEdit(_("Weapon"), NULL, &settings::C_playerIIfire, Vector2f(20,110), 540, 240));
		  tabPlayer2->addWidget(new KeyEdit(_("Special Ability"), NULL, &settings::C_playerIISpecialKey, Vector2f(20,130), 540, 240));
		  tabPlayer2->addWidget(new LabeledBox(_("Ship Settings"), Vector2f(10, 170), 560, 90));
        tabPlayer2->addWidget(new ShipPreview(&settings::C_playerIIColor, &settings::C_playerIITeamColor, &settings::C_playerIIShip, Vector2f(510,210)));
		  tabPlayer2->addWidget(new Slider(_("Ship"), NULL, &settings::C_playerIIShip, 0, SHIP_GRAPHICS_COUNT, 1, Vector2f(20,200), 410, 240, true, generateName::shipNames()));
		  tabPlayer2->addWidget(new ColorPicker(_("Player Color"), &settings::C_playerIIColor, Vector2f(20,220), 410, 240));
		  tabPlayer2->addWidget(new ColorPicker(_("Team Color"), &settings::C_playerIITeamColor, Vector2f(20,240), 410, 240));

        tabList->addTab(tabInterface);
        tabList->addTab(tabGraphics);
        tabList->addTab(tabAudio);
        tabList->addTab(tabGameplay);
        tabList->addTab(tabPlayer2);
        tabList->addTab(tabPlayer1);
        instance_->addWidget(tabList);
    }
    return instance_;
}

void OptionsMenu::checkWidgets() {
    if (kOk_) {
        kOk_ = false;

        int resX, resY, depth;
        std::stringstream sstr1(resolution_);
        sstr1 >> resX;
        char x; sstr1 >> x;
        sstr1 >> resY;

        std::stringstream sstr2(colorDepth_);
        sstr2 >> depth;

        if (fullscreen_ != settings::C_fullScreen || vsync_ != settings::C_vsync || shaders_ != settings::C_shaders
            || resX != settings::C_resX || resY != settings::C_resY || depth != settings::C_colorDepth) {

            settings::C_resX = resX;
            settings::C_resY = resY;
            settings::C_colorDepth = depth;
            settings::C_fullScreen = fullscreen_;
            settings::C_vsync = vsync_;
            settings::C_shaders = shaders_;
            window::create();
        }

        if      (format_ == "BITMAP (*.bmp)") settings::C_screenShotFormat = "bmp";
        else if (format_ == "GIF (*.gif)")    settings::C_screenShotFormat = "gif";
        else if (format_ == "TARGA (*.tga)")  settings::C_screenShotFormat = "tga";
        else if (format_ == "PNG(*.png)")     settings::C_screenShotFormat = "png";
        else if (format_ == "JPEG (*.jpg)")   settings::C_screenShotFormat = "jpg";

        if (starfield_ != settings::C_StarField) {
            settings::C_StarField = starfield_;
            if (settings::C_StarField > 0)
                Star::init();
            else
                Star::clear();
        }

		  locales::load();
        menus::reload();

        settings::save();
        menus::hideWindow();
    }
    if (shaders_ != settings::C_shaders) {
        if (shaders_ && !postFX::supported()) {
            shaders_ = false;
            settings::C_shaders = false;
            menus::showWindow(ShaderError::get());
        }
    }
    if (soundVolume_ != settings::C_soundVolume) {
        settings::C_soundVolume = soundVolume_;
    }
    if (musicVolume_ != settings::C_musicVolume) {
        settings::C_musicVolume = musicVolume_;
        music::setGlobalVolume();
    }
    if (announcerVolume_ != settings::C_announcerVolume) {
        settings::C_announcerVolume = announcerVolume_;
    }
}

void OptionsMenu::onShow() {
    fullscreen_      = settings::C_fullScreen;
    vsync_           = settings::C_vsync;
    shaders_         = settings::C_shaders;
    soundVolume_     = settings::C_soundVolume;
    musicVolume_     = settings::C_musicVolume;
    announcerVolume_ = settings::C_announcerVolume;
    starfield_       = settings::C_StarField;

    if      (settings::C_screenShotFormat == "bmp") format_ = "BITMAP (*.bmp)";
    else if (settings::C_screenShotFormat == "gif") format_ = "GIF (*.gif)";
    else if (settings::C_screenShotFormat == "tga") format_ = "TARGA (*.tga)";
    else if (settings::C_screenShotFormat == "png") format_ = "PNG(*.png)";
    else if (settings::C_screenShotFormat == "jpg") format_ = "JPEG (*.jpg)";

    sf::VideoMode mode(settings::C_resX, settings::C_resY);
    std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
    if (modes.size() > 0 && !mode.isValid()) {
        mode = sf::VideoMode::getFullscreenModes().front();
        settings::C_resX = mode.width;
        settings::C_resY = mode.height;
    }

    std::stringstream sstr1;
    sstr1 << settings::C_resX << " x " << settings::C_resY;
    resolution_ = sstr1.str();

    std::stringstream sstr2;
    sstr2 << settings::C_colorDepth;
    colorDepth_ = sstr2.str();
}

void OptionsMenu::reset() {
    if (instance_)
        delete instance_;
    instance_ = NULL;
}

