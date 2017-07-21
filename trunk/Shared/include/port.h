//
// Micro Windows Implementation
// port.h: the graphics port
//
// 繪圖「埠」, 用來將圖形類別 (Shape class) 的物件「丟」進去.
//
// $Revision: 1.2 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/port.h $
// $Date: 1993/10/03 15:22:23 $
//

#ifndef __port_h
#define __port_h

#ifndef __gobject_h
#   include "gobject.h"
#endif

#ifndef __freearea_h
#   include "freearea.h"
#endif

class Shape;

typedef int PortPenMode;
typedef int PortPenColor;
const PortPenMode  PPM_Normal = 0, PPM_XOR = 1;
const PortPenColor PPC_Black = 0, PPC_Gray = 7, PPC_DarkGray = 8,
                   PPC_White = 15;

const PortPenColor PPC_DarkBlue = 1, PPC_DarkGreen = 2,
                   PPC_DarkCyan = 3, PPC_DarkRed = 4,
                   PPC_DarkPurple = 5, PPC_Brown = 6,
                   PPC_Blue = 9, PPC_Green = 10,
                   PPC_Cyan = 11, PPC_Red = 12,
                   PPC_Purple = 13, PPC_Yellow = 14;


class Port : public GObject
{
public:
    Port (Rect *prect, Port *pparent = NULL);  // parent's coord

    void move (int xo, int yo);
    void resize (int width, int height);
    void show (Rect *toshow=NULL);      // parent's coord
    void hide (Rect *tohide=NULL);

    void getDeviceOrig (Point *receiver);
    void getPortRect (Rect *receiver);

    virtual void draw (Shape *todraw);

    BOOL fullVisible ();
    PortPenColor setPenColor (PortPenColor c);
    PortPenColor setBgColor (PortPenColor c);
    PortPenMode  setPenMode (PortPenMode m);
    PortPenColor getPenColor ()     { return (color); }
    PortPenColor getBgColor ()      { return (bgColor); }
    PortPenMode  getPenMode ()      { return (mode); }
    int getRCode ();

protected:
    Port *parent;
    Rect portRect;
    Rect clippingRect;
    Point deviceOrig;

    FreeArea visibleArea, outputArea;
    PortPenColor color, bgColor;
    PortPenMode mode;
    int rcode, parentRC;

    static Port *activePort;

    void calcDeviceOrig();
    void generateOutputArea();
};

#endif

