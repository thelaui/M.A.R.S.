/* Timer.cpp

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

# include "System/timer.hpp"

# include "System/window.hpp"

namespace timer {

    namespace {
        float frameTime_(0.f);
        float totalTime_(0.f);
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
        if (slowMoTimer_ > 0.f) {
            slowMoTimer_ -= frameTime;
            frameTime_ =  frameTime*0.2;
        }
        else
            frameTime_ =  frameTime;

        totalTime_ += frameTime_;

        // fps
        fpsTimer_  += frameTime;
        frameCount_+= 1;

        // slowmo
        if (exploCounterResetTimer_ > 0.f) {
            exploCounterResetTimer_ -= frameTime_;
            if (exploCounterResetTimer_ <= 0.f) {
                exploCounter_ = 0;
            }
        }

        if (exploCounter_ > 1)
            slowMoTimer_ = 3.f;

        if (fpsTimer_ >= 0.5f) {
            fps_ = static_cast<float>(frameCount_)/fpsTimer_;
            frameCount_ = 0;
            fpsTimer_   = 0.f;
        }
    }

    void onShipExplode() {
        ++exploCounter_;
        exploCounterResetTimer_ = 0.1f;
    }

    float frameTime() { return frameTime_; }

    float totalTime() { return totalTime_; }

    float fps()       { return fps_; }
}
