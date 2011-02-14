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
# include "System/window.hpp"
# include "System/settings.hpp"
# include "Menu/menus.hpp"
# include "System/generateName.hpp"
# include "Media/music.hpp"
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
# include "Tutorial/TutWindow01.hpp"
# include "Tutorial/TutWindow02.hpp"
# include "Tutorial/TutWindow03.hpp"
# include "Tutorial/TutWindow04.hpp"
# include "Tutorial/TutWindow05.hpp"
# include "Tutorial/TutWindow06.hpp"
# include "Tutorial/TutWindow07.hpp"
# include "Tutorial/TutWindow08.hpp"
# include "Tutorial/TutWindow09.hpp"
# include "Tutorial/TutWindow10.hpp"
# include "Tutorial/TutWindow11.hpp"
# include "Tutorial/TutWindow12.hpp"
# include "Tutorial/TutWindow13.hpp"
# include "Tutorial/TutWindow14.hpp"
# include "Tutorial/TutWindow15.hpp"
# include "Tutorial/TutWindow16.hpp"
# include "Tutorial/TutWindow17.hpp"
# include "Tutorial/TutWindow18.hpp"
# include "Tutorial/TutWindow19.hpp"
# include "Tutorial/TutWindow20.hpp"
# include "Shaders/postFX.hpp"

# include <SFML/Window.hpp>
# include <sstream>

UiWindow* OptionsMenu::instance_(NULL);
bool OptionsMenu::kOk_(false);
bool OptionsMenu::fullscreen_(false);
bool OptionsMenu::vsync_(false);
bool OptionsMenu::shaders_(false);
sf::String OptionsMenu::resolution_("");
sf::String OptionsMenu::colorDepth_("");
sf::String OptionsMenu::format_("");
int  OptionsMenu::soundVolume_(0);
int  OptionsMenu::musicVolume_(0);
int  OptionsMenu::announcerVolume_(0);

