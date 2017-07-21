//
// Micro Windows Implementation
// object.h: the base class of all other classes
//
// $DESC$
// This is the revised "root" for the Micro Windows class hierarchy.
// Object class was renamed to MWObject, while you can still use the
// originally if _MW_OBJECTCLASS is defined (default is yes.)
//
// This revision allows you working with other class hierarchies,
// such as Borland OWL or Microsoft Foundation class.
// $ENDDESC$
// $CHNDESC$
// Object class, 所有其他的 class 皆繼承自此 class.
// 值得注意的是這裡用到了 virtual destructor .
// $ENDDESC$
//
// $Revision: 1.2 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/object.h $
// $Date: 1994/01/08 18:09:37 $
//

#ifndef __object_h
#define __object_h

#ifndef __stdhead_h
#   include "stdhead.h"
#endif


#define _MW_OBJECTCLASS

#ifndef _MW_OBJECTROOT
#   define _MW_OBJECTBASE
#else
#   define _MW_OBJECTBASE  : public _MW_OBJECTROOT
#endif     

class MWObject  _MW_OBJECTBASE
{               
public:
    virtual ~MWObject ()  {}
};

#ifdef _MW_OBJECTCLASS
    typedef MWObject    Object;
#endif

#ifdef _MW_OBJECTDEBUG
    void *operator new (size_t s);
    void operator delete (void *ptr);
#endif

#endif

