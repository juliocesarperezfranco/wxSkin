/////////////////////////////////////////////////////////////////////////////////////////
// Name:		wxSkin
// Author:      René Kraus aka upCASE <himself@upcase.de>
// Copyright:   René Kraus (c)
// Licence:     wxWindows license <http://www.wxwidgets.org/licence3.txt>
//				with the addendum that you notify the author about projects using wxSkin
/////////////////////////////////////////////////////////////////////////////////////////
#ifdef __GNUG__
	#pragma implementation "wxSkinRadiobutton.h"
#endif

#include "wxSkinRadiobutton.h"
#include "wxSkinEngine.h"

bool wxSkinRadiobutton::s_checkdone=false;

BEGIN_EVENT_TABLE(wxSkinRadiobutton,wxSkinWindow)
	EVT_LEFT_DOWN(wxSkinRadiobutton::OnLeftClick)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(wxSkinRadiobutton,wxSkinWindow)

wxSkinRadiobutton::wxSkinRadiobutton()
	: wxSkinWindow()
{
	m_checked = false;
}

wxSkinRadiobutton::wxSkinRadiobutton(wxWindow* parent,
 				int id,
				const wxString& label,
  				const wxPoint& pos,
         		const wxSize& size,
           		long style,
				const wxString& name)
	:wxSkinWindow(parent,id,pos,size,style,name,TYPE_RADIOBUTTTON)
{
	SetScaleFill(false);
	m_checked = false;

	wxString tmp_lab = label;
	tmp_lab.Replace(wxT("&"),wxT(""));
	SetLabel(tmp_lab);

	//StartCheck();

	/*Connect(wxID_ANY,
            wxEVT_COMMAND_RADIOBUTTON_SELECTED,
            (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &wxSkinRadiobutton::OnRadio );*/
}

wxSkinRadiobutton::~wxSkinRadiobutton()
{
}

void wxSkinRadiobutton::SetLabel(const wxString& label)
{
	wxSkinWindow::SetLabel(label);
	Refresh();
}
wxSize wxSkinRadiobutton::DoGetBestSize() const
{
/*	int x, y;
	wxClientDC dc((wxWindow*)this);
 	dc.SetFont(GetFont());
	dc.GetTextExtent(GetLabel(),&x,&y);
	
	if(y > bmp_normal.GetHeight() )
		return	wxSize( x+bmp_normal.GetWidth()+5, y );
	else
		return	wxSize( x+bmp_normal.GetWidth()+5, bmp_normal.GetHeight() );
*/
	return	wxSize( bmp_normal.GetWidth(), bmp_normal.GetHeight() );

}

void wxSkinRadiobutton::DrawCustom(wxDC& dc)
{
	wxBitmap temp;
	if( m_checked )
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

void wxSkinRadiobutton::OnLeftClick(wxMouseEvent& WXUNUSED(event))
{
	if( IsEnabled() && !m_checked)
	{
		m_checked = true;
		wxCommandEvent e(wxEVT_COMMAND_RADIOBUTTON_SELECTED, GetId());
		e.SetInt(1);
		e.SetEventObject(this);
		GetEventHandler()->ProcessEvent(e);

		OnCheck();
	}
	else
	{
		
		m_checked = false;
		wxCommandEvent e(wxEVT_COMMAND_RADIOBUTTON_SELECTED, GetId());
		e.SetInt(0);
		e.SetEventObject(this);
		GetEventHandler()->ProcessEvent(e);
	}
	Refresh();
}

bool wxSkinRadiobutton::GetValue() const
{
	return m_checked;
}

void wxSkinRadiobutton::SetValue(const bool state)
{
	m_checked = state;
	if(state == true)
		OnCheck();
	Refresh();
}
void wxSkinRadiobutton::OnCheck()
{
    const wxWindowList& siblings = GetParent()->GetChildren();
    wxWindowList::Node *nodeStart = siblings.Find(this);
    while ( nodeStart )
    {
        if ( !nodeStart->GetPrevious() ||
             (nodeStart->GetData()->GetWindowStyle() & wxRB_GROUP) )
            break;

        nodeStart = nodeStart->GetPrevious();
    }

    while ( nodeStart )
    {
        wxWindow *win = nodeStart->GetData();
        if ( win != this )
        {
        	
        	/*if( win->IsKindOf(CLASSINFO(wxRadioButton)) )
	        {    wxRadioButton* btn = wxDynamicCast(win, wxRadioButton);
	        	 if ( btn )
            	{
                	btn->SetValue(FALSE);
            	}
         	}
         	else
         	{	wxSkinRadiobutton* btn = wxDynamicCast(win, wxSkinRadiobutton);
         		
            	if ( btn )
            	{
                	btn->SetValue(FALSE);
            	}
         	}*/
			if( win->IsKindOf(CLASSINFO(wxSkinRadiobutton)) )
	        {    wxSkinRadiobutton* btn = wxDynamicCast(win, wxSkinRadiobutton);
	        	 if ( btn )
            	{
                	btn->SetValue(FALSE);
            	}
         	}
        }

        nodeStart = nodeStart->GetNext();
        if ( !nodeStart ||
             (nodeStart->GetData()->GetWindowStyle() & wxRB_GROUP) )
        {
            break;
        }
    }
}
/*
void wxSkinRadiobutton::StartCheck()
{	const wxWindowList& siblings = GetParent()->GetChildren();
    wxWindowList::Node *nodeStart = siblings.Find(this);
    while ( nodeStart )
    {
        if ( !nodeStart->GetPrevious() ||
             (nodeStart->GetData()->GetWindowStyle() & wxRB_GROUP) )
            break;

        nodeStart = nodeStart->GetPrevious();
    }

    while ( nodeStart->GetData() != this )
    {
        wxWindow *win = nodeStart->GetData();
        if ( win != this )
        {	
        	if( win->IsKindOf(CLASSINFO(wxRadioButton)) )
	        {   wxRadioButton* btn = wxDynamicCast(win, wxRadioButton);
	        	if ( btn )
            	{	       GetParent()->Connect(btn->GetId(),
                   			wxEVT_COMMAND_RADIOBUTTON_SELECTED,
                   			(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &wxSkinRadiobutton::OnRadio );
					s_checkdone = true;
            	}
         	}

      		nodeStart = nodeStart->GetNext();
        	if ( !nodeStart ||
            	 (nodeStart->GetData()->GetWindowStyle() & wxRB_GROUP) )
        	{
            	break;
        	}
     	}
  	}

}*/
/*
void wxSkinRadiobutton::OnRadio(wxCommandEvent& e)
{
	const wxWindowList& siblings = GetChildren();
    wxWindowList::Node *nodeStart = siblings.GetFirst();

    while ( nodeStart )
    {	
        wxWindow *win = nodeStart->GetData();
        if( win->IsKindOf(CLASSINFO(wxSkinRadiobutton)) )
	    {   wxSkinRadiobutton* btn = wxDynamicCast(win, wxSkinRadiobutton);
	     	if ( btn )
           	{	       
				btn->SetValue(false);
				if(btn->GetWindowStyle() & wxRB_GROUP)
					break;
           	}
        }

      	nodeStart = nodeStart->GetNext();
  	}
	

	e.Skip();
}*/
