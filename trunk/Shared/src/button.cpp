//
// Micro Windows Implementation
// button.h
//
// $Revision: 1.3 $
// $Source: P:/MWINDOWS/LIB/rcs/button.cpp $
// $Date: 1993/11/27 09:53:32 $
//

// note: in Button class, the mouse handler function is
// inherited from Control class, which dispatches mouse messages
// to itself, not clientModel.

#include "button.h"
#include "global.h"
#include "model.h"

Button::Button (char *text, Rect *prect, View *pparent, Model *client,
                BOOL attach, BOOL killModel)
       :Control (prect, pparent, client, FALSE, killModel)
{
    buttonText = new Text (text, &clientRect);
    hitted = FALSE;

    if (attach && parent) parent->attachSubview (this);
}

Button::~Button ()
{
    delete buttonText;
}

void Button::selfDraw (Port *port, Rect *area)
{
    drawButton (port);
}

void Button::leftButtonDown (Point *pnt)
{
    if (hitted == TRUE) return;

    hitted = TRUE;
    drawButton (viewPort);
    mouse->lock (this);
}

void Button::leftButtonUp (Point *pnt)
{
    if (hitted == FALSE) return;

    hitted = FALSE;
    mouse->unlock ();
    drawButton (viewPort);

    if (clientRect.inside (pnt) == FALSE) return;
    clientModel->fromView (this);
}

void Button::drawButton (Port *port)
{
    port->setPenColor (PPC_Gray);
    port->draw (&FilledRectangle (&clientRect));
    port->setPenColor (PPC_Black);
    port->setBgColor (PPC_Gray);
    drawText (port);

    StereoType t = Convex;
    if (hitted == TRUE) t = Concave;

    port->draw (&StereoBorder (&clientRect, t, 1));
}

void Button::drawText (Port *port)
{
    port->draw (buttonText);
}

