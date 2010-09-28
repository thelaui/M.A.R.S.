/* numbers.cpp

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

# include "Hud/numbers.hpp"

# include "System/timer.hpp"
# include "Media/text.hpp"

# include <sstream>

namespace numbers {

    namespace {
        std::list<Number*> activeParticles_;
        const float totalLifeTime_ = 2.f;
    }

    Number::Number(Vector2f const* location, int value):
               sourceLocation_(location),
               value_(value),
               lifeTime_(0.f) {

        if (value_ > 0)
            color_ = Color3f(0.4f, 1.0f, 0.4f);
        else
            color_ = Color3f(1.0f, 0.4f, 0.4f);
    }

    bool Number::update() {
        float time = timer::frameTime();

        color_.v(-1.0/totalLifeTime_*lifeTime_+1.0);

        lifeTime_ += time;

        return (lifeTime_ < totalLifeTime_);
    }

    void Number::draw() const {
        std::stringstream sstr;
        sstr << value_;
        if (value_ > 0)
            text::drawScreenText(sstr.str(), *sourceLocation_ - Vector2f(0.0f, 40.f) - Vector2f(0.f, 20.f)*lifeTime_, font::HandelGotDLig, 20.f, TEXT_ALIGN_CENTER, color_);
        else
            text::drawScreenText(sstr.str(), *sourceLocation_ + Vector2f(0.f, 20.f)*lifeTime_, font::HandelGotDLig, 20.f, TEXT_ALIGN_CENTER, color_);
    }

    void spawn(Vector2f const* location, int value) {
        activeParticles_.push_back(new Number(location, value));
    }

    void draw() {
        text::drawFooText();
        for (std::list<Number*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it)
            (*it)->draw();
        text::drawFooText();
    }

    void update() {
        std::list<Number*>::iterator it = activeParticles_.begin();
        while (it != activeParticles_.end()) {
            if (!(*it)->update()) {
                delete *it;
                it = activeParticles_.erase(it);
            }
            else ++it;
        }
    }

    void clear() {
        for (std::list<Number*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it)
            delete *it;
        activeParticles_.clear();
    }
}


