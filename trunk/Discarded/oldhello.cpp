//
// Micro Windows Example
// hello.cpp
//
// $Revision: 1.1 $
// $Source: D:/DENG/BOOK/MWINDOWS/rcs/oldhello.cpp $
// $Date: 1993/10/03 15:07:53 $
//

#include "port.h"
#include "shape.h"
#include "global.h"
#include "model.h"
#include "button.h"
#include "window.h"
#include "gui.h"
#include "calc.h"
#include <time.h>

class HelloApp : public Model
{
public:
    HelloApp (Rect *r, HelloApp *pparent)
    {
        parentModel = pparent;

        if (pparent == NULL) parent = desktop;
        else parent = pparent->mywindow;

        mywindow = new Window ("Hello, world!", r, parent, this, TRUE, TRUE);
        forkButton = new Button ("Create another...",
                                 &Rect (10, 10, 150, 30), mywindow, this);
        timer->apply (mywindow, 1000);
        idler->apply (mywindow);
    }

    virtual BOOL close (View *from)
    {
        if (from == mywindow)
        {
            idler->remove (mywindow);
            timer->remove (mywindow);
        }

        return (TRUE);
    }

    virtual void leftButtonDown (View *from, Point *pnt)
    {
        Port *port = mywindow->getClientPort ();
        port->setPenColor (PPC_Green);
        port->setBgColor (PPC_Gray);
        port->draw (&Text ("Hello!", pnt));
    }

    virtual void fromView (View *from)
    {
        if (from == forkButton)
        {
            Rect viewRect;
            mywindow->getViewRect (&viewRect);
            viewRect.move (20, 20);

            new HelloApp (&viewRect, parentModel);
        }
    }

    virtual void fromController (View *from, Controller *ctrl)
    {
        if (ctrl != timer)
        {
            PortPenColor ppc = random (16);
            Rect r;
            from->getClientRect (&r);
            int x = random(r.width()), y = random(r.height());
            Port *p = from->getClientPort ();
            p->setPenColor (ppc);
            p->draw (&Line (x, y, x, y));
            return;
        }

        char buf[30];
        time_t t = time (NULL);
        struct tm *tb = localtime (&t);
        sprintf (buf, "%02d:%02d:%02d", tb->tm_hour, tb->tm_min, tb->tm_sec);

        Port *port = from->getClientPort ();
        port->setPenColor (PPC_White);
        port->setBgColor (PPC_Gray);
        port->draw (&Text (buf, &Point(10, 45)));
    }

protected:
    Button *forkButton;
    Window *mywindow;
    View   *parent;
    HelloApp  *parentModel;
};

int main ()
{
    Driver      gui;

    HelloApp *a = new HelloApp (&Rect (100, 100, 400, 300), NULL);
    new HelloApp (&Rect (20, 40, 250, 150), a);
    new Calculator (desktop);

    return (gui.run());
}

