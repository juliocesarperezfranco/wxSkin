/////////////////////////////////////////////////////////////////////////////////////////
// Name:		wxSkin
// Author:      Liyong Zhang <Liyong_zhang@yeah.net>
// Copyright:   Liyong Zhang (c)
// Licence:     wxWindows license <http://www.wxwidgets.org/licence3.txt>
//				with the addendum that you notify the author about projects using wxSkin
/////////////////////////////////////////////////////////////////////////////////////////
#ifdef __GNUG__
  #pragma implementation "wxSkinSplitterWindow.h"
#endif

#include <wxSkinSplitterWindow.h>
#include <wxSkinEngine.h>

BEGIN_EVENT_TABLE(wxSkinSplitterWindow, wxSplitterWindow)
    EVT_MOUSE_EVENTS(wxSkinSplitterWindow::OnMouseEvent)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(wxSkinSplitterWindow,wxSplitterWindow);

void wxSkinSplitterWindow::SetSkin(const ControlInfo* controlInfo)
{
    SplitterWindowControlInfo *info = (SplitterWindowControlInfo*)controlInfo;
    
    bmp_sash = wxSkinEngine::Get()->LoadSkinImage(info->skinDisabled);
    
    bmp_normal_l = wxSkinEngine::Get()->LoadSkinImage(info->skinState1);
    bmp_pressed_l = wxSkinEngine::Get()->LoadSkinImage(info->skinState2);
    bmp_over_l = wxSkinEngine::Get()->LoadSkinImage(info->skinOver);
    
    bmp_normal_r = wxSkinEngine::Get()->LoadSkinImage(info->skinExtra1);
    bmp_pressed_r = wxSkinEngine::Get()->LoadSkinImage(info->skinExtra2);
    bmp_over_r = wxSkinEngine::Get()->LoadSkinImage(info->skinExtra3);
    SetDirect(info->direct);
}

void wxSkinSplitterWindow::SetSashPosition(int position, bool redraw)
{
    // remember the sash position we want to set for later if we can't set it
    // right now (e.g. because the window is too small)
    m_requestedSashPosition = position >= 0 ? position : position + GetWindowSize();
    m_checkRequestedSashPosition = false;

    DoSetSashPosition(m_requestedSashPosition);

    if ( redraw )
    {
        SizeWindows();
    }
}

void wxSkinSplitterWindow::OnEnterSashButton()
{
    SetCursor(wxCursor(wxCURSOR_HAND));
    btn_status = BTN_OVER;
    wxClientDC dc(this);
    DrawSash(dc);
    RedrawIfHotSensitive(true);
}

bool wxSkinSplitterWindow::Unsplit(wxWindow *toRemove)
{
    if ( ! IsSplit() )
        return false;

    int sashPos = 0;
    if ( toRemove == NULL || toRemove == m_windowTwo)
    {
        removedWindow = m_windowTwo ;
        m_windowTwo = (wxWindow *) NULL;
        sashPos = GetWindowSize() - GetBorderSize() - GetSashSize();
    }
    else if ( toRemove == m_windowOne )
    {
        removedWindow = m_windowOne ;
        m_windowOne = (wxWindow *) NULL;
    }
    else
    {
        wxFAIL_MSG(wxT("splitter: attempt to remove a non-existent window"));

        return false;
    }

    m_sashOldPosition = m_sashPosition;

    if ( GetMinimumPaneSize() == 0 || m_permitUnsplitAlways )
    {
        OnUnsplit(removedWindow);
        wxSplitterEvent eventUnsplit(wxEVT_COMMAND_SPLITTER_UNSPLIT, this);
        (void)DoSendEvent(eventUnsplit);
        SetSashPositionAndNotify(sashPos);
        SizeWindows();
    }

    return true;
}

void wxSkinSplitterWindow::OnDoubleClickSash(int x, int y)
{
    // new code should handle events instead of using the virtual functions
    wxSplitterEvent event(wxEVT_COMMAND_SPLITTER_DOUBLECLICKED, this);

    if ( DoSendEvent(event) )
    {
        if (IsSplit())
        {
            if (m_direct)
                Unsplit(m_windowOne);
            else
                Unsplit(m_windowTwo);
        }
        else
        {
            if (m_windowOne == NULL)
                m_windowOne = removedWindow;
            else
                m_windowTwo = removedWindow;

            if (! m_windowOne->IsShown())
                m_windowOne->Show();
            if (! m_windowTwo->IsShown())
                m_windowTwo->Show();
            SetSashPosition(m_sashOldPosition);
        }

    }
    //else: blocked by user
}

