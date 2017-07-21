//
// Micro Windows Implementation
// stackque.h: stack and queue
//
// 堆疊和佇列, 另一種常見的 ADT.
//
// $Revision: 1.1 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/stackque.h $
// $Date: 1993/10/03 03:44:52 $
//
#ifndef __stackque_h
#define __stackque_h

#ifndef __list_h
#   include "list.h"
#endif


class Stack : public Collection
{
public:
    BOOL    put (Object *obj);
    BOOL    hasElement (Object *obj)    { return (stack.hasElement(obj)); }
    Object  *get ();
    void    purge ()    { stack.purge(); }
    BOOL    isEmpty ()  { return (stack.isEmpty()); }

protected:
    List    stack;
};

class Queue : public Collection
{
public:
    BOOL    put (Object *obj);
    BOOL    hasElement (Object *obj)    { return (queue.hasElement(obj)); }
    Object  *get ();
    void    purge ()    { queue.purge(); }
    BOOL    isEmpty ()  { return (queue.isEmpty()); }

protected:
    List    queue;
};

#endif

