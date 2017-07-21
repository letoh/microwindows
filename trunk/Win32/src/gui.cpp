//
// Micro Windows Implementation
// gui.cpp
//
// $Revision: 1.1 $
// $Source: D:/DENG/PRG/MWINDOWS/WIN/LIB/rcs/gui.cpp $
// $Date: 1993/10/18 12:32:27 $
//

#include "mwindows.h"
#include "gui.h"
#include "desktop.h"
#include "list.h"
#include "message.h"
#include "lowgdiw.h"
#include "controlr.h"
#include "mouse.h"
#include "timer.h"
#include "idler.h"

Desktop      *desktop = NULL;
MessageQueue *msgqueue;
Mouse        *mouse;
Timer        *timer;
Idler        *idler;
List         *controllerList;

Driver::Driver ()
{
    if (desktop != NULL)
    {
        mainDriver = FALSE;
        return;
    }

    mainDriver = TRUE;
    _lgOpenGraphics ();

    controllerList = new List;
    initControllers ();

    msgqueue = new MessageQueue;
    desktop = new Desktop (&Rect (0, 0, _lgGetScreenWidth() - 1,
                                        _lgGetScreenHeight() - 1));
}

Driver::~Driver ()
{
    if (mainDriver == FALSE) return;

    deleteControllers ();
    delete controllerList;
    delete msgqueue;

    _lgCloseGraphics ();
}

int Driver::run ()
{
	int returnval=0;
    Message *msg = NULL;
    MSG wmsg;

    while (1)
    {
        if (PeekMessage (&wmsg, NULL, 0, 0, PM_REMOVE))
        {
            if (wmsg.message == WM_QUIT) break;
            TranslateMessage (&wmsg);
            DispatchMessage (&wmsg);
        }

        poolControllers ();

        if ((msg = msgqueue->get()) == NULL) continue;
        if (desktop->isEmpty() == TRUE) PostQuitMessage (0);
        if (msg->sender == desktop && msg->type == MSG_Quit)
        {
            PostQuitMessage (0);
        }

        desktop->messageDispatcher (msg);
		returnval=(int)msg->extra;
        delete msg;
    }

    return (returnval);
}

void Driver::poolControllers ()
{
    Controller *c;

    if ((c = (Controller*)controllerList->next ()) == NULL)
        c = (Controller*)controllerList->first();
    if (c == NULL) return;

    c->pool (msgqueue);
}

void Driver::initControllers ()
{
    mouse = new Mouse;
    timer = new Timer;
    idler = new Idler;
    controllerList->put (mouse);
    controllerList->put (timer);
    controllerList->put (idler);
}

void Driver::deleteControllers ()
{
    Object *o = controllerList->first();

    do
    {
        if (o == NULL) break;
        delete o;
        o = controllerList->next();
    } while (1);
}

