/////////////////////////////////////////////////////////////////////////////////////////
// Name:		wxSkin
// Author:      René Kraus aka upCASE <himself@upcase.de>
// Copyright:   René Kraus (c)
// Licence:     wxWindows license <http://www.wxwidgets.org/licence3.txt>
//				with the addendum that you notify the author about projects using wxSkin
/////////////////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
  #pragma implementation "wxSkinFrame.h"
#endif

#include <wxSkinFrame.h>
#include <wxSkinEngine.h>

#include <wx/dcbuffer.h>


BEGIN_EVENT_TABLE(wxSkinFrame,wxSkinFrameBase)
    EVT_PAINT(wxSkinFrame::OnPaint)
  EVT_SIZE(wxSkinFrame::OnSize)
#ifdef __WXGTK__
    EVT_WINDOW_CREATE(wxSkinFrame::OnWindowCreate)
#endif

END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(wxSkinFrame,wxSkinFrameBase)

//Default
wxSkinFrame::wxSkinFrame()
  : wxSkinFrameBase()
{
}
//Passing images
wxSkinFrame::wxSkinFrame(wxWindow* parent,
        int id,
        const wxString &label,
        const wxImage& bmp,
          const wxPoint& pos,
            const wxSize& size,
              long style,
        const wxString& name)
:m_bmpSkin(bmp), wxSkinFrameBase(parent,id,label,pos,size,style/*|wxFRAME_SHAPED*/ ,name)
{
  m_titlePlacement = wxTITLE_LEFT;
  m_initial_close_x = -1;
  m_initial_min_x = -1;
  m_isMovable = false;
  m_bSizeable = true;
  
  if(!bmp.ISOK())
  {
    m_bmpSkin = wxImage(1,1);
  }

  wxSize s(m_bmpSkin.GetWidth(), m_bmpSkin.GetHeight());
  SetClientSize(s);
#ifndef __WXGTK__
  SetWindowShape(m_bmpSkin);
#endif
}

wxSkinFrame::~wxSkinFrame()
{
}

void wxSkinFrame::SetTopSkin(const wxImage& img)
{
  skin_top = img;
  rect_title.width = img.GetWidth();
  
}
void wxSkinFrame::SetTopLeftSkin(const wxImage& img)
{
  skin_topleft = img;
  if(skin_topleft.HasAlpha())
    skin_topleft.ConvertAlphaToMask();
}
void wxSkinFrame::SetTopRightSkin(const wxImage& img)
{
  skin_topright = img;
  if(skin_topright.HasAlpha())
    skin_topright.ConvertAlphaToMask();
}

void wxSkinFrame::SetBottomSkin(const wxImage& img)
{
  skin_bottom = img;
  if(skin_bottom.HasAlpha())
    skin_bottom.ConvertAlphaToMask();
}
void wxSkinFrame::SetBottomLeftSkin(const wxImage& img)
{
  skin_bottomleft = img;
  if(skin_bottomleft.HasAlpha())
    skin_bottomleft.ConvertAlphaToMask();
}
void wxSkinFrame::SetBottomRightSkin(const wxImage& img)
{
  skin_bottomright = img;
  if(skin_bottomright.HasAlpha())
    skin_bottomright.ConvertAlphaToMask();
}

void wxSkinFrame::SetLeftSkin(const wxImage& img)
{
  skin_left = img;
  if(skin_left.HasAlpha())
    skin_left.ConvertAlphaToMask();
}
void wxSkinFrame::SetRightSkin(const wxImage& img)
{
  skin_right = img;
  if(skin_right.HasAlpha())
    skin_right.ConvertAlphaToMask();
}

