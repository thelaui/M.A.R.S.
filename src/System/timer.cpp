/* Timer.cpp

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

# include "System/timer.hpp"

# include "Hud/hud.hpp"
# include "System/settings.hpp"
# include "Menu/menus.hpp"
# include "Games/games.hpp"

namespace timer {

    namespace {
        float frameTime_(0.f);
        float realFrameTime_(0.f);
        float totalTime_(0.f);
        float realTotalTime_(0.f);
        float fps_(0.f);

        // for fps counting:
        int frameCount_(0);
        float fpsTimer_(0.f);

        // for slow-motion
        float slowMoTimer_(0.f);
        float exploCounterResetTimer_(0.f);
        int   exploCounter_(0);
    }

    void update(float frameTime) {

        realFrameTime_ = frameTime;

        // fps
        fpsTimer_  += frameTime;
        ++frameCount_;

        if (fpsTimer_ >= 0.5f) {
            fps_ = static_cast<float>(frameCount_)/fpsTimer_;
            frameCount_ = 0;
            fpsTimer_   = 0.f;
        }

        if (!menus::visible() && hud::statsVisible())
            frameTime_ =  frameTime*0.05f;
        else if (games::type() == games::gMenu)
            frameTime_ =  frameTime*0.5f;
        else if (slowMoTimer_ > 1.f) {
            if (!menus::visible() || games::type() == games::gMenu)
                slowMoTimer_ -= frameTime;
            frameTime_ =  frameTime*0.15f;
        }
        else if (slowMoTimer_ > 0.f) {
            slowMoTimer_ -= frameTime;
            frameTime_ =  frameTime*(1.f-0.75f*slowMoTimer_);
        }
        else
            frameTime_ =  frameTime;

        totalTime_     += frameTime_;
        realTotalTime_ += realFrameTime_;

        if (settings::C_slowMoKickIn > 0) {
            // reset explosion counter
            if (exploCounterResetTimer_ > 0.f) {
                exploCounterResetTimer_ -= frameTime;
                if (exploCounterResetTimer_ <= 0.f) {
                    exploCounter_ = 0;
                }
            }

            // enable slow motion, when enough ships exploded
            if (exploCounter_ >= settings::C_slowMoKickIn && !menus::visible()) {
                exploCounter_ = 0;
                slowMoTimer_ = 5.f;
            }
        }
    }

    void onShipExplode() {
        if (slowMoTimer_ > 0.f) {
            slowMoTimer_ = 4.5f;
        }
        else {
            ++exploCounter_;
            exploCounterResetTimer_ = 0.1f;
        }
    }

    float frameTime() { return frameTime_; }

    float realFrameTime() { return realFrameTime_; }

    float totalTime() { return totalTime_; }

    float realTotalTime() { return realTotalTime_; }

    float slowMoTime() { return slowMoTimer_ < 0.f ? 0.f : slowMoTimer_*0.2f; }

    void resetSlowMotion() { slowMoTimer_ = 0.f; }

    float fps()       { return fps_; }
}
