/////////////////////////////////////////////////////////////////////////////////////////
// Name:		wxSkin
// Author:      René Kraus aka upCASE <himself@upcase.de>
// Copyright:   René Kraus (c)
// Licence:     wxWindows license <http://www.wxwidgets.org/licence3.txt>
//				with the addendum that you notify the author about projects using wxSkin
/////////////////////////////////////////////////////////////////////////////////////////
#ifdef __GNUG__
	#pragma implementation "wxSkinButton.h"
#endif

#include "wxSkinButton.h"

#if wxCHECK_VERSION(2, 7, 0)
	#define ISOK IsOk
#else
	#define ISOK Ok
#endif

BEGIN_EVENT_TABLE(wxSkinButton,wxSkinWindow)
	EVT_LEFT_DOWN(wxSkinButton::OnLeftClick)
	EVT_LEFT_UP(wxSkinButton::OnLeftRelease)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(wxSkinButton,wxSkinWindow)

//Default
wxSkinButton::wxSkinButton()
	: wxSkinWindow()
{
}
//Passing images
wxSkinButton::wxSkinButton(wxWindow* parent,
 				int id,
 				const wxString& label,
  				const wxPoint& pos,
         		const wxSize& size,
           		long style,
				const wxString& name)
	:wxSkinWindow(parent,id,pos,size,style,name,TYPE_BUTTON)
{
	wxString tmp_lab = label;
	tmp_lab.Replace(wxT("&"),wxT(""));
	SetLabel(tmp_lab);
}

wxSkinButton::~wxSkinButton()
{
}

wxSize wxSkinButton::DoGetBestSize() const
{	
	if(!GetLabel().IsEmpty())
	{	int x, y;
		wxClientDC dc((wxWindow*)this);
		dc.GetTextExtent(GetLabel(),&x,&y);
	
		return	wxSize( x+6, y+6 );
	}
	else
		if(bmp_normal.ISOK())
			return wxSize(bmp_normal.GetWidth(),bmp_normal.GetHeight());

	return wxSize(10,10);
}

void wxSkinButton::SetLabel(const wxString& label)
{
	wxSkinWindow::SetLabel(label);
	Refresh();
}

void wxSkinButton::OnLeftClick(wxMouseEvent& WXUNUSED(event))
{	
	if( IsEnabled() && bmp_state2.ISOK())
	{	
		wxClientDC dc(this);

		int h, w;
		GetSize(&h,&w);
		wxBitmap temp;
		temp = wxBitmap(bmp_state2.Scale(h, w));
		dc.DrawBitmap(temp,0,0);
	}
}

void wxSkinButton::OnLeftRelease(wxMouseEvent& WXUNUSED(event))
{
	if(IsEnabled())
	{	
		Refresh();
		if(HasCapture())
			ReleaseMouse();	
    	wxCommandEvent ev( wxEVT_COMMAND_BUTTON_CLICKED, GetId() );
		ev.SetEventObject(this);
    	GetEventHandler()->ProcessEvent( ev );

	}
}
void wxSkinButton::DrawCustom(wxDC& dc)
{	
	wxString label = GetLabel();
	if(!label.IsEmpty())
	{	int h, w;
		GetSize(&h,&w);

		dc.SetFont(GetFont());
		
		int x, y;
		dc.GetTextExtent(label,&x,&y);
		x = (h - x ) / 2;
		y = (w - y) / 2;
		
		if ( IsEnabled() )
			dc.SetTextForeground(GetForegroundColour());
		else
			dc.SetTextForeground(wxGREY_PEN->GetColour());

		dc.DrawText(label, x , y );
	}
}

