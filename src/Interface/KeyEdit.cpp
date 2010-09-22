/* KeyEdit.cpp

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

# include "Interface/KeyEdit.hpp"

# include "System/settings.hpp"

# include <SFML/OpenGL.hpp>

KeyEdit::KeyEdit (std::string text, sf::Key::Code* value, Vector2f const& topLeft, int width):
    UiElement(topLeft, width, 20),
    value_(value) {

    label_ = new Label(text, TEXT_ALIGN_LEFT, Vector2f(0,0));
    label_->setParent(this);
}

KeyEdit::~KeyEdit () {
    delete label_;
}


void KeyEdit::buttonPressed(sf::Key::Code keyCode) {
    if (hoovered_ && keyCode != 0 && keyCode != 128) {
        *value_ = keyCode;
    }
}

void KeyEdit::draw() const {
    Vector2f origin = parent_->getTopLeft() + topLeft_;
    // draw Button

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Hoover effect
    if (hoovered_)
        glColor4f(1,0.7,0.9,0.4);
    else
        glColor4f(1,1,1,0.1);

    glBegin(GL_QUADS);
        glVertex2f(origin.x_+185, origin.y_+2);
        glVertex2f(width_ + origin.x_, origin.y_+2);
        glColor4f(1,1,1,0.2);
        glVertex2f(width_ + origin.x_, height_ + origin.y_-2);
        glVertex2f(origin.x_+185, height_ + origin.y_-2);
    glEnd();



    std::string key;
    if (static_cast<int>(*value_) > 32 && static_cast<int>(*value_) < 126) key = static_cast<char>(*value_);

    else if (*value_ == sf::Key::Add) key = "Add";
    else if (*value_ == sf::Key::BackSlash) key = "Back Slash";
    else if (*value_ == sf::Key::Comma) key = "Comma";
    else if (*value_ == sf::Key::Dash) key = "Dash";
    else if (*value_ == sf::Key::Delete) key = "Delete";
    else if (*value_ == sf::Key::Divide) key = "Divide";
    else if (*value_ == sf::Key::Down) key = "Down";
    else if (*value_ == sf::Key::End) key = "End";
    else if (*value_ == sf::Key::Equal) key = "Equal";
    else if (*value_ == sf::Key::F1) key = "F1";
    else if (*value_ == sf::Key::F2) key = "F2";
    else if (*value_ == sf::Key::F3) key = "F3";
    else if (*value_ == sf::Key::F4) key = "F4";
    else if (*value_ == sf::Key::F5) key = "F5";
    else if (*value_ == sf::Key::F6) key = "F6";
    else if (*value_ == sf::Key::F7) key = "F7";
    else if (*value_ == sf::Key::F8) key = "F8";
    else if (*value_ == sf::Key::F9) key = "F9";
    else if (*value_ == sf::Key::F10) key = "F10";
    else if (*value_ == sf::Key::F11) key = "F11";
    else if (*value_ == sf::Key::F12) key = "F12";
    else if (*value_ == sf::Key::Home) key = "Home";
    else if (*value_ == sf::Key::Insert) key = "Insert";
    else if (*value_ == sf::Key::LAlt) key = "Left Alt";
    else if (*value_ == sf::Key::LBracket) key = "Left Bracket";
    else if (*value_ == sf::Key::LControl) key = "Left Control";
    else if (*value_ == sf::Key::Left) key = "Left";
    else if (*value_ == sf::Key::LShift) key = "Left Shift";
    else if (*value_ == sf::Key::LSystem) key = "Left Super";
    else if (*value_ == sf::Key::Menu) key = "Menu";
    else if (*value_ == sf::Key::Multiply) key = "Multiply";
    else if (*value_ == sf::Key::Numpad0) key = "Num 0";
    else if (*value_ == sf::Key::Numpad1) key = "Num 1";
    else if (*value_ == sf::Key::Numpad2) key = "Num 2";
    else if (*value_ == sf::Key::Numpad3) key = "Num 3";
    else if (*value_ == sf::Key::Numpad4) key = "Num 4";
    else if (*value_ == sf::Key::Numpad5) key = "Num 5";
    else if (*value_ == sf::Key::Numpad6) key = "Num 6";
    else if (*value_ == sf::Key::Numpad7) key = "Num 7";
    else if (*value_ == sf::Key::Numpad8) key = "Num 8";
    else if (*value_ == sf::Key::Numpad9) key = "Num 9";
    else if (*value_ == sf::Key::PageDown) key = "Page Down";
    else if (*value_ == sf::Key::PageUp) key = "Page Up";
    else if (*value_ == sf::Key::Pause) key = "Pause";
    else if (*value_ == sf::Key::Period) key = "Period";
    else if (*value_ == sf::Key::Quote) key = "Quote";
    else if (*value_ == sf::Key::RAlt) key = "Right Alt";
    else if (*value_ == sf::Key::RBracket) key = "Right Bracket";
    else if (*value_ == sf::Key::RControl) key = "Right Control";
    else if (*value_ == sf::Key::Return) key = "Return";
    else if (*value_ == sf::Key::Right) key = "Right";
    else if (*value_ == sf::Key::RShift) key = "Right Shift";
    else if (*value_ == sf::Key::RSystem) key = "Right Super";
    else if (*value_ == sf::Key::SemiColon) key = "Semi Colon";
    else if (*value_ == sf::Key::Slash) key = "Slash";
    else if (*value_ == sf::Key::Space) key = "Space";
    else if (*value_ == sf::Key::Subtract) key = "Subtract";
    else if (*value_ == sf::Key::Tilde) key = "Tilde";
    else if (*value_ == sf::Key::Tab) key = "Tab";
    else if (*value_ == sf::Key::Up) key = "Up";

    if (pressed_ && hoovered_)
        hud::drawScreenText("Press key while hoovering!", origin + Vector2f((width_+185)/2,1), font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER, Color3f(0.7, 0.7, 0.7));
    else
        hud::drawScreenText(key, origin + Vector2f((width_+185)/2,1), font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER, Color3f(0.7, 0.7, 0.7));

    //draw Label
    label_->draw();
}

