/* Color3f.cpp

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

# include "System/Color3f.hpp"

# include <SFML/OpenGL.hpp>

Color3f::Color3f ():
    r_(1),
    g_(1),
    b_(1) {}

Color3f::Color3f (Color3f const& color):
    r_(color.r_),
    g_(color.g_),
    b_(color.b_){}

Color3f::Color3f (float red, float green, float blue):
    r_(red),
    g_(green),
    b_(blue) {}

/// setters

void Color3f::r(float red) {
    if (red > 1) r_ = 1;
    else if (red < 0) r_ = 0;
    else r_ = red;
}

void Color3f::g(float green) {
    if (green > 1) g_ = 1;
    else if (green < 0) g_ = 0;
    else g_ = green;
}

void Color3f::b(float blue) {
    if (blue > 1) b_ = 1;
    else if (blue < 0) b_ = 0;
    else b_ = blue;
}

void Color3f::h(float hue) {
    setHSV(hue, s(), v());
}

void Color3f::s(float saturation) {
    if (saturation > 1) setHSV(h(), 1, v());
    else if (saturation < 0) setHSV(h(), 0, v());
    else setHSV(h(), saturation, v());
}

void Color3f::v(float value) {
    if (value > 1) setHSV(h(), s(), 1);
    else if (value < 0) setHSV(h(), s(), 0);
    else setHSV(h(), s(), value);
}

void Color3f::setHSV(float hue, float saturation, float value) {
    if(saturation == 0) {
		r_ = value;
		g_ = value;
		b_ = value;
		return;
	}
    hue = fmod(hue, 360);
	hue /= 60;
	int i = floor(hue);
	float f = hue - i;

	switch(i) {
		case 0:
			r_ = value;
			g_ = value * (1 - saturation * (1 - f));
			b_ = value * (1 - saturation);
			break;
		case 1:
			r_ = value * (1 - saturation * f);
			g_ = value;
			b_ = value * (1 - saturation);
			break;
		case 2:
			r_ = value * (1 - saturation);
			g_ = value;
			b_ = value * (1 - saturation * (1 - f));
			break;
		case 3:
			r_ = value * (1 - saturation);
			g_ = value * (1 - saturation * f);
			b_ = value;
			break;
		case 4:
			r_ = value * (1 - saturation * (1 - f));
			g_ = value * (1 - saturation);
			b_ = value;
			break;
		default:
			r_ = value;
			g_ = value * (1 - saturation);
			b_ = value * (1 - saturation * f);
			break;
	}
}

void Color3f::gl3f() const {
    glColor3f(r_, g_, b_);
}

void Color3f::gl4f(float alpha) const {
    glColor4f(r_, g_, b_, alpha);
}

sf::Color Color3f::sfColor() const {
    return sf::Color(r_*255, g_*255, b_*255);
}

Color3f Color3f::random() {
    Color3f result(sf::Randomizer::Random(0.3f, 1.0f), sf::Randomizer::Random(0.3f, 1.0f), sf::Randomizer::Random(0.3f, 1.0f));
    result.s(result.s() + 0.4);
    return result;
}
