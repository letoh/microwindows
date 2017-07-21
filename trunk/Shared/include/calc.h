//
// Micro Windows Modules
// calc.h: a simple-minded calculator
//
// $Revision: 1.2 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/calc.h $
// $Date: 1993/10/05 12:55:54 $
//

#ifndef __calc_h
#define __calc_h

#include "global.h"
#include "model.h"
#include "window.h"
#include "button.h"

class Calculator : public Model
{
public:
    Calculator (View *parent = desktop, Point *spot = NULL);

    virtual void draw (View *from, Port *port, Rect *area);
    virtual void fromView (View *from);

protected:
    void drawCalculator (Port *port);
    Window *calcWindow;
    Button *numBtn[10];
    Button *cBtn, *plusBtn, *minusBtn, *mulBtn, *divBtn, *equBtn;
    char last[20], current[20];
    int action;
    BOOL clean;
    Rect evaluator;
};

#endif

