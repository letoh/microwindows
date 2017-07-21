//
// Micro Windows Implementation
// About.cpp: About Window
//
// $Revision: 1.1 $
// $Source: D:/DENG/PRG/MWINDOWS/APP/rcs/about.cpp $
// $Date: 1993/10/03 15:19:04 $
//

#include "about.h"
#include "window.h"
#include "global.h"
#include "shape.h"

AboutApp::AboutApp ()
{
    new Window ("About MicroWindows", &Rect (70, 190, 570, 290),
                desktop, this, TRUE, TRUE);
}

void AboutApp::draw (View *from, Port *port, Rect *area)
{
    port->setPenColor (PPC_Black);
    port->draw (&FilledRectangle (area));

    Rect r, a, b;
    from->getClientRect (&r);
    r.move (- r.x1, - r.y1);
    a.set (0, 0, r.x2, r.height() / 2);
    b.set (0, r.height()/2, r.x2, r.y2);

    port->setPenColor (PPC_White);
    port->setBgColor (PPC_Black);
    port->draw (&Text ("An experiment on portable and object-oriented GUI", &a));
    port->draw (&Text ("(c) 1993-2006 Deng Liu", &b));
}

