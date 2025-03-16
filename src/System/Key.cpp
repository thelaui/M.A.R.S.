/*Key.cpp

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
this program.  If not, see <http://www.gnu.org/licenses/>.*/

# include "System/Key.hpp"

# include "System/window.hpp"

Key::Key(sf::Keyboard::Key code):
    type_(kKeyBoard),
    navi_(nNone),
    strength_(100),
    joyID_(0) {

    code_.keyBoard_ = code;

    if (code == sf::Keyboard::Key::Enter || code == sf::Keyboard::Key::Space)
        navi_ = nConfirm;
    else if (code == sf::Keyboard::Key::Escape)
        navi_ = nAbort;
    else if ((code == sf::Keyboard::Key::Tab && (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl)))
             || (code == sf::Keyboard::Key::Tab && (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift)))
             || (code == sf::Keyboard::Key::Up))
        navi_ = nUp;
    else if (code == sf::Keyboard::Key::Tab || code == sf::Keyboard::Key::Down)
        navi_ = nDown;
    else if (code == sf::Keyboard::Key::Left)
        navi_ = nLeft;
    else if (code == sf::Keyboard::Key::Right)
        navi_ = nRight;
}

Key::Key(unsigned int joyID, sf::Joystick::Axis joyAxis, int strength):
    type_(kJoyAxis),
    navi_(nNone),
    strength_(0),
    joyID_(joyID) {

    std::pair<AxisType, int> tmp(convertFromSFML(joyAxis, strength));

    code_.joyButton_ = tmp.first;
    strength_ = tmp.second;

    if ((joyAxis == sf::Joystick::Axis::PovY && strength == -100) || (joyAxis == sf::Joystick::Axis::Y && strength == -100) ||
        # if defined __WIN32__
            (joyAxis == sf::Joystick::Axis::R && strength == -100)
        # else
            (joyAxis == sf::Joystick::Axis::V && strength == -100)
        # endif
        )
        navi_ = nUp;
    else if ((joyAxis == sf::Joystick::Axis::PovY && strength == 100) || (joyAxis == sf::Joystick::Axis::Y && strength == 100) ||
        # if defined __WIN32__
            (joyAxis == sf::Joystick::Axis::R && strength == 100)
        # else
            (joyAxis == sf::Joystick::Axis::V && strength == 100)
        # endif
        )
        navi_ = nDown;
    else if ((joyAxis == sf::Joystick::Axis::PovX && strength == -100) || (joyAxis == sf::Joystick::Axis::X && strength == -100) || (joyAxis == sf::Joystick::Axis::U && strength == -100))
        navi_ = nLeft;
    else if ((joyAxis == sf::Joystick::Axis::PovX && strength == 100) || (joyAxis == sf::Joystick::Axis::X && strength == 100) || (joyAxis == sf::Joystick::Axis::U && strength == 100))
        navi_ = nRight;
}

Key::Key(unsigned int joyID, unsigned int joyButton):
    type_(kJoyButton),
    navi_(nNone),
    strength_(100),
    joyID_(joyID) {

    code_.joyButton_ = joyButton;

    if (joyButton == 0 || joyButton == 8 || joyButton == 9  || joyButton == 2)
        navi_ = nConfirm;
    else if (joyButton == 6 ||
        # if defined __WIN32__
             joyButton == 7
        # else
             joyButton == 10
        # endif
        )
        navi_ = nAbort;
    else if (joyButton == 4)
        navi_ = nUp;
    else if (joyButton == 5)
        navi_ = nDown;
}

std::pair<Key::AxisType, int> Key::convertFromSFML(sf::Joystick::Axis joyAxis, int strength) {
    std::pair<AxisType, int> result;
    result.second = std::abs(strength);
    switch (joyAxis) {
        case sf::Joystick::Axis::X:
            if (strength < 0) result.first = aALleft;
            else              result.first = aALright;
            break;
        case sf::Joystick::Axis::Y:
            if (strength < 0) result.first = aALup;
            else              result.first = aALdown;
            break;

        case sf::Joystick::Axis::Z:
            # if defined __WIN32__
                if (strength > 0) {
                    result.first = aLT;
                    result.second = strength;
                }
                else {
                    result.first = aRT;
                    result.second = std::abs(strength);
                }
            # else
                result.first = aLT;
                result.second = strength*0.5 + 50;
            # endif
                break;

        case sf::Joystick::Axis::R:
            # if defined __WIN32__
                if (strength < 0) result.first = aARup;
                else              result.first = aARdown;
            # else
                result.first = aRT;
                result.second = strength*0.5 + 50;
            # endif
                break;

        case sf::Joystick::Axis::U:
            if (strength < 0) result.first = aARleft;
            else              result.first = aARright;
            break;
        case sf::Joystick::Axis::V:
            # if !defined __WIN32__
                if (strength < 0) result.first = aARup;
                else              result.first = aARdown;
            # endif
            break;
        case sf::Joystick::Axis::PovX:
            if (strength < 0) result.first = aPOVleft;
            else              result.first = aPOVright;
            break;
        case sf::Joystick::Axis::PovY:
            if (strength < 0) result.first = aPOVup;
            else              result.first = aPOVdown;
            break;
    }

    return result;

}


sf::Joystick::Axis Key::convertToSFML(AxisType joyAxis) {
    if(joyAxis == aALleft || joyAxis == aALright)
        return sf::Joystick::Axis::X;
    else if(joyAxis == aALup || joyAxis == aALdown)
        return sf::Joystick::Axis::Y;
    else if(joyAxis == aLT)
        return sf::Joystick::Axis::Z;
    else if(joyAxis == aRT) {
        # if defined __WIN32__
            return sf::Joystick::Axis::Z;
        # else
            return sf::Joystick::Axis::R;
        # endif
    }
    else if(joyAxis == aARleft || joyAxis == aARright)
        return sf::Joystick::Axis::U;
    else if(joyAxis == aARup || joyAxis == aARdown)
        # if defined __WIN32__
            return sf::Joystick::Axis::R;
        # else
            return sf::Joystick::Axis::V;
        # endif
    else return sf::Joystick::Axis::PovX;
}

bool operator== (Key const& lhs, Key const& rhs) {
    if (lhs.type_ == rhs.type_) {
        switch (lhs.type_) {
            case Key::kKeyBoard:
                return lhs.code_.keyBoard_ == rhs.code_.keyBoard_;
            case Key::kJoyButton:
                return lhs.code_.joyButton_ == rhs.code_.joyButton_ && lhs.joyID_ == rhs.joyID_;
            case Key::kJoyAxis:
                return lhs.code_.joyAxis_ == rhs.code_.joyAxis_ && lhs.joyID_ == rhs.joyID_;
        }
    }
    return false;
}

std::ostream& operator<< (std::ostream& stream, Key const& input) {
    stream << input.type_ << " " <<input.code_.joyButton_;
    if (input.type_ != Key::kKeyBoard) stream << " " << input.joyID_;
    return stream;
}

std::istream& operator>> (std::istream& stream, Key& output) {
    int type(0), joyButton(0);
    stream >> type >> joyButton;
    output.code_.joyButton_ = joyButton;
    output.type_  = static_cast<Key::KeyType>(type);
    if (output.type_ != Key::kKeyBoard) stream >> output.joyID_;
    return stream;
}
