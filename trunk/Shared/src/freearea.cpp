//
// Micro Windows Implementation
// freearea.cpp
//
// $Revision: 1.3 $
// $Source: P:/MWINDOWS/LIB/rcs/freearea.cpp $
// $Date: 1994/01/08 18:08:32 $
//

#include "freearea.h"

const int RectArray_SizeUnit = 8;

RectArray::RectArray()
{
    current = size = 0;
    array = NULL;
    expand();
}

RectArray::~RectArray()
{
    deleteall();
}

Rect *RectArray::get (int index)
{
    return (array[index]);
}

int RectArray::getsize ()
{
    return (size);
}

int RectArray::amount ()
{
    int a = 0;
    for (int i = 0; i < size ; i++) if (array[i]) a++;
    return (a);
}

void RectArray::add (Rect *prect)
{
    if (prect == NULL) return;

    for ( ; current < size; current++)
    {
        if (array[current] == NULL)
        {
            array[current++] = new Rect (*prect);
            return;
        }
    }

    expand();
    array[current] = new Rect (*prect);
}


void RectArray::copy (RectArray *parray)
{
    Rect *tmp;
    deleteall();
    size = parray->size;
    current = parray->current;
    array = new Rect* [size];
    for (int i = 0 ; i < size; i++)
    {
        if ((tmp = parray->array[i]) != NULL)
        {
            array[i] = new Rect (*tmp);
        }
        else array[i] = NULL;
    }
}

void RectArray::move (int ox, int oy)
{
    Rect *tmp;
    for (int i = 0; i < size; i++)
    {
        if ((tmp = array[i]) != NULL) tmp->move (ox, oy);
    }
}

void RectArray::purge ()
{
    deleteall();
    expand();
}

void RectArray::expand()
{
    Rect **newArray;
    int i;
    int newsize = size + RectArray_SizeUnit;
    newArray = new Rect* [newsize];
    for (i = 0; i<size; i++) newArray[i] = array[i];
    for ( ; i<newsize; i++) newArray[i] = NULL;
    delete [] array;
    size = newsize;
    array = newArray;
}

void RectArray::deleteall()
{
    Rect *tmp;
    for (int i = 0; i < size; i++)
    {
        if ((tmp = array[i]) != NULL) delete tmp;
    }

    delete [] array;
    array = NULL;
    size = current = 0;
}

void freearea_cut (Rect *a, Rect *b, RectArray *result);

void FreeArea::add (Rect *prect)
{
    cut (prect);
    RectArray::add (prect);
}

void FreeArea::cut (Rect *prect)
{
    Rect *tmp, intersec;
    RectArray a;
    int i;
    
    for (i = 0; i < size; i++)
    {
        if ((tmp = array[i]) == NULL) continue;
        if (tmp->intersection (prect, &intersec) == FALSE) continue;
        array[i] = NULL;
        freearea_cut (tmp, &intersec, &a);
        delete tmp;
    }

    int as = a.getsize ();
    for (i = 0; i < as; i++)
    {
        RectArray::add (a.get (i));
    }
}

BOOL FreeArea::intersection (Rect *prect, FreeArea *result)
{
    Rect intersec, *tmp;
    RectArray *fa = result;
    BOOL hasintersec = FALSE;

    for (int i = 0; i<size; i++)
    {
        if ((tmp = array[i]) == NULL) continue;
        if (tmp->intersection (prect, &intersec) == FALSE) continue;
        hasintersec = TRUE;
        fa->add (&intersec);
    }

    return (hasintersec);
}

BOOL FreeArea::intersection (FreeArea *parea, FreeArea *result)
{
    int size = parea->size;
    Rect *tmp;
    BOOL hasintersec = FALSE;

    for (int i = 0; i<size; i++)
    {
        if ((tmp = parea->array[i]) == NULL) continue;
        if (intersection (tmp, result) == TRUE) hasintersec = TRUE;
    }

    return (hasintersec);
}

void FreeArea::add (FreeArea *parea)
{
    int size = parea->size;
    Rect *tmp;

    for (int i = 0; i<size; i++)
    {
        if ((tmp = parea->array[i]) == NULL) continue;
        add (tmp);
    }
}

void FreeArea::cut (FreeArea *parea)
{
    int size = parea->size;
    Rect *tmp;

    for (int i = 0; i<size; i++)
    {
        if ((tmp = parea->array[i]) == NULL) continue;
        cut (tmp);
    }
}


void FreeArea::copy (FreeArea *parea)
{
    RectArray::copy ((RectArray*)parea);
}

void freearea_cut (Rect *a, Rect *b, RectArray *result)
{
    Rect tmp;
    #define ADD(m,n,p,q,r,s)  if (m!=n) { tmp.set (p,q,r,s); \
                              result->add(&tmp); }

    ADD(a->y1, b->y1, a->x1, a->y1, a->x2, b->y1 - 1);
    ADD(a->y2, b->y2, a->x1, b->y2+1, a->x2, a->y2);
    ADD(a->x1, b->x1, a->x1, b->y1, b->x1 - 1, b->y2);
    ADD(a->x2, b->x2, b->x2+1, b->y1, a->x2, b->y2);
}

