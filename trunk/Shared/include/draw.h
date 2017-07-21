//
// Micro Windows Implementation
// draw.h
//
// $Revision: 1.1 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/draw.h $
// $Date: 1993/10/03 15:22:22 $
//

#ifndef __draw_h
#define __draw_h

#include "global.h"
#include "model.h"
#include "window.h"
#include "button.h"
#include "shape.h"

enum DrawType
{
    DT_Line,
    DT_Box,
    DT_Solid
};

class ColorButton;
class ColorPalette;
class DrawApp;

class DrawWindow : public Model
{
public:
    DrawWindow (DrawApp *pap, View *pparent, Rect *drawRect = NULL);
    ~DrawWindow ();

    virtual void draw (View *from, Port *port, Rect *area);
    virtual void leftButtonDown (View *from, Point *pnt);
    virtual void leftButtonUp (View *from, Point *pnt);

protected:
    DrawApp *ap;
    Window *myWindow;

    List shapeList;
    Shape *currentShape;
    BOOL beingDrawn;
};

class DrawApp : public Model
{
public:
    DrawApp (View *pparent = desktop, Rect *drawRect = NULL);

    void createDrawWindow ();

protected:
    friend ColorPalette;
    friend DrawWindow;

    DrawType penType;
    PortPenColor penColor;
    Window *appWindow;
};

class ColorPalette : public Model
{
public:
    ColorPalette (DrawApp *servingAp, View *pparent,
                  Point *attachPoint = NULL);
    virtual void fromView (View *from);

protected:
    ColorButton *colorBtn[16];
    ColorButton *currentClrBtn;
    Button *createBtn, *lineBtn, *boxBtn, *solidBtn;
    DrawApp *ap;
    Window *paletteWindow;
};

class ColorButton : public Button
{
public:
    ColorButton (PortPenColor color, Rect *prect, View *pparent,
                 Model *client, BOOL attach = TRUE, BOOL killModel = FALSE);

    PortPenColor getColor ();
    void setColor (PortPenColor color);

protected:
    virtual void drawText (Port *port);
    PortPenColor btnColor;
};

class ColorObject : public Object
{
public:
    ColorObject (PortPenColor c)    { color = c; }

    PortPenColor color;
};

#endif

