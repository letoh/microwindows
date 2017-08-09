//
// Micro Windows Implementation
// window.h: standard Window
//
// 一個標準的「視窗」出現了! 我們再一次地運用到繼承機制.
//
// $Revision: 1.2 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/window.h $
// $Date: 1993/10/03 15:20:25 $
//

#ifndef __window_h
#define __window_h

#ifndef __multview_h
#   include "multview.h"
#endif

class Text;

class Window : public MultiView
{
public:
    Window (const char *captionText, Rect *prect, View *pparent, Model *client,
            BOOL attach = FALSE,
            BOOL killModel = FALSE);
    ~Window ();

    virtual void    activate ();
    virtual void    deactivate ();
    virtual void    setCaption (const char *captionText);
    virtual void    setResizable (BOOL flag = TRUE);
    virtual void    setAlwaysOnTop (BOOL flag = FALSE);
    virtual void    resize (int width, int height, View *parrent = NULL);

protected:
    virtual void    selfDraw (Port *port, Rect *area);
    virtual void    selfMouse (Message *msg);
    virtual void    drawCaption ();
    virtual void    setupWidgets ();
    ResizePlace     getResizePlace (Point *pnt);

    Text   *caption;
    Rect    captionRect, closeRect, maximizeRect;
    Rect    resizeBorderRect, barRect;
    BOOL    mouseFirstDown;

    Rect    origRect;
    BOOL    maximized;

    BOOL    activeFlag;
    BOOL    resizable;
    BOOL    alwaysOnTop;
};

#endif