void wxSkinSplitterWindow::OnMouseEvent(wxMouseEvent& event)
{
    int x = (int)event.GetX(),
        y = (int)event.GetY();

    if ( GetWindowStyle() & wxSP_NOSASH )
    {
        event.Skip();
        return;
    }

    // with wxSP_LIVE_UPDATE style the splitter windows are always resized
    // following the mouse movement while it drags the sash, without it we only
    // draw the sash at the new position but only resize the windows when the
    // dragging is finished
#if defined( __WXMAC__ ) && defined(TARGET_API_MAC_OSX) && TARGET_API_MAC_OSX == 1
    bool isLive = true ; // FIXME: why?
#else
    bool isLive = HasFlag(wxSP_LIVE_UPDATE);
#endif
    if (event.LeftDown())
    {

        if ( SashHitTest(x, y) )
        {
            SetResizeCursor();

            if ( !isLive )
            {
                // remember the initial sash position and draw the initial
                // shadow sash
                m_sashPositionCurrent = m_sashPosition;

                DrawSashTracker(x, y);
            }

            m_oldX = x;
            m_oldY = y;

            SetResizeCursor();
            
            if (SashButtonHitTest(x, y))
            {
                btn_status = BTN_PRESSED;
                wxClientDC dc(this);
                DrawSash(dc);
            }
            
            return;
        }
    }
    else if (event.LeftUp() && SashButtonHitTest(x, y) && m_dragMode != wxSPLIT_DRAG_DRAGGING)
    {
        SetCursor(* wxSTANDARD_CURSOR);
        if (IsSplit())
        {
            if (m_direct)
                Unsplit(m_windowOne);
            else
                Unsplit(m_windowTwo);
        }
        else
        {
            if (m_windowOne == NULL)
                m_windowOne = removedWindow;
            else
                m_windowTwo = removedWindow;

            if (! m_windowOne->IsShown())
                m_windowOne->Show();
            if (! m_windowTwo->IsShown())
                m_windowTwo->Show();
            SetSashPosition(m_sashOldPosition);
        }
    }   //Click sashButton
    else if (event.LeftUp() && m_dragMode == wxSPLIT_DRAG_DRAGGING)
    {
        // We can stop dragging now and see what we've got.
        m_dragMode = wxSPLIT_DRAG_NONE;

        SetCursor(* wxSTANDARD_CURSOR);

        // exit if unsplit after doubleclick

        // Erase old tracker
        if ( !isLive )
        {
            DrawSashTracker(m_oldX, m_oldY);
        }

        // the position of the click doesn't exactly correspond to
        // m_sashPosition, rather it changes it by the distance by which the
        // mouse has moved
        int diff = m_splitMode == wxSPLIT_VERTICAL ? x - m_oldX : y - m_oldY;

        int posSashOld = isLive ? m_sashPosition : m_sashPositionCurrent;
        int posSashNew = OnSashPositionChanging(posSashOld + diff);
        if ( posSashNew == -1 )
        {
            // change not allowed
            return;
        }

        if ( m_permitUnsplitAlways || m_minimumPaneSize == 0 )
        {
            // Deal with possible unsplit scenarios
            if ( posSashNew == 0 )
            {
                // We remove the first window from the view
                Unsplit(m_windowOne);
            }
            else if ( posSashNew == GetWindowSize() )
            {
                // We remove the second window from the view
                Unsplit(m_windowTwo);
            }
            else
            {
                if (!IsSplit())
                {
                    if (m_windowOne == NULL)
                        m_windowOne = removedWindow;
                    else
                        m_windowTwo = removedWindow;

                    if (! m_windowOne->IsShown())
                        m_windowOne->Show();
                    if (! m_windowTwo->IsShown())
                        m_windowTwo->Show();
                }
                SetSashPositionAndNotify(posSashNew);
            }
        }
        else
        {
            SetSashPositionAndNotify(posSashNew);
        }

        SizeWindows();
    }  // left up && dragging
    else if (event.LeftUp())
    {
        SetCursor(* wxSTANDARD_CURSOR);
    }
    else if ((event.Moving() || event.Leaving() || event.Entering()) && (m_dragMode == wxSPLIT_DRAG_NONE))
    {
        if (event.Leaving() || !SashButtonHitTest(x, y))
        {
            btn_status = BTN_NORMAL;
            wxClientDC dc(this);
            DrawSash(dc);
        }
        else if ( event.Leaving() || !SashHitTest(x, y) )
            OnLeaveSash();
        else if (SashButtonHitTest(x, y))
            OnEnterSashButton();
        else
            OnEnterSash();
    }
    else if (event.Dragging())
    {
        // Start the drag now
        m_dragMode = wxSPLIT_DRAG_DRAGGING;
        int diff = m_splitMode == wxSPLIT_VERTICAL ? x - m_oldX : y - m_oldY;
        if ( !diff )
        {
            // nothing to do, mouse didn't really move far enough
            return;
        }

        int posSashOld = isLive ? m_sashPosition : m_sashPositionCurrent;
        int posSashNew = OnSashPositionChanging(posSashOld + diff);
        if ( posSashNew == -1 )
        {
            // change not allowed
            return;
        }

        if ( posSashNew == m_sashPosition )
            return;

        // Erase old tracker
        if ( !isLive )
        {
            DrawSashTracker(m_oldX, m_oldY);
        }

        if (m_splitMode == wxSPLIT_VERTICAL)
            x = posSashNew;
        else
            y = posSashNew;

        // Remember old positions
        m_oldX = x;
        m_oldY = y;

#ifdef __WXMSW__
        // As we captured the mouse, we may get the mouse events from outside
        // our window - for example, negative values in x, y. This has a weird
        // consequence under MSW where we use unsigned values sometimes and
        // signed ones other times: the coordinates turn as big positive
        // numbers and so the sash is drawn on the *right* side of the window
        // instead of the left (or bottom instead of top). Correct this.
        if ( (short)m_oldX < 0 )
            m_oldX = 0;
        if ( (short)m_oldY < 0 )
            m_oldY = 0;
#endif // __WXMSW__

        // Draw new one
        if ( !isLive )
        {
            m_sashPositionCurrent = posSashNew;

            DrawSashTracker(m_oldX, m_oldY);
        }
        else
        {
            DoSetSashPosition(posSashNew);
            if (!(m_windowOne && m_windowTwo))
            {
                if (m_windowOne == NULL)
                    m_windowOne = removedWindow;
                else
                    m_windowTwo = removedWindow;

                if (! m_windowOne->IsShown())
                    m_windowOne->Show();
                if (! m_windowTwo->IsShown())
                    m_windowTwo->Show();
            }
            m_needUpdating = true;
        }
    }
    else if ( event.LeftDClick() && m_windowOne )
    {
        OnDoubleClickSash(x, y);
    }
    else
    {
        event.Skip();
    }
}

