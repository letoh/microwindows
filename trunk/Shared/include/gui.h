//
// Micro Windows Implementation
// gui.h: the application driver
//
// 應用程視需要載入 Driver class 才能「啟動」整個 GUI.
// Driver::run 是不折不扣的訊息迴路.
//
// $Revision: 1.1 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/gui.h $
// $Date: 1993/10/03 03:44:52 $
//

#ifndef __gui_h
#define __gui_h

#include "port.h"
#include "shape.h"
#include "view.h"
#include "global.h"
#include "model.h"

class Driver : public Object
{
public:
    Driver ();
    ~Driver ();

    int run ();

protected:
    void poolControllers ();
    void initControllers ();
    void deleteControllers ();

    BOOL mainDriver;
};

#endif

