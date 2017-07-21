#include "global.h"
#include "model.h"
#include "window.h"
#include "shape.h"
#include "gui.h"

class HelloApp : public Model
{
public:
	HelloApp ();
	
	virtual void leftButtonDown (View *from, Point *pnt);
	virtual void draw (View *from, Port *port, Rect *area);
	
};	

HelloApp::HelloApp ()
{
	Window *foo = new Window ("Hello!", &Rect (0, 0, 300, 200),
							  desktop, this, TRUE, TRUE);
}

void HelloApp::leftButtonDown (View *from, Point *pnt)
{
	Port *p = from->getClientPort ();
	
	p->setPenColor (PPC_Black);
	p->setBgColor (PPC_White);
	
	Text a ("Hello!", pnt);
	p->draw (&a);
}

void HelloApp::draw (View *from, Port *port, Rect *area)							  
{
	port->setPenColor (PPC_White);
	Rectangle a (area);
	port->draw (&a);
	
	port->draw (&StereoBorder (area, Convex, 3));
}

int mwmain ()
{
	Driver gui;
	new HelloApp;
	return (gui.run ());
	
}	