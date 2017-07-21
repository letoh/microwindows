//
// Micro Windows Implementation
// About.h: About Window
//
// $Revision: 1.1 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/about.h $
// $Date: 1993/10/03 15:19:55 $
//

#ifndef __about_h
#define __about_h

#include "model.h"

class AboutApp : public Model
{
public:
    AboutApp ();

    virtual void draw (View *from, Port *port, Rect *area);
};

#endif

