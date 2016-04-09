/////////////////////////////////////////////////////////////////////////////////////////
// Name:		wxSkin
// Author:      René Kraus aka upCASE <himself@upcase.de>
// Copyright:   René Kraus (c)
// Licence:     wxWindows license <http://www.wxwidgets.org/licence3.txt>
//				with the addendum that you notify the author about projects using wxSkin
/////////////////////////////////////////////////////////////////////////////////////////
#ifndef __WXSKINWINDOW_H
#define __WXSKINWINDOW_H

#include <wx/wx.h>
#include <wx/image.h>

typedef struct TAGFRAMESIMPLE{
  wxString skinFile;
  wxString guibuilderFile;

  wxRect closebox;
  wxString skin_closebox;
  wxString skin_closebox_over;
  wxString skin_closebox_press;

  wxRect maximizebox;
  wxString skin_maximizebox;
  wxString skin_maximizebox_over;
  wxString skin_maximizebox_press;

  wxRect minimizebox;
  wxString skin_minimizebox;
  wxString skin_minimizebox_over;
  wxString skin_minimizebox_press;

  wxRect titlebar;
  wxRect clientarea;
  int titleposition;
  wxFont font;
  bool movable;
}SimpleFrameInfo;
WX_DECLARE_STRING_HASH_MAP( SimpleFrameInfo, MapOfSimpleFrames );

typedef struct TAGFRAMECOMPLEX{
  
  wxString guibuilderFile;

  wxString skin_top;
  wxString skin_topleft;
  wxString skin_topright;

  wxString skin_bottom;
  wxString skin_bottomleft;
  wxString skin_bottomright;

  wxString skin_left;
  wxString skin_right;

  wxString skin_body;

  wxRect closebox;
  int closebox_relateto;
  wxString skin_closebox;
  wxString skin_closebox_over;
  wxString skin_closebox_press;

  wxRect maximizebox;
  wxString skin_maximizebox;
  wxString skin_maximizebox_over;
  wxString skin_maximizebox_press;

  wxRect minimizebox;
  int minimizebox_relateto;
  wxString skin_minimizebox;
  wxString skin_minimizebox_over;
  wxString skin_minimizebox_press;

  //wxRect titlebar;
  int titlebarheight;
  int titleposition;
  wxFont font;
  bool movable;
}ComplexFrameInfo;
WX_DECLARE_STRING_HASH_MAP( ComplexFrameInfo, MapOfComplexFrames );

/*
typedef struct TAGCONTROL{
  int type;
  bool shown;
  wxRect measure;
  wxString skinState1;
  wxString skinState2;
  wxString skinOver;
  wxString skinDisabled;
  wxString skinExtra;
}ControlInfo;
*/

class ControlInfo{
public:
  bool shown;
  wxRect measure;
  wxString skinState1;
  wxString skinState2;
  wxString skinOver;
  wxString skinDisabled;
};

class SliderControlInfo : public ControlInfo{
public:
    bool isVertical;
    wxString skinFill;
};

class StaticTextControlInfo : public ControlInfo{
public:
    bool underlined;
    wxFontWeight weight;
    int style;
    wxString faceName;
    wxFontFamily fontFamily;
    int fontSize;
    wxColour colour;
};

class SplitterWindowControlInfo : public ControlInfo{
public:
    wxString skinExtra1;
    wxString skinExtra2;
    wxString skinExtra3;
    bool direct;
};

WX_DECLARE_STRING_HASH_MAP( ControlInfo*, MapOfControls );

typedef struct TAGCUSTOMWIDGET
{	wxRect measure;
  bool shown;
}CustomWidgetInfo;

WX_DECLARE_STRING_HASH_MAP( CustomWidgetInfo, MapOfWidgets );

WX_DECLARE_STRING_HASH_MAP( wxWindow*, MapOfAssignments );



enum{
  TYPE_BUTTON,
  TYPE_CHECKBOX,
  TYPE_RADIOBUTTON,
  TYPE_GAUGE,
  TYPE_SLIDER,
  TYPE_RADIOBUTTTON,
  TYPE_PANEL,

  TYPE_STATICTEXT,
  TYPE_SPLITTERWINDOW
};


///wxSkinWindow is the abstract base class for all skinned controls in wxSkin
/**wxSkinWindow manages the skin files for all controls in wxSkin. In addition to that it handles the most basic
events, like painting and sizing a control. Drawing the "over" state is also done here.
*/
class wxSkinWindow : public wxWindow
{
  bool m_bInside;
protected:
  wxImage bmp_normal;
  wxImage bmp_state2;
  wxImage bmp_over;
  wxImage bmp_disabled;
  wxImage bmp_fill;
  int m_type;
  bool m_bScaleFill;
public:
  ///Contructor
  wxSkinWindow(){};
  ///Standard Contructor
  wxSkinWindow(wxWindow* parent,
        wxWindowID id,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxNO_BORDER,
        const wxString& name = wxPanelNameStr,
        const int type = -1);

    ///Set the skin of window
    void SetSkin(const ControlInfo* info);

  ///Set the normal, meaning standard skin
  void SetNormalSkin(const wxImage& skin = wxNullImage);
  ///Set the "second state" skin
  /**What this skin image actually contains is up to the control derived from wxSkinWindow.
  For a button for example this is the skin in "pressed" state.
  */
  void SetState2Skin(const wxImage& skin = wxNullImage);
  ///Set the skin displayed when the mouse is over the control.
  void SetOverSkin(const wxImage& skin = wxNullImage);
  ///Set a skin for disabled state.
  void SetDisabledSkin(const wxImage& skin = wxNullImage);

  bool SetShape(const wxRegion& region);
  bool SetShape(const wxImage& img);
  bool SetShape(const wxBitmap& img);

  ///Sets "scaled" mode. In "scaled" mode the skin will be scaled to exactly mathc the controls size.
  void SetScaleFill(bool b){ m_bScaleFill = b;}

  int GetSkinWindowType(){ return m_type; }


  ///Overwrite this method in derived classses
  /**This method will be called after the standard drawing action has been completed. This allows to do some non-standard
  drawing in dervied controls.
  */
  virtual void DrawCustom(wxDC& dc) = 0;
  
  virtual bool HasTransparentBackground() const {return true;}
    
    virtual void SetCustomSkin(const ControlInfo* info) = 0;
private:

  void OnErase(wxEraseEvent& e);
  void OnSize(wxSizeEvent& e);
  void OnPaint(wxPaintEvent& e);
  void OnEnterWindow(wxMouseEvent& WXUNUSED(event));
  void OnLeaveWindow(wxMouseEvent& WXUNUSED(event));

  DECLARE_ABSTRACT_CLASS(wxSkinWindow);
  DECLARE_EVENT_TABLE()
};
#endif //__WXSKINWINDOW_H
