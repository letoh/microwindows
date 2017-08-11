//
// Micro Windows Implementation
// multview.cpp
//
// $Revision: 1.4 $
// $Source: P:/MWINDOWS/LIB/rcs/multview.cpp $
// $Date: 1993/11/27 09:53:32 $
//

#include "multview.h"
#include "viewlist.h"
#include "message.h"
#include "model.h"
#include "global.h"
#include "shape.h"

MultiView::MultiView (Rect *prect, View *pparent, Model *client,
                      BOOL attach, BOOL killModel)
          :View (prect, pparent, client, FALSE, killModel)
{
    subviewList = new ViewList;
    activeView = NULL;
    borderUsing = FALSE;
    borderHitted = FALSE;
    borderView = NULL;
    attachPort = NULL;

    if (attach && parent) parent->attachSubview (this);
}

MultiView::~MultiView ()
{
    if (attachPort) delete attachPort;
}

void MultiView::draw (FreeArea *area)
{
    viewPort->draw (NULL);
    clientPort->draw (NULL);
    if (attachPort) attachPort->draw (NULL);

    FreeAreaBegin(area)
        View::draw (area_tmp);
    FreeAreaEnd
}

void MultiView::draw (Rect *area)
{
    viewPort->draw (NULL);
    clientPort->draw (NULL);
    if (attachPort) attachPort->draw (NULL);
    View::draw (area);
}

BOOL MultiView::close (BOOL immediate)
{
    if (View::close(immediate) == FALSE)
    {
        activeView = (View*)subviewList->first ();
        if (activeView)
        {
            bringTop (activeView);
            activeView->activate();
        }
        return (FALSE);
    }

    activeView = NULL;
    return (TRUE);
}

void MultiView::resize (int width, int height, View *pparent)
{
    View::resize (width, height, pparent);
    if (attachPort)
    {
        attachPort->resize (clientRect.width(), clientRect.height());
	}

    if (clientPort == viewPort) return;

    Rect tmpRect;
    View *tmp = (View*)subviewList->first();
    do
    {
        if (tmp == NULL) break;
        tmp->getViewRect (&tmpRect);
        tmpRect.move (clientRect.x1, clientRect.y1);
        clientPort->hide (&tmpRect);
        tmp = (View*)subviewList->next();
    } while (1);
}


Port *MultiView::getAttachPort ()
{
    if (attachPort == NULL)
    {
        attachPort = new ViewPort (&clientRect, viewPort);
    }

    attachPort->draw (NULL);
    return (attachPort);
}

Port *MultiView::getClientPort ()
{
    viewPort->draw (NULL);
    clientPort->draw (NULL);
    if (attachPort) attachPort->draw (NULL);
    return (clientPort);
}


void MultiView::dispatchMouse (Message *msg)
{
    if (msg->extra == Mouse_Dispatch
        && viewRect.inside (&(msg->pnt)) == FALSE) return;

    (msg->pnt).move (- viewRect.x1, - viewRect.y1);

    Rect tmpRect;
    View *tmp;

    if (activeView && borderUsing == FALSE &&
        clientRect.inside (&(msg->pnt)) )
    {
        Point tmpPnt = msg->pnt;
        tmpPnt.move (- clientRect.x1, - clientRect.y1);

        if ((activeView->getViewRect (&tmpRect))->inside (&tmpPnt) == TRUE)
        {
            msg->pnt = tmpPnt;
            activeView->dispatchMouse (msg);
            return;
        }

        if (msg->type != MSG_LeftButtonDown)
        {
            tmp = NULL;
        }
        else
        {
            tmp = (View*)subviewList->first ();
        }

        do
        {
            if (tmp == NULL) break;
            if ((tmp->getViewRect (&tmpRect))->inside (&tmpPnt) == TRUE)
            {
                View *oldActive = activeView;

                bringTop (tmp);
                activateSubview (tmp);
                msg->pnt = tmpPnt;
                tmp->dispatchMouse (msg);

                oldActive->deactivate ();
                return;
            }
            tmp = (View*)subviewList->next ();
        } while (1);
    }

    selfMouse (msg);
}

void MultiView::attachSubview (View *subview)
{
    if (subviewList->find (subview) == TRUE) return;

    Rect svRect;
    selfHide (subview->getViewRect (&svRect));

    subview->hide (this, NULL);       // hide all
    subviewList->last();
    subviewList->put (subview);

    bringTop (subview);
    activateSubview (subview);
}

void MultiView::removeSubview (View *subview)
{
    if (subviewList->find (subview) == FALSE) return;

    Rect oldRect;
    subview->getViewRect (&oldRect);

    calcOverlapping (subview, NULL, &oldRect);

    subviewList->find (subview);
    subviewList->remove ();

    activeView = (View*)subviewList->first ();

    if (activeView)
    {
        bringTop (activeView);
        activeView->activate();
    }
}

