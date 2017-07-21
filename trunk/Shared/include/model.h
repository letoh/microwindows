//
// Micro Windows Implementation
// model.h: the Model class
//
// MVC 裡 Model 就是應用程式, 要使用 Micro Windows, 必先繼承
// 此 class. 這個 class 接收來自 view 的訊息.
//
// $Revision: 1.1 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/model.h $
// $Date: 1993/10/03 03:44:50 $
//

#ifndef __model_h
#define __model_h

#ifndef __gobject_h
#   include "gobject.h"
#endif

class Controller;
class View;
class Port;

class Model : public Object
{
public:
    virtual void draw (View *from, Port *port, Rect *area);
    virtual void leftButtonDown (View *from, Point *pnt);
    virtual void leftButtonUp (View *from, Point *pnt);
    virtual void mouseMove (View *from, Point *pnt);
    virtual void keyCharIn (View *from, BYTE c);
    virtual void fromView (View *from);
    virtual void fromController (View *from, Controller *ctrl);

    virtual BOOL close (View *from);
};

#endif

