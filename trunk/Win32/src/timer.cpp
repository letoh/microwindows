//
// Micro Windows Implementation
// timer.h: the Timer Controller
//
// NOTE: this module is COMPILER and MACHINE DEPENDENT.
//
// $Revision: 1.1 $
// $Source: D:/DENG/PRG/MWINDOWS/WIN/LIB/rcs/timer.cpp $
// $Date: 1993/10/18 12:32:28 $
//

#include "global.h"
#include "timer.h"

BOOL _winGetTimerFlag ();
void _winSetTimerFlag (BOOL f);

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
}

Timer::~Timer()
{
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
    unsigned long ltick = (msec * 20) / 1000;

    if (!ltick) newobj->tick = 1; else newobj->tick = (unsigned int)ltick;

    unsigned long lamount = 1000 / (msec * 20);
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
    if (_winGetTimerFlag() == FALSE) return;
    _winSetTimerFlag (FALSE);

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

