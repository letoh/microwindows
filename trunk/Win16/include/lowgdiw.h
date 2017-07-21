/****************************************************************************
 *
 * Lowgdi.h: LowGDI for Windows
 *
 ****************************************************************************/

#ifndef __lowgdiw_h
#define __lowgdiw_h

#ifndef __stdhead_h
#   include "stdhead.h"
#endif

BOOL    _lgOpenGraphics(void);
BOOL    _lgCloseGraphics(void);

int     _lgGetScreenWidth(void);
int     _lgGetScreenHeight(void);
int     _lgGetColor (void);
int     _lgGetBgColor (void);
BOOL    _lgGetXOR (void);
void    _lgSetColor (int c);
void    _lgSetBgColor (int c);
void    _lgSetXOR (BOOL xor);

void    _lgPoint    (int x, int y);
void    _lgLine     (int x1, int y1, int x2, int y2);
void    _lgRect     (int x1, int y1, int x2, int y2);
void    _lgSolidRect(int x1, int y1, int x2, int y2);
void    _lgDrawText (int x, int y, char *text);

int     _lgTextWidth  (char *text);
int     _lgTextHeight (char *text);

void    _lgResetClipping(void);
void    _lgSetClippingRect (int x1, int y1, int x2, int y2);

/* Mouse support functions */
/* --------------- */
#define LGM_MOVE            1
#define LGM_BUTTONDOWN      2
#define LGM_BUTTONUP        4
#define LGM_DRAG            8

void _lgHideMouse (void);
void _lgShowMouse (void);
BYTE _lgGetMouseButton (void);
int  _lgGetMouseX (void);
int  _lgGetMouseY (void);
BOOL _lgIsMouseActive(void);
BOOL _lgIsMouseHide (void);

#endif      /* #ifndef __lowgdi_h */

