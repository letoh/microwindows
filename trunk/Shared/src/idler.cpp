//
// Micro Windows Implementation
// idler.h: the Idler
//
// $Revision: 1.2 $
// $Source: P:/MWINDOWS/LIB/rcs/idler.cpp $
// $Date: 1993/11/27 09:53:32 $
//

// when the message queue is empty, idler sends messages
// to its appliers.

#include "idler.h"
#include "global.h"

void Idler::apply (View *applier)
{
    applierList.put (applier);
}

void Idler::remove (View *applier)
{
    View *obj = (View*)applierList.first();

    do
    {
        if (obj == NULL) break;
        if (obj == applier) applierList.remove ();
        obj = (View*)applierList.next();
    } while (1);

    msgqueue->removeBy (this, applier);
}

void Idler::pool (MessageQueue *queue)
{
    if (queue->isEmpty() == FALSE) return;

    View *obj  = (View*)applierList.first ();
    do
    {
        if (obj == NULL) break;
        queue->put (NULL, obj, this);
        obj = (View*)applierList.next ();
    } while (1);
}


