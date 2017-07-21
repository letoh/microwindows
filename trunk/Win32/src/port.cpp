//
// Micro Windows Implementation
// port.cpp
//
// $Revision: 1.1 $
// $Source: D:/DENG/PRG/MWINDOWS/WIN/LIB/rcs/port.cpp $
// $Date: 1993/10/18 12:32:28 $
//

#include "mwindows.h"
#include "wincntlr.h"
#include "port.h"
#include "lowgdiw.h"
#include "shape.h"
#include "portdepd.h"

Port *Port::activePort = NULL;

Port::Port (Rect *prect, Port *pparent)
{
    parent = pparent;
    portRect = *prect;
    clippingRect = portRect;
    clippingRect.move ( - portRect.x1, - portRect.y1);
    visibleArea.add (&clippingRect);
    color = PPC_Black;
    bgColor = PPC_White;
    mode = PPM_Normal;
    rcode = 0;
    if (parent) parentRC = parent->rcode;
    calcDeviceOrig();
    generateOutputArea();
}

void Port::move (int xo, int yo)
{
    portRect.move (xo, yo);
    calcDeviceOrig();
    generateOutputArea();
    rcode++;
}

void Port::resize (int width, int height)
{
    if (width < 1 || height < 1) return;
//  if (fullVisible() == FALSE) return;
    portRect.resize (width, height);
    clippingRect = portRect;
    clippingRect.move ( - portRect.x1, - portRect.y1);
    visibleArea.purge();
    visibleArea.add (&clippingRect);
    generateOutputArea();
    rcode++;
}

void Port::show (Rect *toshow)
{
    Rect newVisible;
    if (toshow == NULL)
    {
        newVisible = portRect;
        visibleArea.purge();
    }
    else
    {
        if (portRect.intersection (toshow, &newVisible) == FALSE) return;
    }

    newVisible.move (- portRect.x1, - portRect.y1);
    visibleArea.add (&newVisible);

    calcDeviceOrig();
    generateOutputArea();
    rcode++;
}

void Port::hide (Rect *tohide)
{
    Rect newHide;

    if (tohide == NULL)
    {
        visibleArea.purge();
    }
    else
    {
        if (portRect.intersection (tohide, &newHide) == FALSE) return;
        newHide.move (- portRect.x1, - portRect.y1);
        visibleArea.cut (&newHide);
    }

    calcDeviceOrig();
    generateOutputArea();
    rcode++;
}

void Port::draw (Shape *todraw)
{
    FreeArea *oa = &outputArea;

    if (parent)
    {
        if (parentRC != parent->rcode)
        {
            calcDeviceOrig();
            generateOutputArea();
        }
    }

    if (todraw == NULL) return;

    if (Port::activePort != this)
    {
//      SETPORT;
        Port::activePort = this;
    }

    _winGetDC();
    SETPORT;

    FreeAreaBegin(oa)
        SETCLIP(oa_tmp->x1 + deviceOrig.x, oa_tmp->y1 + deviceOrig.y,
                oa_tmp->x2 + deviceOrig.x, oa_tmp->y2 + deviceOrig.y);
        todraw->draw (this);
    FreeAreaEnd

    _winReleaseDC();
}

void Port::calcDeviceOrig()
{
    deviceOrig.set (portRect.x1, portRect.y1);
    if (parent) deviceOrig.move (&(parent->deviceOrig));
    rcode++;
}

void Port::generateOutputArea()
{
    FreeArea tmp;

    outputArea.purge();

    visibleArea.intersection (&clippingRect, &tmp);

    if (parent)
    {
        tmp.move (portRect.x1, portRect.y1);
        (parent->outputArea).intersection (&tmp, &outputArea);
        outputArea.move (-portRect.x1, -portRect.y1);

        parentRC = parent->rcode;
    }
    else
    {
        outputArea.copy (&tmp);
    }

    rcode++;
}

BOOL Port::fullVisible ()
{
    if (outputArea.amount()) return (TRUE);
    return (FALSE);
}

PortPenColor Port::setPenColor (PortPenColor c)
{
    int oldcolor = color;
    color = c;
    if (activePort == this)
    {
        SETPENCOLOR;
    }
    return (oldcolor);
}

PortPenColor Port::setBgColor (PortPenColor c)
{
    int oldcolor = bgColor;
    bgColor = c;
    if (activePort == this)
    {
        SETPENCOLOR;
    }
    return (oldcolor);
}

PortPenMode Port::setPenMode (PortPenMode m)
{
    int oldmode = mode;
    mode = m;
    if (activePort == this)
    {
        SETPENMODE;
    }
    return (oldmode);
}

void Port::getDeviceOrig (Point *receiver)
{
    *receiver = deviceOrig;
}

void Port::getPortRect (Rect *receiver)
{
    *receiver = portRect;
}

int Port::getRCode ()
{
    return (rcode);
}

