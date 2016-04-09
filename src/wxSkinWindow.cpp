/////////////////////////////////////////////////////////////////////////////////////////
// Name:		wxSkin
// Author:      René Kraus aka upCASE <himself@upcase.de>
// Copyright:   René Kraus (c)
// Licence:     wxWindows license <http://www.wxwidgets.org/licence3.txt>
//				with the addendum that you notify the author about projects using wxSkin
/////////////////////////////////////////////////////////////////////////////////////////

#include "wxSkinWindow.h"
#include "wxSkinEngine.h"
#include <wx/image.h>

#ifdef __WXGTK__
  #include "wx/gtk/win_gtk.h"
#elif defined(__WXMAC__)
  #include "wx/mac/private.h"
#endif

#include <wx/dcbuffer.h>

BEGIN_EVENT_TABLE(wxSkinWindow,wxWindow)
  EVT_ERASE_BACKGROUND(wxSkinWindow::OnErase)
  EVT_PAINT(wxSkinWindow::OnPaint)
  EVT_SIZE(wxSkinWindow::OnSize)
  EVT_ENTER_WINDOW(wxSkinWindow::OnEnterWindow)
  EVT_LEAVE_WINDOW(wxSkinWindow::OnLeaveWindow)
END_EVENT_TABLE()

IMPLEMENT_ABSTRACT_CLASS(wxSkinWindow,wxWindow);


wxSkinWindow::wxSkinWindow(wxWindow* parent,
        wxWindowID id,
        const wxPoint& pos,
        const wxSize& size,
        long style,
        const wxString& name,
        const int type)
:wxWindow(parent,id,pos,size,style,name)
{
  m_bInside = false;
  m_bScaleFill = true;
  m_type = type;
  wxSkinEngine::Get()->GetDefaultSkin(this);
}

void wxSkinWindow::SetSkin(const ControlInfo* info)
{
    if (info)
    {
      this->SetNormalSkin(wxSkinEngine::Get()->LoadSkinImage(info->skinState1));
      this->SetState2Skin(wxSkinEngine::Get()->LoadSkinImage(info->skinState2));
      this->SetOverSkin(wxSkinEngine::Get()->LoadSkinImage(info->skinOver));
      this->SetDisabledSkin(wxSkinEngine::Get()->LoadSkinImage(info->skinDisabled));

      this->SetSize(info->measure);
      this->Show(info->shown);

      SetCustomSkin(info);
  }
  else
  {
      this->SetDisabledSkin(wxNullImage);
    this->SetState2Skin(wxNullImage);
    this->SetOverSkin(wxNullImage);
    this->SetNormalSkin(wxImage(1,1));
  }
}

