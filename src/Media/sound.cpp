/* sound.cpp

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

# include "Media/sound.hpp"

# include "System/settings.hpp"

# define CHANNELCOUNT 16

namespace sound {

    namespace {
        std::vector<sf::SoundBuffer*> sounds_(COUNT);
        sf::Sound soundChannel_[CHANNELCOUNT];
        bool initialized_(false);

        void loadSound_(SoundType sound, std::string fileName) {
            sounds_[sound] = new sf::SoundBuffer;
            if (!sounds_[sound]->LoadFromFile(fileName))
                std::cout << "Failed to load Soundfile \"" << fileName << "\"!" << std::endl;
        }

        void init_() {
            sf::Listener::SetPosition(640.f, 0.f, 300.f);
            initialized_ = true;
        }
    }

    void playSound(SoundType sound, Vector2f const& position, float volume) {
        if (!initialized_) init_();
        // check if sound is already loaded
        if (sounds_[sound] != NULL) {
            // if its already loaded search for free soundChannel_
            int i(0);
            while (soundChannel_[i].GetStatus() == sf::Sound::Playing && i<CHANNELCOUNT) ++i;
            if (i < CHANNELCOUNT) {
                // play sound with random pitch
                soundChannel_[i].SetBuffer(*sounds_[sound]);
                if (sound != Click && sound != Tab && sound != Check && sound != Countdown && sound != Start )
                    soundChannel_[i].SetPitch(1 + static_cast<float>(rand()%100)/200.f - 0.25f);
                soundChannel_[i].SetVolume((volume < 0.f ? -volume : volume)*static_cast<float>(settings::C_soundVolume)/100.f);
                soundChannel_[i].SetPosition(position.x_, 0.f, 0.f);
                soundChannel_[i].SetAttenuation(0.f);
                soundChannel_[i].Play();
            }
        }
        else {
            // load it from file and...
            switch (sound) {
                case Pump:                loadSound_(sound, settings::C_dataPath + "/audio/toon/pump.wav");                  break;
                case Laser:               loadSound_(sound, settings::C_dataPath + "/audio/toon/laser.wav");                 break;
                case BallExplode:         loadSound_(sound, settings::C_dataPath + "/audio/toon/ballExplode.wav");           break;
                case ShipExplode:         loadSound_(sound, settings::C_dataPath + "/audio/toon/shipExplode.wav");           break;
                case ShipCollide:         loadSound_(sound, settings::C_dataPath + "/audio/toon/shipCollide.wav");           break;
                case BallPlanetCollide:   loadSound_(sound, settings::C_dataPath + "/audio/toon/ballPlanetCollide.wav");     break;
                case Particle:            loadSound_(sound, settings::C_dataPath + "/audio/toon/particle.wav");              break;
                case ShipBallCollide:     loadSound_(sound, settings::C_dataPath + "/audio/toon/shipBallCollide.wav");       break;
                case BallRespawn:         loadSound_(sound, settings::C_dataPath + "/audio/toon/ballRespawn.wav");           break;
                case PlanetExplode:       loadSound_(sound, settings::C_dataPath + "/audio/toon/planetExplode.wav");         break;
                case PumpShipCollide:     loadSound_(sound, settings::C_dataPath + "/audio/toon/pumpShipCollide.wav");       break;
                case ShipPlanetCollide:   loadSound_(sound, settings::C_dataPath + "/audio/toon/ballPlanetCollide.wav");     break;
                case ShipRespawn:         loadSound_(sound, settings::C_dataPath + "/audio/toon/shipRespawn.wav");           break;
                case BlubPop:             loadSound_(sound, settings::C_dataPath + "/audio/toon/blubPop.wav");               break;
                case Fuel:                loadSound_(sound, settings::C_dataPath + "/audio/toon/fuel.wav");                  break;
                case LaserCollide:        loadSound_(sound, settings::C_dataPath + "/audio/toon/laserCollide.wav");          break;
                case NoFuel:              loadSound_(sound, settings::C_dataPath + "/audio/toon/noFuel.wav");                break;
                case Sniper:              loadSound_(sound, settings::C_dataPath + "/audio/toon/sniper.wav");                break;
                case Blub:                loadSound_(sound, settings::C_dataPath + "/audio/toon/blub.wav");                  break;
                case BlubCollide:         loadSound_(sound, settings::C_dataPath + "/audio/toon/blubCollide.wav");           break;
                case SniperShipCollide:   loadSound_(sound, settings::C_dataPath + "/audio/toon/sniperShipCollide.wav");     break;
                case SniperPlanetCollide: loadSound_(sound, settings::C_dataPath + "/audio/toon/sniperPlanetCollide.wav");   break;
                case Countdown:           loadSound_(sound, settings::C_dataPath + "/audio/toon/countdown.wav");             break;
                case Start:               loadSound_(sound, settings::C_dataPath + "/audio/toon/start.wav");                 break;
                case Swish:               loadSound_(sound, settings::C_dataPath + "/audio/toon/swish.wav");                 break;
                case Score:               loadSound_(sound, settings::C_dataPath + "/audio/toon/score.wav");                 break;
                case Click:               loadSound_(sound, settings::C_dataPath + "/audio/interface/click.wav");            break;
                case Tab:                 loadSound_(sound, settings::C_dataPath + "/audio/interface/tab.wav");              break;
                case Check:               loadSound_(sound, settings::C_dataPath + "/audio/interface/check.wav");            break;
                case COUNT: std::cout << "COUNT is not a valid Soundtype..." << std::endl;
            }
            // ... play it afterwards
            if (sounds_[sound] != NULL)
                playSound(sound, position, volume);
        }
    }
}
