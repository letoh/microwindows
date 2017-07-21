//
// Micro Windows Implementation
// view.cpp
//
// $Revision: 1.3 $
// $Source: P:/MWINDOWS/LIB/rcs/view.cpp $
// $Date: 1993/11/27 09:53:32 $
//

#include "view.h"
#include "viewlist.h"
#include "message.h"
#include "model.h"
#include "global.h"
#include "shape.h"

View::View (Rect *prect, View *pparent, Model *client, BOOL attach,
            BOOL killModel)
     : viewRect (*prect), parent (pparent), clientModel (client),
       clientKillable (killModel)
{
    clientRect = viewRect;
    clientRect.move (- viewRect.x1, - viewRect.y1);

    subviewList = NULL;

    if (parent) viewPort = new ViewPort (prect, parent->getAttachPort());
    else viewPort = new ViewPort (prect);
    clientPort = viewPort;

    if (attach && parent) parent->attachSubview (this);
}

View::~View()
{
    if (subviewList) delete subviewList;
    if (clientPort != viewPort) delete clientPort;
    delete viewPort;
}

//
// default message dispatcher for View class
//
void View::messageDispatcher (Message *msg)
{
    // if message is already dispatched (i.e. the receiver is set.)
    if (msg->receiver && (msg->receiver != this))
    {
        msg->receiver->messageDispatcher (msg);
        return;
    }

    // if the message is sent by controller
    if (msg->controller)
    {
        if (msg->controller == mouse)
        {
            dispatchMouse (msg);    // hand in to mouse message dispatcher
            return;
        }
        else    // unrecognizable message, dispatch to clientModel
        {       // (e.g. messages from idler/timer go by this way)
            if (clientModel)
                clientModel->fromController (this, msg->controller);
        }
    }
}

void View::dispatchMouse (Message *msg)
{
    if (msg->extra == Mouse_Dispatch
        && viewRect.inside (&(msg->pnt)) == FALSE) return;

    (msg->pnt).move (- viewRect.x1, - viewRect.y1);

    Rect tmpRect;
    View *tmp;

    if (subviewList)        // dispatch to subviews first!
    {
        Point tmpPnt = msg->pnt;
        tmpPnt.move (- clientRect.x1, - clientRect.y1);

        tmp = (View*)subviewList->first();
        do
        {
            if (tmp == NULL) break;
            if ((tmp->getViewRect (&tmpRect))->inside (&tmpPnt))
            {
                msg->pnt = tmpPnt;
                tmp->dispatchMouse (msg);
                return;
            }

            tmp = (View*)subviewList->next();
        } while (1);
    }

    selfMouse (msg);        // if not, handle by myself.
}

Rect *View::getViewRect (Rect *receiver)
{
    *receiver = viewRect;
    return (receiver);
}

Rect *View::getClientRect (Rect *receiver)
{
    *receiver = clientRect;
    return (receiver);
}

Port *View::getClientPort ()
{
    return (clientPort);
}

Port *View::getViewPort ()
{
    return (viewPort);
}

Port *View::getAttachPort ()
{
    return (clientPort);
}

void View::minimize ()
{
}

void View::maximize ()
{
    if (parent == NULL) return;

    Rect parentCli;

    parent->getClientRect (&parentCli);
    parent->moveResizeSubview (this, &Point (- viewRect.x1, - viewRect.y1),
                               parentCli.width(), parentCli.height());
}

void View::move (Point *offset, View *pparent)
{
    if (pparent == NULL && parent)
    {
        parent->moveSubview (this, offset);
        return;
    }

    viewPort->move (offset->x , offset->y);
    viewRect.move (offset->x , offset->y);
}

void View::resize (int width, int height, View *pparent)
{
    if (pparent == NULL && parent)
    {
        parent->resizeSubview (this, width, height);
    }

    clientRect.resize (clientRect.width() + (width - viewRect.width()),
                       clientRect.height() + (height - viewRect.height()));
    viewRect.resize (width, height);

	if (clientPort != viewPort)
	{
		clientPort->resize (clientRect.width(), clientRect.height());
	}

	viewPort->resize (viewRect.width(), viewRect.height());
}

void View::activate ()
{
}

void View::deactivate ()
{
}

BOOL View::close (BOOL immediate)
{
    if (clientModel) if (clientModel->close(this) == FALSE) return (FALSE);

    if (subviewList)    // terminate subviews first.
    {
        View *subview = (View*)subviewList->first();
        do
        {
            View *next = (View*)subviewList->next();
            if (subview == NULL) break;
            if (subview->close(TRUE) == FALSE) return (FALSE);
            subviewList->find (subview);
            subviewList->remove ();
            subviewList->find (next);
            subview = (View*)subviewList->get ();
        } while (1);
    }

    if (clientModel && clientKillable) delete clientModel;

    // if immediate == FALSE, call parent to remove myself.
    // (via a complicated way.)

    if (parent && !immediate) parent->removeSubview (this);
    delete this;

    return (TRUE);
}

