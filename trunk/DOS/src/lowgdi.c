/*****************************************************************************
 *
 * Lowgdi.c: 低階繪圖介面
 *
 ****************************************************************************/

#include <dos.h>
#include "lowgdi.h"
extern char engfont[1920];

/* useful macros */
#define _lgVideoCall(a, b, c, d)    _DX=d; _CX=c; _BX=b; _AX=a; \
                                    geninterrupt(0x10);
#define _lgMouseCall(a, b, c, d)    _DX=d; _CX=c; _BX=b; _AX=a; \
                                    geninterrupt(0x33);
#define _lgOutportb(a, b)           outportb(a, b)
#define _lgAL                       _AL
#define _lgBL                       _BL
#define _lgAX                       _AX
#define _lgBX                       _BX
#define _lgCX                       _CX
#define _lgDX                       _DX

/* global variables */
int     _lgClipX1 = 0, _lgClipY1 = 0, _lgClipX2 = 639, _lgClipY2 = 479;
BOOL    _lgClipOut = FALSE;
int     _lgRX1 = 0, _lgRY1 = 0, _lgRX2 = 0, _lgRY2 = 0;
int     _lgScreenWidth = 640, _lgScreenHeight = 480;
int     _lgColor = LG_WHITE, _lgBgColor = LG_BLACK;
BOOL    _lgXOR = FALSE;
int     _lgYaddr[500];
BYTE far *_lgDBitmap = (BYTE far*)0xa0000000;
BYTE    _lgMask[8]      ={0x80, 0x40, 0x20, 0x10, 8, 4, 2, 1};
BYTE    _lgLeftMask[8]  ={0xff, 0x7f, 0x3f, 0x1f, 0x0f, 0x07, 0x03, 0x01};
BYTE    _lgRightMask[8] ={0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff};
BYTE    _lgBitPlane [8] ={0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
BOOL    _lgMouseActive = TRUE, _lgCursorTempHide = FALSE;
BYTE    _lgMSavedImage[256];
int     _lgMLastX = 0, _lgMLastY = 0, _lgMButton = 0;
BOOL    _lgMLButtonHitted = FALSE;
BOOL    _lgMouseHided = FALSE;

BYTE _lgMScreenMask[32]=
{
    0x3f, 0xff, 0x1f, 0xff, 0x0f, 0xff, 0x07, 0xff, 0x03, 0xff,
    0x01, 0xff, 0x00, 0xff, 0x00, 0x7f, 0x00, 0x3f, 0x00, 0x1f,
    0x01, 0xff, 0x10, 0xff, 0x30, 0xff, 0xf8, 0x7f, 0xf8, 0x7f,
    0xfc, 0x3f
};

BYTE _lgMCursorMask[32]=
{
    0x00, 0x00, 0x40, 0x00, 0x60, 0x00, 0x70, 0x00, 0x78, 0x00,
    0x7c, 0x00, 0x7e, 0x00, 0x7f, 0x00, 0x7f, 0x80, 0x7f, 0xc0,
    0x7c, 0x00, 0x46, 0x00, 0x06, 0x00, 0x03, 0x00, 0x03, 0x00,
    0x00, 0x00
};

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
void _lgHline (int x1, int x2, int y);
void _lgVline (int x, int y1, int y2);
void _lgDisplayCursor (void);
void _lgUpdateMouse (void);
void _lgMouseTempHide (int x1, int y1, int x2, int y2);
void _lgMouseTempShow (void);
void _lgMSaveBack (void);
void _lgMRestoreBack (void);
void _lgLowPoint (int x, int y);

BOOL    _lgOpenGraphics()
{
    int i;

    _lgVideoCall (0x101a, 0xffff, 0, 0);    /* detect if VGA exists */
    if (_lgBX==0xffff) return (FALSE);      /* VGA not exists! */

    _lgVideoCall (0x12, 0, 0, 0);
    for (i=0; i<500; i++) _lgYaddr[i]=80 * i;

    /* set write mode 2 */
    _lgOutportb (0x3ce, 5);
    _lgOutportb (0x3cf, 2);

    /* set pen normal */
    _lgOutportb (0x3ce, 3);
    _lgOutportb (0x3cf, 0);

    /* initial the mouse */
    _lgMouseCall (0, 0, 0, 0);
    if (!_lgAX)
    {
        _lgMouseActive=FALSE;
        return (TRUE);
    }
    
    _lgMouseCall (2, 0, 0, 0);      /* disable the text cursor */
    _lgMouseCall (4, 0, _lgScreenWidth/2, _lgScreenHeight/2);  /* centerize */
    _lgMouseCall (7, 0, 0, _lgScreenWidth-1);   /* set X range */
    _lgMouseCall (8, 0, 0, _lgScreenHeight-1);  /* set Y range */
    _lgMSaveBack();
    _lgDisplayCursor();

    return (TRUE);
}

BOOL    _lgCloseGraphics()
{
    /* return to VGA colored text mode 80x25 */
    _lgVideoCall (0x03, 0, 0, 0);

    if (_lgMouseActive == TRUE)
    {
        _lgMouseCall (0x31, 0, 0, 0);       /* deinitial the mouse */
    }

    return (TRUE);
}

void    _lgRect     (int x1, int y1, int x2, int y2)
{
    _lgLine (x1, y1, x2, y1);
    _lgLine (x1, y2, x2, y2);
    _lgLine (x1, y1, x1, y2);
    _lgLine (x2, y1, x2, y2);
}

void    _lgPoint    (int x, int y)
{
    unsigned addr=_lgYaddr[y] + (x >> 3);
    BYTE mask = _lgMask[x & 7];

    if (x<_lgClipX1 || y<_lgClipY1 || x>_lgClipX2 || y>_lgClipY2) return;

    _lgMouseTempHide (x, y, x, y);
    _lgOutportb (0x3ce, 8);         /* setup latch register */
    _lgOutportb (0x3cf, mask);
    mask=*(_lgDBitmap+addr);
    *(_lgDBitmap+addr)=_lgColor;
    _lgMouseTempShow ();
}

void    _lgLowPoint (int x, int y)
{
    unsigned addr=_lgYaddr[y] + (x >> 3);
    BYTE mask = _lgMask[x & 7];
    _lgOutportb (0x3ce, 8);         /* setup latch register */
    _lgOutportb (0x3cf, mask);
    mask=*(_lgDBitmap+addr);
    *(_lgDBitmap+addr)=_lgColor;
}

void    _lgLine     (int x1, int y1, int x2, int y2)
{
    int dx, dy, i, e, incx, incy, inc1, inc2, x, y, tmp;

    _lgLineClipper (x1, y1, x2, y2);
    if (_lgClipOut) return;
    _lgMouseTempHide (_lgRX1, _lgRY1, _lgRX2, _lgRY2);

    if (_lgRX1==_lgRX2)
    {
        if (_lgRY1 > _lgRY2)
        {
            tmp=_lgRY2;
            _lgRY2=_lgRY1;
            _lgRY1=tmp;
        }

        _lgVline (_lgRX1, _lgRY1, _lgRY2);
    }
    else if (_lgRY1==_lgRY2)
    {
        if (_lgRX1 > _lgRX2)
        {
            tmp=_lgRX2;
            _lgRX2=_lgRX1;
            _lgRX1=tmp;
        }

        _lgHline (_lgRX1, _lgRX2, _lgRY1);
    }
    else
    {

        dx=_lgRX2-_lgRX1;
        dy=_lgRY2-_lgRY1;

        if (dx<0) dx=-dx;
        if (dy<0) dy=-dy;

        if (x2<x1) incx=-1; else incx=1;
        if (y2<y1) incy=-1; else incy=1;

        x=_lgRX1;
        y=_lgRY1;

        if (dx > dy)
        {
            _lgLowPoint (x, y);

            e       = 2* dy - dx;
            inc1    = 2* (dy-dx);
            inc2    = 2* dy;

            for (i=0; i<dx; i++)
            {
                if (e>=0)
                {
                    y+=incy;
                    e+=inc1;
                }
                else e+=inc2;

                x+=incx;
                _lgLowPoint (x, y);
            }
        }
        else
        {
            _lgLowPoint (x, y);
            e       = 2* dx - dy;
            inc1    = 2* (dx-dy);
            inc2    = 2* dx;

            for (i=0; i<dy; i++)
            {
                if (e>=0)
                {
                    x+=incx;
                    e+=inc1;
                }
                else e+=inc2;

                y+=incy;
                _lgLowPoint (x, y);
            }
        }
    }

    _lgMouseTempShow();
}

void    _lgSolidRect(int x1, int y1, int x2, int y2)
{
    int y;

    _lgRectClipper (x1, y1, x2, y2);
    if (_lgClipOut) return;
    _lgMouseTempHide (_lgRX1, _lgRY1, _lgRX2, _lgRY2);

    for (y=_lgRY1; y<=_lgRY2; y++)
    {
        _lgHline (_lgRX1, _lgRX2, y);
    }

    _lgMouseTempShow();
}

void _lgDrawText (int x, int y, char *text)
{
    int i, l = strlen (text), xc = x;
    char *fontptr;

    _lgRectClipper (x, y, x + l*8 - 1, y + 14);
    if (_lgClipOut) return;
    _lgMouseTempHide (_lgRX1, _lgRY1, _lgRX2, _lgRY2);

    for (i = 0; i < l; i++, xc += 8)
    {
        if ((unsigned)*text > 127) continue;      /* high-byte char omitted */
        fontptr = engfont + (*text++) * 15;
        _lgPutFont (fontptr, xc, y, 8, 15);
    }

    _lgMouseTempShow();
}



void _lgHline (int x1, int x2, int y)
{
    register unsigned int i;
    unsigned int addr=_lgYaddr[y], startx, endx;
    static BYTE mask;

    startx=x1 >> 3;
    endx  =x2 >> 3;

    _lgOutportb (0x3ce, 8);

    if (startx==endx)
    {

        if (x2 & 7) mask=_lgLeftMask[x1 & 7] & _lgRightMask[x2 & 7];
        else mask=_lgLeftMask[x1 & 7];
        _lgOutportb (0x3cf, mask);
        addr+=startx;
        mask=*(_lgDBitmap+addr);
        *(_lgDBitmap+addr)=_lgColor;
    }
    else
    {
        /* draw the left side */
        addr+=startx;
        mask=_lgLeftMask[x1 & 7];
        _lgOutportb (0x3cf, mask);

        mask=*(_lgDBitmap+addr);
        *(_lgDBitmap+addr)=_lgColor;

        _lgOutportb (0x3cf, 0xff);
        for (i=startx+1; i<endx; i++)
        {
            addr++;
            mask=*(_lgDBitmap+addr);
            *(_lgDBitmap+addr)=_lgColor;
        }

        /* draw the right side */
        addr++;
        mask=_lgRightMask[x2 & 7];
        _lgOutportb (0x3cf, mask);
        mask=*(_lgDBitmap+addr);
        *(_lgDBitmap+addr)=_lgColor;
    }
}

void _lgVline (int x, int y1, int y2)
{
    register unsigned int i;
    unsigned addr;
    BYTE mask=_lgMask[x & 7], tmp;

    _lgOutportb (0x3ce, 8);
    _lgOutportb (0x3cf, mask);

    for (i=y1; i<=y2; i++)
    {
        addr = _lgYaddr[i] + (x >> 3);
        tmp = *(_lgDBitmap + addr);
        *(_lgDBitmap+addr)=_lgColor;
    }

    if (tmp) ;
}


int     _lgGetScreenWidth()
{
    return (_lgScreenWidth);
}

int     _lgGetScreenHeight()
{
    return (_lgScreenHeight);
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
    return (_lgMouseActive);
}

void    _lgSetColor (int c)
{
    _lgColor = c;
}

void    _lgSetBgColor (int c)
{
    _lgBgColor = c;
}

void    _lgSetXOR (BOOL xor)
{
    _lgXOR = xor;
    if (xor == TRUE)
    {
        _lgOutportb (0x3ce, 3);
        _lgOutportb (0x3cf, 0x18);
    }
    else
    {
        _lgOutportb (0x3ce, 3);
        _lgOutportb (0x3cf, 0);
    }
}

void    _lgResetClipping()
{
    _lgSetClippingRect (0, 0, _lgScreenWidth-1, _lgScreenHeight-1);
}

void    _lgSetClippingRect(int x1, int y1, int x2, int y2)
{
    int x3, y3, x4, y4;

    /* swapping */
    x3=x1<x2 ? x1:x2;
    x4=x1>x2 ? x1:x2;
    y3=y1<y2 ? y1:y2;
    y4=y1>y2 ? y1:y2;

    if (x3 < 0) x3 = 0;
    if (y3 < 0) y3 = 0;
    if (x4 < 0) x4 = 0;
    if (y4 < 0) y4 = 0;

    if (x3 > _lgScreenWidth-1)     x3=_lgScreenWidth-1;
    if (y3 > _lgScreenHeight-1)    y3=_lgScreenHeight-1;
    if (x4 > _lgScreenWidth-1)     x4=_lgScreenWidth-1;
    if (y4 > _lgScreenHeight-1)    y4=_lgScreenHeight-1;

    _lgClipX1=x3;
    _lgClipY1=y3;
    _lgClipX2=x4;
    _lgClipY2=y4;
}

void _lgMSaveBack()
{
    BOOL xor = _lgXOR;
    if (_lgMouseHided || _lgCursorTempHide) return;

    _lgSetXOR (FALSE);
    _lgGetImage (_lgMSavedImage, _lgMLastX, _lgMLastY,
                                 _lgMLastX+15, _lgMLastY+15);
    _lgSetXOR (xor);
}

void _lgMRestoreBack()
{
    BOOL xor = _lgXOR;
    if (_lgMouseHided || _lgCursorTempHide) return;

    _lgSetXOR (FALSE);
    _lgPutImage (_lgMSavedImage, _lgMLastX, _lgMLastY,
                                 _lgMLastX+15, _lgMLastY+15);
    _lgSetXOR (xor);
}

void _lgDisplayCursor ()
{
    int bc=_lgColor, bgc=_lgBgColor;
    int bcx1=_lgClipX1, bcx2=_lgClipX2, bcy1=_lgClipY1, bcy2=_lgClipY2;

    if (_lgMouseHided || _lgCursorTempHide) return;

    _lgResetClipping();

    _lgOutportb (0x3ce, 3);
    _lgOutportb (0x3cf, 0x08);          /* AND pen */
    _lgSetColor (LG_WHITE);
    _lgSetBgColor (LG_BLACK);
    _lgPutFont (_lgMScreenMask, _lgMLastX, _lgMLastY, 16, 16);
    _lgOutportb (0x3ce, 3);
    _lgOutportb (0x3cf, 0x10);          /* OR pen */
    _lgPutFont (_lgMCursorMask, _lgMLastX, _lgMLastY, 16, 16);
    _lgSetColor   (bc);
    _lgSetBgColor (bgc);
    _lgSetXOR (_lgXOR);                 /* restore pen mode */

    _lgClipX1=bcx1; _lgClipY1=bcy1;
    _lgClipX2=bcx2; _lgClipY2=bcy2;
}

BYTE _lgGetMouseButton ()
{
    _lgUpdateMouse();

    if (_lgMLButtonHitted == TRUE && _lgMButton == LGM_BUTTONUP)
    {
        _lgMLButtonHitted = FALSE;
    }

    return (_lgMButton);
}

int  _lgGetMouseX ()
{
    _lgUpdateMouse();
    return (_lgMLastX);
}

int  _lgGetMouseY ()
{
    _lgUpdateMouse();
    return (_lgMLastY);
}

void _lgMouseTempHide(int x1, int y1, int x2, int y2)
{
    int bcx1=_lgClipX1, bcx2=_lgClipX2, bcy1=_lgClipY1, bcy2=_lgClipY2;
    int oca, ocb;
    BOOL hide = FALSE;

    if (_lgMouseActive == FALSE) return;
    if (_lgCursorTempHide || _lgMouseHided) return;

    if (x1==-1 && y1==-1)
    {
        hide = TRUE;
    }
    else
    {
        _lgClipX1=x1-8;
        _lgClipY1=y1-4;
        _lgClipX2=x2+8;
        _lgClipY2=y2+4;

        oca=_lgClipOutcode (_lgMLastX, _lgMLastY);
        ocb=_lgClipOutcode (_lgMLastX+16, _lgMLastY+16);

        if (_lgClipAccept(oca, ocb))
        {
            if (!_lgClipReject (oca, ocb)) hide = TRUE;
        }
        else
        {
            hide = TRUE;
        }
    }

    if (hide == TRUE)
    {
        _lgMRestoreBack();
        _lgCursorTempHide=TRUE;
    }

    _lgClipX1=bcx1;
    _lgClipY1=bcy1;
    _lgClipX2=bcx2;
    _lgClipY2=bcy2;
}

void _lgMouseTempShow()
{
    if (_lgMouseHided) return;

    if (_lgCursorTempHide==TRUE)
    {
        _lgCursorTempHide=FALSE;
        _lgMSaveBack();
        _lgDisplayCursor();
    }
}

void _lgUpdateMouse ()
{
    int     x, y, state;

    if (_lgMouseActive == FALSE) return;

    _lgMouseCall (3, 0, 0, 0);
    x     = _lgCX;
    y     = _lgDX;
    state = _lgBX;

    if (x!=_lgMLastX || y!=_lgMLastY)   /* mouse moved */
    {
        _lgMRestoreBack();
        _lgMLastX  = x;
        _lgMLastY  = y;
        _lgMSaveBack();
        _lgDisplayCursor();
    }

    _lgMButton = LGM_MOVE;

    if (state & 1)          /* 1 = LBUTTON_DOWN */
    {
        if (_lgMLButtonHitted == TRUE)
        {
            _lgMButton = LGM_DRAG;
        }
        else
        {
            _lgMButton = LGM_BUTTONDOWN;
        }

        _lgMLButtonHitted = TRUE;
    }
    else
    {
        if (_lgMLButtonHitted == TRUE)
        {
            _lgMButton = LGM_BUTTONUP;
        }
    }
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

void    _lgGetImage (char *image, int x1, int y1, int x2, int y2)
{
    register int xc, yc;
    int x3, x4;
    unsigned int yaddr, addr;
    char p;

    x3=x1 >> 3;
    x4=x2 >> 3;

    for (yc=y1; yc<=y2; yc++)
    {
        yaddr=_lgYaddr[yc];

        for (p=0; p<4; p++)
        {
            _lgOutportb (0x3ce, 4);
            _lgOutportb (0x3cf, p);

            for (xc=x3; xc<=x4; xc++)
            {
                addr=yaddr+xc;
                *image++ = *(_lgDBitmap + addr);
            }
        }
    }

}

void    _lgPutImage (char *image, int x1, int y1, int x2, int y2)
{
    register int xc, yc;
    int x3, x4;
    unsigned yaddr, addr;
    char p;

    x3=x1 >> 3;
    x4=x2 >> 3;

    _lgOutportb (0x3ce, 3);
    _lgOutportb (0x3cf, 0);
    _lgOutportb (0x3ce, 5);
    _lgOutportb (0x3cf, 0);
    _lgOutportb (0x3ce, 8);
    _lgOutportb (0x3cf, 0xff);

    for (yc=y1; yc<=y2; yc++)
    {
        yaddr=_lgYaddr[yc];

        for (p=0; p<4; p++)
        {
            _lgOutportb (0x3c4, 2);
            _lgOutportb (0x3c5, _lgBitPlane[p]);

            for (xc=x3; xc<=x4; xc++)
            {
                addr=yaddr+xc;
                *(_lgDBitmap+addr)=*image++;
            }
        }
    }

    _lgOutportb (0x3ce, 5);
    _lgOutportb (0x3cf, 2);
    _lgOutportb (0x3c4, 2);
    _lgOutportb (0x3c5, 0x0f);
    _lgSetXOR (_lgXOR);                 /* restore pen mode */
}

int     _lgImageSize (int x1, int y1, int x2, int y2)
{
    return (((((x2 - x1) << 3) + 1) * (y2 - y1 + 1)) >> 2);
}

void    _lgPutFont  (char *image, int x, int y, int width, int height)
{
    register int xc, yc;
    int  bkc=_lgColor, bytewidth, yaddr, addr;
    BYTE *data, next, offset, mask;
    static BYTE mmask;

    _lgRectClipper (x, y, x+width-1, y+height-1);
    if (_lgClipOut) return;

    if (width & 7) bytewidth=(width >> 3)+1;
    else           bytewidth=(width >> 3);

    _lgOutportb (0x3ce, 8);

    for (yc=_lgRY1; yc<=_lgRY2; yc++)
    {
        data = image + bytewidth * (yc - y);
        next = (*data++);
        offset = (_lgRX1 - x);
        yaddr = _lgYaddr[yc];

        for (xc=_lgRX1; xc<=_lgRX2; xc++)
        {
            addr=yaddr + (xc >> 3);
            if (next & (0x80 >> offset)) _lgColor=bkc;
            else _lgColor=_lgBgColor;

           _lgOutportb (0x3cf, _lgMask[xc & 7]);
            mmask=*(_lgDBitmap+addr);
            *(_lgDBitmap+addr)=_lgColor;

            offset++;

            if (offset & 8)     /* offset > 7 */
            {
                offset=0;
                next=*data++;
            }
        }

    }

    _lgColor=bkc;
}

void    _lgPutDIB   (char *image, int x, int y, int width, int height)
{
    register int xc, yc;
    int  bkc=_lgColor;
    char *data;

    _lgRectClipper (x, y, x+width-1, y+height-1);
    if (_lgClipOut) return;
    _lgMouseTempHide (_lgRX1, _lgRY1, _lgRX2, _lgRY2);

    for (yc=_lgRY1; yc<=_lgRY2; yc++)
    {
        data = image + ((yc - y) * width) + (_lgRX1 - x);

        for (xc=_lgRX1; xc<=_lgRX2; xc++)
        {
            _lgColor = *data++;
            _lgLowPoint (xc, yc);
        }
    }

    _lgColor = bkc;
    _lgMouseTempShow();
}

int _lgTextWidth  (char *text)
{
    return (strlen(text) * 8);
}

int _lgTextHeight (char *text)
{
    if (text) return (15);
    return (0);
}

void _lgHideMouse ()
{
    if (_lgMouseHided == TRUE) return;
    _lgMRestoreBack ();
    _lgMouseHided = TRUE;
}

void _lgShowMouse ()
{
    if (_lgMouseHided == FALSE) return;
    _lgMouseHided = FALSE;
    _lgMSaveBack ();
}

BOOL _lgIsMouseHide (void)
{
    return (_lgMouseHided);
}

