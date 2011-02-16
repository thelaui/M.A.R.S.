/* UiWindow.cpp

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

# include "Interface/UiWindow.hpp"

# include "System/settings.hpp"
# include "System/window.hpp"
# include "Media/texture.hpp"
# include "Media/text.hpp"
# include "Menu/menus.hpp"
# include "Locales/locales.hpp"

# include <SFML/OpenGL.hpp>

UiWindow::UiWindow (int width, int height, Vector2f const& position):
    UiElement(position, width, height),
    topMost_(false),
    focusedWidget_(NULL) {}

UiWindow::~UiWindow() {
    for (std::vector<UiElement*>::iterator i=widgets_.begin(); i != widgets_.end(); ++i)
        delete *i;
}

void UiWindow::mouseMoved(Vector2f const& position) {
    for (std::vector<UiElement*>::iterator i=widgets_.begin(); i != widgets_.end(); ++i)
        (*i)->mouseMoved(position);
}

void UiWindow::mouseLeft(bool down) {
    for (std::vector<UiElement*>::iterator i=widgets_.begin(); i != widgets_.end(); ++i)
        (*i)->mouseLeft(down);
}

void UiWindow::keyEvent(bool down, sf::Key::Code keyCode) {
    if (focusedWidget_)
        focusedWidget_->keyEvent(down, keyCode);
}

bool UiWindow::tabNext() {
    if (focusedWidget_->tabNext()) {
        int i(0);
        while ( widgets_[i] != focusedWidget_) i = (i+1)%widgets_.size();
        i = (i+1)%widgets_.size();
        while (!widgets_[i]->isTabable())      i = (i+1)%widgets_.size();

        menus::clearFocus();
        focusedWidget_ = widgets_[i];
        focusedWidget_->setFocus(focusedWidget_, false);
    }
    return false;
}

bool UiWindow::tabPrevious() {
    if (focusedWidget_->tabPrevious()) {
        int i(0);
        while ( widgets_[i] != focusedWidget_) i = (i-1 + widgets_.size())%widgets_.size();
        i = (i-1 + widgets_.size())%widgets_.size();
        while (!widgets_[i]->isTabable())      i = (i-1 + widgets_.size())%widgets_.size();

        menus::clearFocus();
        focusedWidget_ = widgets_[i];
        focusedWidget_->setFocus(focusedWidget_, true);
    }
    return false;
}

void UiWindow::textEntered(int keyCode) {
    if (focusedWidget_)
        focusedWidget_->textEntered(keyCode);
}

void UiWindow::draw () const {

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Vector2f viewPort = window::getViewPort();
    Vector2f origin;

    origin.x_ = topLeft_.x_ + (viewPort.x_ - width_)/2;
    origin.y_ = topLeft_.y_ + (viewPort.y_ - height_)/2;

    glBegin(GL_QUADS);
        // bg
        glColor4f(0.0,0.0,0.0,0.9);
        glVertex2f(origin.x_-10,origin.y_-10);
        glVertex2f(origin.x_+width_+10,origin.y_-10);
        glVertex2f(origin.x_+width_+10,origin.y_+height_+10);
        glVertex2f(origin.x_-10,origin.y_+height_+10);

        glColor4f(1.0,1.0,1.0,0.2);
        glVertex2f(origin.x_-10,origin.y_-10);
        glVertex2f(origin.x_+width_+10,origin.y_-10);
        glColor4f(1.0,1.0,1.0,0.02);
        glVertex2f(origin.x_+width_+10,origin.y_+height_/4);
        glVertex2f(origin.x_-10,origin.y_+height_/3);
    glEnd();

    glLineWidth(2.f);
    glBegin(GL_LINE_LOOP);
        glColor4f(0.5f, 0.5f, 0.5f, 0.8f);
        glVertex2f(origin.x_-10,origin.y_+height_+10);
        glVertex2f(origin.x_-10,origin.y_-10);
        glVertex2f(origin.x_+width_+10,origin.y_-10);
        glVertex2f(origin.x_+width_+10,origin.y_+height_+10);
    glEnd();

    glColor3f(1.f, 1.f, 1.f);
    glEnable(GL_TEXTURE_2D);

    float offset(0.f);
    if (!topMost_) offset = 200.f/512.f;

    glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Interface));

    glBegin(GL_QUADS);
        // topleft
        glTexCoord2f(0 + offset, 0);                   glVertex2i(origin.x_-60, origin.y_-60);
        glTexCoord2f(0 + offset, 96.f/512.f);          glVertex2i(origin.x_-60, origin.y_+36);
        glTexCoord2f(96.f/512.f + offset, 96.f/512.f); glVertex2i(origin.x_+36, origin.y_+36);
        glTexCoord2f(96.f/512.f + offset, 0);          glVertex2i(origin.x_+36, origin.y_-60);
        // topRight
        glTexCoord2f(96.f/512.f + offset, 0);          glVertex2i(origin.x_-36+width_, origin.y_-60);
        glTexCoord2f(96.f/512.f + offset, 96.f/512.f); glVertex2i(origin.x_-36+width_, origin.y_+36);
        glTexCoord2f(96.f/256.f + offset, 96.f/512.f); glVertex2i(origin.x_+60+width_, origin.y_+36);
        glTexCoord2f(96.f/256.f + offset, 0);          glVertex2i(origin.x_+60+width_, origin.y_-60);
        // bottomleft
        glTexCoord2f(0 + offset, 96.f/512.f);          glVertex2i(origin.x_-60, origin.y_-36+height_);
        glTexCoord2f(0 + offset, 96.f/256.f);          glVertex2i(origin.x_-60, origin.y_+60+height_);
        glTexCoord2f(96.f/512.f + offset, 96.f/256.f); glVertex2i(origin.x_+36, origin.y_+60+height_);
        glTexCoord2f(96.f/512.f + offset, 96.f/512.f); glVertex2i(origin.x_+36, origin.y_-36+height_);
        // bottomRight
        glTexCoord2f(96.f/512.f + offset, 96.f/512.f); glVertex2i(origin.x_-36+width_, origin.y_-36+height_);
        glTexCoord2f(96.f/512.f + offset, 96.f/256.f); glVertex2i(origin.x_-36+width_, origin.y_+60+height_);
        glTexCoord2f(96.f/256.f + offset, 96.f/256.f); glVertex2i(origin.x_+60+width_, origin.y_+60+height_);
        glTexCoord2f(96.f/256.f + offset, 96.f/512.f); glVertex2i(origin.x_+60+width_, origin.y_-36+height_);

        // top
        glTexCoord2f(193.f/512.f + offset, 0);          glVertex2i(origin.x_+36, origin.y_-60);
        glTexCoord2f(193.f/512.f + offset, 96.f/512.f); glVertex2i(origin.x_+36, origin.y_+36);
        glTexCoord2f(199.f/512.f + offset, 96.f/512.f); glVertex2i(origin.x_-36+width_, origin.y_+36);
        glTexCoord2f(199.f/512.f + offset, 0);          glVertex2i(origin.x_-36+width_, origin.y_-60);
        // right
        glTexCoord2f(96.f/512.f + offset, 193.f/512.f); glVertex2i(origin.x_-36+width_, origin.y_+36);
        glTexCoord2f(96.f/512.f + offset, 199.f/512.f); glVertex2i(origin.x_-36+width_, origin.y_-36+height_);
        glTexCoord2f(96.f/256.f + offset, 199.f/512.f); glVertex2i(origin.x_+60+width_, origin.y_-36+height_);
        glTexCoord2f(96.f/256.f + offset, 193.f/512.f); glVertex2i(origin.x_+60+width_, origin.y_+36);
        // left
        glTexCoord2f(0 + offset, 193.f/512.f);          glVertex2i(origin.x_-60, origin.y_+36);
        glTexCoord2f(0 + offset, 199.f/512.f);          glVertex2i(origin.x_-60, origin.y_-36+height_);
        glTexCoord2f(96.f/512.f + offset, 199.f/512.f); glVertex2i(origin.x_+36, origin.y_-36+height_);
        glTexCoord2f(96.f/512.f + offset, 193.f/512.f); glVertex2i(origin.x_+36, origin.y_+36);
        // bottom
        glTexCoord2f(193.f/512.f + offset, 96.f/512.f); glVertex2i(origin.x_+36, origin.y_-36+height_);
        glTexCoord2f(193.f/512.f + offset, 96.f/256.f); glVertex2i(origin.x_+36, origin.y_+60+height_);
        glTexCoord2f(199.f/512.f + offset, 96.f/256.f); glVertex2i(origin.x_-36+width_, origin.y_+60+height_);
        glTexCoord2f(199.f/512.f + offset, 96.f/512.f); glVertex2i(origin.x_-36+width_, origin.y_-36+height_);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    text::drawFooText();

    for (std::vector<UiElement*>::const_iterator i=widgets_.begin(); i != widgets_.end(); ++i)
        (*i)->draw();
}

void UiWindow::setFocus(UiElement* toBeFocused, bool isPrevious) {
    UiElement::setFocus(this, isPrevious);
    focusedWidget_ = toBeFocused;
}

void UiWindow::clearFocus() {
    UiElement::clearFocus();
    for (std::vector<UiElement*>::iterator i=widgets_.begin(); i != widgets_.end(); ++i)
        (*i)->clearFocus();
}

void UiWindow::addWidget(UiElement* toBeAdded) {
    toBeAdded->setParent(this);
    widgets_.push_back(toBeAdded);
    if (!focusedWidget_) {
        focusedWidget_ = toBeAdded;
        focusedWidget_->setFocus(focusedWidget_, false);
    }
}

void UiWindow::setTopMost(bool active) {
    topMost_ = active;
}

Vector2f UiWindow::getTopLeft() const {
    Vector2f viewPort = window::getViewPort();
    Vector2f origin;

    if (locales::getCurrentLocale().LTR_) {
        origin.x_ = topLeft_.x_ + (viewPort.x_ - width_)/2;
        origin.y_ = topLeft_.y_ + (viewPort.y_ - height_)/2;
    }
    else {
        origin.x_ = topLeft_.x_ + (viewPort.x_ + width_)/2;
        origin.y_ = topLeft_.y_ + (viewPort.y_ - height_)/2;
    }

    return origin;
}
