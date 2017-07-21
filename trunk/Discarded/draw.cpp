//
// Micro Windows Example
// A Simple Draw
//
// $Revision$
// $Source$
// $Date$
//

#include "port.h"
#include "shape.h"
#include "global.h"
#include "model.h"
#include "button.h"
#include "window.h"
#include "gui.h"

class ColorButton : public Button
{
public:
    ColorButton (PortPenColor color, Rect *prect, View *pparent,
            Model *client,
            BOOL attach = TRUE, BOOL killModel = FALSE)
        : Button ("", prect, pparent, client, attach, killModel)
    {
        c = color;
    }

    virtual void selfDraw (Port *port, Rect *area)
    {
        Rect pr = clientRect;
        pr.move (2, 2);
        pr.resize (pr.width() -2, pr.height() -2);
        port->setPenColor (c);
        port->draw (&FilledRectangle (&pr));
    }

    void changeColor (PortPenColor cc)
    {
        c = cc;
        draw (NULL);
    }

    PortPenColor getColor ()
    {
        return c;
    }

protected:
    PortPenColor c;
};

class ColorPalette : public Model
{
public:
    ColorPalette (Point *orig)
    {
        beingClosed = FALSE;

        paletteWindow = new Window ("Color", &Rect (orig->x, orig->y,
                                           orig->x + 80, orig->y + 180 + 26),
                                    desktop, this, TRUE, FALSE);

        for (int i = 0; i < 16; i++)
        {
            buttonArray[i] = new ColorButton (i, &Rect(0, i*10, 69, i*10 + 8),
                                              paletteWindow, this);
        }

        currentColor = new ColorButton (0, &Rect (0, i*10 + 5, 39, i*10+13),
                                        paletteWindow, this);
    }

    ~ColorPalette ()
    {
        beingClosed = TRUE;
    }


    virtual BOOL close (View *from)
    {
        return (beingClosed);
    }

    virtual void fromView (View *from)
    {
        for (int i = 0; i< 16; i++) if (buttonArray[i] == from)
            currentColor->changeColor (i);
    }

    PortPenColor getColor ()
    {
        return (currentColor->getColor());
    }

protected:
    Window *paletteWindow;
    ColorButton *buttonArray[16];
    ColorButton *currentColor;
    BOOL beingClosed;
};

class ColorObject : public Object
{
public:
    ColorObject (int c)
    {
        color = c;
    }

    int color;
};

class DrawWindow : public Model
{
public:
    DrawWindow (Point *p, View *pparent, ColorPalette *ppalette)
    {
        lineList = new List;
        palette = ppalette;
        new Window ("Draw Window", &Rect (p->x, p->y, p->x + 200, p->y +100),
                    pparent, this, TRUE, TRUE);
        beingDrawn = FALSE;
    }

    ~DrawWindow ()
    {
        Object *p = lineList->first();
        do
        {
            if (p == NULL) break;
            delete p;
            p = lineList->next();
        } while (1);

        delete lineList;
    }

    virtual void draw (View *from, Port *port, Rect *area)
    {
        port->setPenColor (PPC_White);
        port->draw (&FilledRectangle (area));
        Line *line = (Line*)lineList->first ();
        do
        {
            if (line == NULL) break;
            ColorObject *obj = (ColorObject*)lineList->next();
            port->setPenColor (obj->color);
            port->draw (line);
            line = (Line*)lineList->next ();
        } while (1);
    }

    virtual void leftButtonDown (View *from, Point *pnt)
    {
        if (beingDrawn == FALSE)
        {
            beingDrawn = TRUE;
            currentLine = new Line (pnt, pnt);
            (from->getClientPort())->setPenMode (PPM_XOR);
            (from->getClientPort())->draw (currentLine);
            return;
        }

        (from->getClientPort())->setPenMode (PPM_XOR);
        (from->getClientPort())->draw (currentLine);

        pnt->move (20, 20);
        currentLine->setDest (pnt);
        (from->getClientPort())->draw (currentLine);
    }

    virtual void leftButtonUp (View *from, Point *pnt)
    {
        if (beingDrawn == FALSE) return;
        beingDrawn = FALSE;
        (from->getClientPort())->setPenMode (PPM_XOR);
        (from->getClientPort())->draw (currentLine);
        (from->getClientPort())->setPenMode (PPM_Normal);
        (from->getClientPort())->setPenColor (palette->getColor());
        (from->getClientPort())->draw (currentLine);
        lineList->put (currentLine);
        lineList->put (new ColorObject (palette->getColor()));
    }

protected:
    ColorPalette *palette;
    List *lineList;

    Line *currentLine;
    BOOL beingDrawn;
};

class DrawApp : public Model
{
public:
    DrawApp ()
    {
        myWindow = new Window ("Simple Draw", &Rect (100, 0, 639, 479), desktop,
                               this, TRUE, TRUE);
        palette = new ColorPalette (&Point (10, 10));
        closeButton = new Button ("Close", &Rect (10, 300, 60, 320),
                                  desktop, this, TRUE);
        newButton = new Button ("New", &Rect (10, 340, 60, 360),
                                desktop, this, TRUE);
        x = 0;
    }

    ~DrawApp ()
    {
    }

    virtual void fromView (View *from)
    {
        if (from == closeButton)
        {
            delete palette;
            desktop->quit ();
        }
        if (from == newButton)
        {
            if (x > 100) x = 0;
            new DrawWindow (&Point (x, x), myWindow, palette);
            x+=10;
        }
    }

protected:
    int x;
    Window *myWindow;
    ColorPalette *palette;
    Button *closeButton, *newButton;
};

int main ()
{
    Driver g;
    new DrawApp;
    return (g.run());
}

