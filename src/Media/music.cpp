/* music.cpp

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

# include "Media/music.hpp"

# include "System/settings.hpp"
# include "System/timer.hpp"
# include "Hud/hud.hpp"
# include "Locales/locales.hpp"
# include "defines.hpp"
# include "Hud/musicNotify.hpp"

# include <sys/types.h>
# include <dirent.h>

namespace music {

    namespace {
        // for Music there is only one channel... who wants to have multiple music files played at once?
        sf::Music                musicChannel_;
        bool                     initialized_(false);
        std::vector<std::string> files_;
        std::vector<int>         playList_;

        void init() {
            // get files
            DIR *dp;
            struct dirent *dirp;
            if((dp = opendir((settings::C_dataPath + "/audio/music/").c_str())) == NULL) {
                std::cout << "Error opening " << settings::C_dataPath << "/audio/music/" << std::endl;
            }

            while ((dirp = readdir(dp)) != NULL) {
                std::string file(dirp->d_name);
                if (file.size() > 0 && file[0] != '.')
                files_.push_back(file);
            }
            closedir(dp);

            musicChannel_.SetRelativeToListener(true);
            sf::Listener::SetPosition(SPACE_X_RESOLUTION*0.5f, 0.f, 300.f);
            setGlobalVolume();
            initialized_ = true;
        }

        void play(int nextTrack) {
            sf::String fileName(settings::C_dataPath + "/audio/music/" + files_[nextTrack]);
            musicChannel_.OpenFromFile(fileName);
            musicChannel_.Play();
            musicChannel_.SetLoop(false);
            musicNotify::show(files_[nextTrack]);
        }

    }

    void update() {
        if (settings::C_musicVolume > 0) {
            if (!initialized_) init();

            if (musicChannel_.GetStatus() == sf::Music::Stopped && files_.size() > 0)
                playGameMusic();

            float slowMoTime(timer::slowMoTime());
            if (slowMoTime > 0.75f) {
                musicChannel_.SetPitch(slowMoTime*0.666f);
            }
            else if (slowMoTime > 0.25f) {
                musicChannel_.SetPitch(0.5f);
            }
            else if (slowMoTime > 0.0f) {
                musicChannel_.SetPitch(1.f-slowMoTime*2.f);
            }
            else musicChannel_.SetPitch(1.f);
        }
        else if (musicChannel_.GetStatus() == sf::Music::Playing)
            stop();
    }

    void playMenuMusic() {
        if (settings::C_musicVolume > 0) {
            if (!initialized_) init();

            musicChannel_.SetLoop(true);
            sf::String fileName(settings::C_dataPath + "/audio/thisistheday.ogg");
            musicChannel_.OpenFromFile(fileName);
            musicChannel_.Play();
            playList_.clear();
        }
    }

    void playGameMusic() {
        if (settings::C_musicVolume > 0) {
            if (!initialized_) init();

            if (files_.size() > 0) {

                int nextTrack(0);

                if (files_.size() > 1) {
                    if (settings::C_audioRandom) {
                        if (playList_.empty())
                            nextTrack = sf::Randomizer::Random(0, static_cast<int>(files_.size()-1));
                        else {
                            nextTrack = playList_.back();
                            while (nextTrack == playList_.back())
                                nextTrack = sf::Randomizer::Random(0, static_cast<int>(files_.size()-1));
                        }
                    }
                    else {
                        if (!playList_.empty())
                            nextTrack = (playList_.back()+1)%files_.size();
                    }
                }

                playList_.push_back(nextTrack);

                play(nextTrack);
            }
            else
                stop();
        }
    }

    void next() {
        if (!playList_.empty() && settings::C_musicVolume) {
            stop();
            hud::displayMessage(*locales::getLocale(locales::NextTrackNotify));
        }
    }

    void previous() {
        if (!playList_.empty() && settings::C_musicVolume) {
            stop();
            hud::displayMessage(*locales::getLocale(locales::PreviousTrackNotify));

            if (playList_.size() > 1)
                playList_.pop_back();

            play(playList_.back());
        }
    }

    void stop() {
        musicChannel_.Stop();
    }

    void setGlobalVolume() {
        musicChannel_.SetVolume(static_cast<float>(settings::C_musicVolume));
    }
}
