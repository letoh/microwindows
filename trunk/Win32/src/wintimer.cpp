//
// %%name
// wintimer.cpp
//
// $Revision: 1.1 $
// $Source: D:/DENG/PRG/MWINDOWS/WIN/LIB/rcs/wintimer.cpp $
// $Date: 1993/10/18 12:32:29 $
//

#include "mwindows.h"
#include "wincntlr.h"

BOOL _winGetTimerFlag ()
{
    return (_winTimerOn);
}

void _winSetTimerFlag (BOOL f)
{
    _winTimerOn = f;
}

