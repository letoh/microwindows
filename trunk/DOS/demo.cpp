//
// Micro Windows Implementation
// demo.cpp: a more sophisticated demonstration
//
// $Revision: 1.2 $
// $Source: D:/DENG/PRG/MWINDOWS/rcs/demo.cpp $
// $Date: 1993/10/05 12:50:49 $
//

#include "shape.h"
#include "global.h"
#include "model.h"
#include "button.h"
#include "window.h"
#include "gui.h"
#include "about.h"
#include "calc.h"
#include "cube.h"
#include "digclock.h"
#include "draw.h"

int main ()
{
    Driver gui;
    new DrawApp (desktop, &Rect (40, 50, 500, 440));
    new DigitalClock (desktop, &Point (500, 410));
    new Calculator (desktop, &Point (0, 0));
    new CubeDemo (desktop, &Rect (0, 360, 120, 479));
    new CubeDemo (desktop, &Rect (400, 0, 639, 140));
    new AboutApp;
    return (gui.run());
}

