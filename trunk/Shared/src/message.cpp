//
// Micro Windows Implementation
// message.cpp
//
// $Revision: 1.2 $
// $Source: P:/MWINDOWS/LIB/rcs/message.cpp $
// $Date: 1993/11/27 09:53:32 $
//

#include "message.h"

BOOL MessageQueue::put (Message *msg)
{
	Message *nmsg = new Message (*msg);
    return (queue.put (nmsg));
}

BOOL MessageQueue::put (View *s, View *r, Controller *c,
                        MsgType t, Point *p, Rect *a, MsgData e)
{
    return (queue.put (new Message (s, r, c, t, p, a, e)));
}

Message *MessageQueue::get ()
{
    return ((Message*)queue.get());
}

BOOL MessageQueue::isEmpty ()
{
    return (queue.isEmpty());
}

//
// this function removes all messages in the message queue that
// matches:
//      (1) it was sent by Controller *c and,
//      (2) it would be received by View *r.
//
// normally this is used in some controllers that supports
// "remove" method. Normally "remove" is called after a view
// is killed/destructed. Thus the controller must clean up
// ALL existing messages that is going to be sent to the
// no longer existed view.
//

void MessageQueue::removeBy (Controller *c, View *r)
{
    Queue another;

    while (1)
    {
        Message *msg;
        if ((msg = (Message*)queue.get()) == NULL) break;

        if (msg->controller == c && msg->receiver == r)
        {
            delete msg;
        }
        else
        {
            another.put (msg);
        }
    }

    while (1)
    {
        Object *obj;
        if ((obj = another.get()) == NULL) break;
        queue.put (obj);
    }
}