bool wxSkinSplitterWindow::SashHitTest(int x, int y, int tolerance)
{
    int z = m_splitMode == wxSPLIT_VERTICAL ? x : y;
    int hitMin = m_sashPosition - tolerance;
    int hitMax = m_sashPosition + GetSashSize() + tolerance;

    return z >=  hitMin && z <= hitMax;
}

bool wxSkinSplitterWindow::SashButtonHitTest(int x, int y, int tolerance)
{
    int z = m_splitMode == wxSPLIT_VERTICAL ? y : x;
    int l = m_splitMode == wxSPLIT_VERTICAL ? GetSize().y : GetSize().x;
    int hitMin = l / 2 - GetButtonSize() / 2 - tolerance;
    int hitMax = l / 2 + GetButtonSize() / 2 + tolerance;

    return z >=  hitMin && z <= hitMax && SashHitTest(x, y, tolerance);
}

void wxSkinSplitterWindow::DrawSash(wxDC& dc)
{
    int w, h;
    GetSize(&w, &h);
    int border = GetBorderSize();
    
    if (m_direct)
    {
        if (IsSplit())
        {
            if (btn_status == BTN_NORMAL)   
                btn = bmp_normal_l;
            else if (btn_status == BTN_PRESSED)
                btn = bmp_pressed_l;
            else if (btn_status == BTN_OVER)
                btn = bmp_over_l;
        }
        else
        {
            if (m_windowOne)
            {
                if (btn_status == BTN_NORMAL)   
                    btn = bmp_normal_l;
                else if (btn_status == BTN_PRESSED)
                    btn = bmp_pressed_l;
                else if (btn_status == BTN_OVER)
                    btn = bmp_over_l;
            }
            else
            {
                if (btn_status == BTN_NORMAL)   
                    btn = bmp_normal_r;
                else if (btn_status == BTN_PRESSED)
                    btn = bmp_pressed_r;
                else if (btn_status == BTN_OVER)
                    btn = bmp_over_r;
            }
        }
    }
    else
    {
        if (IsSplit())
        {
            if (btn_status == BTN_NORMAL)   
                btn = bmp_normal_r;
            else if (btn_status == BTN_PRESSED)
                btn = bmp_pressed_r;
            else if (btn_status == BTN_OVER)
                btn = bmp_over_r;
        }
        else
        {
            if (m_windowOne)
            {
                if (btn_status == BTN_NORMAL)   
                    btn = bmp_normal_l;
                else if (btn_status == BTN_PRESSED)
                    btn = bmp_pressed_l;
                else if (btn_status == BTN_OVER)
                    btn = bmp_over_l;
            }
            else
            {
                if (btn_status == BTN_NORMAL)   
                    btn = bmp_normal_r;
                else if (btn_status == BTN_PRESSED)
                    btn = bmp_pressed_r;
                else if (btn_status == BTN_OVER)
                    btn = bmp_over_r;
            }
        }
    }
    
    if (m_splitMode == wxSPLIT_VERTICAL)
    {
        int x = m_sashPosition + border;
        int y = border;
  
        dc.DrawBitmap(bmp_sash.Scale(bmp_sash.GetWidth(), h - 2 * border), x, y);

        x = border + m_sashPosition + bmp_sash.GetWidth()  / 2 - btn.GetWidth() / 2;
        y = border + (h - 2 * border) / 2 - btn.GetHeight() / 2;
        dc.DrawBitmap(btn, x, y);
    }
    else
    {
        int y = m_sashPosition + border;
        int x = border;
        
        dc.DrawBitmap(bmp_sash.Scale(w - 2 * border, bmp_sash.GetHeight()), x, y);
        
        y = border + m_sashPosition + bmp_sash.GetHeight() / 2 - btn.GetHeight() / 2;
        x = border + (w - 2 * border) / 2 - btn.GetWidth() / 2;
        dc.DrawBitmap(btn, x, y);
    }
/*
    if (HasFlag(wxSP_3DBORDER))
        wxRendererNative::Get().DrawSplitterBorder
                            (
                                this,
                                dc,
                                GetClientRect()
                            );

    // nor if we're configured to not show it
    if ( HasFlag(wxSP_NOSASH) )
        return;

    wxRendererNative::Get().DrawSplitterSash
                            (
                                this,
                                dc,
                                GetClientSize(),
                                m_sashPosition,
                                m_splitMode == wxSPLIT_VERTICAL ? wxVERTICAL
                                                                : wxHORIZONTAL,
                                m_isHot ? (int)wxCONTROL_CURRENT : 0
                            );
*/
}

