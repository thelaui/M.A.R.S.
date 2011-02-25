/* Message.cpp

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

# include "Hud/Message.hpp"

# include "Media/text.hpp"
# include "defines.hpp"

Message::Message(sf::String const& message, Color3f const& color):
    text_(message),
    color_(color),
    location_(SPACE_X_RESOLUTION*0.5f, 200),
    timer_(0.f),
    alpha_(1.f),
    speed_(0.f) {}

void Message::draw() const {
    text::drawMobileSpaceText(text_, location_+Vector2f(1.f, 1.f), 20.f, TEXT_ALIGN_CENTER, Color3f(0.f, 0.f, 0.f), alpha_);
    text::drawMobileSpaceText(text_, location_, 20.f, TEXT_ALIGN_CENTER, color_, alpha_);
}

void Message::update() {
    timer_ += timer::frameTime();
    if (timer_ > 1.f) {
        speed_       += timer::frameTime();
        location_.y_ += speed_;
        alpha_       -= timer::frameTime();
    }
}

bool Message::isDead() const {
    return timer_ > 2.f;
}
