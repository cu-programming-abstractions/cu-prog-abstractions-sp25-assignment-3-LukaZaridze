/******************************************************************************
 * File: FlagOfRecursia.cpp
 * Author: Keith Schwarz (htiek@cs.stanford.edu)
 *
 * Code to draw the Flag of Recursia. You do not need to edit this file when
 * working through Assignment 3. Instead, you'll just work through it in the
 * debugger.
 */
#include "FlagOfRecursia.h"
#include "vector.h"
using namespace std;

/* Constants controlling the colors on the flag. */
const Color kCardinal   = Color(196,  30,  58);
const Color kSandstone  = Color(245, 242, 225);

/* The golden ratio, used to subdivide the triangles. */
const double kPhi = (1 + sqrt(5.0)) / 2;

int drawObtuseTriangle(const Point& apex,
                       const Point& base1,
                       const Point& base2,
                       int order);

int drawAcuteTriangle(const Point& apex,
                      const Point& base1,
                      const Point& base2,
                      int order) {
    if (order == 0) {
        drawTriangle(apex, base1, base2, kCardinal);
        return 1;
    } else {
        Point sideMid = apex + (base1 - apex) / kPhi;

        int trianglesDrawn = 0;
        trianglesDrawn += drawAcuteTriangle(base2, sideMid, base1, order - 1);
        trianglesDrawn += drawObtuseTriangle(sideMid, base2, apex, order - 1);
        return trianglesDrawn;
    }
}

int drawObtuseTriangle(const Point& apex,
                       const Point& base1,
                       const Point& base2,
                       int order) {
    if (order == 0) {
        drawTriangle(apex, base1, base2, kSandstone);
        return 1;
    } else {
        Point baseMid = base1 + (base2 - base1) / kPhi;
        Point sideMid = base1 + (apex - base1) / kPhi;

        int trianglesDrawn = 0;
        trianglesDrawn += drawObtuseTriangle(sideMid, baseMid, base1, order - 1);
        trianglesDrawn += drawObtuseTriangle(baseMid, base2, apex, order - 1);
        trianglesDrawn += drawAcuteTriangle(baseMid, sideMid, apex, order - 1);
        return trianglesDrawn;
    }
}

Vector<Point> placeDecagonIn(const Rectangle& bounds) {
    Rectangle square;
    if (bounds.width >= bounds.height) {
        square.x = bounds.x + (bounds.width - bounds.height) / 2;
        square.y = bounds.y;
        square.width = square.height = bounds.height;
    } else {
        square.x = bounds.x;
        square.y = bounds.y + (bounds.height - bounds.width) / 2;
        square.width = square.height = bounds.width;
    }

    Point center = { square.x + square.width / 2, square.y + square.height / 2 };
    int radius   = square.width * 0.4;

    Vector<Point> decagonPoints;
    for (int i = 0; i < 10; i++) {
        Point pt;
        pt.x = center.x - radius * cos(i * M_PI / 5 + M_PI / 10);
        pt.y = center.y + radius * sin(i * M_PI / 5 + M_PI / 10);
        decagonPoints += pt;
    }

    return decagonPoints;
}

int drawFlagOfRecursia(const Rectangle& bounds) {
    Vector<Point> decagonPoints = placeDecagonIn(bounds);
    Point center = { bounds.x + bounds.width / 2, bounds.y + bounds.height / 2 };

    int numTriangles = 0;
    for (int i = 0; i < 10; i++) {
        Point p0 = decagonPoints[i];
        Point p1 = decagonPoints[(i + 1) % 10];

        int triangles = drawAcuteTriangle(center, p0, p1, i);
        numTriangles += triangles;
    }

    return numTriangles;
}
