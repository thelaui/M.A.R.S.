/* announcer.cpp

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

# include "Media/announcer.hpp"

# include "System/settings.hpp"
# include "System/timer.hpp"
# include "defines.hpp"
# include "System/randomizer.hpp"

# include <iostream>
# include <SFML/Audio.hpp>

namespace announcer {

    namespace {
        enum SoundType {Bam, Impressive, NiceOne, NotFunny, ThatWasGreat, WellDone,
                        YouSuck,
                        COUNT};

        std::vector<sf::SoundBuffer*> sounds_(COUNT);
        sf::Sound soundChannel_;

        void loadSound_(SoundType sound, std::string fileName) {
            sounds_[sound] = new sf::SoundBuffer;
            if (!sounds_[sound]->LoadFromFile(fileName))
                std::cout << "Failed to load Soundfile \"" << fileName << "\"!" << std::endl;
        }

        void playSound(SoundType sound) {
            // check if sound is already loaded
            if (sounds_[sound] != NULL) {
                if (soundChannel_.GetStatus() != sf::Sound::Playing) {
                    // play sound
                    soundChannel_.SetBuffer(*sounds_[sound]);
                    soundChannel_.SetVolume(static_cast<float>(settings::C_announcerVolume));
                    soundChannel_.SetPosition(SPACE_X_RESOLUTION*0.5f, 0.f, 0.f);
                    soundChannel_.SetAttenuation(0.f);
                    soundChannel_.Play();
                }
            }
            else {
                // load it from file and...
                switch (sound) {
                    case Bam:                 loadSound_(sound, settings::C_dataPath + "audio/announcer/bam.wav");              break;
                    case Impressive:          loadSound_(sound, settings::C_dataPath + "audio/announcer/impressive.wav");       break;
                    case NiceOne:             loadSound_(sound, settings::C_dataPath + "audio/announcer/niceOne.wav");          break;
                    case NotFunny:            loadSound_(sound, settings::C_dataPath + "audio/announcer/notFunny.wav");         break;
                    case ThatWasGreat:        loadSound_(sound, settings::C_dataPath + "audio/announcer/thatWasGreat.wav");     break;
                    case WellDone:            loadSound_(sound, settings::C_dataPath + "audio/announcer/wellDone.wav");         break;
                    case YouSuck:             loadSound_(sound, settings::C_dataPath + "audio/announcer/youSuck.wav");          break;
                    case COUNT: std::cout << "COUNT is not a valid Soundtype..." << std::endl;
                }
                // ... play it afterwards
                if (sounds_[sound] != NULL)
                    playSound(sound);
            }
        }
    }

    void update() {
        if (settings::C_announcerVolume > 0) {
            float slowMoTime(timer::slowMoTime());
            if (slowMoTime > 0.75f) {
                soundChannel_.SetPitch(slowMoTime*0.666f);
            }
            else if (slowMoTime > 0.25f) {
                soundChannel_.SetPitch(0.5f);
            }
            else if (slowMoTime > 0.0f) {
                soundChannel_.SetPitch(1.f-slowMoTime*2.f);
            }
            else soundChannel_.SetPitch(1.f);
        }
    }

    void announce (SoundMood mood) {
        if (settings::C_announcerVolume > 0) {
            switch (mood) {
                case Affronting:
                    switch (randomizer::random(0,2)) {
                        case 0: playSound(YouSuck); break;
                        case 1: playSound(NotFunny); break;
                        default:;
                    } break;
                case Praising:
                    switch (randomizer::random(0,4)) {
                        case 0: playSound(Impressive); break;
                        case 1: playSound(NiceOne); break;
                        case 2: playSound(ThatWasGreat); break;
                        case 3: playSound(WellDone); break;
                        default:;
                    } break;
                case Neutral:
                    switch (randomizer::random(0,1)) {
                        case 0: playSound(Bam); break;
                        default:;
                    } break;
                default:;
            }
        }
    }
}
