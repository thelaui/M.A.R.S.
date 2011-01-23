/* Checkbox.cpp

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

# include "Interface/Checkbox.hpp"

# include "System/settings.hpp"
# include "Media/sound.hpp"
# include "Media/text.hpp"
# include "Media/texture.hpp"
# include "Locales/locales.hpp"
# include "Interface/toolTip.hpp"

# include <SFML/OpenGL.hpp>

Checkbox::Checkbox (sf::String* text, sf::String* toolTip, bool* value, Vector2f const& topLeft, int width):
    UiElement(topLeft, width, 20),
    value_(value),
    toolTip_(toolTip) {

    label_ = new Label(text, TEXT_ALIGN_LEFT, Vector2f(15,0));
    label_->setParent(this);
}

Checkbox::~Checkbox () {
    delete label_;
}

void Checkbox::mouseMoved(Vector2f const& position) {
    UiElement::mouseMoved(position);
    label_->mouseMoved(position);

    if (hovered_ && toolTip_)
        toolTip::show(toolTip_);
}

void Checkbox::mouseLeft(bool down) {
    UiElement::mouseLeft(down);
    if (!pressed_ && hovered_ && focused_) {
        *value_ = !*value_;
        sound::playSound(sound::Check);
    }
}

void Checkbox::keyEvent(bool down, sf::Key::Code keyCode) {
    if (keyCode == sf::Key::Return || keyCode == sf::Key::Space) {
        pressed_ = down;
        if (!pressed_) {
            *value_ = !*value_;
            sound::playSound(sound::Click);
        }
    }
}

void Checkbox::draw() const {
    Vector2f origin = getTopLeft();

    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Widgets));

    int x(0), y(0);

    if (hovered_ && pressed_) {
        if (*value_) {
            x = 2;
            y = 2;
        }
        else {
            x = 2;
            y = 3;
        }
    }
    else if (hovered_) {
        if (*value_) {
            x = 1;
            y = 2;
        }
        else {
            x = 1;
            y = 3;
        }
    }
    else {
        if (*value_) {
            x = 0;
            y = 2;
        }
        else {
            x = 0;
            y = 3;
        }
    }

    int mirror(locales::getCurrentLocale().LTR_ ? 1 : -1);

    glColor3f(1.f, 1.f, 1.f);
    glBegin(GL_QUADS);
        glTexCoord2f(x*0.25f, y*0.25f+0.25f);       glVertex2f(origin.x_-4.f*mirror, origin.y_+16.f);
        glTexCoord2f(x*0.25f+0.25f, y*0.25f+0.25f); glVertex2f(origin.x_+12.f*mirror, origin.y_+16.f);
        glTexCoord2f(x*0.25f+0.25f, y*0.25f);       glVertex2f(origin.x_+12.f*mirror, origin.y_-0.f);
        glTexCoord2f(x*0.25f, y*0.25f);             glVertex2f(origin.x_-4.f*mirror, origin.y_-0.f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    // draw Label
    text::drawFooText();
    label_->draw();
}

void Checkbox::setFocus (UiElement* toBeFocused, bool isPrevious) {
    UiElement::setFocus(this, isPrevious);
    label_->setFocus(this, isPrevious);
}

void Checkbox::clearFocus() {
    UiElement::clearFocus();
    label_->clearFocus();
}
