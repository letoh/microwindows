#include <bios.h>
#include <dos.h>
#include <stdlib.h>
#include <conio.h>

unsigned long counter = 0;
unsigned char tick = 18;

void interrupt (*old1ch)(...);

void interrupt new1ch (...)
{
    tick --;
    if (!tick)
    {
        tick = 18;
        gotoxy (1, 1);
        cprintf ("%lu objects/second", counter);
        counter = 0;
    }
}

void main ()
{
    old1ch = getvect (0x1c);
    setvect (0x1c, new1ch);

    typedef unsigned long word32;

    struct object
    {
        word32  identifier;
    };

    clrscr();

    while (!bioskey(1))
    {
        object *o = new object;
        delete o;
        counter++;
    }

    bioskey(0);
    setvect (0x1c, old1ch);
}

