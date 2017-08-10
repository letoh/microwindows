//
// Micro Windows Implementation
// gui.cpp
//
// $Revision: 1.1 $
// $Source: D:/DENG/PRG/MWINDOWS/WIN/LIB/rcs/gui.cpp $
// $Date: 1993/10/18 12:32:27 $
//

#include "gui.h"
#include "desktop.h"
#include "list.h"
#include "message.h"
#include "lowgdi.h"
#include "controlr.h"
#include "mouse.h"
#include "timer.h"
#include "idler.h"
#include <X11/Xlib.h>
#include <X11/Xatom.h>

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
    Message *msg = NULL;
    XEvent e;
    extern Display *dpy;
    extern Window topwin;
    extern int _winButtonState;
    extern int _winMouseX;
    extern int _winMouseY;
    static int buttonPressed = 0;

    Atom wm_delete_window = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(dpy, topwin, &wm_delete_window, 1);

    while (1)
    {
        if (XPending(dpy)) {
            XNextEvent(dpy, &e);
            if (e.type == Expose)
                XFlush(dpy);
            else if (e.type == MotionNotify) {
                // fprintf(stderr, "motion: %d, %d\n", e.xmotion.x, e.xmotion.y);
                _winMouseX = e.xmotion.x;
                _winMouseY = e.xmotion.y;
                _winButtonState = buttonPressed? LGM_DRAG : LGM_MOVE;
            }
            else if (e.type == ButtonPress) {
                _winButtonState = LGM_BUTTONDOWN;
                buttonPressed = 1;
            }
            else if (e.type == ButtonRelease) {
                _winButtonState = LGM_BUTTONUP;
                buttonPressed = 0;
            }
            else if (e.type == Expose && e.xexpose.count == 0) {
                desktop->draw ();
            }
            else if (e.type == ClientMessage) {
                if ((Atom)e.xclient.data.l[0] == wm_delete_window) {
                    break;
                }
            }
        }

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
        msg = NULL;
    }

    int returnval = 0;
    if (msg) {
        returnval = (int)msg->extra;
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

