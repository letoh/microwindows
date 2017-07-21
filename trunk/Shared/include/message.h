//
// Micro Windows Implementation
// message.h: the message carrier
//
// 由於 C++ 在傳送訊息上不像 Smalltalk, Objective-C 等來得 "OOP",
// 因此我們必須製作自己的「訊息」物件.
//
// $Revision: 1.1 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/message.h $
// $Date: 1993/10/03 03:44:50 $
//

#ifndef __message_h
#define __message_h

#ifndef __stackque_h
#   include "stackque.h"
#endif

#ifndef __gobject_h
#   include "gobject.h"
#endif

class View;
class Controller;

typedef WORD    MsgType;
typedef DWORD   MsgData;

//
// general messages (messages from view)
//
const MsgType MSG_Quit = 0xffff;


class Message : public Object
{
public:
    Message (View *s = NULL, View *r = NULL, Controller *c = NULL,
                 MsgType t = 0, Point *p = NULL, Rect *a = NULL,
                 MsgData e = 0)
    {
        sender = s;
        receiver = r;
        controller = c;
        type = t;
        extra = e;
        if (p) pnt = *p;
        if (a) area =*a;
    }

    View *sender;
    View *receiver;
    Controller *controller;
    MsgType type;
    Point   pnt;
    Rect    area;
    MsgData extra;
};

class MessageQueue : public Object
{
public:
    BOOL    put (Message *msg);
    BOOL    put (View *s = NULL, View *r = NULL, Controller *c = NULL,
                 MsgType t = 0, Point *p = NULL, Rect *a = NULL,
                 MsgData e = 0);
    Message *get ();
    BOOL    isEmpty ();
    void    removeBy (Controller *c, View *r);

protected:
    Queue   queue;
};

#endif

