/////////////////////////////////////////////////////////////////////////////////////////
// Name:		wxSkin
// Author:      Liyong Zhang <Liyong_zhang@yeah.net>
// Copyright:   Liyong Zhang (c)
// Licence:     wxWindows license <http://www.wxwidgets.org/licence3.txt>
//				with the addendum that you notify the author about projects using wxSkin
/////////////////////////////////////////////////////////////////////////////////////////
#ifndef __WXSKINSPLITTERWINDOW_H
#define __WXSKINSPLITTERWINDOW_H

#ifdef __GNUG__
  #pragma interface "wxSkinSplitterWindow.h"
#endif

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include <wx/image.h>
#include "wxSkinWindow.h"
///wxSkinStaticText is a transparent text controls with an optional image label.
/**The text controls will be transparent as it is shaped to exactly fit the text displayed.
*/

#include "wx/splitter.h"
#include "wx/renderer.h"
#include "wx/dcclient.h"

class wxSkinSplitterWindow : public wxSplitterWindow
{
public:
  wxSkinSplitterWindow(){}

  wxSkinSplitterWindow(wxWindow* parent,
        int id,
          const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
              long style = wxSP_3D,
        const wxString& name = wxT("wxSkinSplitterWindow"))
      :wxSplitterWindow(parent,id,pos,size,style,name)
  {
        m_sashOldPosition = m_sashPosition;
        m_direct = true; // left(up)
        btn_status = BTN_NORMAL;
    }

  ~wxSkinSplitterWindow(){}

    // Draws the sash
    virtual void DrawSash(wxDC& dc);
    virtual bool SashHitTest(int x, int y, int tolerance = 5);
    bool SashButtonHitTest(int x, int y, int tolerance = 5);

    bool m_checkRequestedSashPosition;

    void SetNeedUpdating(bool needUpdatingValue){ this->m_needUpdating = needUpdatingValue; }
    bool GetNeedUpdating(){ return this->m_needUpdating; }
    bool m_neddUpdating;

    // Resizes subwindows
    virtual void SizeWindows();

    // Called when the sash is double-clicked.
    // The default behaviour is to remove the sash if the
    // minimum pane size is zero.
    virtual void OnDoubleClickSash(int x, int y);

    void SetDirect(bool direct = true) { m_direct = direct; }
    bool GetDirect() { return m_direct; }

    // Gets the sash size
    int GetSashSize() const { return m_splitMode == wxSPLIT_VERTICAL ? bmp_sash.GetWidth() : btn.GetHeight();}

    // Is the window split?
    bool IsSplit() const { return (m_windowTwo != NULL) && (m_windowOne != NULL); }
    int GetButtonSize() const { return m_splitMode == wxSPLIT_VERTICAL ? btn.GetHeight() : btn.GetWidth(); }
    void OnEnterSashButton();
    bool Unsplit(wxWindow *toRemove = (wxWindow *) NULL);

    void OnMouseEvent(wxMouseEvent& event);
    void SetSashPosition(int position, bool redraw = true);

  void SetSkin(const ControlInfo* controlInfo);

private:
    
    enum{
        BTN_NORMAL,
        BTN_PRESSED,
        BTN_OVER
    };
    int btn_status;
    wxImage bmp_sash;
    wxImage bmp_normal_l;
    wxImage bmp_pressed_l;
    wxImage bmp_over_l;
    wxImage bmp_normal_r;
    wxImage bmp_pressed_r;
    wxImage bmp_over_r;

    wxImage btn;
    
    bool m_direct; // split direction
    wxWindow *removedWindow;
    int m_sashOldPosition; // Number of pixels from left or top
    DECLARE_EVENT_TABLE()

  DECLARE_DYNAMIC_CLASS(wxSkinSplitterWindow);
};


#endif

