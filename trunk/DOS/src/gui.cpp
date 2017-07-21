//
// Micro Windows Implementation
// gui.cpp
//
// $Revision: 1.2 $
// $Source: P:/MWINDOWS/LIB/rcs/gui.cpp $
// $Date: 1993/11/27 09:53:32 $
//

// message loop and system (de)initialization is here....

#include "gui.h"
#include "desktop.h"
#include "list.h"
#include "message.h"
#include "lowgdi.h"
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
    _lgOpenGraphics ();         // initialized LowGDI

    if (_lgIsMouseActive() == FALSE)
    {
        _lgCloseGraphics ();
        printf ("Micro Windows: mouse driver should be loaded before running me.\n");
        abort ();       // it's a function in STDLIB.H
    }

    controllerList = new List;
    initControllers ();

    // create global objects

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

// -----------------------
//  STANDARD MESSAGE LOOP
// -----------------------

// Deng's comment: however, in MODAL DIALOGUE BOX (not supported
// in Micro Windows/Lite) a standalone message box is required.

int Driver::run ()
{
    Message *msg;

    while (1)
    {
        poolControllers ();         // pool controllers

        // is the queue empty?
        if ((msg = msgqueue->get()) == NULL) continue;

        // does desktop contain no subview anymore?
        if (desktop->isEmpty() == TRUE) break;

        // is it a quit message?
        if (msg->sender == desktop && msg->type == MSG_Quit) break;

        // if not, dispatch it
        desktop->messageDispatcher (msg);

        // and then destroy the message object
        delete msg;
    }

    int returnval = (int)msg->extra;
    delete msg;
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

