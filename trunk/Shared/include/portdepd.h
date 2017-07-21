//
// Micro Windows Implementation
// portdepd.h: port-related dependencies
//
// 和 port 有關的公用巨集, ViewPort 有用到.
//
// $Revision: 1.1 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/portdepd.h $
// $Date: 1993/10/03 03:44:51 $
//

#ifndef __portdepd_h
#define __portdepd_h

#define SETPENMODE  if (mode == PPM_Normal) \
                    _lgSetXOR (FALSE); else _lgSetXOR (TRUE)
#define SETPENCOLOR _lgSetColor (color);  _lgSetBgColor (bgColor)
#define SETPORT     SETPENMODE; SETPENCOLOR
#define SETCLIP(a,b,c,d)    _lgSetClippingRect (a,b,c,d);

#endif

