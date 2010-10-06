/* Line.cpp

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

# include "Interface/Line.hpp"

# include <SFML/OpenGL.hpp>

Line::Line (Vector2f const& begin, Vector2f const& end):
    UiElement(Vector2f(std::min(begin.x_, end.x_), std::min(begin.y_, end.y_)), std::abs(static_cast<int>(begin.x_ - end.x_)), std::abs(static_cast<int>(begin.y_ - end.y_))),
    begin_(begin),
    end_(end) {}

void Line::draw() const {
    Vector2f begin = parent_->getTopLeft() + begin_;
    Vector2f end   = parent_->getTopLeft() + end_;


    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glLineWidth(2.f);

    glBegin(GL_LINES);
        if (isTopMost())  glColor4f(1.f, 0.5f, 0.8f, 1.0f);
        else              glColor4f(0.4f, 0.4f, 0.4f, 1.0f);
        glVertex2f(begin.x_,begin.y_);
        if (isTopMost())  glColor4f(1.f, 0.5f, 0.8f, 0.0f);
        else              glColor4f(0.4f, 0.4f, 0.4f, 0.0f);
        glVertex2f(end.x_,end.y_);
    glEnd();


}


