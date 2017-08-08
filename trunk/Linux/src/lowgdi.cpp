/*****************************************************************************
 *
 * Lowgdi.c: 低階繪圖介面
 *
 ****************************************************************************/

#define NEED(fmt, ...) \
    fprintf(stderr, "NeedImpl: %s:%d: " fmt "\n", __func__, __LINE__, ## __VA_ARGS__)

#include "lowgdi.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>

Display *dpy;
int screen;
unsigned long white, black;
Window topwin;
GC gc;
Colormap colormap;

int _winScrWidth = 640;
int _winScrHeight = 480;
int _winButtonState = LGM_MOVE;
int _winMouseX;
int _winMouseY;


/* global variables */
int     _lgClipX1 = 0, _lgClipY1 = 0, _lgClipX2 = 639, _lgClipY2 = 479;
BOOL    _lgClipOut = FALSE;
int     _lgRX1 = 0, _lgRY1 = 0, _lgRX2 = 0, _lgRY2 = 0;
int     _lgColor = 15, _lgBgColor = 0;
BOOL    _lgXOR = FALSE;
BOOL    _lgMouseHide = FALSE;

#define _lgClipAccept(a, b)     (a|b)
#define _lgClipReject(a, b)     (a&b)
#define _lgClipTop              0x08
#define _lgClipBottom           0x04
#define _lgClipRight            0x02
#define _lgClipLeft             0x01
#define _lgClipInside           0x00

void _lgLineClipper(int x1, int y1, int x2, int y2);
void _lgRectClipper(int x1, int y1, int x2, int y2);
int  _lgClipOutcode(int x , int y);

XColor colors[16];

BOOL    _lgOpenGraphics()
{
    dpy = XOpenDisplay(NULL);
    if (NULL == dpy) return FALSE;
    screen = DefaultScreen(dpy);
    white = WhitePixel(dpy, screen);
    black = BlackPixel(dpy, screen);

    topwin = XCreateSimpleWindow(dpy, RootWindow(dpy, screen),
                                 0, 0, _winScrWidth, _winScrHeight,
                                 0, black, white);
    XSetStandardProperties(dpy, topwin, "MicroWindow (Demo)", "MicroWindow", None, NULL, 0, NULL);
    XSelectInput(dpy, topwin, ExposureMask|PointerMotionMask|ButtonPressMask|ButtonReleaseMask);
    gc = XCreateGC(dpy, topwin, 0, NULL);

    colormap = DefaultColormap(dpy, screen);
    XParseColor(dpy, colormap, "rgb:00/00/00", colors);     // PPC_Black
    XParseColor(dpy, colormap, "rgb:00/00/80", colors+1);   // PPC_DarkBlue
    XParseColor(dpy, colormap, "rgb:00/80/00", colors+2);   // PPC_DarkGreen
    XParseColor(dpy, colormap, "rgb:00/80/80", colors+3);   // PPC_DarkCyan
    XParseColor(dpy, colormap, "rgb:80/00/00", colors+4);   // PPC_DarkRed
    XParseColor(dpy, colormap, "rgb:80/00/80", colors+5);   // PPC_DarkPurple
    XParseColor(dpy, colormap, "rgb:80/80/00", colors+6);   // PPC_Brown
    XParseColor(dpy, colormap, "rgb:c0/c0/c0", colors+7);   // PPC_Gray
    XParseColor(dpy, colormap, "rgb:80/80/80", colors+8);   // PPC_DarkGray
    XParseColor(dpy, colormap, "rgb:00/00/ff", colors+9);   // PPC_Blue
    XParseColor(dpy, colormap, "rgb:00/ff/00", colors+10);  // PPC_Green
    XParseColor(dpy, colormap, "rgb:00/ff/ff", colors+11);  // PPC_Cyan
    XParseColor(dpy, colormap, "rgb:ff/00/00", colors+12);  // PPC_Red
    XParseColor(dpy, colormap, "rgb:ff/00/ff", colors+13);  // PPC_Purple
    XParseColor(dpy, colormap, "rgb:ff/ff/00", colors+14);  // PPC_Yellow
    XParseColor(dpy, colormap, "rgb:ff/ff/ff", colors+15);  // PPC_White
    for (int i = 0; i < 16; ++i)
        XAllocColor(dpy, colormap, &colors[i]);

    _lgSetColor(_lgColor);
    _lgSetBgColor(_lgBgColor);

    XMapWindow(dpy, topwin);
    for (XEvent e;;) {
        XNextEvent(dpy, &e);
        if (e.type == Expose) break;
    }
    return (TRUE);
}

BOOL    _lgCloseGraphics()
{
    for (int i = 0; i < 16; ++i)
        XFreeColors(dpy, colormap, &colors[i].pixel, 1, 0);
    XFreeGC(dpy, gc);
    XDestroyWindow(dpy, topwin);
    return (TRUE);
}

void    _lgRect     (int x1, int y1, int x2, int y2)
{
    XDrawRectangle(dpy, topwin, gc, x1, y1, x2 - x1 + 1, y2 - y1 + 1);
}

void    _lgPoint    (int x, int y)
{
    XDrawPoint(dpy, topwin, gc, x, y);
}

void    _lgLine     (int x1, int y1, int x2, int y2)
{
    XDrawLine(dpy, topwin, gc, x1, y1, x2, y2);
}

void    _lgSolidRect(int x1, int y1, int x2, int y2)
{
    XFillRectangle(dpy, topwin, gc, x1, y1, x2 - x1 + 1, y2 - y1 + 1);
}

void _lgDrawText (int x, int y, char *text)
{
    XDrawString(dpy, topwin, gc, x, y, text, strlen(text));
}

int     _lgGetScreenWidth()
{
    return _winScrWidth;
}

int     _lgGetScreenHeight()
{
    return _winScrHeight;
}

int     _lgGetColor ()
{
    return (_lgColor);
}

int     _lgGetBgColor ()
{
    return (_lgBgColor);
}

BOOL    _lgGetXOR ()
{
    return (_lgXOR);
}

BOOL _lgIsMouseActive()
{
    return (TRUE);
}

void    _lgSetColor (int c)
{
    _lgColor = c % 16;
    XSetForeground(dpy, gc, colors[_lgColor].pixel);
}

void    _lgSetBgColor (int c)
{
    _lgBgColor = c % 16;
    XSetBackground(dpy, gc, colors[_lgBgColor].pixel);
}

void    _lgSetXOR (BOOL xor_)
{
    _lgXOR = xor_;

    if (xor_ == TRUE)
    {
        XSetFunction(dpy, gc, GXxor);
    }
    else
    {
        XSetFunction(dpy, gc, GXcopy);
    }
}

void    _lgResetClipping()
{
    // _lgSetClippingRect (0, 0, _winScrWidth - 1, _winScrHeight -1);
    XSetClipMask(dpy, gc, None);
}

void    _lgSetClippingRect (int x1, int y1, int x2, int y2)
{
    XRectangle rects[] = {{ .x = x1, .y = y1, .width = x2 - x1 + 1, .height = y2 - y1 + 1 }};
    XSetClipRectangles(dpy, gc, 0, 0, rects, 1, Unsorted);
}

BYTE _lgGetMouseButton ()
{
    // LGM_MOVE, LGM_BUTTONDOWN, LGM_BUTTONUP, LGM_DRAG
    return _winButtonState;
}

int  _lgGetMouseX ()
{
    return (_winMouseX);
}

int  _lgGetMouseY ()
{
    return (_winMouseY);
}

int _lgTextWidth  (char *text)
{
    return 6*strlen(text);
}

int _lgTextHeight (char *text)
{
    return -9;
}

void _lgHideMouse ()
{
    if (_lgMouseHide == TRUE) return;
    _lgMouseHide = TRUE;
}

void _lgShowMouse ()
{
    if (_lgMouseHide == FALSE) return;
    _lgMouseHide = FALSE;
}

BOOL _lgIsMouseHide (void)
{
    return (_lgMouseHide);
}

void _lgLineClipper(int x1, int y1, int x2, int y2)
{
    int  tmp;
    int  outcode1=_lgClipOutcode(x1, y1), outcode2=_lgClipOutcode(x2, y2);
    int  swap=0;
    long buf, dx, dy;

    _lgClipOut=1;

    while (_lgClipAccept(outcode1, outcode2))
    {
        if (_lgClipReject (outcode1, outcode2)) break;
        if (outcode1==_lgClipInside)
        {
            tmp=x2; x2=x1; x1=tmp;
            tmp=y2; y2=y1; y1=tmp;
            tmp=outcode2; outcode2=outcode1; outcode1=tmp;
            swap=1;
        }

        dx=x2-x1;
        dy=y2-y1;

        if (outcode1 & _lgClipTop)
        {
            if (dy) buf=dx*(long)(_lgClipY1-y1)/dy;
            x1+=buf;
            y1 =_lgClipY1;
        }
        else if (outcode1 & _lgClipBottom)
        {
            if (dy) buf=dx*(long)(_lgClipY2-y1)/dy;
            x1+=buf;
            y1 =_lgClipY2;
        }
        else if (outcode1 & _lgClipRight)
        {
            if (dx) buf=dy*(long)(_lgClipX2-x1)/dx;
            y1+=buf;
            x1 =_lgClipX2;
        }
        else if (outcode1 & _lgClipLeft)
        {
            if (dx) buf=dy*(long)(_lgClipX1-x1)/dx;
            y1+=buf;
            x1 =_lgClipX1;
        }
        outcode1=_lgClipOutcode(x1, y1);
    }

    if (!_lgClipAccept(outcode1, outcode2))
    {
        if (swap)
        {
            _lgRX1=x2;
            _lgRY1=y2;
            _lgRX2=x1;
            _lgRY2=y1;
        }
        else
        {
            _lgRX1=x1;
            _lgRY1=y1;
            _lgRX2=x2;
            _lgRY2=y2;
        }
        _lgClipOut=0;   /* still a drawable line */
    }
}

void _lgRectClipper(int x1, int y1, int x2, int y2)
{
    int tmp;
    int outcode1=_lgClipOutcode(x1, y1), outcode2=_lgClipOutcode(x2, y2);
    int x3, y3, x4, y4;

    _lgClipOut=1;
    while (_lgClipAccept(outcode1, outcode2))
    {
        if (_lgClipReject (outcode1, outcode2)) break;
        if (outcode1==_lgClipInside)
        {
            tmp=x2; x2=x1; x1=tmp;
            tmp=y2; y2=y1; y1=tmp;
            tmp=outcode2; outcode2=outcode1; outcode1=tmp;
        }

        if (outcode1 & _lgClipTop)
        {
            y1 =_lgClipY1;
        }
        else if (outcode1 & _lgClipBottom)
        {
            y1 =_lgClipY2;
        }
        else if (outcode1 & _lgClipRight)
        {
            x1 =_lgClipX2;
        }
        else if (outcode1 & _lgClipLeft)
        {
            x1 =_lgClipX1;
        }
        outcode1=_lgClipOutcode(x1, y1);
    }

    if (!_lgClipAccept(outcode1, outcode2))
    {
        x3=x1; x4=x2; y3=y1; y4=y2;
        if (x1>x2)
        {
            x3=x2;
            x4=x1;
        }
        if (y1>y2)
        {
            y3=y2;
            y4=y1;
        }

        _lgRX1=x3;
        _lgRY1=y3;
        _lgRX2=x4;
        _lgRY2=y4;
        _lgClipOut=0;
    }
}

/**********************************************************
 *
 *      1001 | 1000 | 1010
 *     ------+------+------
 *      0001 | 0000 | 0010      Cohen-Sutherland's outcode
 *     ------+------+------
 *      0101 | 0100 | 0110
 *
 *********************************************************/

int  _lgClipOutcode(int x , int y)
{
    int out;
    if (y<_lgClipY1) out=_lgClipTop;
    else if (y>_lgClipY2) out=_lgClipBottom;
    else out=_lgClipInside;

    if (x<_lgClipX1) out|=_lgClipLeft;
    else if (x>_lgClipX2) out|=_lgClipRight;

    return (out);
}

