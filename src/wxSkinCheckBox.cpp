/////////////////////////////////////////////////////////////////////////////////////////
// Name:		wxSkin
// Author:      René Kraus aka upCASE <himself@upcase.de>
// Copyright:   René Kraus (c)
// Licence:     wxWindows license <http://www.wxwidgets.org/licence3.txt>
//				with the addendum that you notify the author about projects using wxSkin
/////////////////////////////////////////////////////////////////////////////////////////
#ifdef __GNUG__
	#pragma implementation "wxSkinCheckBox.h"
#endif

#include "wxSkinCheckBox.h"
#include "wxSkinEngine.h"

#if wxCHECK_VERSION(2, 7, 0)
	#define ISOK IsOk
#else
	#define ISOK Ok
#endif

BEGIN_EVENT_TABLE(wxSkinCheckBox,wxSkinWindow)
	EVT_LEFT_DOWN(wxSkinCheckBox::OnLeftClick)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(wxSkinCheckBox,wxSkinWindow)

wxSkinCheckBox::wxSkinCheckBox()
	: wxSkinWindow()
{
	m_checked = false;
}

wxSkinCheckBox::wxSkinCheckBox(wxWindow* parent,
 				int id,
				const wxString& label,
  				const wxPoint& pos,
         		const wxSize& size,
           		long style,
				const wxString& name)
	:wxSkinWindow(parent,id,pos,size,style,name,TYPE_CHECKBOX)
{
	SetScaleFill(true);
	m_checked = false;

	wxString tmp_lab = label;
	tmp_lab.Replace(wxT("&"),wxT(""));
	SetLabel(tmp_lab);
}

wxSkinCheckBox::~wxSkinCheckBox()
{
}

void wxSkinCheckBox::SetLabel(const wxString& label)
{
	wxSkinWindow::SetLabel(label);
	Refresh();
}
wxSize wxSkinCheckBox::DoGetBestSize() const
{
	/*int x, y;
	wxClientDC dc((wxWindow*)this);
 	dc.SetFont(GetFont());
	dc.GetTextExtent(GetLabel(),&x,&y);
	
	if(y > bmp_normal.GetHeight() )
		return	wxSize( x+bmp_normal.GetWidth()+5, y );
	else
		return	wxSize( x+bmp_normal.GetWidth()+5, bmp_normal.GetHeight() );
*/
	if(bmp_normal.ISOK())
		return	wxSize( bmp_normal.GetWidth(), bmp_normal.GetHeight() );
	return wxSize(25,25);
}

void wxSkinCheckBox::DrawCustom(wxDC& dc)
{
	wxBitmap temp;
	if( m_checked && bmp_state2.ISOK() )
	{	temp = wxBitmap(bmp_state2);
		dc.DrawBitmap( temp, 0,0, true);
	}
	
	/*int x,y;
	GetPosition(&x,&y);

	int xext,yext;

	dc.SetFont(GetFont());
	dc.GetTextExtent(GetLabel(),&xext,&yext);

	if ( IsEnabled() )
		dc.SetTextForeground(GetForegroundColour());
	else
		dc.SetTextForeground(wxGREY_PEN->GetColour());
	if(yext > temp.GetHeight()+1)
		y -= temp.GetHeight()/2;

	dc.DrawText(GetLabel(), x+temp.GetWidth() + 5 , y );*/

}

void wxSkinCheckBox::OnLeftClick(wxMouseEvent& WXUNUSED(event))
{
	if( IsEnabled() && !m_checked)
	{
		m_checked = true;
		wxCommandEvent e(wxEVT_COMMAND_CHECKBOX_CLICKED, GetId());
		e.SetInt(1);
		e.SetEventObject(this);
		GetEventHandler()->ProcessEvent(e);
	}
	else
	{
		
		m_checked = false;
		wxCommandEvent e(wxEVT_COMMAND_CHECKBOX_CLICKED, GetId());
		e.SetInt(0);
		e.SetEventObject(this);
		GetEventHandler()->ProcessEvent(e);
	}
	Refresh();
}

bool wxSkinCheckBox::GetValue() const
{
	return m_checked;
}
bool wxSkinCheckBox::IsChecked() const
{
	return m_checked;
}
void wxSkinCheckBox::SetValue(const bool state)
{
	m_checked = state;
	Refresh();
}

