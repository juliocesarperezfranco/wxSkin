/////////////////////////////////////////////////////////////////////////////////////////
// Name:		wxSkin
// Author:      René Kraus aka upCASE <himself@upcase.de>
// Copyright:   René Kraus (c)
// Licence:     wxWindows license <http://www.wxwidgets.org/licence3.txt>
//				with the addendum that you notify the author about projects using wxSkin
/////////////////////////////////////////////////////////////////////////////////////////
#ifdef __GNUG__
  #pragma implementation "wxSkinSimpleFrame.h"
#endif

#include <wxSkinSimpleFrame.h>


#if wxCHECK_VERSION(2, 7, 0)
  #define ISOK IsOk
#else
  #define ISOK Ok
#endif

BEGIN_EVENT_TABLE(wxSkinSimpleFrame,wxSkinFrameBase)
    EVT_PAINT(wxSkinSimpleFrame::OnPaint)
  EVT_SIZE(wxSkinSimpleFrame::OnSize)
#ifdef __WXGTK__
    EVT_WINDOW_CREATE(wxSkinSimpleFrame::OnWindowCreate)
#endif
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(wxSkinSimpleFrame,wxSkinFrameBase)

wxSkinSimpleFrame::wxSkinSimpleFrame()
  : wxSkinFrameBase()
{
}

wxSkinSimpleFrame::wxSkinSimpleFrame(wxWindow* parent,
        int id,
        const wxString &label,
        const wxImage& bmp,
          const wxPoint& pos,
            const wxSize& size,
              long style,
        const wxString& name)
:m_bmpSkin(bmp),m_bmpSkinMaster(bmp),
wxSkinFrameBase(parent,id,label,pos,size,style,name)
{
  m_titlePlacement = wxTITLE_LEFT;
  m_isMovable = false;
  m_bSizeable = false;
  //Create(parent,id,label,pos,size,style|wxFRAME_SHAPED ,name);
  
  if(!bmp.ISOK())
  {
    m_bmpSkin = wxImage(1,1);
    m_bmpSkinMaster = wxImage(1,1);
  }

  wxSize s(m_bmpSkin.GetWidth(), m_bmpSkin.GetHeight());
  SetClientSize(s);
#ifndef __WXGTK__
  SetWindowShape(m_bmpSkin);
#endif
}

wxSkinSimpleFrame::~wxSkinSimpleFrame()
{
}
void wxSkinSimpleFrame::DoSize(wxSize s)
{
  if(m_bmpSkinMaster.ISOK())
  {	m_bmpSkin = m_bmpSkinMaster.Scale(s.GetWidth(),s.GetHeight());
    SetWindowShape(m_bmpSkin);
  }
  
}
void wxSkinSimpleFrame::SetSizer(wxSizer* sizer)
{
  if(m_bmpSkin.ISOK())
  {	
    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* lineSizer = new wxBoxSizer(wxHORIZONTAL);
    topSizer->AddSpacer(rect_clientarea.y);

    lineSizer->AddSpacer(rect_clientarea.x);
    lineSizer->Add(sizer,1,wxEXPAND,0);
    int s = m_bmpSkin.GetWidth()-rect_clientarea.x-rect_clientarea.width;
    lineSizer->AddSpacer(s);

    topSizer->Add(lineSizer,1,wxEXPAND,0);

    s = m_bmpSkin.GetHeight()-rect_clientarea.y-rect_clientarea.height;
    topSizer->AddSpacer(s);

    wxTopLevelWindow::SetSizer(topSizer);
    Layout();
  }
}
void wxSkinSimpleFrame::RecreateSizer()
{
  if(m_bmpSkin.ISOK())
  {	wxSizer* topSizer = GetSizer();

    if(topSizer)
    {	if(topSizer->GetItem((size_t)0) && topSizer->GetItem((size_t)0)->IsSpacer())
        topSizer->GetItem((size_t)0)->AssignSpacer(wxSize(rect_clientarea.y,rect_clientarea.y));
    
      wxSizer* lineSizer = topSizer->GetItem((size_t)1)->GetSizer();
      if(lineSizer && lineSizer->GetItem((size_t)0) &&lineSizer->GetItem((size_t)0)->IsSpacer())
      {	
        lineSizer->GetItem((size_t)0)->AssignSpacer(wxSize(rect_clientarea.x,rect_clientarea.x));
      }
      if(lineSizer && lineSizer->GetItem((size_t)2) && lineSizer->GetItem((size_t)2)->IsSpacer())
      {	
        int s = m_bmpSkin.GetWidth()-rect_clientarea.x-rect_clientarea.width;
        lineSizer->GetItem((size_t)2)->AssignSpacer(wxSize(s,s));
      }

      if(topSizer->GetItem((size_t)2) && topSizer->GetItem((size_t)2)->IsSpacer())
      {	int s = m_bmpSkin.GetHeight()-rect_clientarea.y-rect_clientarea.height;
        topSizer->GetItem((size_t)2)->AssignSpacer(wxSize(s,s));
      }
    }
    Layout();
  }
}
void wxSkinSimpleFrame::OnPaint(wxPaintEvent& event)
{
  wxPaintDC dc(this);
  
  dc.DrawBitmap(m_bmpSkin, 0, 0, true);
  dc.SetTextForeground(GetForegroundColour());

  wxString title = wxTopLevelWindow::GetTitle();
  dc.SetFont(GetFont());
  wxCoord w,h;
  dc.GetTextExtent(title,&w,&h);

  //dc.DrawRectangle(rect_title);
  int x,y;

  y = (rect_title.GetBottom()-h);
  
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
}
#ifdef __WXGTK__
void wxSkinSimpleFrame::OnWindowCreate(wxWindowCreateEvent& evt)
{
    SetWindowShape(m_bmpSkin);
}
#endif