void wxSkinWindow::SetNormalSkin(const wxImage& skin)
{ 
  bmp_normal = skin; 
}
void wxSkinWindow::SetState2Skin(const wxImage& skin)
{ 
  bmp_state2 = skin; 
}
void wxSkinWindow::SetOverSkin(const wxImage& skin)
{
  bmp_over = skin;
}
void wxSkinWindow::SetDisabledSkin(const wxImage& skin)
{ 
  bmp_disabled = skin; 
}
bool wxSkinWindow::SetShape(const wxImage& imgorg)
{
  wxImage img = imgorg;
  wxRegion region;
  if(img.HasAlpha())
    img.ConvertAlphaToMask();

  region = wxRegion(wxBitmap(img));

  return SetShape(region);
}
bool wxSkinWindow::SetShape(const wxBitmap& img)
{
  wxRegion region;
  region = wxRegion(img);
  
  return SetShape(region);
}
bool wxSkinWindow::SetShape(const wxRegion& region)
{
  
#if defined(__WXMSW__) && !defined(__WXWINCE__)
  // The empty region signifies that the shape should be removed from the
    // window.
    if ( region.IsEmpty() )
    {
        if (::SetWindowRgn(GetHwnd(), NULL, TRUE) == 0)
        {
            wxLogLastError(_T("SetWindowRgn"));
            return false;
        }
        return true;
    }

    DWORD noBytes = ::GetRegionData(GetHrgnOf(region), 0, NULL);
    RGNDATA *rgnData = (RGNDATA*) new char[noBytes];
    ::GetRegionData(GetHrgnOf(region), noBytes, rgnData);
    HRGN hrgn = ::ExtCreateRegion(NULL, noBytes, rgnData);
    delete[] (char*) rgnData;
   
    RECT rect;
    DWORD dwStyle =   ::GetWindowLong(GetHwnd(), GWL_STYLE);
    DWORD dwExStyle = ::GetWindowLong(GetHwnd(), GWL_EXSTYLE);
    ::GetClientRect(GetHwnd(), &rect);
    ::AdjustWindowRectEx(&rect, dwStyle, FALSE, dwExStyle);
    ::OffsetRgn(hrgn, -rect.left, -rect.top);

    if (::SetWindowRgn(GetHwnd(), hrgn, TRUE) == 0)
    {
        wxLogLastError(_T("SetWindowRgn"));
        return false;
    }
    return true;
#elif defined(__WXMAC__)
  if ( region.IsEmpty() )
    {
        wxSize sz = GetClientSize();
        wxRegion rgn(0, 0, sz.x, sz.y);
        return SetShape(rgn);
    }

    // Make a copy of the region
    RgnHandle  shapeRegion = NewRgn();
    CopyRgn( (RgnHandle)region.GetWXHRGN(), shapeRegion );

    // Dispose of any shape region we may already have
    RgnHandle oldRgn = (RgnHandle)GetWRefCon( (WindowRef)GetHandle() );
    if ( oldRgn )
        DisposeRgn(oldRgn);

    // Save the region so we can use it later
    SetWRefCon((WindowRef)GetHandle(), (SInt32)shapeRegion);

    // Tell the window manager that the window has changed shape
    ReshapeCustomWindow((WindowRef)GetHandle());
    return true;
#elif defined(__WXGTK__)
  if(region.IsEmpty())
  {
    if(m_wxwindow && !GTK_WIDGET_NO_WINDOW(m_wxwindow))
      gtk_widget_shape_combine_mask(m_wxwindow,NULL,0,0);
    if(m_widget && !GTK_WIDGET_NO_WINDOW(m_widget))
      gtk_widget_shape_combine_mask(m_widget,NULL,0,0);
  }
  else
  {	wxBitmap bmp = region.ConvertToBitmap();
    bmp.SetMask(new wxMask(bmp, *wxBLACK));
    GdkBitmap* mask = bmp.GetMask()->GetBitmap();

    if(m_wxwindow && !GTK_WIDGET_NO_WINDOW(m_wxwindow))
      gtk_widget_shape_combine_mask(m_wxwindow,mask,0,0);
    if(m_widget && !GTK_WIDGET_NO_WINDOW(m_widget))
      gtk_widget_shape_combine_mask(m_widget,mask,0,0);
  }
  return true;
#else
  return false;
#endif
}
void wxSkinWindow::OnSize(wxSizeEvent& e)
{
  if(bmp_normal.ISOK())
  {	int h, w;
    GetSize(&h,&w);
    if(w>0 && h>0)
    {	wxBitmap temp = wxBitmap(bmp_normal.Scale(h+1, w+1));
      SetShape(temp);
    }
  }
}
void wxSkinWindow::OnErase(wxEraseEvent& event)
{
  
}
void wxSkinWindow::OnPaint(wxPaintEvent& e)
{
  wxBufferedPaintDC dcp(this);

  if(m_bScaleFill)
  {	int h, w;
    GetSize(&h,&w);
    
    if(m_bInside && bmp_over.ISOK())
      dcp.DrawBitmap(wxBitmap(bmp_over.Scale(h, w)),0,0,true);
    else if(IsEnabled() && bmp_normal.ISOK())
      dcp.DrawBitmap(wxBitmap(bmp_normal.Scale(h, w)),0,0,true);
    else if( bmp_disabled.ISOK() )
      dcp.DrawBitmap(wxBitmap(bmp_disabled.Scale(h, w)),0,0,true);
  }
  else
  {
    if(m_bInside && bmp_over.ISOK())
      dcp.DrawBitmap(bmp_over,0,0,true);
    if(IsEnabled() && bmp_normal.ISOK())
      dcp.DrawBitmap(bmp_normal,0,0,true);
    else if( bmp_disabled.ISOK() )
      dcp.DrawBitmap(bmp_disabled,0,0,true);
  }


  DrawCustom(dcp);
}
void wxSkinWindow::OnEnterWindow(wxMouseEvent& WXUNUSED(event))
{
  m_bInside = true;
  if(bmp_over.ISOK())
  {
    Refresh();
  }
}

void wxSkinWindow::OnLeaveWindow(wxMouseEvent& WXUNUSED(event))
{
  m_bInside = false;
  if(HasCapture())
    ReleaseMouse();	
  Refresh();
}
