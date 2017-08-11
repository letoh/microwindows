//
// Micro Windows Implementation
// draw.cpp
//
// $Revision: 1.2 $
// $Source: D:/DENG/PRG/MWINDOWS/APP/rcs/draw.cpp $
// $Date: 1993/10/05 12:55:27 $
//

#include "draw.h"

DrawWindow::DrawWindow (DrawApp *pap, View *pparent, Rect *drawRect)
{
    Rect r;
    if (drawRect == NULL) pparent->getRandomAttachRect (&r); else r = *drawRect;

    ap = pap;
    myWindow = new Window ("Draw Window", &r, pparent, this, TRUE, TRUE);
    beingDrawn = FALSE;
}

DrawWindow::~DrawWindow ()
{
    Object *p = shapeList.first ();

    do
    {
        if (p == NULL) break;
        delete p;
        p = shapeList.next ();
    } while (1);
}

void DrawWindow::draw (View *from, Port *port, Rect *area)
{
    port->setPenMode (PPM_Normal);
    port->setPenColor (PPC_White);
    port->draw (_(FilledRectangle (area)));

    Shape *s = (Shape*)shapeList.first();
    do
    {
        if (s == NULL) break;
        ColorObject *o = (ColorObject*)shapeList.next ();

        port->setPenColor (o->color);
        port->draw (s);
        s = (Shape*)shapeList.next ();
    } while (1);
}

void DrawWindow::leftButtonDown (View *from, Point *pnt)
{
    Port *p = from->getClientPort();

    if (beingDrawn == TRUE)
    {
        p->setPenMode (PPM_XOR);
        p->setPenColor (PPC_White);
        p->draw (currentShape);
        currentShape->setDest (pnt);
        p->draw (currentShape);
        return;
    }

    beingDrawn = TRUE;
    mouse->lock (myWindow);
    mouse->hide();
    p->setPenMode (PPM_XOR);
    p->setPenColor (PPC_White);

    switch (ap->penType)
    {
        case DT_Line:
            currentShape = new Line (pnt, pnt);
            break;

        case DT_Solid:
            currentShape = new FilledRectangle (pnt, pnt);
            break;

        case DT_Box:
            currentShape = new Rectangle (pnt, pnt);
            break;
    }

    p->draw (currentShape);
    p->setPenMode (PPM_Normal);
}

void DrawWindow::leftButtonUp (View *from, Point *pnt)
{
    Port *p = from->getClientPort();
    if (beingDrawn == FALSE) return;

    beingDrawn = FALSE;
    mouse->show();
    mouse->unlock ();

    p->setPenMode (PPM_Normal);
    p->setPenColor (ap->penColor);
    p->draw (currentShape);

    shapeList.put (currentShape);
    shapeList.put (new ColorObject (ap->penColor));
}


DrawApp::DrawApp (View *pparent, Rect *drawRect)
{
    Rect r;
    if (drawRect == NULL) pparent->getRandomAttachRect (&r); else r = *drawRect;
    appWindow = new Window ("Simple Draw", &r, pparent, this, TRUE, TRUE);
    new ColorPalette (this, appWindow, _(Point (10, 10)));
    new DrawWindow (this, appWindow, _(Rect (100, 20, 300, 200)));
}

void DrawApp::createDrawWindow ()
{
    new DrawWindow (this, appWindow);
}

ColorPalette::ColorPalette (DrawApp *servingAp, View *pparent,
                            Point *attachPoint)
{
    ap = servingAp;

    Rect cpRect;
    Point tmp;
    if (attachPoint == NULL) pparent->getRandomAttachPoint (&tmp);
    else tmp = *attachPoint;
    cpRect.set (&tmp, &tmp);
    cpRect.resize (83, 350);

    paletteWindow = new Window ("---", &cpRect, pparent, this, TRUE, TRUE);
//  paletteWindow->setAlwaysOnTop (TRUE);

    ap->penType = DT_Line;
    ap->penColor = PPC_Black;

    Rect btnRect (5, 5, 60, 25);
    currentClrBtn = new ColorButton (PPC_Black, &btnRect, paletteWindow,
                                     this);
    currentClrBtn->draw();
    btnRect.move (0, 30);
    createBtn = new Button ("Create", &btnRect, paletteWindow, this);
    btnRect.move (0, 30);
    lineBtn = new Button ("Line", &btnRect, paletteWindow, this);
    btnRect.move (0, 30);
    boxBtn = new Button ("Box", &btnRect, paletteWindow, this);
    btnRect.move (0, 30);
    solidBtn = new Button ("Solid", &btnRect, paletteWindow, this);
    btnRect.move (0, 30);
    btnRect.resize (btnRect.width(), 10);

    for (int i = 0; i < 16; i++)
    {
        colorBtn[i] = new ColorButton ((PortPenColor)i, &btnRect,
                                       paletteWindow, this);
        colorBtn[i]->draw();
        btnRect.move (0, 10);
    }
}

void ColorPalette::fromView (View *from)
{
    for (int i = 0; i < 16; i++)
    {
        if (from == colorBtn[i])
        {
            ap->penColor = colorBtn[i]->getColor();
            currentClrBtn->setColor (ap->penColor);
            return;
        }
    }

    if (from == createBtn) ap->createDrawWindow ();
    if (from == lineBtn) ap->penType = DT_Line;
    if (from == boxBtn) ap->penType = DT_Box;
    if (from == solidBtn) ap->penType = DT_Solid;
}

ColorButton::ColorButton (PortPenColor color, Rect *prect, View *pparent,
                          Model *client, BOOL attach, BOOL killModel)
            : Button ("", prect, pparent, client, attach, killModel)
{
    btnColor = color;
    if (attach && parent) parent->attachSubview (this);
}

PortPenColor ColorButton::getColor ()
{
    return (btnColor);
}

void ColorButton::setColor (PortPenColor color)
{
    btnColor = color;
    draw ();
}

void ColorButton::drawText (Port *port)
{
    Rect r;
    port->getPortRect (&r);
    r.move (- r.x1, - r.y1);

    r.move (2, 2);
    r.resize (r.width() -4, r.height() -4);
    port->setPenColor (btnColor);
    port->draw (_(FilledRectangle (&r)));
}

