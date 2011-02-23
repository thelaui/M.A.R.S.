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

Key::Key(sf::Key::Code code):
    type_(kKeyBoard),
    navi_(nNone),
    strength_(100),
    joyID_(0) {

    code_.keyBoard_ = code;

    if (code == sf::Key::Return || code == sf::Key::Space)
        navi_ = nConfirm;
    else if (code == sf::Key::Escape)
        navi_ = nAbort;
    else if ((code == sf::Key::Tab && (window::getInput().IsKeyDown(sf::Key::LControl) || window::getInput().IsKeyDown(sf::Key::RControl)))
             || (code == sf::Key::Tab && (window::getInput().IsKeyDown(sf::Key::LShift) || window::getInput().IsKeyDown(sf::Key::RShift)))
             || (code == sf::Key::Up))
        navi_ = nUp;
    else if (code == sf::Key::Tab || code == sf::Key::Down)
        navi_ = nDown;
    else if (code == sf::Key::Left)
        navi_ = nLeft;
    else if (code == sf::Key::Right)
        navi_ = nRight;
}

Key::Key(unsigned int joyID, sf::Joy::Axis joyAxis, int strength):
    type_(kJoyAxis),
    navi_(nNone),
    strength_(0),
    joyID_(joyID) {

    std::pair<AxisType, int> tmp(convertFromSFML(joyAxis, strength));

    code_.joyButton_ = tmp.first;
    strength_ = tmp.second;

    if ((joyAxis == 6 && strength == 0) || (joyAxis == 1 && strength == -100) || (joyAxis == 5 && strength == -100))
        navi_ = nUp;
    else if ((joyAxis == 6 && strength == 180) || (joyAxis == 1 && strength == 100) || (joyAxis == 5 && strength == 100))
        navi_ = nDown;
    else if ((joyAxis == 6 && strength == 270) || (joyAxis == 0 && strength == -100) || (joyAxis == 4 && strength == -100))
        navi_ = nLeft;
    else if ((joyAxis == 6 && strength == 90) || (joyAxis == 0 && strength == 100) || (joyAxis == 4 && strength == 100))
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
    else if (joyButton == 6 || joyButton == 10)
        navi_ = nAbort;
    else if (joyButton == 4)
        navi_ = nUp;
    else if (joyButton == 5)
        navi_ = nDown;
}

std::pair<Key::AxisType, int> Key::convertFromSFML(sf::Joy::Axis joyAxis, int strength) {
    std::pair<AxisType, int> result;
    result.second = std::abs(strength);
    switch (joyAxis) {
        case sf::Joy::AxisX:
            if (strength < 0) result.first = aALleft;
            else              result.first = aALright;
            break;
        case sf::Joy::AxisY:
            if (strength < 0) result.first = aALup;
            else              result.first = aALdown;
            break;
        case sf::Joy::AxisZ:
            result.first = aLT;
            result.second = strength*0.5 + 50;
            break;
        case sf::Joy::AxisR:
            result.first = aRT;
            result.second = strength*0.5 + 50;
            break;
        case sf::Joy::AxisU:
            if (strength < 0) result.first = aARleft;
            else              result.first = aARright;
            break;
        case sf::Joy::AxisV:
            if (strength < 0) result.first = aARup;
            else              result.first = aARdown;
            break;
        case sf::Joy::AxisPOV:
            result.second = (strength == -1 ? 0 : 100);
            if (strength <= 45)       result.first = aPOVup;
            else if (strength <= 135) result.first = aPOVright;
            else if (strength <= 225) result.first = aPOVdown;
            else if (strength <= 315) result.first = aPOVleft;
            else                      result.first = aPOVup;
            break;
    }

    return result;

}


sf::Joy::Axis Key::convertToSFML(AxisType joyAxis) {
    if(joyAxis == aALleft || joyAxis == aALright)
        return sf::Joy::AxisX;
    else if(joyAxis == aALup || joyAxis == aALdown)
        return sf::Joy::AxisY;
    else if(joyAxis == aLT)
        return sf::Joy::AxisZ;
    else if(joyAxis == aRT)
        return sf::Joy::AxisR;
    else if(joyAxis == aARleft || joyAxis == aARright)
        return sf::Joy::AxisU;
    else if(joyAxis == aARup || joyAxis == aARdown)
        return sf::Joy::AxisV;
    else return sf::Joy::AxisPOV;
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
