//
// Micro Windows Implementation
//
//
// $Revision: 1.2 $
// $Source: P:/MWINDOWS/LIB/rcs/shape.cpp $
// $Date: 1993/10/18 12:33:19 $
//

#include "shape.h"
#include "lowgdi.h"

void Shape::setOrig (Point *pnt)
{
    orig = *pnt;
}

void Shape::setDest (Point *pnt)
{
    dest = *pnt;
}

void Shape::setBoundary (Rect *boundary)
{
	orig.set (boundary->x1, boundary->y1);
	dest.set (boundary->x2, boundary->y2);
}

BOOL Shape::inside (Point *p)
{
    Rect r (&orig, &dest);
    return (r.inside (p));
}

Line::Line (Point *porig, Point *pdest)
{
    orig = *porig;
    dest = *pdest;
}

Line::Line (int x1, int y1, int x2, int y2)
{
    orig.set (x1, y1);
    dest.set (x2, y2);
}

void Line::draw (Port *port)
{
    Point deviceOrig;
    port->getDeviceOrig (&deviceOrig);
    int x = deviceOrig.x, y = deviceOrig.y;
    _lgLine (x + orig.x, y + orig.y, x + dest.x, y + dest.y);
}

Rectangle::Rectangle (Point *porig, Point *pdest)
{
    orig = *porig;
    dest = *pdest;
}

Rectangle::Rectangle (int x1, int y1, int x2, int y2)
{
    orig.set (x1, y1);
    dest.set (x2, y2);
}

Rectangle::Rectangle (Rect *r)
{
    orig.set (r->x1, r->y1);
    dest.set (r->x2, r->y2);
}

void Rectangle::draw (Port *port)
{
    Point deviceOrig;
    port->getDeviceOrig (&deviceOrig);
    int x = deviceOrig.x, y = deviceOrig.y;
    _lgRect (x + orig.x, y + orig.y, x + dest.x, y + dest.y);
}

FilledRectangle::FilledRectangle (Point *porig, Point *pdest)
                :Rectangle (porig, pdest)
{
}

FilledRectangle::FilledRectangle (int x1, int y1, int x2, int y2)
                :Rectangle (x1, y1, x2, y2)
{
}

FilledRectangle::FilledRectangle (Rect *r)
                :Rectangle (r)
{
}

void FilledRectangle::draw (Port *port)
{
    Point deviceOrig;
    port->getDeviceOrig (&deviceOrig);
    int x = deviceOrig.x, y = deviceOrig.y;
    _lgSolidRect (x + orig.x, y + orig.y, x + dest.x, y + dest.y);
}

Text::Text (const char *pstring, Point *porig, Point *pdest)
{
    copyString (pstring);
    orig = *porig;

    if (pdest)
    {
        dest = *pdest;
        bounded = TRUE;
    }
    else bounded = FALSE;
}

Text::Text (const char *pstring, Rect *boundbox)
{
    copyString (pstring);
    bounded = TRUE;
    orig.set (boundbox->x1, boundbox->y1);
    dest.set (boundbox->x2, boundbox->y2);
}

Text::Text (Text &ptext)
{
    copyString (ptext.string);
    orig = ptext.orig;
    dest = ptext.dest;
}

Text::~Text()
{
    delete string;
}

void Text::draw (Port *port)
{
    Point deviceOrig;
    port->getDeviceOrig (&deviceOrig);

    Rect boundary (&orig, &dest);

    if (bounded == FALSE)
    {
        _lgDrawText (deviceOrig.x + orig.x, deviceOrig.y + orig.y, string);
        return;
    }

    _lgDrawText (deviceOrig.x +
                    orig.x + (boundary.width() - _lgTextWidth (string)) /2,
                 deviceOrig.y +
                    orig.y + (boundary.height() - _lgTextHeight (string)) /2,
                 string);
}

void Text::copyString (const char *str)
{
    string = new char [strlen(str) + 1];
    strcpy (string, str);
}

StereoBorder::StereoBorder (Point *porig, Point *pdest, StereoType t,
              int pdepth)
{
    setParam (&Rect (porig, pdest), t, pdepth);
}

StereoBorder::StereoBorder (int x1, int y1, int x2, int y2, StereoType t,
              int pdepth)
{
    setParam (&Rect (x1, y1, x2, y2), t, pdepth);
}

StereoBorder::StereoBorder (Rect *r, StereoType t, int pdepth)
{
    setParam (r, t, pdepth);
}


void StereoBorder::draw (Port *port)
{
    PortPenColor c = port->getPenColor();
    Point deviceOrig;
    port->getDeviceOrig (&deviceOrig);

    int x1 = deviceOrig.x + orig.x, y1 = deviceOrig.y + orig.y,
        x2 = deviceOrig.x + dest.x, y2 = deviceOrig.y + dest.y;
    PortPenColor up = PPC_Black, down = PPC_White;

    if (type == Convex)
    {
        up = PPC_White;
        down = PPC_Black;
    }

    for (int i = 0; i < depth; i++)
    {
        _lgSetColor (up);
        _lgLine (x1 + i, y1 + i, x2 - i, y1 + i);
        _lgLine (x1 + i, y1+1 + i, x1 + i, y2 - i);
        _lgSetColor (down);
        _lgLine (x1+1 + i, y2 - i , x2 - i, y2 - i);
        _lgLine (x2 - i, y1+1 + i , x2 - i, y2-1 -i);
    }

    port->setPenColor (c);
}

void StereoBorder::setParam (Rect *r, StereoType t, int pdepth)
{
    orig.set (r->x1, r->y1);
    dest.set (r->x2, r->y2);
    type = t;
    depth = pdepth;
}

