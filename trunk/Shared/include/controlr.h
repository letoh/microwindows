//
// Micro Windows Implementation
// controlr.h: the Controller class
//
// 在 MVC 架構裡, Controller 負責的是產生訊息, 我們用 pool 這個 method
// 來做到這個動作. 在訊息迴路裡, 只要呼叫 Controller::pool ,
// 不必知道實際上目前在 "pool" 的到底是那個 Controller (如 mouse),
// 這是繼承和 dynamic-binding 的一大優點.
//
// $Revision: 1.1 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/controlr.h $
// $Date: 1993/10/03 03:44:51 $
//

#ifndef __controlr_h
#define __controlr_h

#ifndef __message_h
#   include "message.h"
#endif

class Controller : public Object
{
public:
    virtual void pool (MessageQueue *queue)
    {
        if (queue) ;        // to avoid warnings from the compiler
    }
};

#endif

