//
// Micro Windows Implementation
// digclock.h: A Digital Clock
//
// $Revision: 1.1 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/digclock.h $
// $Date: 1993/10/03 15:21:42 $
//

#ifndef __digclock_h
#define __digclock_h

#include "global.h"
#include "model.h"
#include "window.h"

class DigitalClock : public Model
{
public:
    DigitalClock (View *parent = desktop, Point *spot = NULL);

    virtual void fromController (View *from, Controller *ctrl);
    virtual BOOL close (View *from);

protected:
    Window *clockWindow;
};

#endif