void wxSkinSplitterWindow::SizeWindows()
{
    // check if we have delayed setting the real sash position
    if ( m_checkRequestedSashPosition && m_requestedSashPosition != INT_MAX )
    {
        int newSashPosition = m_requestedSashPosition;
        if ( newSashPosition != m_sashPosition )
        {
            DoSetSashPosition(newSashPosition);
        }

        if ( newSashPosition <= m_sashPosition
            && newSashPosition >= m_sashPosition - GetBorderSize() )
        {
            // don't update it any more
            m_requestedSashPosition = INT_MAX;
        }
    }

    int w, h;
    GetClientSize(&w, &h);

    if ( !GetWindow1() && GetWindow2() )
    {
        if ( GetSplitMode() == wxSPLIT_VERTICAL )
            GetWindow2()->SetSize(GetBorderSize() + GetSashSize(), GetBorderSize(),
                              w - 2*GetBorderSize() - GetSashSize(), h - 2*GetBorderSize());
        else
            GetWindow2()->SetSize(GetBorderSize(), GetBorderSize() + GetSashSize(),
                              w - 2*GetBorderSize(), h - 2*GetBorderSize() - GetSashSize());
    }
    else if ( GetWindow1() && !GetWindow2() )
    {
        if ( GetSplitMode() == wxSPLIT_VERTICAL )
            GetWindow1()->SetSize(GetBorderSize(), GetBorderSize(),
                              w - 2*GetBorderSize() - GetSashSize(), h - 2*GetBorderSize());
        else
            GetWindow1()->SetSize(GetBorderSize(), GetBorderSize(),
                              w - 2*GetBorderSize(), h - 2*GetBorderSize() - GetSashSize());
    }
    else if ( GetWindow1() && GetWindow2() )
    {
        const int border = GetBorderSize(),
                  sash = GetSashSize();

        int size1 = GetSashPosition() - border,
            size2 = GetSashPosition() + sash;

        int x2, y2, w1, h1, w2, h2;
        if ( GetSplitMode() == wxSPLIT_VERTICAL )
        {
            w1 = size1;
            w2 = w - 2*border - sash - w1;
            if (w2 < 0)
                w2 = 0;
            h2 = h - 2*border;
            if (h2 < 0)
                h2 = 0;
            h1 = h2;
            x2 = size2;
            y2 = border;
        }
        else // horz splitter
        {
            w2 = w - 2*border;
            if (w2 < 0)
                w2 = 0;
            w1 = w2;
            h1 = size1;
            h2 = h - 2*border - sash - h1;
            if (h2 < 0)
                h2 = 0;
            x2 = border;
            y2 = size2;
        }

        GetWindow2()->SetSize(x2, y2, w2, h2);
        GetWindow1()->SetSize(border, border, w1, h1);
    }

    wxClientDC dc(this);
    DrawSash(dc);

    SetNeedUpdating(false);
}

