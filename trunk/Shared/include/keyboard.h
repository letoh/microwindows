//
// %%name
// keyboard.h: The Keyboard Controller
//
// $Revision: 1.1 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/keyboard.h $
// $Date: 1993/11/14 05:44:35 $
//

#ifndef __keyboard_h
#define __keyboard_h

#ifndef __controlr_h
#   include "controlr.h"
#endif

const MsgType MSG_AsciiChar = 1,
              MSG_VirtualKey = 2;   // see vkey.h (os dependent module)
                                    // for the detail

class Keyboard : public Controller
{
public:
    virtual void pool (MessageQueue *queue);
};

#endif

