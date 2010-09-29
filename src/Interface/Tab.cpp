/* Tab.cpp

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

# include <SFML/OpenGL.hpp>

Tab::Tab (std::string const& name, int width, bool* activated):
    UiElement(Vector2f(), width, 20),
    name_(name),
    activated_(activated),
    active_(false) {}

Tab::~Tab() {
    for (std::vector<UiElement*>::iterator i=widgets_.begin(); i != widgets_.end(); ++i)
        delete *i;
}

void Tab::mouseMoved(Vector2f const& position) {
    Vector2f topLeftAbs(parent_->getTopLeft() + topLeft_);
    if (topLeftAbs.x_+width_ > position.x_ && topLeftAbs.y_+height_ > position.y_ && topLeftAbs.x_ < position.x_ && topLeftAbs.y_ < position.y_)
        hoovered_ = true;
    else
        hoovered_ = false;
    if (active_)
        for (std::vector<UiElement*>::iterator i=widgets_.begin(); i != widgets_.end(); ++i)
            (*i)->mouseMoved(position);
}

void Tab::mouseLeft(bool down) {
    UiElement::mouseLeft(down);
    if (!pressed_ && hoovered_) {
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

void Tab::buttonPressed(sf::Key::Code keyCode) {
    if (active_)
        for (std::vector<UiElement*>::iterator i=widgets_.begin(); i != widgets_.end(); ++i)
            (*i)->buttonPressed(keyCode);
}

void Tab::textEntered(int keyCode) {
    if (active_)
        for (std::vector<UiElement*>::iterator i=widgets_.begin(); i != widgets_.end(); ++i)
            (*i)->textEntered(keyCode);
}

void Tab::draw () const {
    Vector2f origin = parent_->getTopLeft() + topLeft_;


    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (hoovered_) {



        /*glBegin(GL_QUADS);
            glColor4f(0.5,0.25,0.4,0.0);
            glVertex2f(origin.x_,origin.y_+height_*0.5);
            glVertex2f(origin.x_+width_,origin.y_+height_*0.5);
            glColor4f(1,0.5,0.8,0.2);
            glVertex2f(origin.x_+width_,origin.y_+height_);
            glVertex2f(origin.x_,origin.y_+height_);
        glEnd();*/
    }

    if (active_) {
        glLineWidth(2.f);
        glBegin(GL_LINE_STRIP);
            glColor4f(1.f, 0.5f, 0.8f, 1.0f);
            glVertex2f(origin.x_,origin.y_+height_);
            glVertex2f(origin.x_,origin.y_);
            glVertex2f(origin.x_+width_,origin.y_);
            glVertex2f(origin.x_+width_,origin.y_+height_);
        glEnd();

        /*glBegin(GL_QUADS);
            glColor4f(0.5,0.25,0.4,0.0);
            glVertex2f(origin.x_,origin.y_+height_*0.5);
            glVertex2f(origin.x_+width_,origin.y_+height_*0.5);
            glColor4f(1,0.5,0.8,0.4);
            glVertex2f(origin.x_+width_,origin.y_+height_);
            glVertex2f(origin.x_,origin.y_+height_);
        glEnd();*/
    }
    else {
        glLineWidth(2.f);
        glBegin(GL_LINES);
            glColor4f(1.f, 0.5f, 0.8f, 1.0f);
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

void Tab::addWidget(UiElement* toBeAdded) {
    toBeAdded->setParent(this);
    widgets_.push_back(toBeAdded);
}

void Tab::clearWidgets () {
    for (std::vector<UiElement*>::iterator i=widgets_.begin(); i != widgets_.end(); ++i)
        delete *i;
    widgets_.clear();
}

Vector2f Tab::getTopLeft() {
    return UiElement::getTopLeft() - topLeft_ + Vector2f(0.f, 10.f);
}
