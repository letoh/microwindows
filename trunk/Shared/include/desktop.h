//
// Micro Windows Implementation
// desktop.h: The Desktop
//
// GUI 的桌面在前面幾個 base class 一路繼承下來, 已經顯得不需要
// 再添油加醋了, 不壞吧!
//
// $Revision: 1.2 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/desktop.h $
// $Date: 1993/10/03 15:21:53 $
//

#ifndef __desktop_h
#define __desktop_h

#ifndef __multview_h
#   include "multview.h"
#endif

class Desktop : public MultiView
{
public:
    Desktop (Rect *prect);

    BOOL    quit (int errorlevel = 0);
    BOOL    isEmpty ();

protected:
    virtual void    selfDraw (Port *port, Rect *area);
};

#endif

