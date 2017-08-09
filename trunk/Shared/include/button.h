//
// Micro Windows Implementation
// button.h: Standard push-button
//
// 按鈕 (button) 是 GUI 裡常見的物件, 現在只要簡單 new 一下
// 就出來一個, 何樂不為呢?
//
// $Revision: 1.2 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/button.h $
// $Date: 1993/10/03 15:19:34 $
//

#ifndef __button_h
#define __button_h

#ifndef __control_h
#   include "control.h"
#endif

#ifndef __shape_h
#   include "shape.h"
#endif

class Button : public Control
{
public:
    Button (const char *text, Rect *prect, View *pparent, Model *client,
            BOOL attach = TRUE, BOOL killModel = FALSE);
    ~Button ();

protected:
    virtual void selfDraw (Port *port, Rect *area);

    virtual void leftButtonDown (Point *pnt);
    virtual void leftButtonUp (Point *pnt);

    virtual void drawButton (Port *port);
    virtual void drawText (Port *port);
    BOOL hitted;

    Text *buttonText;
};

#endif

