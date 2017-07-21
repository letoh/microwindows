//
// Micro Windows Implementation
// list.cpp: list implementation
//
// $Revision: 1.1 $
// $Source: P:/MWINDOWS/LIB/rcs/list.cpp $
// $Date: 1993/10/03 03:45:14 $
//
#include "list.h"

List::List ()
{
    head = new ListNode;
    tail = new ListNode;

    head->prev = head;
    head->next = tail;

    tail->prev = head;
    tail->next = tail;

    current = head;
}

List::~List ()
{
    purge();
    delete head;
    delete tail;
}

BOOL List::put (Object *obj)
{
    ListNode *newNode = new ListNode (obj, current, current->next);

    current->next->prev = newNode;
    current->next = newNode;
    current = newNode;

    return (TRUE);
}

BOOL List::hasElement (Object *obj)
{
    ListNode *n = head;

    while ((n = n->next) != tail) if (n == obj) return (TRUE);
    return (FALSE);
}

void List::purge ()
{
    ListNode *n = head->next, *b;

    while (1)
    {
        if (n == tail) break;
        b = n->next;
        delete n;
        n = b;
    }

    head->next = tail;
    tail->prev = head;
}

BOOL List::isEmpty ()
{
    if (head->next == tail) return (TRUE);
    return (FALSE);
}

Object *List::first ()
{
    if ((current = head->next) == tail) current = head;
    return (current->obj);
}

Object *List::next ()
{
    if (current->next == tail) return (NULL);
    current = current->next;
    return (current->obj);
}

Object *List::last ()
{
    current = tail->prev;
    return (current->obj);
}

Object *List::previous ()
{
    if (current->prev == head) return (NULL);
    current = current->prev;
    return (current->obj);
}

BOOL List::remove ()
{
    if (current == head) return (FALSE);

    current->prev->next = current->next;
    current->next->prev = current->prev;

    ListNode *cp = current->prev;

    delete current;
    current = cp;
    if (current == head) first();

    return (TRUE);
}

