/* OptionsMenu.cpp

Copyright (c) 2010 by Felix Lauer and Simon Schneegans

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
# include "System/window.hpp"
# include "System/settings.hpp"
# include "Menu/menus.hpp"
# include "System/generateName.hpp"
# include "Media/music.hpp"
# include "Media/sound.hpp"
# include "Locales/locales.hpp"
# include "Menu/ChooseLanguage.hpp"
# include "Menu/ShaderError.hpp"
# include "Shaders/postFX.hpp"

UiWindow* OptionsMenu::instance_(NULL);
bool OptionsMenu::kOk_(false);
bool OptionsMenu::kChooseLanguage_(false);
bool OptionsMenu::fullscreen_(false);
bool OptionsMenu::vsync_(false);
bool OptionsMenu::shaders_(false);
int  OptionsMenu::soundVolume_(0);
int  OptionsMenu::musicVolume_(0);
int  OptionsMenu::announcerVolume_(0);
int  OptionsMenu::hue1_(0);
int  OptionsMenu::sat1_(0);
int  OptionsMenu::val1_(0);
int  OptionsMenu::hue2_(0);
int  OptionsMenu::sat2_(0);
int  OptionsMenu::val2_(0);

UiWindow* OptionsMenu::get() {
    if (instance_ == NULL) {
        instance_ = new OptionsMenu(480, 350);

        instance_->addWidget(new Label(locales::getLocale(locales::Options), TEXT_ALIGN_LEFT, Vector2f(10,10), 20.f));
        instance_->addWidget(new Button(locales::getLocale(locales::Ok), &kOk_, Vector2f(400,320), 70, 20));

        TabList* tabList  = new TabList(Vector2f(10,55), 460, 250);
        Tab* tabInterface = new Tab(locales::getLocale(locales::Interface), 90);
        Tab* tabGraphics  = new Tab(locales::getLocale(locales::Display), 80);
        Tab* tabAudio     = new Tab(locales::getLocale(locales::Audio), 60);
        Tab* tabPlayer1   = new Tab(&settings::C_playerIName, 90);
        Tab* tabPlayer2   = new Tab(&settings::C_playerIIName, 90);

        tabPlayer1->addWidget(new TextEdit(locales::getLocale(locales::Name), &settings::C_playerIName, Vector2f(10,30), 440, TEXT_EDIT, 12));
        tabPlayer1->addWidget(new KeyEdit(locales::getLocale(locales::Accelerate), &settings::C_playerIup, Vector2f(10,50), 440));
        tabPlayer1->addWidget(new KeyEdit(locales::getLocale(locales::TurnLeft), &settings::C_playerIleft, Vector2f(10,70), 440));
        tabPlayer1->addWidget(new KeyEdit(locales::getLocale(locales::TurnRight), &settings::C_playerIright, Vector2f(10,90), 440));
        tabPlayer1->addWidget(new KeyEdit(locales::getLocale(locales::Weapon), &settings::C_playerIfire, Vector2f(10,110), 440));
        tabPlayer1->addWidget(new ShipPreview(&settings::C_playerIColor, &settings::C_playerIShip, Vector2f(20,180)));
        tabPlayer1->addWidget(new Slider(locales::getLocale(locales::ShipName), &settings::C_playerIShip, 1, 11, Vector2f(60,150), 380, 135, true, generateName::shipNames()));
        tabPlayer1->addWidget(new Slider(locales::getLocale(locales::Hue), &hue1_, 0, 360, Vector2f(60,170), 380, 135, true));
        tabPlayer1->addWidget(new Slider(locales::getLocale(locales::Saturation), &sat1_, 0, 255, Vector2f(60,190), 380, 135, true));
        tabPlayer1->addWidget(new Slider(locales::getLocale(locales::Value), &val1_, 0, 255, Vector2f(60,210), 380, 135, true));

        tabPlayer2->addWidget(new TextEdit(locales::getLocale(locales::Name), &settings::C_playerIIName, Vector2f(10,30), 440, TEXT_EDIT, 12));
        tabPlayer2->addWidget(new KeyEdit(locales::getLocale(locales::Accelerate), &settings::C_playerIIup, Vector2f(10,50), 440));
        tabPlayer2->addWidget(new KeyEdit(locales::getLocale(locales::TurnLeft), &settings::C_playerIIleft, Vector2f(10,70), 440));
        tabPlayer2->addWidget(new KeyEdit(locales::getLocale(locales::TurnRight), &settings::C_playerIIright, Vector2f(10,90), 440));
        tabPlayer2->addWidget(new KeyEdit(locales::getLocale(locales::Weapon), &settings::C_playerIIfire, Vector2f(10,110), 440));
        tabPlayer2->addWidget(new ShipPreview(&settings::C_playerIIColor, &settings::C_playerIIShip, Vector2f(20,180)));
        tabPlayer2->addWidget(new Slider(locales::getLocale(locales::ShipName), &settings::C_playerIIShip, 1, 11, Vector2f(60,150), 380, 135, true, generateName::shipNames()));
        tabPlayer2->addWidget(new Slider(locales::getLocale(locales::Hue), &hue2_, 0, 360, Vector2f(60,170), 380, 135, true));
        tabPlayer2->addWidget(new Slider(locales::getLocale(locales::Saturation), &sat2_, 0, 255, Vector2f(60,190), 380, 135, true));
        tabPlayer2->addWidget(new Slider(locales::getLocale(locales::Value), &val2_, 0, 255, Vector2f(60,210), 380, 135, true));

        tabGraphics->addWidget(new Slider(locales::getLocale(locales::ParticleCountSlider), &settings::C_globalParticleCount, 1, 300, Vector2f(10,130), 440));
        tabGraphics->addWidget(new Slider(locales::getLocale(locales::ParticleLifetime), &settings::C_globalParticleLifeTime, 1, 300, Vector2f(10,150), 440));
        tabGraphics->addWidget(new Label(locales::getLocale(locales::WindowSettings), TEXT_ALIGN_LEFT, Vector2f(10,30), 12.f));
        tabGraphics->addWidget(new Checkbox(locales::getLocale(locales::Fullscreen), &fullscreen_, Vector2f(10,50), 150));
        tabGraphics->addWidget(new Checkbox(locales::getLocale(locales::VerticalSynchronisation), &vsync_, Vector2f(10,70), 150));
        tabGraphics->addWidget(new Checkbox(locales::getLocale(locales::Shaders), &shaders_, Vector2f(10,90), 150));
        tabGraphics->addWidget(new Label(locales::getLocale(locales::ShowStars), TEXT_ALIGN_LEFT, Vector2f(210,30), 12.f));
        RadioGroup* group = new RadioGroup();
            group->addRadioButton(new RadioButton(locales::getLocale(locales::StarsHigh), &settings::C_StarsHigh, Vector2f(210,50), 150));
            group->addRadioButton(new RadioButton(locales::getLocale(locales::StarsLow), &settings::C_StarsLow, Vector2f(210,70), 150));
            group->addRadioButton(new RadioButton(locales::getLocale(locales::StarsNo), &settings::C_StarsNo, Vector2f(210,90), 150));
        tabGraphics->addWidget(group);

        tabInterface->addWidget(new Label(locales::getLocale(locales::DebuggingInformation), TEXT_ALIGN_LEFT, Vector2f(210,30), 12.f));
        tabInterface->addWidget(new Checkbox(locales::getLocale(locales::BotsOrientation), &settings::C_drawBotOrientation, Vector2f(210,50), 150));
        tabInterface->addWidget(new Checkbox(locales::getLocale(locales::Zones), &settings::C_drawZones, Vector2f(210,70), 150));
        tabInterface->addWidget(new Checkbox(locales::getLocale(locales::AIPaths), &settings::C_drawAIPath, Vector2f(210,90), 150));
        tabInterface->addWidget(new Label(locales::getLocale(locales::GameInformation), TEXT_ALIGN_LEFT, Vector2f(10,30), 12.f));
        tabInterface->addWidget(new Checkbox(locales::getLocale(locales::FramesPerSecond), &settings::C_showFPS, Vector2f(10,50), 150));
        tabInterface->addWidget(new Checkbox(locales::getLocale(locales::ParticleCount), &settings::C_showParticleCount, Vector2f(10,70), 150));
        tabInterface->addWidget(new Button(new sf::String("Select Language"), &kChooseLanguage_, Vector2f(10,130), 120, 20));

        tabAudio->addWidget(new Slider(locales::getLocale(locales::MusicVolume), &musicVolume_, 0, 100, Vector2f(10,30), 440, 185, true));
        tabAudio->addWidget(new Slider(locales::getLocale(locales::SoundVolume), &soundVolume_, 0, 100, Vector2f(10,50), 440, 185, true));
        tabAudio->addWidget(new Slider(locales::getLocale(locales::AnnouncerVolume), &announcerVolume_, 0, 100, Vector2f(10,70), 440, 185, true));

        tabList->addTab(tabInterface);
        tabList->addTab(tabGraphics);
        tabList->addTab(tabAudio);
        tabList->addTab(tabPlayer1);
        tabList->addTab(tabPlayer2);
        instance_->addWidget(tabList);
    }
    return instance_;
}

void OptionsMenu::checkWidgets() {
    if (kOk_) {
        kOk_ = false;
        settings::save();
        menus::hideWindow();
    }
    if (kChooseLanguage_) {
        kChooseLanguage_ = false;
        menus::showWindow(ChooseLanguage::get());
    }
    if (fullscreen_ != settings::C_fullScreen) {
        settings::C_fullScreen = fullscreen_;
        window::create();
    }
    if (vsync_ != settings::C_vsync) {
        settings::C_vsync = vsync_;
        window::applyGlobalSettings();
    }
    if (shaders_ != settings::C_shaders) {
        settings::C_shaders = shaders_;
        if (shaders_ && !postFX::supported()) {
            shaders_ = false;
            settings::C_shaders = false;
            menus::showWindow(ShaderError::get());
        }
        else
            window::applyGlobalSettings();
    }
    if ((hue1_ != settings::C_playerIColor.h()) | (sat1_ != settings::C_playerIColor.s()*255) | (val1_ != settings::C_playerIColor.v()*255)) {
        settings::C_playerIColor.h(hue1_);
        settings::C_playerIColor.s(static_cast<float>(sat1_)/255.f);
        settings::C_playerIColor.v(static_cast<float>(val1_)/255.f);
    }
    if ((hue2_ != settings::C_playerIIColor.h()) | (sat2_ != settings::C_playerIIColor.s()*255) | (val2_ != settings::C_playerIIColor.v()*255)) {
        settings::C_playerIIColor.h(hue2_);
        settings::C_playerIIColor.s(static_cast<float>(sat2_)/255.f);
        settings::C_playerIIColor.v(static_cast<float>(val2_)/255.f);
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
    fullscreen_ = settings::C_fullScreen;
    vsync_      = settings::C_vsync;
    shaders_    = settings::C_shaders;
    soundVolume_= settings::C_soundVolume;
    musicVolume_= settings::C_musicVolume;
    announcerVolume_= settings::C_announcerVolume;

    hue1_ = settings::C_playerIColor.h();
    sat1_ = settings::C_playerIColor.s()*255;
    val1_ = settings::C_playerIColor.v()*255;
    hue2_ = settings::C_playerIIColor.h();
    sat2_ = settings::C_playerIIColor.s()*255;
    val2_ = settings::C_playerIIColor.v()*255;
}



