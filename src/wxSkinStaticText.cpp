/////////////////////////////////////////////////////////////////////////////////////////
// Name:		wxSkin
// Author:      René Kraus aka upCASE <himself@upcase.de>
// Copyright:   René Kraus (c)
// Licence:     wxWindows license <http://www.wxwidgets.org/licence3.txt>
//				with the addendum that you notify the author about projects using wxSkin
/////////////////////////////////////////////////////////////////////////////////////////
#ifdef __GNUG__
  #pragma implementation "wxSkinStaticText.h"
#endif

#include <wxSkinStaticText.h>
#include <wxSkinEngine.h>


#ifdef __WXMSW__
IMPLEMENT_DYNAMIC_CLASS(wxSkinStaticText,wxSkinWindow)

//Default
wxSkinStaticText::wxSkinStaticText()
  : wxSkinWindow()
{
}
//Passing images
wxSkinStaticText::wxSkinStaticText(wxWindow* parent,
        int id,
        const wxString& label,
          const wxPoint& pos,
            const wxSize& size,
              long style,
        const wxString& name)
  :wxSkinWindow(parent,id,pos,size,style,name,TYPE_STATICTEXT)
{

    SetLabel(label);
}

wxSkinStaticText::~wxSkinStaticText()
{
}

wxSize wxSkinStaticText::DoGetBestSize() const
{	
  return wxSize(bmp_normal.GetWidth(),bmp_normal.GetHeight());

}

void wxSkinStaticText::SetLabel(const wxString& label)
{
  wxSkinWindow::SetLabel(label);
  CreateLabel();
  SetSize(bmp_normal.GetWidth(),bmp_normal.GetHeight());
  Refresh();
}

void wxSkinStaticText::CreateLabel()
{
  wxMemoryDC dc;
  dc.SetFont(GetFont());
  int w,h;
  wxString label = GetLabel();
  dc.GetTextExtent(label,&w,&h);

  wxBitmap bmp(w,h,-1);
  
  dc.SelectObject(bmp);
  dc.SetBackgroundMode(wxTRANSPARENT);

  dc.SetBackground(wxBrush(wxSkinEngine::Get()->GetTransparencyColour()));

  dc.Clear();
  dc.SetTextForeground(GetForegroundColour());
  dc.DrawText(label,0,0);
  dc.SelectObject(wxNullBitmap);

  bmp_normal = bmp.ConvertToImage();
  wxColour col = wxSkinEngine::Get()->GetTransparencyColour();
  bmp_normal.SetMaskColour(col.Red(),col.Green(),col.Blue());

  SetShape(bmp_normal);

}

void wxSkinStaticText::DrawCustom(wxDC& dc)
{	
}
void wxSkinStaticText::SetCustomSkin(const ControlInfo* controlInfo)
{
  StaticTextControlInfo *info = (StaticTextControlInfo*)controlInfo;

    this->SetFont(wxFont(info->fontSize, info->fontFamily, info->style, info->weight, info->underlined, info->faceName));
    this->SetForegroundColour(info->colour);
}

#else
  IMPLEMENT_DYNAMIC_CLASS(wxSkinStaticText,wxStaticText)
#endif
