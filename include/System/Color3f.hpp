/* Color3f.hpp

Copyright (c) 2010 by Felix Lauer und Simon Schneegans

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

        float r() const;
        float g() const;
        float b() const;
        float h() const;
        float s() const;
        float v() const;

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

# endif //COLOR4F_HPP_INCLUDED
