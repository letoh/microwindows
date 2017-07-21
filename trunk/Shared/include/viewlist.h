//
// Micro Windows Implementation
// viewlist.h: The ViewList
//
// 這是用來裝入 View instance 的串列. 用雙向串列的好處是可以
// 清楚地表達 view 和 view 之間的層次關係.
//
// 這個 class 被用在 View class 的 subviewList member 裡.
//
// $Revision: 1.1 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/viewlist.h $
// $Date: 1993/10/03 03:44:52 $
//

#ifndef __viewlist_h
#define __viewlist_h

#ifndef __list_h
#   include "list.h"
#endif

class View;

class ViewList : public List
{
public:
    BOOL    find (View *obj);
    void    bringTop ();
};

#endif

