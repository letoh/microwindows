//
// Micro Windows Implementation
// idler.h: the Idler
//
// $Revision: 1.1 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/idler.h $
// $Date: 1993/10/03 03:44:49 $
//

#ifndef __idler_h
#define __idler_h

#ifndef __controlr_h
#   include "controlr.h"
#endif

class Idler : public Controller
{
public:
    void apply (View *applier);
    void remove (View *applier);

    virtual void pool (MessageQueue *queue);

protected:
    List applierList;
};

#endif

