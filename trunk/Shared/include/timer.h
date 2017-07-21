//
// Micro Windows Implementation
// timer.h: the Timer Controller
//
// NOTE: this module is COMPILER and MACHINE DEPENDENT.
//
// $Revision: 1.1 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/timer.h $
// $Date: 1993/10/03 03:44:50 $
//

#ifndef __timer_h
#define __timer_h

#ifndef __controlr_h
#   include "controlr.h"
#endif

class Timer : public Controller
{
public:
    Timer ();
    ~Timer();

    void apply (View *applier, unsigned int msec = 1000);
    void remove (View *applier);

    virtual void pool (MessageQueue *queue);

protected:
    BOOL findApplier (View *applier);
    List applierList;
};

#endif

