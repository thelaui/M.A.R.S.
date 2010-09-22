/* Color3f.hpp

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

# ifndef COLOR4F_HPP_INCLUDED
# define COLOR4F_HPP_INCLUDED

# include <cmath>
# include <algorithm>
# include <SFML/Graphics.hpp>

struct Color3f {
    public:
        Color3f ();
        Color3f (Color3f const& color);
        Color3f (float red, float green, float blue);

        inline float r() const;
        inline float g() const;
        inline float b() const;
        inline float h() const;
        inline float s() const;
        inline float v() const;

        void r(float red);
        void g(float green);
        void b(float blue);
        void h(float hue);
        void s(float saturation);
        void v(float value);

        void gl3f() const;
        void gl4f(float alpha = 1.f) const;

        sf::Color sfColor() const;

        static Color3f random();

    private:
        void setHSV (float hue, float saturation, float value);
        float r_, g_ , b_;
};

/// inlined getters

float Color3f::r() const {
    return r_;
}

float Color3f::g() const {
    return g_;
}

 float Color3f::b() const {
    return b_;
}

float Color3f::h() const {
    if (s() > 0) {
        float maxi = std::max(std::max(r_, g_), b_);
        float mini = std::min(std::min(r_, g_), b_);

        if (maxi == r_)
            return fmod(60*((g_-b_)/(maxi-mini)), 360);
        else if (maxi == g_)
            return fmod(60*(2 + (b_-r_)/(maxi-mini)), 360);
        else
            return fmod(60*(4 + (r_-g_)/(maxi-mini)), 360);
    }
    else return 0;
}

float Color3f::s() const {
    if (v()==0) return 0;
    else return ((v()-std::min(std::min(r_, g_), b_)) / v());
}

float Color3f::v() const {
    return std::max(std::max(r_, g_), b_);
}

# endif //COLOR4F_HPP_INCLUDED
