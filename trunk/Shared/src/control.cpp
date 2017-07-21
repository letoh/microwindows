//
// Micro Windows Implementation
// control.h
//
// $Revision: 1.2 $
// $Source: P:/MWINDOWS/LIB/rcs/control.cpp $
// $Date: 1993/11/27 09:53:32 $
//

// Control class does nothing but handles mouse class by its own.

#include "control.h"
#include "message.h"
#include "mouse.h"

Control::Control (Rect *prect, View *pparent, Model *client,
                  BOOL attach, BOOL killModel)
        :View (prect, pparent, client, FALSE, killModel)
{
    if (attach && parent) parent->attachSubview (this);
}

void Control::messageDispatcher (Message *msg)
{
    View::messageDispatcher (msg);
}

void Control::draw (Rect *area)
{
    Model *cli = NULL;

    if (clientModel)
    {
        cli = clientModel;
        clientModel = NULL;
    }

    View::draw (area);

    if (cli) clientModel = cli;
}

//
// dispatch mouse messages to methods of itself.
//
void Control::selfMouse (Message *msg)
{
    switch (msg->type)
    {
        case MSG_MouseMove:
            mouseMove (&(msg->pnt));
            return;

        case MSG_LeftButtonDown:
            leftButtonDown (&(msg->pnt));
            return;

        case MSG_LeftButtonUp:
            leftButtonUp (&(msg->pnt));
            return;
    }
}

void Control::selfDraw (Port *port, Rect *area)
{
    View::selfDraw (port, area);
}

void Control::mouseMove (Point *pnt)
{
}

void Control::leftButtonDown (Point *pnt)
{
}

void Control::leftButtonUp (Point *pnt)
{
}

