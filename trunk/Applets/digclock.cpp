//
// Micro Windows Implementation
// digclock.cpp
//
// $Revision: 1.2 $
// $Source: D:/DENG/PRG/MWINDOWS/APP/rcs/digclock.cpp $
// $Date: 1993/11/14 05:43:42 $
//

#include "digclock.h"
#include "shape.h"
#include <time.h>

DigitalClock::DigitalClock (View *parent, Point *spot)
{
    Rect clkRect;
    Point tmp;
    if (spot == NULL) parent->getRandomAttachPoint (&tmp); else tmp = *spot;
    clkRect.set (&tmp, &tmp);
    clkRect.resize (120, 60);

    clockWindow = new Window ("Clock", &clkRect, parent, this, TRUE, TRUE);
    timer->apply (clockWindow, 500);
}

void DigitalClock::fromController (View *from, Controller *ctrl)
{
    char buf[30];
    time_t t = time (NULL);
    struct tm *tb = localtime (&t);
    sprintf (buf, "%02d:%02d:%02d", tb->tm_hour, tb->tm_min, tb->tm_sec);

    Rect clkRect;
    from->getClientRect (&clkRect);
    clkRect.move (- clkRect.x1, - clkRect.y1);

    Port *port = from->getClientPort ();
    port->setPenColor (PPC_White);
    port->setBgColor (PPC_Gray);
    port->draw (&Text (buf, &clkRect));
}

BOOL DigitalClock::close (View *from)
{
    timer->remove (clockWindow);
    return (TRUE);
}

