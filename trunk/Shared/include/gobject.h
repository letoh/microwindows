//
// Micro Windows Implementation
// gobject.h: elementary graph representives
//
// 點 (Point) 和矩形 (Rectangle) 是電腦繪圖裡的兩個最基本元件,
// 在此用 C++ 表示出來. 注意別把 GObject (圖元) 和 Shape (圖形)
// 混在一起了. GObject 的矩形叫 Rect class , 而 Shape 的矩形則叫
// Rectangle. 沒有理由解釋為何這樣取名, 只是為了不要搞混罷了.
//
// $Revision: 1.2 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/gobject.h $
// $Date: 1993/11/27 09:52:44 $
//

#ifndef __gobject_h
#define __gobject_h

#ifndef __object_h
#   include "object.h"
#endif

class GObject : public Object
{
public:
};

class Point : public GObject
{
public:
    Point (int px=0, int py=0);
    void move (int px, int py);
    void move (Point *offset);
    void set (int px, int py);
    int x, y;
};


class Rect : public GObject
{
public:
    Rect (int px1=0, int py1=0, int px2=0, int py2=0);
    Rect (Point *orig, Point *corn);
    void set (int px1, int py1, int px2, int py2);
    void set (Point *orig, Point *corn);
    void move (int ox, int oy);
    void move (Point *offset);
    void resize (int width, int height);
    void resize (Rect *reference);
    int  width();
    int  height();

    BOOL inside(int px, int py);        // inside = TRUE if the point
    BOOL inside(Point *p);              // is inside the rectangle.

    // intersection = TRUE if two rect (*this & *toDeal) has intersection
    // (put in *result) otherwise returns FALSE.
    BOOL intersection (Rect *toDeal, Rect *result);

    int x1, y1, x2, y2;

protected:
    void normalize();
};
#endif

