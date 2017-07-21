//
// Micro Windows Implementation
// shape.h
//
// 圖形類別, 其下的衍生 class 均是用來代表一種圖形. 不要和 GObject
// 一支搞混了.
//
// $Revision: 1.2 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/shape.h $
// $Date: 1993/11/27 09:52:44 $
//
#ifndef __shape_h
#define __shape_h

#ifndef __gobject_h
#   include "gobject.h"
#endif

#ifndef __port_h
#   include "port.h"
#endif

class Shape : public Object
{
public:
    virtual void    draw (Port *port)=0;
    virtual BOOL    inside (Point *p);
    virtual void    setOrig (Point *pnt);
    virtual void    setDest (Point *pnt);
    virtual void    setBoundary (Rect *boundary);

protected:
    Point orig, dest;
};

class Line : public Shape
{
public:
    Line (Point *porig, Point *pdest);
    Line (int x1, int y1, int x2, int y2);

    virtual void    draw (Port *port);
};


class Rectangle : public Shape
{
public:
    Rectangle (Point *porig, Point *pdest);
    Rectangle (int x1, int y1, int x2, int y2);
    Rectangle (Rect *r);

    virtual void    draw (Port *port);
};

class FilledRectangle : public Rectangle
{
public:
    FilledRectangle (Point *porig, Point *pdest);
    FilledRectangle (int x1, int y1, int x2, int y2);
    FilledRectangle (Rect *r);
    virtual void    draw (Port *port);
};

class Text : public Shape
{
public:
    Text (char *pstring, Point *porig, Point *pdest = NULL);
    Text (char *pstring, Rect *boundbox);
    Text (Text &ptext);
    ~Text();

    virtual void    draw (Port *port);

protected:
    void copyString (char *str);
    char *string;
    BOOL bounded;
};

enum StereoType
{
    Concave = 0,        // Concave 為凹進去之意
    Convex              // Convex 當然就是相反囉
};

class StereoBorder : public Shape
{
public:
    StereoBorder (Point *porig, Point *pdest, StereoType t = Convex,
                  int pdepth = 2);
    StereoBorder (int x1, int y1, int x2, int y2, StereoType t = Convex,
                  int pdepth = 2);
    StereoBorder (Rect *r, StereoType t = Convex, int pdepth = 2);

    virtual void    draw (Port *port);

protected:
    StereoType type;
    int depth;
    void setParam (Rect *r, StereoType t, int pdepth);
};


#endif

