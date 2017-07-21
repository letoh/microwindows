//
// Micro Windows Implementation
// mouse.h: the Mouse class
//
// GUI 少不了滑鼠. 而事實上, Mouse class 只可能有一個 instance,
// 在 global.h 裡提到過了.
//
// $Revision: 1.2 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/mouse.h $
// $Date: 1993/10/03 15:21:45 $
//

#ifndef __mouse_h
#define __mouse_h

#ifndef __controlr_h
#   include "controlr.h"
#endif

const MsgType MSG_MouseMove = 1,
              MSG_LeftButtonDown = 2,
              MSG_LeftButtonUp = 3;

const MsgType Mouse_Dispatch = 0, Mouse_Direct = 1;

class Port;

class Mouse : public Controller
{
public:
    Mouse ();

    BOOL lock (View *sendTo, Port *lockRange = NULL);
    void unlock ();
    void hide ();
    void show ();

    virtual void pool (MessageQueue *queue);

protected:
    int lastX, lastY, lastButton;
    View *lockTo;
    Point rangeOffset;
};

#endif

