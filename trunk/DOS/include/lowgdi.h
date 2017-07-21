/****************************************************************************
 *
 * Lowgdi.h: 低階繪圖介面
 *
 * 這個模組屬於和硬體關係較大的部份。由於本模組物件化程度不高，
 * 加上效率的考量，故以 C 撰寫。
 *
 * 這個模組支援的 VGA 標準顯示模式，即 640x480, 16 色，並且
 * 支援滑鼠。
 *
 ****************************************************************************/

#ifndef __lowgdi_h
#   define __lowgdi_h

#ifndef __stdhead_h
#   include "stdhead.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define LG_BLACK        (0)
#define LG_DARKBLUE     (1)
#define LG_DARKGREEN    (2)
#define LG_DARKCYAN     (3)
#define LG_DARKRED      (4)
#define LG_DARKPURPLE   (5)
#define LG_BROWN        (6)
#define LG_GRAY         (7)
#define LG_DARKGARY     (8)
#define LG_BLUE         (9)
#define LG_GREEN        (10)
#define LG_CYAN         (11)
#define LG_RED          (12)
#define LG_PURPLE       (13)
#define LG_YELLOW       (14)
#define LG_WHITE        (15)

/* 繪圖起始設定 */
/* ---------------- */
BOOL    _lgOpenGraphics(void);
BOOL    _lgCloseGraphics(void);

/* 系統參數取得／設定 */
/* ---------------- */
int     _lgGetScreenWidth(void);
int     _lgGetScreenHeight(void);
int     _lgGetColor (void);
int     _lgGetBgColor (void);
BOOL    _lgGetXOR (void);
void    _lgSetColor (int c);
void    _lgSetBgColor (int c);
void    _lgSetXOR (BOOL xor);

/* 基本繪圖元件 */
/* ---------------- */
void    _lgPoint    (int x, int y);
void    _lgLine     (int x1, int y1, int x2, int y2);
void    _lgRect     (int x1, int y1, int x2, int y2);
void    _lgSolidRect(int x1, int y1, int x2, int y2);
void    _lgDrawText (int x, int y, char *text);

int     _lgTextWidth  (char *text);
int     _lgTextHeight (char *text);

/* 設定「剪裁」(clipping) 矩形區域, 在重疊式視窗中肩負重大任務 */
/* ---------------- */
void    _lgResetClipping(void);
void    _lgSetClippingRect (int x1, int y1, int x2, int y2);

/* 位元對映圖形 (bitmapped graphics) 處理 */
/* GetImage、PutImage、ImageSize 為供一般狀況使用之函數 */
/* ---------------- */
void    _lgGetImage (char *image, int x1, int y1, int x2, int y2);
void    _lgPutImage (char *image, int x1, int y1, int x2, int y2);
int     _lgImageSize(int x1, int y1, int x2, int y2);

/* PutFont 放上一張單色 bitmap, PutDIB 放上一張彩色 bitmap (1 Byte/pixel) */
/* ---------------- */
void    _lgPutFont  (char *image, int x, int y, int width, int height);
void    _lgPutDIB   (char *image, int x, int y, int width, int height);

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

#ifdef __cplusplus
}
#endif

#endif      /* #ifndef __lowgdi_h */

