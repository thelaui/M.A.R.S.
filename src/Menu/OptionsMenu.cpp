/* OptionsMenu.cpp

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
# include "System/window.hpp"
# include "System/settings.hpp"
# include "Menu/menus.hpp"
# include "System/generateName.hpp"
# include "Media/music.hpp"
# include "Media/sound.hpp"

UiWindow* OptionsMenu::instance_(NULL);
bool OptionsMenu::kOk_(false);
bool OptionsMenu::fullscreen_(false);
bool OptionsMenu::vsync_(false);
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

        instance_->addWidget(new Label("Options", TEXT_ALIGN_LEFT, Vector2f(10,10), 20.f));
        instance_->addWidget(new Button("Ok", &kOk_, Vector2f(400,320), 70, 20));

        TabList* tabList = new TabList(Vector2f(10,55), 460, 250);
        Tab* tabView     = new Tab("View", 50);
        Tab* tabGraphics = new Tab("Graphic", 50);
        Tab* tabAudio    = new Tab("Audio", 50);
        Tab* tabPlayer1  = new Tab("Player I", 60);
        Tab* tabPlayer2  = new Tab("Player II", 60);

        tabPlayer1->addWidget(new TextEdit("Name", &settings::C_playerIName, Vector2f(10,30), 440, TEXT_EDIT, 12));
        tabPlayer1->addWidget(new KeyEdit("Accelerate", &settings::C_playerIup, Vector2f(10,50), 440));
        tabPlayer1->addWidget(new KeyEdit("Turn Left", &settings::C_playerIleft, Vector2f(10,70), 440));
        tabPlayer1->addWidget(new KeyEdit("Turn Right", &settings::C_playerIright, Vector2f(10,90), 440));
        tabPlayer1->addWidget(new KeyEdit("Weapon", &settings::C_playerIfire, Vector2f(10,110), 440));
        tabPlayer1->addWidget(new ShipPreview(&settings::C_playerIColor, &settings::C_playerIShip, Vector2f(20,180)));
        tabPlayer1->addWidget(new Slider("Ship", &settings::C_playerIShip, 1, 11, Vector2f(60,150), 380, 135, true, generateName::shipNames()));
        tabPlayer1->addWidget(new Slider("Hue", &hue1_, 0, 360, Vector2f(60,170), 380, 135, true));
        tabPlayer1->addWidget(new Slider("Saturation", &sat1_, 0, 255, Vector2f(60,190), 380, 135, true));
        tabPlayer1->addWidget(new Slider("Value", &val1_, 0, 255, Vector2f(60,210), 380, 135, true));

        tabPlayer2->addWidget(new TextEdit("Name", &settings::C_playerIIName, Vector2f(10,30), 440, TEXT_EDIT, 12));
        tabPlayer2->addWidget(new KeyEdit("Accelerate", &settings::C_playerIIup, Vector2f(10,50), 440));
        tabPlayer2->addWidget(new KeyEdit("Turn Left", &settings::C_playerIIleft, Vector2f(10,70), 440));
        tabPlayer2->addWidget(new KeyEdit("Turn Right", &settings::C_playerIIright, Vector2f(10,90), 440));
        tabPlayer2->addWidget(new KeyEdit("Weapon", &settings::C_playerIIfire, Vector2f(10,110), 440));
        tabPlayer2->addWidget(new ShipPreview(&settings::C_playerIIColor, &settings::C_playerIIShip, Vector2f(20,180)));
        tabPlayer2->addWidget(new Slider("Ship", &settings::C_playerIIShip, 1, 11, Vector2f(60,150), 380, 135, true, generateName::shipNames()));
        tabPlayer2->addWidget(new Slider("Hue", &hue2_, 0, 360, Vector2f(60,170), 380, 135, true));
        tabPlayer2->addWidget(new Slider("Saturation", &sat2_, 0, 255, Vector2f(60,190), 380, 135, true));
        tabPlayer2->addWidget(new Slider("Value", &val2_, 0, 255, Vector2f(60,210), 380, 135, true));

        tabGraphics->addWidget(new Slider("Particle Count", &settings::C_globalParticleCount, 1, 300, Vector2f(10,30), 440));
        tabGraphics->addWidget(new Slider("Particle Lifetime", &settings::C_globalParticleLifeTime, 1, 300, Vector2f(10,50), 440));
        //tabGraphics->addWidget(new Checkbox("Adaptive Particle Count", &settings::C_adaptiveParticleCount, Vector2f(10,70), 170));
        tabGraphics->addWidget(new Checkbox("Stars", &settings::C_showStars, Vector2f(10,90), 170));
        tabGraphics->addWidget(new Checkbox("Fullscreen", &fullscreen_, Vector2f(10,110), 170));
        tabGraphics->addWidget(new Checkbox("V-Sync", &vsync_, Vector2f(10,130), 170));

        tabAudio->addWidget(new Slider("Music Volume", &musicVolume_, 0, 100, Vector2f(10,30), 440, 185, true));
        tabAudio->addWidget(new Slider("Sound Volume", &soundVolume_, 0, 100, Vector2f(10,50), 440, 185, true));
        tabAudio->addWidget(new Slider("Announcer Volume", &announcerVolume_, 0, 100, Vector2f(10,70), 440, 185, true));

        tabView->addWidget(new Checkbox("Show Local Names", &settings::C_drawLocalNames, Vector2f(10,30), 170));
        tabView->addWidget(new Checkbox("Show Remote Names", &settings::C_drawRemoteNames, Vector2f(10,50), 170));
        tabView->addWidget(new Checkbox("Show Bots Names", &settings::C_drawBotNames, Vector2f(10,70), 170));
        tabView->addWidget(new Checkbox("Show Bots Orientation", &settings::C_drawBotOrientation, Vector2f(10,90), 170));
        tabView->addWidget(new Checkbox("Show Zones", &settings::C_drawZones, Vector2f(10,110), 170));
        tabView->addWidget(new Checkbox("Show AI-Paths", &settings::C_drawAIPath, Vector2f(10,130), 170));
        tabView->addWidget(new Checkbox("Show FPS", &settings::C_showFPS, Vector2f(10,150), 170));
        tabView->addWidget(new Checkbox("Show Particle Count", &settings::C_showParticleCount, Vector2f(10,170), 170));
        //tabView->addWidget(new Checkbox("Show Latency", &settings::C_showLatency, Vector2f(10,190), 170));

        tabList->addTab(tabView);
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
    if (fullscreen_ != settings::C_fullScreen) {
        settings::C_fullScreen = fullscreen_;
        window::create();
    }
    if (vsync_ != settings::C_vsync) {
        settings::C_vsync = vsync_;
        window::create();
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
        sound::setGlobalVolume();
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