void wxSkinFrame::SetBodySkin(const wxImage& img)
{
  skin_body = img;
  if(skin_body.HasAlpha())
    skin_body.ConvertAlphaToMask();
}
bool wxSkinFrame::SetSizer(wxSizer* sizer)
{
  if(skin_top.ISOK() && skin_topleft.ISOK() && skin_topright.ISOK() &&
    skin_bottom.ISOK() && skin_bottomleft.ISOK() && skin_bottomright.ISOK() &&
    skin_left.ISOK() && skin_right.ISOK())
  {
    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* lineSizer = new wxBoxSizer(wxHORIZONTAL);
    topSizer->AddSpacer(skin_top.GetHeight());

    lineSizer->AddSpacer(skin_left.GetWidth());
    lineSizer->Add(sizer,1,wxEXPAND,0);
    lineSizer->AddSpacer(skin_right.GetWidth());

    topSizer->Add(lineSizer,1,wxEXPAND,0);
    topSizer->AddSpacer(skin_bottom.GetHeight());

    wxTopLevelWindow::SetSizer(topSizer);
    Layout();
    return true;
  }
  return false;
}
bool wxSkinFrame::SetTopSizer(wxSizer* sizer)
{
  wxSizer* topSizer = GetSizer();
  if(topSizer)
  {
    if(topSizer->GetItem((size_t)0))
    {	topSizer->Remove((int)0);
      sizer->SetMinSize(-1,skin_top.GetHeight());
      topSizer->Insert(0,sizer,0,wxEXPAND,0);
    }
    
    Layout();
    return true;
  }
  return false;
}
bool wxSkinFrame::SetBottomSizer(wxSizer* sizer)
{
  wxSizer* topSizer = GetSizer();
  if(topSizer)
  {
    if(topSizer->GetItem((size_t)2))
    {	topSizer->Remove((int)2);
      sizer->SetMinSize(-1,skin_bottom.GetHeight());
      topSizer->Add(sizer,0,wxEXPAND,0);
    }
    
    Layout();
    return true;
  }
  return false;
}
void wxSkinFrame::RecreateSizer()
{
  if(skin_top.ISOK() && skin_topleft.ISOK() && skin_topright.ISOK() &&
    skin_bottom.ISOK() && skin_bottomleft.ISOK() && skin_bottomright.ISOK() &&
    skin_left.ISOK() && skin_right.ISOK())
  {	
    wxSizer* topSizer = GetSizer();

    if(topSizer)
    {	if(topSizer->GetItem((size_t)0) && topSizer->GetItem((size_t)0)->IsSpacer())
        topSizer->GetItem((size_t)0)->AssignSpacer(wxSize(skin_top.GetHeight(),skin_top.GetHeight()));
    
      wxSizer* lineSizer = topSizer->GetItem((size_t)1)->GetSizer();
      if(lineSizer && lineSizer->GetItem((size_t)0) &&lineSizer->GetItem((size_t)0)->IsSpacer())
      {	
        lineSizer->GetItem((size_t)0)->AssignSpacer(wxSize(skin_left.GetWidth(),skin_left.GetWidth()));
      }
      if(lineSizer && lineSizer->GetItem((size_t)2) && lineSizer->GetItem((size_t)2)->IsSpacer())
      {	
        lineSizer->GetItem((size_t)2)->AssignSpacer(wxSize(skin_right.GetWidth(),skin_right.GetWidth()));
      }

      if(topSizer->GetItem((size_t)2) && topSizer->GetItem((size_t)2)->IsSpacer())
      {	topSizer->GetItem((size_t)2)->AssignSpacer(wxSize(skin_bottom.GetHeight(),skin_bottom.GetHeight()));
      }
    }
    Layout();
  }
}
bool wxSkinFrame::CreateFrame(wxSize s)
{
  if(skin_top.ISOK() && skin_topleft.ISOK() && skin_topright.ISOK() &&
    skin_bottom.ISOK() && skin_bottomleft.ISOK() && skin_bottomright.ISOK() &&
    skin_left.ISOK() && skin_right.ISOK())
  {
    
    int w = s.GetWidth();
    int h = s.GetHeight();
  
    int widthTop = w - skin_topleft.GetWidth() - skin_topright.GetWidth();
    if(widthTop <= 0) widthTop = 1;
    int widthBottom = w - skin_bottomleft.GetWidth() - skin_bottomright.GetWidth();
    if(widthBottom <= 0) widthBottom = 1;
    int hLeft = h- skin_topleft.GetHeight() - skin_bottomleft.GetHeight();
    if(hLeft <= 0) hLeft = 1;
    int hRight = h- skin_topright.GetHeight() - skin_bottomright.GetHeight();
    if(hRight <= 0) hRight = 1;

    SetMinSize(wxSize(skin_topleft.GetWidth() + skin_topright.GetWidth()+1,skin_topleft.GetHeight() + skin_bottomleft.GetHeight()+1));

    /*if(m_sizeMode != 0)
      return true;*/

    wxBitmap blitMap(w,h,-1);
    wxMemoryDC dc;
    dc.SelectObject(blitMap);
    dc.SetBackgroundMode(wxTRANSPARENT);
    //dc.SetBackground(wxBrush(wxColour(255,0,255),wxTRANSPARENT));
    dc.SetBackground(wxBrush(wxSkinEngine::Get()->GetTransparencyColour()));

    dc.Clear();

    dc.DrawBitmap(skin_body.Scale(widthTop,hLeft),skin_topleft.GetWidth(),skin_top.GetHeight(),true); 

    dc.DrawBitmap(skin_topleft,0,0,true);
    dc.DrawBitmap(skin_top.Scale(widthTop,skin_top.GetHeight()),skin_topleft.GetWidth(),0,true);
    dc.DrawBitmap(skin_topright,widthTop+skin_topleft.GetWidth(),0,true);

    rect_title.x = skin_topleft.GetWidth();
    rect_title.width = widthTop;

    if(closebox_relateto == RELATE_TOP)
    {
      if(m_bClose)
      {	wxRect rect_close = m_bClose->GetRect();
      
        if(m_initial_close_x == -1)
          m_initial_close_x = rect_close.x;

        int change = widthTop - skin_top.GetWidth();
        rect_close.x = m_initial_close_x+ change;
        if(rect_close.x < 0)
          rect_close.x = skin_topleft.GetWidth();

        m_bClose->SetSize(rect_close);
      }
    }
    if(minimizebox_relateto == RELATE_TOP)
    {
      if(m_bMin)
      {	wxRect rect_min = m_bMin->GetRect();

        if(m_initial_min_x == -1)
          m_initial_min_x = rect_min.x;

        int change = widthTop - skin_top.GetWidth();
        rect_min.x = m_initial_min_x+ change;
        if(rect_min.x < 0)
          rect_min.x = skin_topleft.GetWidth();
        
        m_bMin->SetSize(rect_min);
      }
    }
    

    dc.DrawBitmap(skin_bottomleft,0, h-skin_bottomleft.GetHeight(),true);
    dc.DrawBitmap(skin_bottom.Scale(widthBottom,skin_bottom.GetHeight()),skin_bottomleft.GetWidth(), h-skin_bottom.GetHeight(),true);
    dc.DrawBitmap(skin_bottomright,widthBottom+skin_bottomleft.GetWidth(), h-skin_bottomright.GetHeight(),true);

    
    dc.DrawBitmap(skin_left.Scale(skin_left.GetWidth(),hLeft),0,skin_topleft.GetHeight(),true);
    dc.DrawBitmap(skin_right.Scale(skin_right.GetWidth(),hRight),w-skin_right.GetWidth(),skin_topright.GetHeight(),true);

    dc.SelectObject(wxNullBitmap);

    m_bmpSkin = blitMap.ConvertToImage();
    wxColour col = wxSkinEngine::Get()->GetTransparencyColour();
    m_bmpSkin.SetMaskColour(col.Red(),col.Green(),col.Blue());

    return true;
  }
  return false;
}
void wxSkinFrame::OnSize(wxSizeEvent& e)
{	
  if( CreateFrame(GetClientSize()))
  {	SetWindowShape(m_bmpSkin);
  }
  /*if(m_sizeMode == 0)
  {*/
    Refresh();
    Layout();
  //}
    
  e.Skip();
}
void wxSkinFrame::OnPaint(wxPaintEvent& event)
{
  wxBufferedPaintDC dc(this);

  /*if( m_sizeMode == 0)
  {*/	dc.DrawBitmap(m_bmpSkin, 0, 0, true);
    dc.SetTextForeground(GetForegroundColour());

    wxString title = wxTopLevelWindow::GetTitle();
    wxCoord w,h;
    dc.GetTextExtent(title,&w,&h);

    //dc.DrawRectangle(rect_title);
    int x,y;
    y = (rect_title.GetHeight()-h)/2;

    switch(m_titlePlacement)
    {
      case wxTITLE_LEFT:
        x = rect_title.GetX()+1;
      break;
      case wxTITLE_MIDDLE:
        x = rect_title.GetLeft() + rect_title.GetWidth()/2 - w/2;
      break;
      case wxTITLE_RIGHT:
        x = rect_title.GetRight()-w;
    }
    
    dc.DrawText(title,x,y);
  /*}
  else
  {	dc.Clear();
    dc.DrawRectangle(GetClientRect());
  }*/
  
}
#ifdef __WXGTK__
void wxSkinFrame::OnWindowCreate(wxWindowCreateEvent& evt)
{
    SetWindowShape(m_bmpSkin);
}
#endif
