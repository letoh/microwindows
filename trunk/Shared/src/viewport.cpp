//
// Micro Windows Implementation
// viewport.cpp
//
// $Revision: 1.2 $
// $Source: P:/MWINDOWS/LIB/rcs/viewport.cpp $
// $Date: 1993/10/18 12:33:19 $
//

#include "viewport.h"

ViewPort::ViewPort (Rect *prect, Port *pparent)
         :Port (prect, pparent)
{
}

void ViewPort::getInvisibleArea (FreeArea *receiver)
{
    Rect fullRect = portRect;
    fullRect.move (- portRect.x1, - portRect.y1);

    receiver->add (&fullRect);
    receiver->cut (&visibleArea);
    receiver->move (portRect.x1, portRect.y1);
}

