//
// Micro Windows Implementation
// window.cpp
//
// $Revision: 1.3 $
// $Source: P:/MWINDOWS/LIB/rcs/window.cpp $
// $Date: 1993/11/27 09:53:32 $
//

#include "window.h"
#include "port.h"
#include "shape.h"
#include "message.h"
#include "mouse.h"

Window::Window (const char *captionText, Rect *prect, View *pparent,
                Model *client, BOOL attach, BOOL killModel)
       :MultiView (prect, pparent, client, FALSE, killModel)
{
    activeFlag = FALSE;
    alwaysOnTop = FALSE;
    resizable = TRUE;
    caption = NULL;

    clientRect.move (6, 24);
    clientRect.resize (clientRect.width()-13, clientRect.height()-29);
    clientPort = new ViewPort (&clientRect, viewPort);

    activeFlag = TRUE;
    maximized = FALSE;
    setupWidgets ();
    caption = new Text (captionText, &captionRect);

    if (attach && parent) parent->attachSubview (this);
}

Window::~Window ()
{
    delete caption;
}

void Window::resize (int width, int height, View *pparent)
{
    MultiView::resize (width, height, pparent);
    drawCaption ();
}

void Window::activate ()
{
    activeFlag = TRUE;
    drawCaption ();
}

void Window::deactivate ()
{
    activeFlag = FALSE;
    drawCaption ();

    if (alwaysOnTop) parent->bringTop (this);
}

void Window::setCaption (const char *captionText)
{
    if (caption) delete caption;
    caption = new Text (captionText, &captionRect);
    drawCaption ();
}

void Window::selfDraw (Port *port, Rect *area)
{
    View::selfDraw (port, area);

    Rect origRect = viewRect;
    origRect.move (- viewRect.x1, - viewRect.y1);

    port->draw (_(StereoBorder (&origRect, Convex, 1)));

    origRect.move (2, 2);
    origRect.resize (origRect.width() - 4, origRect.height() - 4);
    port->draw (_(StereoBorder (&origRect, Concave, 1)));

    origRect = viewRect;
    origRect.move (- viewRect.x1, - viewRect.y1);
    origRect.move (5, 23);
    origRect.resize (origRect.width() - 10, origRect.height() - 26);
    port->draw (_(StereoBorder (&origRect, Convex, 1)));

    drawCaption ();
}

void Window::selfMouse (Message *msg)
{
    Point *pnt = &(msg->pnt);
    if (clientRect.inside (pnt) == TRUE || borderUsing == TRUE)
    {
        MultiView::selfMouse (msg);
        return;
    }

    if (msg->type == MSG_LeftButtonDown)
    {
        setupWidgets ();

        if (mouseFirstDown == FALSE)
        {
            mouseFirstDown = TRUE;
            return;
        }

        // is mouse falls in the caption bar?

        if (captionRect.inside (pnt) == TRUE)
        {
            mouseFirstDown = FALSE;
            parent->beginMove (this);   // if yes, perform "move."
            return;
        }

        // is mouse falls in resize border?

        if (resizeBorderRect.inside (pnt) == FALSE && resizable)
        {
            mouseFirstDown = FALSE;     // if yes, perform "resize."
            parent->beginResize (this, getResizePlace (pnt));
            return;
        }
    }

    if (msg->type == MSG_LeftButtonUp)
    {
        mouseFirstDown = FALSE;

        // if mouse is in "close" button ("X")
        if (closeRect.inside (pnt) == TRUE)
        {
            close ();
            return;
        }

        // if mouse is in "maximize" button ("*"/".")
        else if (maximizeRect.inside (pnt) == TRUE)
        {
            if (maximized == TRUE)
            {
                if (parent == NULL) return;
                parent->moveResizeSubview (this, _(Point(origRect.x1,
                        origRect.y1)), origRect.width(), origRect.height());
                maximized = FALSE;
                drawCaption ();
            }
            else
            {
                origRect = viewRect;
                maximized = TRUE;
                maximize ();
                drawCaption ();
            }
        }
    }
}

void Window::setupWidgets ()
{
    Rect origRect = viewRect;
    origRect.move (- viewRect.x1, - viewRect.y1);

    barRect.set (5, 5, origRect.x2 - 5, 21);
    resizeBorderRect.set (4, 4, origRect.x2 - 4, origRect.y2 - 4);

    closeRect.set (barRect.x1, barRect.y1, barRect.x1 + 16, barRect.y2);
    maximizeRect.set (barRect.x2 - 16, barRect.y1, barRect.x2, barRect.y2);
    captionRect.set (closeRect.x2 + 2, barRect.y1, maximizeRect.x1 -2, barRect.y2);
    if (caption) caption->setBoundary (&captionRect);
}

void Window::drawCaption ()
{
    setupWidgets ();

    StereoType t;

    if (maximized) t = Convex; else t = Concave;

//  viewPort->draw (&StereoBorder (&barRect, Concave, 1));
    if (activeFlag) viewPort->setPenColor (PPC_Blue);
    else viewPort->setPenColor (PPC_Gray);
    viewPort->draw (_(FilledRectangle (&captionRect)));

    viewPort->setPenColor (PPC_Black);
    viewPort->setBgColor (PPC_Gray);
    viewPort->draw (_(Text ("X", &closeRect)));
    if (maximized) viewPort->draw (_(Text (".", &maximizeRect)));
    else viewPort->draw (_(Text ("*", &maximizeRect)));

    if (activeFlag)
    {
        viewPort->setPenColor (PPC_White);
        viewPort->setBgColor (PPC_Blue);
    }
    else
    {
        viewPort->setPenColor (PPC_Black);
        viewPort->setBgColor (PPC_Gray);
    }

    viewPort->draw (caption);

    viewPort->draw (_(StereoBorder (&closeRect, Convex, 1)));
    viewPort->draw (_(StereoBorder (&maximizeRect, t, 1)));
    viewPort->draw (_(StereoBorder (&captionRect, Convex, 1)));

}

ResizePlace Window::getResizePlace (Point *pnt)
{
    ResizePlace rp = 0;

    Rect origRect = viewRect;
    origRect.move (- viewRect.x1, - viewRect.y1);

    Rect topRect (0, 0, origRect.x2, 10);
    Rect bottomRect (0, origRect.y2 - 10, origRect.x2, origRect.y2);
    Rect leftRect (0, 0, 10, origRect.y2);
    Rect rightRect (origRect.x2 - 10, 0, origRect.x2, origRect.y2);

    if (topRect.inside (pnt)) rp |= MVRP_Top;
    if (bottomRect.inside (pnt)) rp |= MVRP_Bottom;
    if (leftRect.inside (pnt)) rp |= MVRP_Left;
    if (rightRect.inside (pnt)) rp |= MVRP_Right;
    return (rp);
}

void Window::setResizable (BOOL flag)
{
    resizable = flag;
}

void Window::setAlwaysOnTop (BOOL flag)
{
    alwaysOnTop = flag;
}