void MultiView::moveSubview (View *subview, Point *offset)
{
    if (subviewList->find (subview) == FALSE) return;
    if (!offset->x && !offset->y) return;

    Rect oldRect, newRect;

    subview->getViewRect (&oldRect);
    subview->move (offset, this);
    subview->getViewRect (&newRect);

    calcOverlapping (subview, &newRect, &oldRect);
}

void MultiView::resizeSubview (View *subview, int width, int height)
{
    if (subviewList->find (subview) == FALSE) return;

    Rect oldRect, newRect;

    subview->getViewRect (&oldRect);
    subview->resize (width, height, this);
    subview->getViewRect (&newRect);

    calcOverlapping (subview, &newRect, &oldRect);
}

void MultiView::moveResizeSubview (View *subview, Point *offset,
                                   int width, int height)
{
    if (subviewList->find (subview) == FALSE) return;

    Rect oldRect, newRect;

    subview->getViewRect (&oldRect);
    subview->move (offset, this);
    subview->resize (width, height, this);
    subview->getViewRect (&newRect);

    calcOverlapping (subview, &newRect, &oldRect);
}

void MultiView::bringTop (View *subview)
{
    if (subviewList->find (subview) == FALSE) return;
    subviewList->bringTop ();

    FreeArea invisible;
    ((ViewPort*)subview->getViewPort())->getInvisibleArea (&invisible);
    subview->show (this, NULL);
    subview->draw (&invisible);

    Rect svRect, dvRect, toHide;
    subview->getViewRect (&svRect);

    View *dv = (View*)subviewList->first();
    while ((dv = (View*)subviewList->next()) != NULL)
    {
        if (svRect.intersection (dv->getViewRect (&dvRect), &toHide)
            == TRUE)
        {
            dv->hide (this, &toHide);
        }
    }
}

void MultiView::beginMove (View *subview)
{
    if (subviewList->find (subview) == FALSE) return;

    borderHitted = FALSE;
    borderUsing = TRUE;
    borderType = MVBdr_Move;
    borderView = subview;
    subview->getViewRect (&borderRect);

    drawBorder();
}

void MultiView::beginResize (View *subview, ResizePlace rp)
{
    if (subviewList->find (subview) == FALSE) return;

    borderHitted = FALSE;
    borderUsing = TRUE;
    borderType = MVBdr_Resize;
    borderView = subview;
    borderPlace = rp;
    subview->getViewRect (&borderRect);

    drawBorder();
}

void MultiView::hide (View *pparent, Rect *area)
{
    View::hide (pparent, area);
    viewPort->draw (NULL);
    clientPort->draw (NULL);
    if (attachPort) attachPort->draw (NULL);
}

void MultiView::show (View *pparent, Rect *area)
{
    View::show (pparent, area);
    viewPort->draw (NULL);
    clientPort->draw (NULL);
    if (attachPort) attachPort->draw (NULL);
}

void MultiView::activateSubview (View *subview)
{
    if (subview != NULL)
    {
        if (subviewList->find (subview) == FALSE) return;
    }

    if (activeView) activeView->deactivate();
    activeView = subview;
    if (activeView) activeView->activate();
}

// -----------------------------------------------------------------
//
// a "changed" (resized/moved/vanished) subview is given here,
// with a new location that it occupied (newRect == NULL if the
// subview is destroyed), and the old location it occupied.
// the calcOverlapping function computes THE CHANGE AMONG
// SUBVIEWS, and then do REQUIRED PROCESSING (send DRAW
// message to EXPOSED VIEW, send HIDE message to OVERLAPPED VIEW)

// a figure:

//      upper 1
//      upper 2
//      oldRect ---> newRect
//      lower 1
//      lower 2
//
// (1) computes the difference between old and new rect
//     (i.e. exposeArea = newRect - oldRect)
// (2) computes the upper region
//     (i.e. realExposedArea = exposeArea - upperArea)
// (3) computes the difference between realExposedArea and lower views:
//     (a) if realExposedArea and view has intersection,
//         the intersection should be hided
//     (b) if not, but the view was originally hided,
//         now the area is exposed, it should be repainted.
//     (c) do (a) and (b) until the bottom of viewList
// (4) and surely don't forget the parent's attachPort, etc....
//
// --------------------------------------------------------------------

// comment: so why should I comment on this member function?
// this is the most MYSTERIOUS part of overlapped window interface... :)

