/* UiElement.cpp

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

# include "Interface/UiElement.hpp"

# include "Media/sound.hpp"

# include <SFML/OpenGL.hpp>
# include <iostream>

UiElement::UiElement(Vector2f const& topLeft, int width, int height):
    parent_(NULL),
    topLeft_(topLeft),
    width_(width),
    height_(height),
    hoovered_(false),
    pressed_(false) {}

void UiElement::mouseMoved(Vector2f const& position) {
    Vector2f topLeftAbs(getTopLeft());
    if (topLeftAbs.x_+width_ > position.x_ && topLeftAbs.y_+height_ > position.y_ && topLeftAbs.x_ < position.x_ && topLeftAbs.y_ < position.y_) {
        hoovered_ = true;
    }
    else
        hoovered_ = false;
}

void UiElement::mouseLeft(bool down) {
    if (down)   pressed_ = true;
    else        pressed_ = false;
}

void UiElement::setParent(UiElement* newParent) {
    parent_ = newParent;
}

Vector2f UiElement::getTopLeft() {
    if (parent_ != NULL)
        return topLeft_ + parent_->getTopLeft();
    else
        return topLeft_;
}
