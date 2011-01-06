/* TabList.cpp

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

# include "Interface/TabList.hpp"

# include "System/settings.hpp"
# include "System/window.hpp"
# include "Media/text.hpp"
# include "Menu/menus.hpp"

# include <SFML/OpenGL.hpp>

TabList::TabList (Vector2f const& topLeft, int width, int height):
    UiElement(topLeft, width, height),
    focusedTab_(NULL),
    contentFocused_(false),
    lastTabEnd_(0) {}

TabList::~TabList() {
    for (std::vector<Tab*>::iterator i=tabs_.begin(); i != tabs_.end(); ++i)
        delete *i;
}

void TabList::mouseMoved(Vector2f const& position) {
    for (std::vector<Tab*>::iterator i=tabs_.begin(); i != tabs_.end(); ++i)
        (*i)->mouseMoved(position);
}

void TabList::mouseLeft(bool down) {
    for (std::vector<Tab*>::iterator i=tabs_.begin(); i != tabs_.end(); ++i)
        (*i)->mouseLeft(down);
}

void TabList::keyEvent(bool down, sf::Key::Code keyCode) {
    if (focusedTab_) {
        focusedTab_->keyEvent(down, keyCode);

        if (!contentFocused_) {
            if (down && keyCode == sf::Key::Left) {
                int i(0);
                while ( tabs_[i] != focusedTab_) i = (i-1 + tabs_.size())%tabs_.size();
                i = (i-1 + tabs_.size())%tabs_.size();
                while (!tabs_[i]->isTabable())      i = (i-1 + tabs_.size())%tabs_.size();

                menus::clearFocus();
                focusedTab_->active_=false;
                focusedTab_ = tabs_[i];
                focusedTab_->setFocus(focusedTab_);
                focusedTab_->active_=true;
            }
            else if (down && keyCode == sf::Key::Right) {
                int i(0);
                while ( tabs_[i] != focusedTab_) i = (i+1)%tabs_.size();
                i = (i+1)%tabs_.size();
                while (!tabs_[i]->isTabable())      i = (i+1)%tabs_.size();

                menus::clearFocus();
                focusedTab_->active_=false;
                focusedTab_ = tabs_[i];
                focusedTab_->setFocus(focusedTab_);
                focusedTab_->active_=true;
            }
        }
        else if (down && (keyCode == sf::Key::Down || keyCode == sf::Key::Tab))
            contentFocused_ = !focusedTab_->allWidgetsFocused();
    }
}

void TabList::textEntered(int keyCode) {
    if (focusedTab_)
        focusedTab_->textEntered(keyCode);
}

bool TabList::allWidgetsFocused() const {
    bool result(true);
    if (contentFocused_ && !focusedTab_->allWidgetsFocused())
        result = false;
    contentFocused_ = !focusedTab_->allWidgetsFocused();
    return result;
}

void TabList::draw () const {
    Vector2f origin = parent_->getTopLeft() + topLeft_;


    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glLineWidth(2.f);
    glBegin(GL_LINES);
        if (isTopMost())  glColor4f(1.f, 0.5f, 0.8f, 1.0f);
        else              glColor4f(0.4f, 0.4f, 0.4f, 1.0f);
        glVertex2f(origin.x_,origin.y_+20);
        if (isTopMost())  glColor4f(1.f, 0.5f, 0.8f, 0.0f);
        else              glColor4f(0.4f, 0.4f, 0.4f, 0.0f);
        glVertex2f(origin.x_,origin.y_+height_);

        if (isTopMost())  glColor4f(1.f, 0.5f, 0.8f, 1.0f);
        else              glColor4f(0.4f, 0.4f, 0.4f, 1.0f);
        glVertex2f(origin.x_+lastTabEnd_,origin.y_+20);
        if (isTopMost())  glColor4f(1.f, 0.5f, 0.8f, 0.0f);
        else              glColor4f(0.4f, 0.4f, 0.4f, 0.0f);
        glVertex2f(origin.x_+width_,origin.y_+20);
    glEnd();

    for (std::vector<Tab*>::const_iterator i=tabs_.begin(); i != tabs_.end(); ++i)
        (*i)->draw();
}

void TabList::setFocus(UiElement* toBeFocused) {
    UiElement::setFocus(this);
    Tab* toFocus = dynamic_cast<Tab*>(toBeFocused);
    if (toFocus) {
        focusedTab_ = toFocus;
        if (toFocus->allWidgetsFocused())
            contentFocused_ = false;
    }
}

void TabList::clearFocus() {
    UiElement::clearFocus();
    for (std::vector<Tab*>::iterator i=tabs_.begin(); i != tabs_.end(); ++i)
        (*i)->clearFocus();
}

void TabList::addTab(Tab* toBeAdded) {
    toBeAdded->setParent(this);

    if (tabs_.size() == 0)
        toBeAdded->active_ = true;

    toBeAdded->topLeft_ = Vector2f(lastTabEnd_, 0.f);

    toBeAdded->label_ = new Label(toBeAdded->name_, TEXT_ALIGN_CENTER, Vector2f(lastTabEnd_+toBeAdded->width_/2, -6));
    toBeAdded->label_->setParent(toBeAdded);

    tabs_.push_back(toBeAdded);

    lastTabEnd_ += toBeAdded->width_;

    if (!focusedTab_)
        focusedTab_ = toBeAdded;
}

void TabList::deactivateAll() {
    for (std::vector<Tab*>::iterator i=tabs_.begin(); i != tabs_.end(); ++i)
        (*i)->active_ = false;
}

void TabList::clearTabs() {
    for (std::vector<Tab*>::iterator i=tabs_.begin(); i != tabs_.end(); ++i)
        delete *i;
    tabs_.clear();
}

