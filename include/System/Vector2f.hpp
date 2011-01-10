/* Vector2f.hpp

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
this program.  If not, see <http://www.gnu.org/licenses/>. */

# ifndef VECTOR2F_HPP_INCLUDED
# define VECTOR2F_HPP_INCLUDED

# include <iostream>

/// A struct representing a 2D-Vector.
/// It provides more math functions then the SFML Vector
/// and is more optimized for MARS.

struct Vector2f {
    /// Default ctor (0, 0).
    Vector2f ();
    /// Copy ctor.
    Vector2f (Vector2f const& point);
    /// Ctor from a \a x and a \a y value.
    Vector2f (float x, float y);

    /// Sets the length of the vector to 1.
    Vector2f normalize()    const;

    /// Returns the length of the vector.
    /// Use Vector2f::lengthSquare for comparing the length
    /// of vectors, because it's much faster.
    float    length()       const;

    /// Returns the squared length of the vector.
    float    lengthSquare() const;

    /// Creates a vector with a random direction and a lenght of 1.
    /// This one is faster than Vector2f::randDirLen!
    static Vector2f const randDir();

    /// Creates a vector with a random direction and lenght.
    /// This one is slower than Vector2f::randDir!
    static Vector2f const randDirLen();

    /// Overload for the operator += with another vector.
    Vector2f& operator+=(Vector2f const& rhs);

    /// Overload for the operator -= with another vector.
    Vector2f& operator-=(Vector2f const& rhs);

    /// Overload for the operator *= with a scalar.
    Vector2f& operator*=(double rhs);

    /// Overload for the operator /= with a scalar.
    Vector2f& operator/=(double rhs);

    /// \name Data
    /// Members storing the information of the vector.
    ///@{
    float x_, y_;
    ///@}
};

/// Addition of two vectors.
Vector2f      operator+  (Vector2f const& lhs, Vector2f const& rhs);

/// Subtraction of two vectors.
Vector2f      operator-  (Vector2f const& lhs, Vector2f const& rhs);

/// Scalar multiplication of two vectors.
float         operator*  (Vector2f const& lhs, Vector2f const& rhs);

/// Multiplication of a vector with a scalar.
Vector2f      operator*  (Vector2f const& lhs, float rhs);

/// Multiplication of a scalar with a vector.
Vector2f      operator*  (float const& lhs, Vector2f rhs);

/// Division of a vector by a scalar.
Vector2f      operator/  (Vector2f const& lhs, float rhs);

/// Comparision of two vectors.
bool          operator== (Vector2f const& lhs, Vector2f const& rhs);

/// Comparision of two vectors.
bool          operator!= (Vector2f const& lhs, Vector2f const& rhs);

/// Comparision of two vectors.
bool          operator<  (Vector2f const& lhs, Vector2f const& rhs);

/// Comparision of two vectors.
bool          operator>  (Vector2f const& lhs, Vector2f const& rhs);

/// Stream operator for a vector.
/// Creates an output like [x, y].
std::ostream& operator<< (std::ostream& os, Vector2f const& rhs);

# endif //VECTOR2F_HPP_INCLUDED
