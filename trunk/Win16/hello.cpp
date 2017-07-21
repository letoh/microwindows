//
// Micro Windows Implementation
// Hello.cpp:
//
// $Revision: 1.1 $
// $Source: D:/DENG/PRG/MWINDOWS/WIN/rcs/hello.cpp $
// $Date: 1993/10/18 12:36:20 $
//

#include "shape.h"
#include "global.h"
#include "model.h"
#include "button.h"
#include "window.h"
#include "gui.h"

class HelloApp : public Model
{
public:
    HelloApp (View *pparent = desktop);

    virtual void leftButtonUp (View *from, Point *pnt);
    virtual void fromView (View *from);
    virtual void draw (View *from, Port *port, Rect *area);

protected:
    Button *createBtn, *insideBtn;
    Window *helloWindow;
    View *windowParent;
};

HelloApp::HelloApp (View *pparent)
{
    Rect tmp;
    pparent->getRandomAttachRect (&tmp);
    windowParent = pparent;

    helloWindow = new Window ("Hello, world!", &tmp, pparent,
                              this, TRUE, TRUE);
    createBtn = new Button ("Create", &Rect (10, 10, 70, 30),
                            helloWindow, this);
    insideBtn = new Button ("Inside", &Rect (10, 40, 70, 60),
                            helloWindow, this);
}

void HelloApp::leftButtonUp (View *from, Point *pnt)
{
    Port *p = helloWindow->getClientPort ();

    p->setPenColor (PPC_White);
    p->setBgColor (PPC_Gray);
    p->draw (&Text ("Hello!", pnt));
}

void HelloApp::fromView (View *from)
{
    if (from == createBtn) new HelloApp (windowParent);
    else new HelloApp (helloWindow);
}

void HelloApp::draw (View *from, Port *port, Rect *area)
{
    Rect r;
    from->getClientRect (&r);
    r.move (- r.x1, - r.y1);
    port->setPenColor (PPC_Yellow);
    port->setBgColor (PPC_Gray);
    port->draw (&Text ("Hello, world!", &r));
}

int mwmain ()         // that is a little tricky... ;-)
{
    Driver gui;
    new HelloApp;
    return (gui.run());
}

