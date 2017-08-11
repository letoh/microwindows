/***********************************************************
 *
 * Micro Windows Implementation
 * stdhead.h: machine-independent type definitions
 *
 * $CHNDESC$
 * 這個標頭檔將常用的 include files 包含進來, 並且定義
 * BOOL, BYTE, WORD, DWORD 等和機器無關的型別.
 *
 * 舉例來說, 如果要將此系統移值到純 32-bit 的作業系統上,
 * WORD 應該改為 unsigned short (16-bit), 而 DWORD 則是
 * unsigned int (32-bit).
 * $ENDDESC$
 *
 * $Revision: 1.3 $
 * $Source: P:/MWINDOWS/INCLUDE/rcs/stdhead.h $
 * $Date: 1994/01/08 18:09:37 $
 *
 **********************************************************/

#ifndef __stdhead_h
#define __stdhead_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef __TURBOC__
#	include <malloc.h>
#else	      
#	include <alloc.H>
#endif

	
/* #include <mem.h> */
/* #include <dos.h> */  /* no longer required on future development */

#ifndef __WINDOWS_H     /* now that windows.h defined those... */
#ifndef _INC_WINDOWS
    typedef int                 BOOL;
    typedef unsigned char       BYTE;
    typedef unsigned int        WORD;
    typedef unsigned long       DWORD;

    #ifndef __cplusplus
    #   define TRUE     ((BOOL)1)
    #   define FALSE    ((BOOL)0)
    #else
        const BOOL TRUE = 1, FALSE = 0;
    #endif      /* __cplusplus */

#endif
#endif      /* __WINDOWS_H */


#ifdef __cplusplus
template <class T> T * _(const T & tmp) { return const_cast<T *>(&tmp); }
#endif

#endif

