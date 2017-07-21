//
// Micro Windows Implementation
// global.h: global objects
//
// 所有共用的物件均在此宣告.
//
// $Revision: 1.1 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/global.h $
// $Date: 1993/10/03 03:44:52 $
//

#ifndef __global_h
#define __global_h

#ifndef __mouse_h
#   include "mouse.h"
#endif

#ifndef __timer_h
#   include "timer.h"
#endif

#ifndef __idler_h
#   include "idler.h"
#endif

#ifndef __message_h
#   include "message.h"
#endif

#ifndef __desktop_h
#   include "desktop.h"
#endif

extern MessageQueue *msgqueue;
extern Mouse        *mouse;
extern Timer        *timer;
extern Idler        *idler;
extern Desktop      *desktop;

#endif

