//
// Micro Windows Implementation
// timer.h: the Timer Controller
//
// NOTE: this module is COMPILER and MACHINE DEPENDENT.
//
// $Revision: 1.2 $
// $Source: P:/MWINDOWS/LIB/rcs/timer.cpp $
// $Date: 1993/11/27 09:53:32 $
//

#include <dos.h>
#include "timer.h"
#include "global.h"

static BOOL timer_on;
static void interrupt (*timer_oldtimer)(...) = NULL;
static void interrupt timer_newtimer (...);

struct TimerObject : public Object
{
    TimerObject (View *apr)
    {
        applier = apr;
        counter = tick = amount = 0;
    }

    View *applier;

    unsigned int counter, tick, amount;
};


Timer::Timer ()
{
    if (timer_oldtimer) return;     // Int 1ch handler is occupied

    timer_oldtimer = getvect (0x1c);
    setvect (0x1c, timer_newtimer);
}

Timer::~Timer()
{
    setvect (0x1c, timer_oldtimer);

    Object *obj = applierList.first ();

    do
    {
        if (obj == NULL) break;
        delete obj;
        obj = applierList.next ();
    } while (1);
}

void Timer::apply (View *applier, unsigned int msec)
{
    if (findApplier (applier) == TRUE) return;

    TimerObject *newobj = new TimerObject (applier);

    // calc the tick and amount of the timer object
    unsigned long ltick = (msec * 18) / 1000;

    if (!ltick) newobj->tick = 1; else newobj->tick = (unsigned int)ltick;

    unsigned long lamount = 1000 / (msec * 18);
    if (!ltick)
    {
        if (!lamount) newobj->amount = 1;
        else newobj->amount = (unsigned int)lamount;
    }
    else newobj->amount = 1;

    newobj->counter = newobj->tick;

    applierList.put (newobj);
}

void Timer::remove (View *applier)
{
    if (findApplier (applier) == FALSE) return;

    TimerObject *obj = (TimerObject*)applierList.here ();
    applierList.remove ();

    // remove all messages that sent by me to the applier,
    // otherwise, those undispatched messages will be
    // sent to the destructed one... :Q
    //
    msgqueue->removeBy (this, obj->applier);

    delete obj;
}

void Timer::pool (MessageQueue *queue)
{
    // if Int 1ch (i.e. the timer) is triggered, send timer messages
    // to ALL the appliers.
    //
    if (timer_on == FALSE) return;
    timer_on = FALSE;

    TimerObject *obj = (TimerObject*)applierList.first();

    do
    {
        if (obj == NULL) break;

        obj->counter --;

        if (!obj->counter)
        {
            for (int i = 0; i < obj->amount; i++)
            {
                queue->put (NULL, obj->applier, this);
            }

            obj->counter = obj->tick;
        }

        obj = (TimerObject*)applierList.next();
    } while (1);
}

BOOL Timer::findApplier (View *applier)
{
    TimerObject *obj;
    obj = (TimerObject*)applierList.first ();

    do
    {
        if (obj == NULL) break;
        if (obj->applier == applier) return (TRUE);
        obj = (TimerObject*)applierList.next();
    } while (1);

    return (FALSE);
}

static void interrupt timer_newtimer (...)
{
    timer_oldtimer (...);
    timer_on = TRUE;
}

