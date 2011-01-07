/* Tab.cpp

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

# include "Interface/Tab.hpp"

# include "System/settings.hpp"
# include "System/window.hpp"
# include "Interface/TabList.hpp"
# include "Media/sound.hpp"
# include "Menu/menus.hpp"

# include <SFML/OpenGL.hpp>

Tab::Tab (sf::String* name, int width, bool* activated):
    UiElement(Vector2f(), width, 20),
    focusedWidget_(NULL),
    name_(name),
    activated_(activated),
    active_(false) {}

Tab::~Tab() {
    for (std::vector<UiElement*>::iterator i=widgets_.begin(); i != widgets_.end(); ++i)
        delete *i;
}

void Tab::mouseMoved(Vector2f const& position) {
    Vector2f topLeftAbs(parent_->getTopLeft() + topLeft_);
    if ((!window::getInput().IsMouseButtonDown(sf::Mouse::Left) || pressed_) && topLeftAbs.x_+width_ > position.x_ && topLeftAbs.y_+height_ > position.y_ && topLeftAbs.x_ < position.x_ && topLeftAbs.y_ < position.y_)
        hovered_ = true;
    else
        hovered_ = false;
    if (active_)
        for (std::vector<UiElement*>::iterator i=widgets_.begin(); i != widgets_.end(); ++i)
            (*i)->mouseMoved(position);
}

void Tab::mouseLeft(bool down) {
    pressed_ = hovered_ && down;

    if (!pressed_ && hovered_) {
        menus::clearFocus();
        setFocus(this);
        dynamic_cast<TabList*>(parent_)->deactivateAll();
        active_ = true;
        if(activated_)
            *activated_ = true;
        sound::playSound(sound::Tab);
    }
    if (active_)
        for (std::vector<UiElement*>::iterator i=widgets_.begin(); i != widgets_.end(); ++i)
            (*i)->mouseLeft(down);
}

void Tab::keyEvent(bool down, sf::Key::Code keyCode) {
    if (active_ && focusedWidget_)
        focusedWidget_->keyEvent(down, keyCode);

    if (down && ((keyCode == sf::Key::Tab && (window::getInput().IsKeyDown(sf::Key::LControl) || window::getInput().IsKeyDown(sf::Key::RControl)))
     || (keyCode == sf::Key::Tab && (window::getInput().IsKeyDown(sf::Key::LShift) || window::getInput().IsKeyDown(sf::Key::RShift)))
     || (keyCode == sf::Key::Up))) {
        if (focusedWidget_ && focusedWidget_->allWidgetsFocused(false)) {
            int i(widgets_.size()-1);
            while ( widgets_[i] != focusedWidget_ && i>0) --i;
            if (i > 0) --i;
            while (!widgets_[i]->isTabable() && i>0)      --i;

            if (widgets_[i]->isTabable() && widgets_[i] != focusedWidget_) {
                menus::clearFocus();
                focusedWidget_ = widgets_[i];
                focusedWidget_->setFocus(focusedWidget_);
            }
            else {
                menus::clearFocus();
                focusedWidget_ = NULL;
                setFocus(this);
            }
        }
    }
    else if (down && (keyCode == sf::Key::Tab || keyCode == sf::Key::Down)) {
        if (!focusedWidget_ && widgets_.size() > 0) {
            int i(0);
            while (!widgets_[i]->isTabable() && i<widgets_.size()-1) ++i;
            if (i<widgets_.size()) {
                focusedWidget_ = widgets_[0];
                focusedWidget_->setFocus(focusedWidget_);
            }
        }
        else if (focusedWidget_->allWidgetsFocused(true)) {
            int i(0);
            while ( widgets_[i] != focusedWidget_ && i<widgets_.size()-1) ++i;
            if (i<widgets_.size()-1) ++i;
            while (!widgets_[i]->isTabable() && i<widgets_.size()-1)      ++i;

            if (widgets_[i]->isTabable() && widgets_[i] != focusedWidget_) {
                menus::clearFocus();
                focusedWidget_ = widgets_[i];
                focusedWidget_->setFocus(focusedWidget_);
            }
            else {
                menus::clearFocus();
                focusedWidget_ = NULL;
                setFocus(this);
            }
        }
    }
}

void Tab::textEntered(int keyCode) {
    if (active_ && focusedWidget_)
        focusedWidget_->textEntered(keyCode);
}

bool Tab::allWidgetsFocused(bool tabNext) const {
    return !focusedWidget_;
}

void Tab::draw () const {
    Vector2f origin = parent_->getTopLeft() + topLeft_;


    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (!focusedWidget_ && focused_ && isTopMost()) {
        glBegin(GL_QUADS);
            glColor4f(1,0.5,0.8,0.7);
            glVertex2f(origin.x_+width_,origin.y_);
            glVertex2f(origin.x_,origin.y_);
            glColor4f(0.5,0.25,0.4,0.0);
            glVertex2f(origin.x_,origin.y_+height_);
            glVertex2f(origin.x_+width_,origin.y_+height_);
        glEnd();
    }

    if (active_) {
        if (isTopMost()) {
            glBegin(GL_QUADS);
                glColor4f(1,0.5,0.8,0.7);
                glVertex2f(origin.x_+width_,origin.y_);
                glVertex2f(origin.x_,origin.y_);
                glColor4f(0.5,0.25,0.4,0.0);
                glVertex2f(origin.x_,origin.y_+height_*0.5);
                glVertex2f(origin.x_+width_,origin.y_+height_*0.5);
            glEnd();
        }

        glLineWidth(2.f);
        glBegin(GL_LINE_STRIP);
            if (isTopMost())  glColor4f(1.f, 0.5f, 0.8f, 1.0f);
            else              glColor4f(0.4f, 0.4f, 0.4f, 1.0f);
            glVertex2f(origin.x_,origin.y_+height_);
            glVertex2f(origin.x_,origin.y_);
            glVertex2f(origin.x_+width_,origin.y_);
            glVertex2f(origin.x_+width_,origin.y_+height_);
        glEnd();
    }
    else {
        glLineWidth(2.f);
        glBegin(GL_LINES);
            if (isTopMost())  glColor4f(1.f, 0.5f, 0.8f, 1.0f);
            else              glColor4f(0.4f, 0.4f, 0.4f, 1.0f);
            glVertex2f(origin.x_,origin.y_+height_);
            glVertex2f(origin.x_+width_,origin.y_+height_);
        glEnd();

    }

    // draw Label
    label_->draw();

    if (active_)
        for (std::vector<UiElement*>::const_iterator i=widgets_.begin(); i != widgets_.end(); ++i)
            (*i)->draw();
}

void Tab::setFocus(UiElement* toBeFocused) {
    UiElement::setFocus(this);
    if (toBeFocused != this) focusedWidget_ = toBeFocused;
    else                     focusedWidget_ = NULL;
    label_->setFocus(this);
}

void Tab::clearFocus() {
    UiElement::clearFocus();
    focusedWidget_ = NULL;
    for (std::vector<UiElement*>::iterator i=widgets_.begin(); i != widgets_.end(); ++i)
        (*i)->clearFocus();
    label_->clearFocus();
}

void Tab::addWidget(UiElement* toBeAdded) {
    toBeAdded->setParent(this);
    widgets_.push_back(toBeAdded);
}

Vector2f Tab::getTopLeft() {
    return UiElement::getTopLeft() - topLeft_ + Vector2f(0.f, 10.f);
}
