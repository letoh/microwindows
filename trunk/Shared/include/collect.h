//
// Micro Windows Implementation
// collect.h: Collection and IndexedCollection class
//
// 容器 (Containers/Collections) 是比較傾向 Smalltalk 式的 OOP 程式
// 風格. 我們在這裡用 C++ 做了一份. 由於 Collection 裡所能擺入 (put)
// 或取出 (get) 的物件都是繼承是 Object class, 因此這個 Collection
// 實際上是「異質」的. 異質的 Collection 在 C++ 上會出現什麼問題,
// 我們在第 10 章和第 12 章均已提及.
//
// $Revision: 1.1 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/collect.h $
// $Date: 1993/10/03 03:44:53 $
//

#ifndef __collect_h
#define __collect_h

#ifndef __object_h
#   include "object.h"
#endif

class Collection : public Object
{
public:
    virtual BOOL    put (Object *obj)           { if (obj); return (FALSE); }
    virtual BOOL    hasElement (Object *obj)    { if (obj); return (FALSE); }
    virtual Object  *get ()     { return (NULL); }
    virtual void    purge ()    {}
    virtual BOOL    isEmpty ()  { return (TRUE); }
};

class IndexedCollection : public Collection
{
public:
    virtual Object  *first ()       { return (NULL); }
    virtual Object  *next ()        { return (NULL); }
    virtual Object  *last ()        { return (NULL); }
    virtual Object  *previous ()    { return (NULL); }
    virtual Object  *here ()        { return (NULL); }
};

#endif

