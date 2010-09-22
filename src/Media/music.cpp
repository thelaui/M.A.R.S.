/* music.cpp

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

namespace music {

    namespace {
        // for Music there is only one channel... who wants to have multiple music files played at once?
        sf::Music musicChannel_;
        bool      initialized_(false);

        void init() {
            musicChannel_.SetRelativeToListener(true);
            musicChannel_.SetLoop(true);
            sf::Listener::SetPosition(640.f, 0.f, 300.f);
            setGlobalVolume();
            initialized_ = true;
        }
    }

    void playMusic(MusicType music) {
        if (!initialized_) init();
        switch (music) {
            case Space:                   musicChannel_.OpenFromFile("data/wav/space.ogg");             break;
            case Menu:                    musicChannel_.OpenFromFile("data/wav/menu.ogg");              break;
            case Rock:                    musicChannel_.OpenFromFile("data/wav/thisistheday.ogg");      break;
        }
        musicChannel_.Play();
    }

    void setGlobalVolume() {
        musicChannel_.SetVolume(static_cast<float>(settings::C_musicVolume));
    }
}