void View::draw (FreeArea *area)
{
    FreeAreaBegin(area)
        draw (area_tmp);
    FreeAreaEnd
}

void View::draw (Rect *area)
{
    Rect realToDraw = viewRect;

    if (area != NULL)       // computes the area to draw
    {
        if (viewRect.intersection (area, &realToDraw) == FALSE)
            return;
    }

    realToDraw.move (- viewRect.x1, - viewRect.y1);

    selfDraw (viewPort, &realToDraw);   // ask myself to draw
    clientDraw (&realToDraw);       // ask clientModel to draw

    realToDraw.move (- clientRect.x1, - clientRect.y1);

    if (subviewList)        // then finally demand subviews to view
    {
        View *subview = (View*)subviewList->first();
        do
        {
            if (subview == NULL) break;
            subview->draw (&realToDraw);
            subview = (View*)subviewList->next ();
        } while (1);
    }
}


void View::hide (View *pparent, Rect *area)
{
    if (pparent != parent) return;
    viewPort->hide (area);
}

void View::show (View *pparent, Rect *area)
{
    if (pparent != parent) return;
    viewPort->show (area);
}

void View::attachSubview (View *subview)
{
    if (subviewList == NULL) return;

    if (subviewList->find (subview) == TRUE) return;
    subviewList->last ();
    subviewList->put (subview);
    subview->draw ();
}

void View::removeSubview (View *subview)
{
    if (subviewList == NULL) return;

    if (subviewList->find (subview) == FALSE) return;

    Rect subviewRect, toDrawRect, realDraw;
    View *toDraw;

    subview->getViewRect (&subviewRect);

    selfDraw (viewPort, &subviewRect);
    clientDraw (&subviewRect);

    while ((toDraw = (View*)subviewList->next()) != NULL)
    {
        toDraw->getViewRect (&toDrawRect);
        if (subviewRect.intersection (&toDrawRect, &realDraw) == TRUE)
        {
            toDraw->draw (&realDraw);
        }
    }

    subviewList->find (subview);
    subviewList->remove ();
}

void View::moveSubview (View *subview, Point *offset)
{
    subview->move (offset, this);
}

void View::resizeSubview (View *subview, int width, int height)
{
    subview->resize (width, height, this);
}

void View::moveResizeSubview (View *subview, Point *offset,
                              int width, int height)
{
    moveSubview (subview, offset);
    resizeSubview (subview, width, height);
}

void View::clientDraw (Rect *area)
{
    Rect realToDraw;

    if (area->intersection (&clientRect, &realToDraw) == FALSE) return;

    if (clientModel)
    {
        realToDraw.move (- clientRect.x1, - clientRect.y1);
        clientModel->draw (this, clientPort, &realToDraw);
    }
}

void View::beginMove (View *subview)
{
}

void View::beginResize (View *subview, ResizePlace rp)
{
}

void View::selfDraw (Port *port, Rect *area)
{
    port->setPenColor (PPC_Gray);
    port->draw (&FilledRectangle (area));
}


//
// standard mouse message dispatcher
//
void View::selfMouse (Message *msg)
{
    if (clientModel)
    {
        if (clientRect.inside (&(msg->pnt)) == TRUE)
        {
            (msg->pnt).move (- clientRect.x1, - clientRect.y1);

            switch (msg->type)
            {
                case MSG_MouseMove:
                    clientModel->mouseMove (this, &(msg->pnt));
                    return;

                case MSG_LeftButtonDown:
                    clientModel->leftButtonDown (this, &(msg->pnt));
                    return;

                case MSG_LeftButtonUp:
                    clientModel->leftButtonUp (this, &(msg->pnt));
                    return;
            }
        }
    }
}

Point *View::getRandomAttachPoint (Point *receiver)
{
    View *tmp = (View*)subviewList->first();

    if (tmp == NULL) receiver->set (20, 20);
    else
    {
        Rect r;
        tmp->getViewRect (&r);
        receiver->set (r.x1 + 20, r.x2 + 20);
    }

    return (receiver);
}

Rect *View::getRandomAttachRect (Rect *receiver)
{
    View *tmp = (View*)subviewList->first();

    if (tmp == NULL) receiver->set (20, 20, 270, 220);
    else
    {
        Rect r;
        tmp->getViewRect (&r);
        *receiver = r;
        receiver->move (20, 20);
        receiver->resize (250, 200);
    }

    return (receiver);
}

void View::bringTop (View *subview)
{
}

