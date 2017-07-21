//
// Micro Windows Implementation
// cube.cpp
//
// $Revision: 1.3 $
// $Source: D:/DENG/PRG/MWINDOWS/APP/rcs/cube.cpp $
// $Date: 1993/11/14 05:43:42 $
//

#include "cube.h"
#include "shape.h"

#define ANGLE     5

CubeDemo::CubeDemo (View *parent, Rect *cubeRect)
{
    erase = TRUE;
    delay = 6000;
    delayCounter = delay;

    rho = 10;
    theta = 0;
    phi = 0;
    ox = oy = oz = - 0.5;
    cx = cy = xr = yr = 0;

    Rect r;
    if (cubeRect == NULL) parent->getRandomAttachRect (&r); else r = *cubeRect;
    cubeWindow = new Window ("3D Cube", &r, parent, this, TRUE, TRUE);
    speedUp = new Button ("Faster", &Rect(5, 5, 60, 25), cubeWindow, this);
    speedDown = new Button ("Slower", &Rect(5, 26, 60, 46), cubeWindow, this);

    idler->apply (cubeWindow);
}

BOOL CubeDemo::close (View *from)
{
    idler->remove (cubeWindow);
    return (TRUE);
}

void CubeDemo::draw (View *from, Port *port, Rect *area)
{
    Rect r;
    from->getClientRect (&r);
    r.move (- r.x1, - r.y1);

    port->setPenColor (PPC_Black);
    port->draw (&FilledRectangle (&r));

    erase = TRUE;
    drawcube (port);
}

void CubeDemo::fromView (View *from)
{
    erase = TRUE;
    drawcube (from->getClientPort());

    if (from == speedUp)
    {
        if (delay > 5) delay-=5;
    }
    else if (from == speedDown)
    {
        delay += 5;
    }

    from->draw ();
}


void CubeDemo::fromController (View *from, Controller *ctrl)
{
    double pi = 3.14159;
    double adegree = ANGLE * pi / 180;
    if (erase == TRUE)
    {
        drawcube (from->getClientPort());
        erase = FALSE;
        delayCounter = delay;
    }
    else
    {
        if (delayCounter >= delay)
        {
            theta += adegree;
            phi += adegree;

            if (theta > 2 * pi)
            {
                theta = - 2 * pi;
                phi = - 2 * pi;
            }

            drawcube (from->getClientPort());
        }

        delayCounter --;
        if (!delayCounter) erase = TRUE;
    }
}

void CubeDemo::calc3d (double x, double y, double z)
{
    x += ox;
    y += oy;
    z += oz;
    double xe = -x * s1 + y * c1;
    double ye = -x * c1 * c2 - y*s1*c2 +  z * s2;
    double ze = -x * s2 * c1 - y*s2*s1 - z * c2 + rho;
    xr = (int) (d * xe / ze) + cx;
    yr = cy - (int)(d * ye / ze);
}

void CubeDemo::moveto3d (double x, double y, double z)
{
    calc3d (x, y, z);
    orig = Point (xr, yr);
}

void CubeDemo::lineto3d (Port *p, double x, double y, double z)
{
    calc3d (x, y, z);

    Point dest (xr, yr);
    p->draw (&Line (&orig, &dest));
    orig = dest;
}

void CubeDemo::drawcube (Port *p)
{
    Rect r;
    p->getPortRect (&r);
    if (erase) p->setPenColor (PPC_Black); else p->setPenColor (PPC_White);

    cx = r.width() / 2;
    cy = r.height () / 2;
    d = cx * 6;
    s1 = sin(theta);
    c1 = cos(theta);
    s2 = sin(phi);
    c2 = cos(phi);

    moveto3d (0, 0, 0);
    lineto3d (p, 1, 0, 0);
    lineto3d (p, 1, 1, 0);
    lineto3d (p, 0, 1, 0);
    lineto3d (p, 0, 0, 0);
    moveto3d (0, 0, 1);
    lineto3d (p, 1, 0, 1);
    lineto3d (p, 1, 1, 1);
    lineto3d (p, 0, 1, 1);
    lineto3d (p, 0, 0, 1);

    moveto3d (1, 0, 0);
    lineto3d (p, 1, 0, 1);
    moveto3d (1, 1, 0);
    lineto3d (p, 1, 1, 1);
    moveto3d (0, 1, 0);
    lineto3d (p, 0, 1, 1);
    moveto3d (0, 0, 0);
    lineto3d (p, 0, 0, 1);

    // draw a word "X" on the cube face
    moveto3d (0.3, 0.3, 0);
    lineto3d (p, 0.7, 0.7, 0);
    moveto3d (0.7, 0.3, 0);
    lineto3d (p, 0.3, 0.7, 0);
}

