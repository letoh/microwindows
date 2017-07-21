//
// %%name
// Controller Class Dependencies for Windows
//
// $Revision: 1.1 $
// $Source: D:/DENG/PRG/MWINDOWS/WIN/INCLUDE/rcs/wincntlr.h $
// $Date: 1993/10/18 12:32:22 $
//

#ifndef __wincntlr_h
#define __wincntlr_h

extern HANDLE   phHandle;
extern HWND     pfWindow;
extern BOOL _winTimerOn;
extern BYTE _winMouseButton;
extern int  _winMouseX, _winMouseY;
extern int  _winScrWidth, _winScrHeight;
extern HDC  _winDC;
extern BOOL _winUsingDC;
extern BOOL _winRepaintCycle;

void   _winGetDC ();
void   _winReleaseDC ();

#endif

