//
// Micro Windows Implementation
// stackque.cpp: stack and queue implementations
//
// $Revision: 1.1 $
// $Source: P:/MWINDOWS/LIB/rcs/stackque.cpp $
// $Date: 1993/10/03 03:45:15 $
//

#include "stackque.h"

BOOL Stack::put (Object *obj)
{
    stack.last();
    return (stack.put(obj));
}

Object *Stack::get ()
{
    Object *toreturn = stack.last();
    stack.remove();
    return (toreturn);
}

BOOL Queue::put (Object *obj)
{
    queue.last();
    return (queue.put(obj));
}

Object *Queue::get ()
{
    Object *toreturn = queue.first();
    queue.remove();
    return (toreturn);
}

