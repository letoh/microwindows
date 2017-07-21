//
// Micro Windows Implementation
// gobject.cpp:
//
// $Revision: 1.2 $
// $Source: P:/MWINDOWS/LIB/rcs/gobject.cpp $
// $Date: 1993/11/27 09:53:32 $
//
// This module provides enough primitive that constructs the
// whole graphical environment.
//


#include "gobject.h"

Point::Point (int px, int py)
{
    x = px;
    y = py;
}

void Point::move (int px, int py)
{
    x += px;
    y += py;
}

void Point::move (Point *offset)
{
    x += offset->x;
    y += offset->y;
}

void Point::set (int px, int py)
{
    x = px;
    y = py;
}

Rect::Rect (int px1, int py1, int px2, int py2)
{
    set (px1, py1, px2, py2);
}

Rect::Rect (Point *orig, Point *corn)
{
    set (orig, corn);
}

//
// Note that instances of Rect class are *always* normalized.
//
void Rect::set (int px1, int py1, int px2, int py2)
{
    x1 = px1;
    y1 = py1;
    x2 = px2;
    y2 = py2;
    normalize();
}

void Rect::set (Point *orig, Point *corn)
{
    x1 = orig->x;
    y1 = orig->y;
    x2 = corn->x;
    y2 = corn->y;
    normalize();
}

void Rect::move (int ox, int oy)
{
    x1 += ox;
    y1 += oy;
    x2 += ox;
    y2 += oy;
}

void Rect::move (Point *offset)
{
    x1 += offset->x;
    y1 += offset->y;
    x2 += offset->x;
    y2 += offset->y;
}

void Rect::resize (int width, int height)
{
    x2 = x1 + width -1;
    y2 = y1 + height -1;
    normalize();
}

void Rect::resize (Rect *reference)
{
    resize (reference->width(), reference->height());
}

int  Rect::width()
{
    return (x2 - x1 + 1);
}

int  Rect::height()
{
    return (y2 - y1 + 1);
}


BOOL Rect::inside (int px, int py)
{
    if (px < x1 || px > x2 || py < y1 || py > y2) return (FALSE);
    return (TRUE);
}

BOOL Rect::inside (Point *p)
{
    return (inside (p->x, p->y));
}

//
// Rect::intersection is a simple algorithm that computes intersection
// area between two rectangles.
//

BOOL Rect::intersection (Rect *toDeal, Rect *result)
{
    #define min(x, y)   ((x < y) ? x : y)
    #define max(x, y)   ((x > y) ? x : y)

    normalize();
    toDeal->normalize();

    result->x1 = max(x1, toDeal->x1);
    result->y1 = max(y1, toDeal->y1);
    result->x2 = min(x2, toDeal->x2);
    result->y2 = min(y2, toDeal->y2);

    //
    // if there is an intersection, the instance "result" should
    // be *normalized*, otherwise, there is no intersection.
    //
    if (result->x1 > result->x2 || result->y1 > result->y2) return (FALSE);

    return (TRUE);
}

#define SWAP(x, y)      tmp = x; x = y; y = tmp;
void Rect::normalize()
{
    int tmp;
    if (x1 > x2) { SWAP(x1, x2); }
    if (y1 > y2) { SWAP(y1, y2); }
}

