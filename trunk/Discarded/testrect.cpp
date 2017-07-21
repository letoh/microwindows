#include "gobject.h"

void main ()
{
    printf ("before allocation: %lu\n", coreleft());;
    Rect **p = (Rect**)calloc (1, sizeof(Rect*));
    printf ("allocation via calloc: %lu\n", coreleft());;
    Rect **r = new Rect* [1];
    printf ("allocation via new: %lu\n", coreleft());;
    delete [] r;
    printf ("deallocation via delete[]: %lu\n", coreleft());;
    free (p);
    printf ("deallocation via free: %lu\n", coreleft());;
}

