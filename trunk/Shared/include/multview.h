//
// Micro Windows Implementation
// multview.h: overlapped-window interface implementaion: MultiView
//
// 重疊式視窗介面的演算法被移植在這個稱作 MultiView 的 class 中.
//
// $Revision: 1.1 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/multview.h $
// $Date: 1993/10/03 03:44:49 $
//

#ifndef __multview_h
#define __multview_h

#ifndef __view_h
#   include "view.h"
#endif

enum MultiViewBorder
{
    MVBdr_Move,
    MVBdr_Resize
};


class MultiView : public View
{
public:
    MultiView (Rect *prect, View *pparent, Model *client,
               BOOL attach = FALSE, BOOL killModel = FALSE);
    ~MultiView ();

    virtual Port   *getAttachPort ();
    virtual Port   *getClientPort ();
    virtual void    draw (FreeArea *area);
    virtual void    draw (Rect *area = NULL);
    virtual BOOL    close (BOOL immediate = FALSE);
    virtual void    resize (int width, int height, View *pparent = NULL);

    virtual void    dispatchMouse (Message *msg);
    virtual void    attachSubview (View *subview);
    virtual void    removeSubview (View *subview);
    virtual void    moveSubview (View *subview, Point *offset);
    virtual void    resizeSubview (View *subview, int width, int height);
    virtual void    moveResizeSubview (View *subview, Point *offset,
                                       int width, int height);
    virtual void    bringTop (View *subview);

    virtual void    beginMove (View *subview);
    virtual void    beginResize (View *subview, ResizePlace rp);

    virtual void    hide (View *pparent, Rect *area = NULL);
    virtual void    show (View *pparent, Rect *area = NULL);

protected:
    virtual void    selfMouse (Message *msg);
    virtual void    activateSubview (View *subview);
    virtual void    calcOverlapping (View *from, Rect *newRect,
                                     Rect *oldRect);
    virtual void    drawBorder ();
    virtual void    freeAreaHide (View *subview, FreeArea *area);
    virtual void    freeAreaShow (View *subview, FreeArea *area);
    virtual void    selfShowDraw (FreeArea *area);
    virtual void    selfHide (Rect *area);

    ViewPort *attachPort;

    View *activeView;

    Point borderSpot;
    Rect borderRect;
    BOOL borderUsing;
    BOOL borderHitted;
    View *borderView;
    MultiViewBorder borderType;
    ResizePlace borderPlace;
};

#endif

