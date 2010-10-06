/* announcer.cpp

Copyright (c) 2010 by Felix Lauer und Simon Schneegans

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
                    soundChannel_.SetPosition(640, 0.f, 0.f);
                    soundChannel_.SetAttenuation(0.f);
                    soundChannel_.Play();
                }
            }
            else {
                // load it from file and...
                switch (sound) {
                    case Bam:                 loadSound_(sound, "data/wav/announcer/bam.wav");              break;
                    case Impressive:          loadSound_(sound, "data/wav/announcer/impressive.wav");       break;
                    case NiceOne:             loadSound_(sound, "data/wav/announcer/niceOne.wav");          break;
                    case NotFunny:            loadSound_(sound, "data/wav/announcer/notFunny.wav");         break;
                    case ThatWasGreat:        loadSound_(sound, "data/wav/announcer/thatWasGreat.wav");     break;
                    case WellDone:            loadSound_(sound, "data/wav/announcer/wellDone.wav");         break;
                    case YouSuck:             loadSound_(sound, "data/wav/announcer/youSuck.wav");          break;
                    case COUNT: std::cout << "COUNT is not a valid Soundtype..." << std::endl;
                }
                // ... play it afterwards
                if (sounds_[sound] != NULL)
                    playSound(sound);
            }
        }
    }

    void announce (SoundMood mood) {
        switch (mood) {
            case Affronting:
                switch (sf::Randomizer::Random(0,2)) {
                    case 0: playSound(YouSuck); break;
                    case 1: playSound(NotFunny); break;
                    default:;
                } break;
            case Praising:
                switch (sf::Randomizer::Random(0,4)) {
                    case 0: playSound(Impressive); break;
                    case 1: playSound(NiceOne); break;
                    case 2: playSound(ThatWasGreat); break;
                    case 3: playSound(WellDone); break;
                    default:;
                } break;
            case Neutral:
                switch (sf::Randomizer::Random(0,1)) {
                    case 0: playSound(Bam); break;
                    default:;
                } break;
            default:;
        }
    }
}