UiWindow* OptionsMenu::get() {
    if (instance_ == NULL) {
        instance_ = new OptionsMenu(600, 370);

        instance_->addWidget(new Button(locales::getLocale(locales::Ok), NULL, &kOk_, Vector2f(520,340), 70, 20));
        instance_->addWidget(new Label(locales::getLocale(locales::Options), TEXT_ALIGN_LEFT, Vector2f(10,10), 20.f, Color3f(1.f, 0.5f, 0.9f), false));

        TabList* tabList  = new TabList(Vector2f(10,55), 580, 250);
        Tab* tabInterface = new Tab(locales::getLocale(locales::Interface), 90);
        Tab* tabGameplay  = new Tab(locales::getLocale(locales::Gameplay), 90);
        Tab* tabGraphics  = new Tab(locales::getLocale(locales::Display), 80);
        Tab* tabAudio     = new Tab(locales::getLocale(locales::Audio), 80);
        Tab* tabPlayer1   = new Tab(&settings::C_playerIName, 100);
        Tab* tabPlayer2   = new Tab(&settings::C_playerIIName, 100);

        tabInterface->addWidget(new Label(locales::getLocale(locales::GameInformation), TEXT_ALIGN_LEFT, Vector2f(10,30), 12.f, Color3f(1.f, 0.5f, 0.9f), false));
        tabInterface->addWidget(new Checkbox(locales::getLocale(locales::FramesPerSecond), locales::getLocale(locales::ttFramesPerSecond), &settings::C_showFPS, Vector2f(10,50), 150));
        tabInterface->addWidget(new Checkbox(locales::getLocale(locales::ParticleCount), locales::getLocale(locales::ttParticleCount), &settings::C_showParticleCount, Vector2f(10,70), 150));
        tabInterface->addWidget(new Checkbox(locales::getLocale(locales::ShowToolTips), locales::getLocale(locales::ttShowToolTips), &settings::C_showToolTips, Vector2f(10,90), 150));
        tabInterface->addWidget(new Label(locales::getLocale(locales::DebuggingInformation), TEXT_ALIGN_LEFT, Vector2f(210,30), 12.f, Color3f(1.f, 0.5f, 0.9f), false));
        tabInterface->addWidget(new Checkbox(locales::getLocale(locales::AIJobs), locales::getLocale(locales::ttBotsOrientation), &settings::C_drawBotJobs, Vector2f(210,50), 150));
        tabInterface->addWidget(new Checkbox(locales::getLocale(locales::Zones), locales::getLocale(locales::ttZones), &settings::C_drawZones, Vector2f(210,70), 150));
        tabInterface->addWidget(new Checkbox(locales::getLocale(locales::AIPaths), locales::getLocale(locales::ttAIPaths), &settings::C_drawAIPath, Vector2f(210,90), 150));
        tabInterface->addWidget(new LanguageButton(locales::getLocale(locales::Language), Vector2f(10,130), 350, 185));
        std::vector<sf::String> fileFormats;
            fileFormats.push_back("BITMAP (*.bmp)");
            fileFormats.push_back("GIF (*.gif)");
            fileFormats.push_back("JPEG (*.jpg)");
            fileFormats.push_back("PNG(*.png)");
            fileFormats.push_back("TARGA (*.tga)");
        tabInterface->addWidget(new ComboBox(locales::getLocale(locales::ScreenShotFormat), locales::getLocale(locales::ttScreenShotFormat), &format_, fileFormats, Vector2f(10,160), 350, 185));
        tabInterface->addWidget(new KeyEdit(locales::getLocale(locales::ScreenShotKey), locales::getLocale(locales::ttScreenShotKey), &settings::C_screenShotKey, Vector2f(10,180), 350));

        tabGraphics->addWidget(new Checkbox(locales::getLocale(locales::Fullscreen), locales::getLocale(locales::ttFullscreen), &fullscreen_, Vector2f(10,50), 150));
        tabGraphics->addWidget(new Label(locales::getLocale(locales::WindowSettings), TEXT_ALIGN_LEFT, Vector2f(10,30), 12.f, Color3f(1.f, 0.5f, 0.9f), false));
        tabGraphics->addWidget(new Checkbox(locales::getLocale(locales::VerticalSynchronisation), locales::getLocale(locales::ttVerticalSynchronisation), &vsync_, Vector2f(10,70), 150));
        tabGraphics->addWidget(new Checkbox(locales::getLocale(locales::Shaders), locales::getLocale(locales::ttShaders), &shaders_, Vector2f(10,90), 150));
        std::vector<sf::VideoMode> modes = sf::VideoMode::GetFullscreenModes();
        std::vector<sf::String> resolutions;
        std::vector<sf::String> colorDepths;
        for (std::vector<sf::VideoMode>::iterator it = modes.begin(); it != modes.end(); ++it) {
            if (it->Width >= 800 && it->BitsPerPixel >= 8) {
                std::stringstream res, depth;
                res << it->Width << " x " << it->Height;
                depth << it->BitsPerPixel;
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
        tabGraphics->addWidget(new ComboBox(locales::getLocale(locales::Resolution), locales::getLocale(locales::ttResolution), &resolution_, resolutions, Vector2f(10,110), 360, 185));
        tabGraphics->addWidget(new Label(locales::getLocale(locales::GameSettings), TEXT_ALIGN_LEFT, Vector2f(10,170), 12.f, Color3f(1.f, 0.5f, 0.9f), false));
        tabGraphics->addWidget(new Checkbox(locales::getLocale(locales::StarsHigh), locales::getLocale(locales::ttStarsHigh), &settings::C_StarsHigh, Vector2f(10,190), 150));
        tabGraphics->addWidget(new Slider(locales::getLocale(locales::ParticleCountSlider), locales::getLocale(locales::ttParticleCountSlider), &settings::C_globalParticleCount, 1, 300, Vector2f(10,210), 560));
        tabGraphics->addWidget(new Slider(locales::getLocale(locales::ParticleLifetime), locales::getLocale(locales::ttParticleLifetime), &settings::C_globalParticleLifeTime, 1, 300, Vector2f(10,230), 560));

        std::vector<sf::String> off;
        off.push_back(*locales::getLocale(locales::SlowMoOff));
        tabGameplay->addWidget(new Slider(locales::getLocale(locales::SlowMoKickIn), locales::getLocale(locales::ttSlowMoKickIn), &settings::C_slowMoKickIn, 0, 10, Vector2f(10,30), 560, 210, true, off));

        tabAudio->addWidget(new Slider(locales::getLocale(locales::MusicVolume), locales::getLocale(locales::ttMusicVolume), &musicVolume_, 0, 100, Vector2f(10,30), 560, 210, true));
        tabAudio->addWidget(new Slider(locales::getLocale(locales::SoundVolume), locales::getLocale(locales::ttSoundVolume), &soundVolume_, 0, 100, Vector2f(10,50), 560, 210, true));
        tabAudio->addWidget(new Slider(locales::getLocale(locales::AnnouncerVolume), locales::getLocale(locales::ttAnnouncerVolume), &announcerVolume_, 0, 100, Vector2f(10,70), 560, 210, true));

        tabPlayer1->addWidget(new TextEdit(locales::getLocale(locales::Name), &settings::C_playerIName, Vector2f(10,30), 560, TEXT_EDIT, 12));
        tabPlayer1->addWidget(new KeyEdit(locales::getLocale(locales::Accelerate), NULL, &settings::C_playerIup, Vector2f(10,50), 560));
        tabPlayer1->addWidget(new KeyEdit(locales::getLocale(locales::TurnLeft), NULL, &settings::C_playerIleft, Vector2f(10,70), 560));
        tabPlayer1->addWidget(new KeyEdit(locales::getLocale(locales::TurnRight), NULL, &settings::C_playerIright, Vector2f(10,90), 560));
        tabPlayer1->addWidget(new KeyEdit(locales::getLocale(locales::Fire), NULL, &settings::C_playerIfire, Vector2f(10,110), 560));
        tabPlayer1->addWidget(new KeyEdit(locales::getLocale(locales::SpecialKey), NULL, &settings::C_playerISpecialKey, Vector2f(10,130), 560));
        tabPlayer1->addWidget(new ShipPreview(&settings::C_playerIColor, &settings::C_playerITeamColor, &settings::C_playerIShip, Vector2f(500,200)));
        tabPlayer1->addWidget(new Slider(locales::getLocale(locales::ShipName), NULL, &settings::C_playerIShip, 0, 10, Vector2f(10,170), 420, 185, true, generateName::shipNames()));
        tabPlayer1->addWidget(new ColorPicker(locales::getLocale(locales::PlayerColor), &settings::C_playerIColor, Vector2f(10,200), 420, 185));
        tabPlayer1->addWidget(new ColorPicker(locales::getLocale(locales::TeamColor), &settings::C_playerITeamColor, Vector2f(10,230), 420, 185));

        tabPlayer2->addWidget(new TextEdit(locales::getLocale(locales::Name), &settings::C_playerIIName, Vector2f(10,30), 560, TEXT_EDIT, 12));
        tabPlayer2->addWidget(new KeyEdit(locales::getLocale(locales::Accelerate), NULL, &settings::C_playerIIup, Vector2f(10,50), 560));
        tabPlayer2->addWidget(new KeyEdit(locales::getLocale(locales::TurnLeft), NULL, &settings::C_playerIIleft, Vector2f(10,70), 560));
        tabPlayer2->addWidget(new KeyEdit(locales::getLocale(locales::TurnRight), NULL, &settings::C_playerIIright, Vector2f(10,90), 560));
        tabPlayer2->addWidget(new KeyEdit(locales::getLocale(locales::Fire), NULL, &settings::C_playerIIfire, Vector2f(10,110), 560));
        tabPlayer2->addWidget(new KeyEdit(locales::getLocale(locales::SpecialKey), NULL, &settings::C_playerIISpecialKey, Vector2f(10,130), 560));
        tabPlayer2->addWidget(new ShipPreview(&settings::C_playerIIColor, &settings::C_playerIITeamColor, &settings::C_playerIIShip, Vector2f(500,200)));
        tabPlayer2->addWidget(new Slider(locales::getLocale(locales::ShipName), NULL, &settings::C_playerIIShip, 0, 10, Vector2f(10,170), 420, 185, true, generateName::shipNames()));
        tabPlayer2->addWidget(new ColorPicker(locales::getLocale(locales::PlayerColor), &settings::C_playerIIColor, Vector2f(10,200), 420, 185));
        tabPlayer2->addWidget(new ColorPicker(locales::getLocale(locales::TeamColor), &settings::C_playerIITeamColor, Vector2f(10,230), 420, 185));

        tabList->addTab(tabInterface);
        tabList->addTab(tabGameplay);
        tabList->addTab(tabGraphics);
        tabList->addTab(tabAudio);
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

    if      (settings::C_screenShotFormat == "bmp") format_ = "BITMAP (*.bmp)";
    else if (settings::C_screenShotFormat == "gif") format_ = "GIF (*.gif)";
    else if (settings::C_screenShotFormat == "tga") format_ = "TARGA (*.tga)";
    else if (settings::C_screenShotFormat == "png") format_ = "PNG(*.png)";
    else if (settings::C_screenShotFormat == "jpg") format_ = "JPEG (*.jpg)";

    sf::VideoMode mode(settings::C_resX, settings::C_resY);
    if (!mode.IsValid()) {
        mode = sf::VideoMode::GetFullscreenModes().front();
        settings::C_resX = mode.Width;
        settings::C_resY = mode.Height;
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

