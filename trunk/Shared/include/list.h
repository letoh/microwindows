//
// Micro Windows Implementation
// list.h: abstract data type, an dobule-linked list.
//
// 雙向連結串列 (double linked-list) 是一種常用的 ADT. 如果不
// 靠 C++ 提供的 class 和封裝功能, 用 C 來寫可累人的.
//
// $Revision: 1.1 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/list.h $
// $Date: 1993/10/03 03:44:53 $
//
#ifndef __list_h
#define __list_h

#ifndef __collect_h
#   include "collect.h"
#endif

class ListNode : public Object
{
public:
    ListNode (Object *pobj=NULL, ListNode *pprev=NULL, ListNode *pnext=NULL)
    {
        obj = pobj;
        next = pnext;
        prev = pprev;
    }

    Object      *obj;
    ListNode    *next, *prev;
};

class List : public IndexedCollection
{
public:
    List ();
    ~List();
    BOOL    put (Object *obj);
    BOOL    hasElement (Object *obj);
    Object  *get ()         { return (here()); }
    void    purge ();
    BOOL    isEmpty ();
    Object  *first ();
    Object  *next ();
    Object  *last ();
    Object  *previous ();
    Object  *here ()        { return (current->obj); }
    BOOL     remove ();
protected:
    ListNode    *head, *tail, *current;
};

#endif

