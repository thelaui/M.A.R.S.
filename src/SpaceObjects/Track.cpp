/* Track.cpp

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

# include "SpaceObjects/Track.hpp"

# include "SpaceObjects/Home.hpp"
# include "Zones/zones.hpp"
# include "Zones/RasterZone.hpp"
# include "defines.hpp"
# include "System/randomizer.hpp"

Track::Track(Home* home){
    calcTrack();
}

void Track::draw() const {
    if (points_.size() > 2) {
        const int posX = 1;
        const int posY = 1;

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Particles));
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glBegin(GL_QUAD_STRIP);
        glColor3f(0.5f, 0.2f, 0.4f);

        Vector2f lastLeft, lastRight;

        for (int i=0; i<points_.size()+1; ++i) {
            Vector2f p0(points_[(i+0)%points_.size()]);
            Vector2f p1(points_[(i+1)%points_.size()]);
            Vector2f p2(points_[(i+2)%points_.size()]);

            Vector2f toNext = (p0 - p2).normalize()*10.f;
            Vector2f left(p1.x_ + toNext.y_, p1.y_ - toNext.x_);
            Vector2f right(p1.x_ - toNext.y_, p1.y_ + toNext.x_);

            if (i>0) {
                if (clockWise(lastRight-p0, right-lastRight) && !clockWise(lastRight-p0, p1-lastRight) && clockWise(right-p1, p0-right) && !clockWise(right-p1, lastRight-right))
                    right=lastRight;
                else if (!clockWise(lastLeft-p0, left-lastLeft) && clockWise(lastLeft-p0, p1-lastLeft) && !clockWise(left-p1, p0-left) && clockWise(left-p1, lastLeft-left))
                    left=lastLeft;

            }

            glTexCoord2f((posX + 0.5)*0.125f,     posY*0.125f);
                glVertex2f(left.x_,left.y_);
            glTexCoord2f((posX + 0.5)*0.125f, (posY+1)*0.125f);
                glVertex2f(right.x_, right.y_);

            lastLeft = left;
            lastRight = right;
        }

        glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Track::calcTrack() {
    findAnchors();

    sortAnchors();

  //  removeSharpCorners();

    createBezier();
}

void Track::findAnchors() {
    if (randomizer::random(0,1)==1)
        addAnchor(Vector2f(40.f,40.f));
    if (randomizer::random(0,1)==1)
        addAnchor(Vector2f(SPACE_X_RESOLUTION-40.f, 40.f));
    if (randomizer::random(0,1)==1)
        addAnchor(Vector2f(SPACE_X_RESOLUTION-40.f, SPACE_Y_RESOLUTION-40.f));
    if (randomizer::random(0,1)==1)
        addAnchor(Vector2f(40.f, SPACE_Y_RESOLUTION-40.f));

    int tries(0);
    while (anchors_.size() < 10 && ++tries<100)
        addAnchor(Vector2f(randomizer::random(0, SPACE_X_RESOLUTION), randomizer::random(0, SPACE_Y_RESOLUTION)));
}

void Track::addAnchor(Vector2f const& point) {
    for (std::vector<Vector2f>::iterator it = anchors_.begin(); it != anchors_.end(); ++it)
        if((*it-point).lengthSquare() < 90000.f)
            return;

    for (std::vector<SpaceObject*>::const_iterator it = spaceObjects::getObjects().begin(); it != spaceObjects::getObjects().end(); ++it)
        if(((*it)->location()-point).lengthSquare() < std::pow((*it)->radius()+200.f, 2))
            return;

    anchors_.push_back(point);
    zones::addTutorialZone(point, 20.f);
}

void Track::sortAnchors() {
    sortLTR();
    // find middle
    Vector2f middle;
    for (std::vector<Vector2f>::iterator it = anchors_.begin(); it != anchors_.end(); ++it) {
        middle+=*it;
    }
    middle/=anchors_.size();
    zones::addTutorialZone(middle, 50.f);
    int i(0);

    while (anchors_[i++].x_ < middle.x_ && i<anchors_.size()) ;

    sortHalf(middle, 0, i-2, true, true);
    sortHalf(middle, i-1, anchors_.size()-1, true, true);
}


void Track::sortLTR() {
    bool sorted(false);
    while (!sorted) {
        sorted = true;

        for (std::vector<Vector2f>::iterator it = anchors_.begin(); it != --anchors_.end(); ++it) {
            std::vector<Vector2f>::iterator next(it);
            ++next;

            if (it->x_ > next->x_) {
                sorted = false;
                std::swap(*it, *next);
            }
        }
    }
}

void Track::sortHalf (Vector2f const& origin, int startIndex, int endIndex, bool rightHalf, bool CW) {
    bool sorted(false);
    while (!sorted) {
        sorted = true;

        for (int i(startIndex); i<endIndex; ++i) {
            if (rightHalf == CW) {
                if ((anchors_[i] - origin).normalize().y_ > (anchors_[i+1] - origin).normalize().y_) {
                    sorted = false;
                    std::swap(anchors_[i], anchors_[i+1]);
                }
            }
            else {
                if ((anchors_[i] - origin).normalize().y_ < (anchors_[i+1] - origin).normalize().y_) {
                    sorted = false;
                    std::swap(anchors_[i], anchors_[i+1]);
                }
            }
        }
    }
}

void Track::removeSharpCorners() {
    for (int i(0); i<anchors_.size(); ++i) {
        Vector2f current(anchors_[i]);
        Vector2f next(anchors_[(i+1)%anchors_.size()]);
        Vector2f uberNext(anchors_[(i+2)%anchors_.size()]);

        if ((next-current).normalize()*(uberNext-next).normalize() < std::cos(35.f*M_PI/180.f))
            anchors_[i] = Vector2f(0.f, 0.f);
    }
}

void Track::createBezier() {
    for (int i(0); i<anchors_.size(); ++i) {

        Vector2f p0(anchors_[i]);
        Vector2f p1(anchors_[(i+1)%anchors_.size()]);
        Vector2f p2(anchors_[(i+2)%anchors_.size()]);

        Vector2f s0((p1+p0)*0.5f);
        Vector2f s1((p2+p1)*0.5f);

        const float step(20.f);
        for (float t=0.f; t<1.f; t+=1.f/step) {
            points_.push_back(std::pow(1.f-t, 2)*s0 + 2*t*(1.f-t)*p1 + t*t*s1);
        }
    }
}





