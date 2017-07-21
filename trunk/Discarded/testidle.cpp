//
// Micro Windows Implementation
// Testidle.cpp
//

#include "global.h"
#include "model.h"
#include "shape.h"
#include "window.h"
#include "gui.h"

class TestApp : public Model
{
protected:
    Window *myWindow;
    long idleCounter;

public:
    TestApp ()
    {
        myWindow = new Window ("Test Idler", &Rect (0, 0, 300, 100),
                               desktop, this, TRUE, TRUE);
        idleCounter = 0;
        idler->apply (myWindow);
        timer->apply (myWindow);
    }

    BOOL close (View *from)
    {
        idler->remove (myWindow);
        timer->remove (myWindow);
        return (TRUE);
    }

    void fromController (View *from, Controller *ctrl)
    {
        if (ctrl == idler) idleCounter++;
        else
        {
            char buf[20];
            sprintf (buf, "Count: %lu", idleCounter);
            Rect r;
            Port *p;
            myWindow->getClientRect (&r);
            p = myWindow->getClientPort();
            p->setPenColor (PPC_White);
            p->setBgColor (PPC_Gray);
            Text t (buf, &r);
            p->draw (&t);
            idleCounter = 0;
        }
    }

};


int main ()         // that is a little tricky... ;-)
{
    Driver gui;
    new TestApp;
    return (gui.run());
}

