//
// Micro Windows Modules
// calc.h: a simple-minded calculator
//
// $Revision: 1.3 $
// $Source: D:/DENG/PRG/MWINDOWS/APP/rcs/calc.cpp $
// $Date: 1993/10/18 12:35:36 $
//

#include "shape.h"
#include "calc.h"

Calculator::Calculator (View *parent, Point *spot)
{
    strcpy (last, "");
    strcpy (current, "");
    action = 0;
    clean = FALSE;

    Rect calcRect;
    Point tmp;
    if (spot == NULL) parent->getRandomAttachPoint (&tmp); else tmp = *spot;
    calcRect.set (&tmp, &tmp);
    calcRect.resize (200, 170);
    evaluator.set (10, 10, 10, 10);
    evaluator.resize (160, 20);

    calcWindow = new Window ("Calculator", &calcRect, parent, this,
                                 TRUE, TRUE);
    calcWindow->setResizable (FALSE);

    Point orig (10, 40);
    Rect btnRect (0, 0, 20, 20);
    char btnText[2];
    for (int i = 1; i < 10; i++)
    {
        btnRect.set (0, 0, 20, 20);
        Point btnPtn (((i-1) % 3) * 30, ((i-1) / 3) * 30);
        btnPtn.move (&orig);
        btnRect.move (&btnPtn);
        itoa (i, btnText, 10);

        numBtn[i] = new Button (btnText, &btnRect, calcWindow, this);
    }

    btnRect.set (90 + 10, 40, 110 + 10, 60);
    cBtn = new Button ("C", &btnRect, calcWindow, this);
    btnRect.move (0, 30);
    equBtn = new Button ("=", &btnRect, calcWindow, this);
    btnRect.move (0, 30);
    numBtn[0] = new Button ("0", &btnRect, calcWindow, this);
    btnRect.move (30, -60);
    plusBtn = new Button ("+", &btnRect, calcWindow, this);
    btnRect.move (0, 30);
    minusBtn = new Button ("-",&btnRect, calcWindow, this);
    btnRect.move (0, 30);
    mulBtn = new Button ("*", &btnRect, calcWindow, this);
    btnRect.move (30, -60);
    divBtn = new Button ("/", &btnRect, calcWindow, this);
}

void Calculator::fromView (View *from)
{
    if (from == equBtn)
    {
        long x = atol (last), y = atol (current), z = y;

        switch (action)
        {
            case 1: z = x+y; break;
            case 2: z = x-y; break;
            case 3: z = x*y; break;
            case 4:
                 if (!y)
                 {
                    strcpy (last, "");
                    strcpy (current, "");
                    drawCalculator (calcWindow->getClientPort());
                    action = 0;
                    return;
                 }
                 else
                 {
                    z = x/y;
                 }
                 break;
        }
        strcpy (last, "");
        ltoa (z, current, 10);
        action = -1;
    }
    else if (from == cBtn)
    {
        action = 0;
        strcpy (last, "");
        strcpy (current, "");
    }
    else if (from == plusBtn)
    {
        action = 1;
        strcpy (last, current);
        clean = TRUE;
    }
    else if (from == minusBtn)
    {
        action = 2;
        strcpy (last, current);
        clean = TRUE;
    }
    else if (from == mulBtn)
    {
        action = 3;
        strcpy (last, current);
        clean = TRUE;
    }
    else if (from == divBtn)
    {
        action = 4;
        strcpy (last, current);
        clean = TRUE;
    }
    else
    {
        if (action == -1) fromView (cBtn);
        if (clean)
        {
            clean = FALSE;
            strcpy (current, "");
        }

        if (strlen (current) > 10)
        {
            return;
        }

        char toAdd[2];
        toAdd[1] = 0;

        for (int i = 0; i < 10; i++)
            if (from == numBtn[i]) toAdd[0] = i + '0';

        strcat (current, toAdd);
    }

    drawCalculator (calcWindow->getClientPort());
}

void Calculator::drawCalculator (Port *port)
{
    char bar[25];
    long num = atol (current);

    switch (action)
    {
        case -1: strcpy (bar, "="); break;
        case 1: strcpy (bar, "+"); break;
        case 2: strcpy (bar, "-"); break;
        case 3: strcpy (bar, "*"); break;
        case 4: strcpy (bar, "/"); break;
        default: strcpy (bar, " "); break;
    }

    sprintf (bar + 1, "%13ld", num);

    port->draw (&StereoBorder (&evaluator, Concave, 1));
    port->setPenColor (PPC_White);
    port->setBgColor (PPC_Gray);
    port->draw (&Text (bar, &evaluator));
}

void Calculator::draw (View *from, Port *port, Rect *area)
{
    drawCalculator (port);
}

