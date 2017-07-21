/*****************************************************************************
 *
 * Lowgdi.c: 低階繪圖介面
 *
 ****************************************************************************/

#include <windows.h>
#include "lowgdiw.h"
#include "wincntlr.h"

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

HPEN    _lgColorPen[16];
HBRUSH  _lgColorBrush[16];
COLORREF _lgColorIndex[16] = {
    RGB(0, 0, 0), RGB(0, 0, 128), RGB(0, 128, 0), RGB(0, 128, 128),
    RGB(128, 0, 0), RGB(128, 0, 128), RGB(128, 128, 0), RGB(192, 192, 192),
    RGB(128, 128, 128), RGB(0, 0, 255), RGB(0, 255, 0), RGB(0, 255, 255),
    RGB(255, 0, 0), RGB(255, 0, 255), RGB(255, 255, 0), RGB(255, 255, 255)};

#define createPenBrush(cell, color)     \
    _lgColorPen[cell] = CreatePen(PS_SOLID, 1, color); \
    _lgColorBrush[cell] = CreateSolidBrush(color);


BOOL    _lgOpenGraphics()
{
    for (int x = 0; x < 16; x++)
    {
        createPenBrush (x, _lgColorIndex[x]);
    }

    return (TRUE);
}

BOOL    _lgCloseGraphics()
{
    for (int x = 0; x < 16; x ++)
    {
        DeleteObject (_lgColorPen[x]);
        DeleteObject (_lgColorBrush[x]);
    }
    return (TRUE);
}

void    _lgRect     (int x1, int y1, int x2, int y2)
{
    if (!_winUsingDC) return;
    MoveTo (_winDC, x1, y1);
    LineTo (_winDC, x2, y1);
    LineTo (_winDC, x2, y2);
    LineTo (_winDC, x1, y2);
    LineTo (_winDC, x1, y1);
}

void    _lgPoint    (int x, int y)
{
    if (!_winUsingDC) return;
//  if (x < _lgClipX1 || x > _lgClipX2 ||
//      y < _lgClipY1 || y > _lgClipY2) return;
    SetPixel (_winDC, x, y, _lgColorPen[_lgColor]);
}

void    _lgLine     (int x1, int y1, int x2, int y2)
{
    if (!_winUsingDC) return;
//  _lgLineClipper (x1, y1, x2, y2);
//  MoveTo (_winDC, _lgRX1, _lgRY1);
//  LineTo (_winDC, _lgRX2, _lgRY2);
    MoveTo (_winDC, x1, y1);
    LineTo (_winDC, x2, y2);
}

void    _lgSolidRect(int x1, int y1, int x2, int y2)
{
    if (!_winUsingDC) return;
//  _lgRectClipper (x1, y1, x2, y2);
//  Rectangle (_winDC, _lgRX1, _lgRY1, _lgRX2, _lgRY2);
    Rectangle (_winDC, x1, y1, x2 + 1, y2 + 1);
}

void _lgDrawText (int x, int y, char *text)
{
    if (!_winUsingDC) return;
    SelectObject (_winDC, GetStockObject (SYSTEM_FIXED_FONT));
    TextOut (_winDC, x, y, text, strlen(text));
}

int     _lgGetScreenWidth()
{
    return (_winScrWidth);
}

int     _lgGetScreenHeight()
{
    return (_winScrHeight);
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
    if (_winUsingDC)
    {
        SelectObject (_winDC, _lgColorPen[_lgColor]);
        SelectObject (_winDC, _lgColorBrush[_lgColor]);
        SetTextColor (_winDC, _lgColorIndex[_lgColor]);
    }
}

void    _lgSetBgColor (int c)
{
    _lgBgColor = c % 16;
    if (_winUsingDC) SetBkColor (_winDC, _lgColorIndex[_lgBgColor]);
}

void    _lgSetXOR (BOOL xor)
{
    if (!_winUsingDC) return;

    _lgXOR = xor;

    if (xor == TRUE)
    {
        SetROP2 (_winDC, R2_XORPEN);
    }
    else
    {
        SetROP2 (_winDC, R2_COPYPEN);
    }
}

void    _lgResetClipping()
{
//  _lgClipX1 = 0;
//  _lgClipY1 = 0;
//  _lgClipX2 = _winScrWidth - 1;
//  _lgClipY2 = _winScrHeight - 1;
    _lgSetClippingRect (0, 0, _winScrWidth - 1, _winScrHeight -1);
}

void    _lgSetClippingRect (int x1, int y1, int x2, int y2)
{
//  _lgClipX1 = x1;
//  _lgClipY1 = y1;
//  _lgClipX2 = x2;
//  _lgClipY2 = y2;
    if (!_winUsingDC) return;
    HRGN r= CreateRectRgn (x1, y1, x2 + 1, y2 + 1);
    SelectClipRgn (_winDC, r);
    DeleteObject (r);
}

BYTE _lgGetMouseButton ()
{
    BYTE btn = _winMouseButton;
    if (btn == LGM_BUTTONUP) _winMouseButton = 0;
    return (btn);
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
    if (!_winUsingDC) return (0);
    SelectObject (_winDC, GetStockObject (SYSTEM_FIXED_FONT));
    DWORD ext = GetTextExtent (_winDC, text, strlen(text));
    return (LOWORD(ext));
}

int _lgTextHeight (char *text)
{
    if (!_winUsingDC) return (0);
    SelectObject (_winDC, GetStockObject (SYSTEM_FIXED_FONT));
    DWORD ext = GetTextExtent (_winDC, text, strlen(text));
    return (HIWORD(ext));
}

void _lgHideMouse ()
{
    if (_lgMouseHide == TRUE) return;
    ShowCursor (FALSE);
    _lgMouseHide = TRUE;
}

void _lgShowMouse ()
{
    if (_lgMouseHide == FALSE) return;
    ShowCursor (TRUE);
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
    int  x3, y3, x4, y4, swap=0;
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

