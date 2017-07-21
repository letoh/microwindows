//
// %%name
// Windows Platform Initializer
//
// $Revision: 1.1 $
// $Source: D:/DENG/PRG/MWINDOWS/WIN/LIB/rcs/winmain.cpp $
// $Date: 1993/10/18 12:32:29 $
//

#include <windows.h>
#include "wincntlr.h"
#include "global.h"
#include "lowgdiw.h"

#define TIMER_ID        1000

HANDLE  pfHandle;
HWND    pfWindow;
BOOL    _winTimerOn = FALSE;
BYTE    _winMouseButton = 0;
int     _winMouseX = 0, _winMouseY = 0;
int     _winScrWidth = 0, _winScrHeight = 0;
HDC     _winDC;
BOOL    _winUsingDC = FALSE;
BOOL    _winRepaintCycle = FALSE;

int  mwmain ();                     // our application set here
BOOL InitPfClass (HANDLE prev);
HWND CreatePfWindow ();
long FAR PASCAL PfProc (HWND wh, unsigned msg, WORD wParam, LONG lParam);

int PASCAL WinMain (HANDLE instHandle, HANDLE prevInstHandle,
                    LPSTR commandLine, int showCommand)
{
    pfHandle = instHandle;

    if ((InitPfClass (prevInstHandle)) == FALSE) return (0);
    if (!(pfWindow = CreatePfWindow())) return (0);

    SetTimer (pfWindow, TIMER_ID, 50, NULL);

    ShowWindow (pfWindow, showCommand);
    UpdateWindow (pfWindow);

    mwmain ();

    KillTimer (pfWindow, TIMER_ID);

    return (1);
}

// ----------------------------------------------------------------------------

BOOL InitPfClass (HANDLE prev)
{
    WNDCLASS    windowClass;

    if (prev)
    {
        MessageBox (NULL,
                    "Micro Windows Platform for MS-Windows already loaded",
                    "CAUTION", MB_OK | MB_ICONHAND);
        return (FALSE);
    }

    windowClass.style           = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc     = (WNDPROC)PfProc;
    windowClass.cbClsExtra      = 0;
    windowClass.cbWndExtra      = 0;
    windowClass.hInstance       = pfHandle;
    windowClass.hCursor         = LoadCursor (NULL, IDC_ARROW);
    windowClass.hIcon           = NULL;
    windowClass.hbrBackground   = CreateSolidBrush(RGB(192, 192, 192));
    windowClass.lpszMenuName    = NULL;
    windowClass.lpszClassName   = "MicroWindows";

    if (!RegisterClass (&windowClass)) return (FALSE);
    return (TRUE);
}

// ----------------------------------------------------------------------------

HWND CreatePfWindow ()
{
    RECT r;
    GetWindowRect (GetDesktopWindow(), &r);

    _winScrWidth = r.right + 1;
    _winScrHeight = r.bottom + 1;

    return (CreateWindow ("MicroWindows", "Micro Windows Platform",
                          WS_POPUP, 0, 0, r.right, r.bottom,
                          NULL, NULL, pfHandle, NULL));
}

// ----------------------------------------------------------------------------

void   _winGetDC ()
{
    if (_winUsingDC || _winRepaintCycle) return;
    _winDC = GetDC (pfWindow);
    _winUsingDC = TRUE;
}

void   _winReleaseDC ()
{
    if (!_winUsingDC || _winRepaintCycle) return;
    ReleaseDC (pfWindow, _winDC);
    _winUsingDC = FALSE;
}

long FAR PASCAL PfProc (HWND wh, unsigned msg, WORD wParam, LONG lParam)
{
    switch (msg)
    {
        case WM_MOUSEMOVE:
            if (_winMouseButton != LGM_BUTTONUP) _winMouseButton = LGM_MOVE;
            if (wParam == MK_LBUTTON) _winMouseButton = LGM_DRAG;
            _winMouseX = LOWORD (lParam);
            _winMouseY = HIWORD (lParam);
            break;

        case WM_LBUTTONUP:
            _winMouseButton = LGM_BUTTONUP;
            _winMouseX = LOWORD (lParam);
            _winMouseY = HIWORD (lParam);
            break;

        case WM_LBUTTONDOWN:
            if (_winMouseButton != LGM_BUTTONUP)
                _winMouseButton = LGM_BUTTONDOWN;
            _winMouseX = LOWORD (lParam);
            _winMouseY = HIWORD (lParam);
            break;

        case WM_PAINT:
            _winRepaintCycle = TRUE;
            _winUsingDC = TRUE;
            PAINTSTRUCT ps;

            _winDC = BeginPaint (wh, &ps);
            if (desktop)
            {
                desktop->draw (&Rect (ps.rcPaint.left, ps.rcPaint.top,
                                      ps.rcPaint.right, ps.rcPaint.bottom));
            }
            // PAINT PROCEDURE !!!!!!
            EndPaint (wh, &ps);

            _winUsingDC = FALSE;
            _winRepaintCycle = FALSE;

            break;

        case WM_DESTROY:
            PostQuitMessage (0);
            break;

        case WM_TIMER:
            _winTimerOn = TRUE;
            break;

        default:
            return (DefWindowProc (wh, msg, wParam, lParam));
    }

    return (0L);
}

