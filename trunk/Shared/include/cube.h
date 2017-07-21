//
// Micro Windows Implementation
// cube.h
//
// $Revision: 1.3 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/cube.h $
// $Date: 1993/11/14 05:44:35 $
//

#ifndef __cube_h
#define __cube_h

#include "global.h"
#include "model.h"
#include "window.h"
#include "button.h"
#include <math.h>

class CubeDemo : public Model
{
public:
    CubeDemo (View *parent = desktop, Rect *cubeRect = NULL);

    virtual void draw (View *from, Port *port, Rect *area);
    virtual void fromController (View *from, Controller *ctrl);
    virtual void fromView (View *from);
    virtual BOOL close (View *from);

protected:
    void calc3d (double x, double y, double z);
    void moveto3d (double x, double y, double z);
    void lineto3d (Port *p, double x, double y, double z);
    void drawcube (Port *p);

    BOOL erase;
    int delay;
    int delayCounter;
    double rho, theta, phi, d,s1, s2, c1, c2, ox, oy, oz;
    int cx, cy;
    int xr, yr;
    Point orig;

    Window *cubeWindow;
    Button *speedUp, *speedDown;
};

#endif

