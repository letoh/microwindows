//
// Micro Windows Implementation
// freearea.h: the FreeArea class for irregular regions
//
// 「自由區域」又稱 Region , 是 GUI 裡的重要觀念, 在此我們用一個
// 矩形的陣列來表示. 詳細的演算法見 freearea.CPP.
//
// $Revision: 1.2 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/freearea.h $
// $Date: 1994/01/08 18:09:37 $
//

#ifndef __freearea_h
#define __freearea_h

#ifndef __gobject_h
#   include "gobject.h"
#endif

class FreeArea;

class RectArray : public GObject
{
public:
    RectArray ();
    ~RectArray();

    void  add    (Rect *prect);
    void  copy   (RectArray *parray);
    void  move   (int ox, int oy);
    void  purge  ();
    int   amount ();
    int   getsize();
    Rect *get (int index);

protected:
    void expand();
    void deleteall();

    Rect **array;
    int current, size;
    friend FreeArea;
};


#define FreeAreaBegin(x) Rect *x##_tmp; int x##_size=x->getsize(), x##_t;\
                         for (x##_t = 0; x##_t < x##_size; x##_t++) {\
                         if ((x##_tmp=x->get(x##_t))==NULL) continue;
#define FreeAreaEnd      }


class FreeArea : public RectArray
{
public:
    BOOL    intersection (Rect *prect, FreeArea *result);
    BOOL    intersection (FreeArea *parea, FreeArea *result);

    void    add (Rect *prect);
    void    add (FreeArea *parea);
    void    cut (Rect *prect);
    void    cut (FreeArea *parea);
    void    copy (FreeArea *parea);
};

#endif
