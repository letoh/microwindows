//
// Micro Windows Implementation
// view.h: the view class
//
// 這裡我們完成了 GUI 裡的重要元件 -- 視界 (views). 一個 view
// 是一塊矩形區域, 可在上面作繪圖動作.
//
// 這個 View class 功能很簡單, 除了有派送訊息的 messageDispatcher
// 外, 另外有關閉或改變視窗狀態的 minimize, maximize, close,
// move, resize 等.
//
// 一個 view 下面也有可有多個 view , 並且管理它們. 另外, 一個
// view 可以和一個 Model class 的 instance 「連結」起來, 詳細
// 的動作請看書內對 MVC 架構的說明.
//
// View class 主要被兩個衍生類別繼承, 一是像 Button, Scrollbar
// 這一類的 Control class (不要和 Controller class 搞混了), 另
// 外是一種叫 "MultiView" 的 class. 後者其實就是具有重疊視窗介
// 面的 view. 透過 C++ 的繼承機制, 我們只要修改幾個 member
// function, 便能將這個演算法移值進去.
//
// $Revision: 1.2 $
// $Source: P:/MWINDOWS/INCLUDE/rcs/view.h $
// $Date: 1993/10/03 15:21:28 $
//

#ifndef __view_h
#define __view_h

#ifndef __viewport_h
#   include "viewport.h"
#endif

class ViewList;
class Message;
class Model;

typedef BYTE ResizePlace;
const ResizePlace MVRP_Top        = 1, MVRP_Bottom     = 2,
                  MVRP_Left       = 4, MVRP_Right      = 8,
                  MVRP_LeftTop    = 5, MVRP_LeftBottom = 6,
                  MVRP_RightTop   = 9, MVRP_RightBottm = 10;

class View : public Object
{
public:
    View (Rect *prect, View *pparent, Model *client, BOOL attach = FALSE,
          BOOL killModel = FALSE);
    ~View();


    // PART 1: 訊息遞送的 method
    // --------------------------
    virtual void    messageDispatcher (Message *msg);
    virtual void    dispatchMouse (Message *msg);


    // PART 2: 關於資料的存取; 為了避免讓其他物件任意更動 viewRect
    //         等的內部資料, 因此 getViewRect 要接收一個參數, 把
    //         viewRect 拷貝到這參數裡
    // ------------------------------------------------------------
    virtual Rect   *getViewRect (Rect *receiver);
    virtual Rect   *getClientRect (Rect *receiver);
    virtual Port   *getClientPort ();
    virtual Port   *getViewPort ();
    virtual Port   *getAttachPort ();


    // PART 3: 和 view 有關的操作, 如縮小 (minimize, 在沒 icon
    //         前是沒用的), 放大 (maximize), 關閉 (close),
    //         重繪 (draw), 搬移 (move), 伸縮 (resize) 等.
    // -------------------------------------------------------
    virtual void    minimize ();
    virtual void    maximize ();
    virtual BOOL    close (BOOL immediate = FALSE);
    virtual void    draw (FreeArea *area);
    virtual void    draw (Rect *area = NULL);
    virtual void    move (Point *offset, View *pparent = NULL);
    virtual void    resize (int width, int height, View *pparent = NULL);


    // PART 4: 只供 view 的親代 (parent) 使用的 method,
    //         包括隱藏／暴露某一塊區域 (hide/show), 啟動／不啟動
    //         (activate, deactivate) 等. hide 和 show 不能供其他
    //         物件使用, 因此接收一個 View *pparent 參數做為核對
    // ----------------------------------------------------------
    virtual void    hide (View *pparent, Rect *area = NULL);
    virtual void    show (View *pparent, Rect *area = NULL);
    virtual void    activate ();
    virtual void    deactivate ();


    // PART 5: view 和 subview 之間的關係, 在 View class 裡
    //         只有 attachSubview 和 removeSubview 能正常工作.
    //         重疊式視窗介面 (overlapped windows) 要到 MultiView
    //         class 才算完備
    // ------------------------------------------------------------
    virtual void    attachSubview (View *subview);
    virtual void    removeSubview (View *subview);

    // PART 6: 以下為供 MultiView class 使用的 method
    // -------------------------------------------------
    virtual void    moveSubview (View *subview, Point *offset);
    virtual void    resizeSubview (View *subview, int width, int height);
    virtual void    moveResizeSubview (View *subview, Point *offset,
                                       int width, int height);
    virtual void    beginMove (View *subview);
    virtual void    beginResize (View *subview, ResizePlace rp);
    virtual void    bringTop (View *subview);


    // PART 7: 雜項 method, 包括要求一塊矩形以供 subview attach 等
    // ------------------------------------------------------------
    virtual Point  *getRandomAttachPoint (Point *receiver);
    virtual Rect   *getRandomAttachRect (Rect *receiver);

protected:

    // PART 8: 和 messageDispatcher 有關的 protected member
    // ----------------------------------------------------
    virtual void    selfDraw (Port *port, Rect *area);
    virtual void    selfMouse (Message *msg);
    virtual void    clientDraw (Rect *area);

    Rect viewRect, clientRect;
    ViewPort *viewPort, *clientPort;
    View *parent;
    Model *clientModel;
    ViewList *subviewList;
    BOOL clientKillable;
};

#endif