void MultiView::calcOverlapping (View *subview, Rect *newRect,
                                 Rect *oldRect)
{
    View *topView = (View*)subviewList->first();

    FreeArea exposed;
    exposed.add (oldRect);
    if (newRect) exposed.cut (newRect);

    Rect tmpRect;
    FreeArea upper;
    View *tmpView = topView;

    do
    {
        if (tmpView == subview) break;
        upper.add (tmpView->getViewRect (&tmpRect));
        tmpView = (View*)subviewList->next();
    } while (1);

    // now tmpView == subview, next...

    FreeArea toshow;

    if (newRect)
    {
        toshow.add (&exposed);
        toshow.cut (&upper);
        freeAreaHide (subview, &upper);
        if (subview == topView)
        {
            subview->show (this, NULL);
            subview->draw ();
        }
        else
        {
            freeAreaShow (subview, &toshow);
            subview->draw (&toshow);
        }

        upper.add (newRect);
    }

    tmpView = (View*)subviewList->next();

    do
    {
        if (tmpView == NULL) break;

        toshow.purge ();
        toshow.add (&exposed);
        toshow.cut (&upper);

        freeAreaShow (tmpView, &toshow);
        if (newRect) tmpView->hide (this, newRect);
        tmpView->draw (&toshow);
        upper.add (tmpView->getViewRect (&tmpRect));

        tmpView = (View*)subviewList->next ();
    } while (1);

    toshow.purge ();
    toshow.add (&exposed);
    toshow.cut (&upper);
    toshow.move (clientRect.x1, clientRect.y1);
    if (newRect) selfHide (newRect);
    selfShowDraw (&toshow);
}

void MultiView::selfMouse (Message *msg)
{
    Rect origRect;

    if (borderUsing == FALSE)
    {
        View::selfMouse (msg);
        return;
    }

    (msg->pnt).move (- clientRect.x1, - clientRect.y1);

    //
    // deal with move/resize borders
    //
    switch (msg->type)
    {
        case MSG_MouseMove:
             drawBorder();
             break;

        //
        // mouse button hitted, lock mouse and perform
        // "border" functions.
        //
        case MSG_LeftButtonDown:
            if (borderHitted == FALSE)
            {
                borderHitted = TRUE;
                mouse->lock (this);
                borderSpot = msg->pnt;
                return;
            }

            drawBorder ();

            switch (borderType)
            {
                case MVBdr_Move:
                    borderRect.move ((msg->pnt).x - borderSpot.x,
                                     (msg->pnt).y - borderSpot.y);
                    borderSpot = msg->pnt;
                    break;

                case MVBdr_Resize:
                    if (borderRect.width() < 40 || borderRect.height()
                        < 40)
                    {
                        borderSpot = msg->pnt;
                        borderRect.resize (40, 40);
                    }

                    int ox = (msg->pnt).x - borderSpot.x,
                        oy = (msg->pnt).y - borderSpot.y;

                    Point orig (borderRect.x1, borderRect.y1);
                    Point dest (borderRect.x2, borderRect.y2);

                    if (borderPlace & MVRP_Top) orig.y += oy;
                    if (borderPlace & MVRP_Bottom) dest.y += oy;
                    if (borderPlace & MVRP_Left) orig.x += ox;
                    if (borderPlace & MVRP_Right) dest.x += ox;
                    borderRect.set (&orig, &dest);

                    borderSpot = msg->pnt;
                    break;
            }

            drawBorder ();

            return;

        //
        // mouse released, perform necessary change
        //
        case MSG_LeftButtonUp:
            if (borderUsing) borderUsing = FALSE;
            drawBorder();
            if (borderHitted == FALSE) return;

            switch (borderType)
            {
                case MVBdr_Move:
                    borderView->getViewRect (&origRect);
                    moveSubview (borderView,
                                 _(Point (borderRect.x1 - origRect.x1,
                                          borderRect.y1 - origRect.y1)));
                    break;

                case MVBdr_Resize:
                    borderView->getViewRect (&origRect);
                    moveResizeSubview (borderView,
                                       _(Point (borderRect.x1 - origRect.x1,
                                                borderRect.y1 - origRect.y1)),
                                       borderRect.width(),
                                       borderRect.height());
                    break;
            }

            borderHitted = FALSE;
            mouse->unlock ();

            return;
    }
}

void MultiView::drawBorder ()
{
    Port *pp;
    if (attachPort) pp = attachPort; else pp = clientPort;
    PortPenMode p = pp->getPenMode ();
    PortPenColor c = pp->getPenColor ();

    Rectangle a (&borderRect);
    Rectangle b (borderRect.x1 +2, borderRect.y1 +2,
                 borderRect.x2 -2, borderRect.y2 -2);

    pp->setPenMode (PPM_XOR);
    pp->setPenColor (PPC_White);
    pp->draw (&a);
    pp->setPenColor (PPC_Gray);
    pp->draw (&b);
    pp->setPenColor (c);
    pp->setPenMode (p);
}

void MultiView::freeAreaHide (View *subview, FreeArea *area)
{
    FreeAreaBegin(area)
        subview->hide (this, area_tmp);
    FreeAreaEnd
}

void MultiView::freeAreaShow (View *subview, FreeArea *area)
{
    FreeAreaBegin(area)
        subview->show (this, area_tmp);
    FreeAreaEnd
}

void MultiView::selfShowDraw (FreeArea *area)
{
    FreeAreaBegin(area)
        clientPort->show (area_tmp);
        selfDraw (viewPort, area_tmp);
        clientDraw (area_tmp);
    FreeAreaEnd
}

void MultiView::selfHide (Rect *area)
{
    Rect realHide = *area;
    realHide.move (clientRect.x1, clientRect.y1);

    if (clientPort != viewPort) clientPort->hide (&realHide);
}

