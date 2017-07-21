//
// Micro Windows Implementation
// control.h: Control class
//
// Control class 是所有 GUI 控制元件的 base class, 幾乎不做任何事.
//
// $Revision: 1.1 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/control.h $
// $Date: 1993/10/03 03:44:50 $
//

#ifndef __control_h
#define __control_h

#ifndef __view_h
#   include "view.h"
#endif

class Control : public View
{
public:
    Control (Rect *prect, View *pparent, Model *client,
             BOOL attach = TRUE, BOOL killModel = FALSE);

    virtual void messageDispatcher (Message *msg);
    virtual void draw (Rect *area = NULL);

protected:
    virtual void selfMouse (Message *msg);
    virtual void selfDraw (Port *port, Rect *area);

    virtual void mouseMove (Point *pnt);
    virtual void leftButtonDown (Point *pnt);
    virtual void leftButtonUp (Point *pnt);
};

#endif

