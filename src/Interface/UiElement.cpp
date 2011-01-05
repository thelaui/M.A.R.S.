/* UiElement.cpp

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

# include "Interface/UiElement.hpp"

# include "Media/sound.hpp"
# include "Menu/menus.hpp"
# include "System/timer.hpp"
# include "System/window.hpp"

# include <SFML/OpenGL.hpp>
# include <iostream>

UiElement::UiElement(Vector2f const& topLeft, int width, int height):
    parent_(NULL),
    topLeft_(topLeft),
    width_(width),
    height_(height),
    hovered_(false),
    focused_(false),
    pressed_(false),
    hoveredFadeTime_(0.f),
    focusedFadeTime_(0.f) {}

void UiElement::mouseMoved(Vector2f const& position) {
    Vector2f topLeftAbs(getTopLeft());
    if ((!window::getInput().IsMouseButtonDown(sf::Mouse::Left) || pressed_) && topLeftAbs.x_+width_ > position.x_ && topLeftAbs.y_+height_ > position.y_ && topLeftAbs.x_ < position.x_ && topLeftAbs.y_ < position.y_) {
        hovered_ = true;
    }
    else
        hovered_ = false;
}

void UiElement::mouseLeft(bool down) {
    pressed_ = down && hovered_;
    if (isTabable() && down && hovered_) {
        menus::clearFocus();
        this->setFocus(true);
    }
}

void UiElement::draw() const {
    if      (hovered_)             hoveredFadeTime_ = 1.f;
    else if (hoveredFadeTime_ > 0) hoveredFadeTime_ -= timer::frameTime()*5.f;
    if      (hoveredFadeTime_ < 0) hoveredFadeTime_ = 0.f;
    if      (focused_)             focusedFadeTime_ = 1.f;
    else if (focusedFadeTime_ > 0) focusedFadeTime_ -= timer::frameTime()*5.f;
    if      (focusedFadeTime_ < 0) focusedFadeTime_ = 0.f;
}

void UiElement::setParent(UiElement* newParent) {
    parent_ = newParent;
}

void UiElement::setFocus (bool focus) {
    if (focus && parent_) parent_->setFocus(true);
    focused_ = focus;
}

Vector2f UiElement::getTopLeft() {
    if (parent_ != NULL)
        return topLeft_ + parent_->getTopLeft();
    else
        return topLeft_;
}
