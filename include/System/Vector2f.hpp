/* Vector2f.hpp

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

# ifndef VECTOR2F_HPP_INCLUDED
# define VECTOR2F_HPP_INCLUDED

# include <iostream>

struct Vector2f {
    Vector2f ();
    Vector2f (Vector2f const& point);
    Vector2f (float x, float y);

    Vector2f normalize()    const;
    float    length()       const;
    float    lengthSquare() const;

    static Vector2f randDir();
    static Vector2f randDirLen();

    Vector2f& operator+=(Vector2f const& rhs);
    Vector2f& operator-=(Vector2f const& rhs);
    Vector2f& operator*=(double rhs);
    Vector2f& operator/=(double rhs);

    float x_, y_;
};

Vector2f      operator+  (Vector2f const& lhs, Vector2f const& rhs);
Vector2f      operator-  (Vector2f const& lhs, Vector2f const& rhs);
float         operator*  (Vector2f const& lhs, Vector2f const& rhs);
Vector2f      operator*  (Vector2f const& lhs, float rhs);
Vector2f      operator*  (float const& lhs, Vector2f rhs);
Vector2f      operator/  (Vector2f const& lhs, float rhs);
bool          operator== (Vector2f const& lhs, Vector2f const& rhs);
bool          operator!= (Vector2f const& lhs, Vector2f const& rhs);
std::ostream& operator<< (std::ostream& os, Vector2f const& rhs);

# endif //VECTOR2F_HPP_INCLUDED
