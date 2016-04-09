/////////////////////////////////////////////////////////////////////////////////////////
// Name:		wxSkin
// Author:      René Kraus aka upCASE <himself@upcase.de>
// Copyright:   René Kraus (c)
// Licence:     wxWindows license <http://www.wxwidgets.org/licence3.txt>
//				with the addendum that you notify the author about projects using wxSkin
/////////////////////////////////////////////////////////////////////////////////////////
#ifndef __WXSKINSLIDER_H
#define __WXSKINSLIDER_H

#ifdef __GNUG__
  #pragma interface "wxSkinSlider.h"
#endif

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include <wx/image.h>
#include <wxSkinWindow.h>
#include <wxSkinEngine.h>


///Implements a simple Slider control. When being dragged it emits a wxEVT_COMMAND_SLIDER_UPDATED. Currently on horizontal.
class wxSkinSlider : public wxSkinWindow
{
private:
  bool m_isvertical;
  int m_maxvalue;
  int m_currentvalue;
  wxRect m_sliderRect;
  bool m_bOver;
  wxImage bmp_fill_orign;
public:
  wxSkinSlider();

  wxSkinSlider(wxWindow* parent,
        int id,
        int max=100,
          const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
              long style = wxNO_BORDER,
        const wxString& name = wxT("wxSkinGauge"));

  ~wxSkinSlider();
  
  wxSize DoGetBestSize() const;
  ///Get the maximum range of the slider
  int GetRange() const;
  ///Get the current value of the slider
  int GetValue() const;
  ///Set the maximum range of the slider
  void SetRange(int);
  ///Set the current value of the slider
  void SetValue(int);
  
  //Get/Set the type of the slider(vertical or horizontal)
  bool GetType() const;
  void SetType(bool type);
  
  virtual void DrawCustom(wxDC& dc);
  virtual void SetCustomSkin(const ControlInfo* info);
private:
    void OnLeftDown(wxMouseEvent& event);
  void OnMouseMotion(wxMouseEvent& event);
  void OnEnterWindowDummy(wxMouseEvent& WXUNUSED(e)){}
  void OnLeaveWindow(wxMouseEvent& e)
  { 
    m_bOver = false;
    if(HasCapture() && !e.LeftIsDown())
      ReleaseMouse();
    Refresh();
  }
  void OnLeftUp(wxMouseEvent& evt);
  DECLARE_DYNAMIC_CLASS(wxSkinSlider)
  DECLARE_EVENT_TABLE()
};

#endif
