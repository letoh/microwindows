//
// Micro Windows Implementation
// viewlist.cpp
//
// $Revision: 1.1 $
// $Source: P:/MWINDOWS/LIB/rcs/viewlist.cpp $
// $Date: 1993/10/03 03:45:19 $
//

#include "viewlist.h"

BOOL ViewList::find (View *obj)
{
    View *tmp;

    if (first() == NULL) return (FALSE);
    if ((View*)first() == obj) return (TRUE);

    while ((tmp = (View*)next()) != NULL) if (tmp == obj) return (TRUE);

    return (FALSE);
}

void ViewList::bringTop ()
{
    if (current == head) return;
    if (current == head->next) return;

    ListNode *curPrev = current->prev, *curNext = current->next;

    curPrev->next = curNext;
    curNext->prev = curPrev;

    current->prev = head;
    current->next = head->next;
    head->next->prev = current;
    head->next = current;

    return;
}

