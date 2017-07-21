//
// Micro Windows Implementation
// mouse.cpp
//
// $Revision: 1.3 $
// $Source: P:/MWINDOWS/LIB/rcs/mouse.cpp $
// $Date: 1993/11/27 09:53:32 $
//

#include "lowgdi.h"
#include "mouse.h"
#include "port.h"
#include "view.h"

Mouse::Mouse ()
{
    lastX = lastY = -1;
    lastButton = 0xff;
    lockTo = NULL;
}

//
// when lock is called, the mouse will only send messages
// to the locked view.
//
BOOL Mouse::lock (View *sendTo, Port *lockRange)
{
    Rect tmpRect;

    if (lockTo) return (FALSE);

    lockTo = sendTo;
    Point o;
    Port *r = lockRange;
    if (r == NULL) r = sendTo->getClientPort ();
    r->getDeviceOrig (&o);

    rangeOffset.set (- o.x, - o.y);

    sendTo->getViewRect (&tmpRect);
    rangeOffset.move (tmpRect.x1, tmpRect.y1);
    sendTo->getClientRect (&tmpRect);
    rangeOffset.move (tmpRect.x1, tmpRect.y1);

    return (TRUE);
}

void Mouse::unlock ()
{
    lockTo = NULL;
}

void Mouse::hide ()
{
    if (lockTo == NULL) return;
    _lgHideMouse ();
}

void Mouse::show ()
{
    _lgShowMouse ();
}

void Mouse::pool (MessageQueue *queue)
{
    Message m;

    int x = _lgGetMouseX(), y = _lgGetMouseY(), b = _lgGetMouseButton();

    if (x != lastX || y != lastY || b != lastButton)
    {
        lastX = x;
        lastY = y;
        lastButton = b;

        m.extra = Mouse_Dispatch;
        m.controller = this;
        m.pnt.set (x, y);

        if (lockTo)
        {
            m.receiver = lockTo;
            m.pnt.move (&rangeOffset);
            m.extra = Mouse_Direct;
        }

        switch (b)
        {
            case LGM_MOVE:
                m.type = MSG_MouseMove;
                break;

            case LGM_BUTTONDOWN:
            case LGM_DRAG:
                m.type = MSG_LeftButtonDown;
                break;

            case LGM_BUTTONUP:
                m.type = MSG_LeftButtonUp;
                break;
        }

        queue->put (&m);
    }
}

