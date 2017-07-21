//
// Micro Windows Implementation
// viewport.h: Port that specially designed for views
//
// ViewPort 是專門供 View 使用的 Port. 這裡對 draw 這個 method
// 做了些修改, 可以使在知道 clipping rectangle 的情況下減少
// clipping 的需要.
//
// $Revision: 1.1 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/viewport.h $
// $Date: 1993/10/03 03:44:44 $
//

#ifndef __viewport_h
#define __viewport_h

#ifndef __port_h
#   include "port.h"
#endif

class View;

class ViewPort : public Port
{
public:
    ViewPort (Rect *prect, Port *pparent = NULL);

    void getInvisibleArea (FreeArea *receiver);
};

#endif

