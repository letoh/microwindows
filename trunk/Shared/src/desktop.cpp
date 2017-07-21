//
// Micro Windows Implementation
// desktop.cpp
//
// $Revision: 1.4 $
// $Source: P:/MWINDOWS/LIB/rcs/desktop.cpp $
// $Date: 1993/11/27 09:53:32 $
//

#include "desktop.h"
#include "global.h"
#include "message.h"
#include "viewlist.h"
#include "shape.h"

Desktop::Desktop (Rect *prect)
        :MultiView (prect, NULL, NULL, FALSE, FALSE)
{
    clientPort = new ViewPort (&clientRect, viewPort);
    draw ();
}

//
// quit generates "quit" message and quits the message queue.
//
BOOL Desktop::quit (int errorlevel)
{
    if (close() == FALSE) return (FALSE);
    msgqueue->put (this, NULL, NULL, MSG_Quit, NULL, NULL, errorlevel);
    return (TRUE);
}

BOOL Desktop::isEmpty ()
{
    return (subviewList->isEmpty ());
}

//
// you can change this function to "draw" your desktop!!!
//
void Desktop::selfDraw (Port *port, Rect *area)
{
    port->setPenColor (PPC_Gray);
    port->draw (&FilledRectangle (area));
}

