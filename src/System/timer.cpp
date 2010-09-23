/* Timer.cpp

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
    }

    void update() {
        frameTime_ =  window::getMainWindow()->GetFrameTime();
        totalTime_ += frameTime_;
        fpsTimer_  += frameTime_;
        frameCount_+= 1;

        if (fpsTimer_ >= 0.5f) {
            fps_ = static_cast<float>(frameCount_)/fpsTimer_;
            frameCount_ = 0;
            fpsTimer_   = 0.f;
        }
    }

    float frameTime() { return frameTime_; }

    float totalTime() { return totalTime_; }

    float fps()       { return fps_; }
}
