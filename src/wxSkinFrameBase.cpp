/////////////////////////////////////////////////////////////////////////////////////////
// Name:		wxSkin
// Author:      René Kraus aka upCASE <himself@upcase.de>
// Copyright:   René Kraus (c)
// Licence:     wxWindows license <http://www.wxwidgets.org/licence3.txt>
//				with the addendum that you notify the author about projects using wxSkin
/////////////////////////////////////////////////////////////////////////////////////////
#ifdef __GNUG__
	#pragma implementation "wxSkinFrameBase.h"
#endif

#include <wxSkinFrameBase.h>

#if wxCHECK_VERSION(2, 7, 0)
	#define ISOK IsOk
	#define CONTAINS Contains
#else
	#define ISOK Ok
	#define CONTAINS Inside
#endif

#ifdef __WXGTK__
	#include "wx/gtk/win_gtk.h"
#endif

BEGIN_EVENT_TABLE(wxSkinFrameBase,wxFrame)
	EVT_ERASE_BACKGROUND(wxSkinFrameBase::OnErase)

	EVT_LEFT_DOWN(wxSkinFrameBase::OnLeftDown)
	EVT_RIGHT_DOWN(wxSkinFrameBase::PopupSystemMenu)
    EVT_LEFT_UP(wxSkinFrameBase::OnLeftUp)
    EVT_MOTION(wxSkinFrameBase::OnMouseMove)

	EVT_MENU(wxID_CLOSE_FRAME,wxSkinFrameBase::OnCloseFrame)
	EVT_MENU(wxID_ICONIZE_FRAME,wxSkinFrameBase::OnMinimizeFrame)
	EVT_MENU(wxID_MAXIMIZE_FRAME,wxSkinFrameBase::OnMaximizeFrame)

	EVT_BUTTON(wxID_CLOSE_FRAME,wxSkinFrameBase::OnCloseFrame)
	EVT_BUTTON(wxID_ICONIZE_FRAME,wxSkinFrameBase::OnMinimizeFrame)
	EVT_BUTTON(wxID_MAXIMIZE_FRAME,wxSkinFrameBase::OnMaximizeFrame)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(wxSkinFrameBase,wxFrame)


wxSkinFrameBase::wxSkinFrameBase()
	: wxFrame()
{
	m_bClose = m_bMin = m_bMax = NULL;
	m_sizeMode = 0;
	m_titlePlacement = wxTITLE_LEFT;
	m_isMovable = false;
}

wxSkinFrameBase::wxSkinFrameBase(wxWindow* parent,
 				int id,
				const wxString &label,
  				const wxPoint& pos,
         		const wxSize& size,
           		long style,
				const wxString& name)
:wxFrame(parent,id,label,pos,size,style | wxFRAME_SHAPED,name)
{
	m_bClose = m_bMin = m_bMax = NULL;
	m_sizeMode = 0;
	m_titlePlacement = wxTITLE_LEFT;
	m_isMovable = false;
	
//wxWidgets/GTK has a problem with creating our frame without a caption and border, but yet
//let it be min/max-able by the user...
//Another problem was creating the shape properly.
//Therefor we tell it here specifically what we want
#ifdef __WXGTK__
	m_gdkDecor = (long)0;
	m_gdkFunc = GDK_FUNC_MOVE;
	if(style & wxCLOSE_BOX)
		m_gdkFunc |= GDK_FUNC_CLOSE;
	if(style & wxMINIMIZE_BOX)
		m_gdkFunc |= GDK_FUNC_MINIMIZE;
	if(style & wxMAXIMIZE_BOX)
		m_gdkFunc |= GDK_FUNC_MAXIMIZE;
#endif
}

wxSkinFrameBase::~wxSkinFrameBase()
{
}

void wxSkinFrameBase::SetWindowShape(wxImage& img)
{
	wxRegion region;
	if(img.HasAlpha() && !img.HasMask())
		img.ConvertAlphaToMask();
	region = wxRegion(img);
#ifndef __WXGTK__
    SetShape(region);	
#else
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
#endif
}

void wxSkinFrameBase::SetTitle(const wxString& title)
{	
	wxTopLevelWindow::SetTitle(title);
	Refresh();    
}
void wxSkinFrameBase::SetClose(wxRect& r, const wxImage& img)
{
	if(img.ISOK())
	{
		if(!m_bClose)
		{
			m_bClose = new wxSkinButton(this,wxID_CLOSE_FRAME,wxT(""),wxPoint(r.x,r.y),wxSize(img.GetWidth(),img.GetHeight()));
			m_bClose->SetNormalSkin(img);
		}
		else
		{
			m_bClose->SetPosition(wxPoint(r.x,r.y));
			m_bClose->SetSize(wxSize(img.GetWidth(),img.GetHeight()));
			m_bClose->SetNormalSkin(img);
		}
	}
	else
	{	delete m_bClose;
		m_bClose = NULL;
	}
	
}
void wxSkinFrameBase::SetCloseOver(const wxImage& imgover)
{
	if(m_bClose)
	{
		m_bClose->SetOverSkin(imgover);
	}
}
void wxSkinFrameBase::SetClosePressed(const wxImage& imgover)
{
	if(m_bClose)
	{
		m_bClose->SetState2Skin(imgover);
	}
}
void wxSkinFrameBase::SetMinimize(wxRect& r, const wxImage& img)
{
	if(img.ISOK())
	{
		if(!m_bMin)
		{
			m_bMin = new wxSkinButton(this,wxID_ICONIZE_FRAME,wxT(""),wxPoint(r.x,r.y),wxSize(img.GetWidth(),img.GetHeight()));
			m_bMin->SetNormalSkin(img);
		}
		else
		{
			m_bMin->SetPosition(wxPoint(r.x,r.y));
			m_bMin->SetSize(wxSize(img.GetWidth(),img.GetHeight()));
			m_bMin->SetNormalSkin(img);
		}
	}
	else
	{	delete m_bMin;
		m_bMin = NULL;
	}
}
void wxSkinFrameBase::SetMinimizeOver(const wxImage& img)
{
	if(m_bMin)
	{
		m_bMin->SetOverSkin(img);
	}
}
void wxSkinFrameBase::SetMinimizePressed(const wxImage& img )
{
	if(m_bMin)
	{
		m_bMin->SetState2Skin(img);
	}
}
void wxSkinFrameBase::SetMaximize(wxRect& r, const wxImage& img)
{
	if(img.ISOK())
	{
		if(!m_bMax)
		{
			m_bMax = new wxSkinButton(this,wxID_MAXIMIZE_FRAME,wxT(""),wxPoint(r.x,r.y),wxSize(img.GetWidth(),img.GetHeight()));
			m_bMax->SetNormalSkin(img);
		}
		else
		{
			m_bMax->SetPosition(wxPoint(r.x,r.y));
			m_bMax->SetSize(wxSize(img.GetWidth(),img.GetHeight()));
			m_bMax->SetNormalSkin(img);
		}
	}
	else
	{	delete m_bMax;
		m_bMax = NULL;
	}
}
void wxSkinFrameBase::SetMaximizeOver(const wxImage& img)
{
	if(m_bMax)
	{
		m_bMax->SetOverSkin(img);
	}
}
void wxSkinFrameBase::SetMaximizePressed(const wxImage& img)
{
	if(m_bMax)
	{
		m_bMax->SetState2Skin(img);
	}
}

#if defined(__WXGTK__)
#include <gtk/gtk.h>
bool wxSkinFrameBase::SetTransparent(wxByte alpha)
{
    if (!m_widget)
        return false;
    float trans = (float)alpha / 255.0;
    gtk_window_set_opacity(GTK_WINDOW(m_widget), trans);

    return true;

}
#endif

void wxSkinFrameBase::OnErase(wxEraseEvent& event)
{	
}

void wxSkinFrameBase::OnLeftDown(wxMouseEvent& evt)
{
    wxPoint msps = evt.GetPosition(); // mouse position
	if( m_isMovable || rect_title.CONTAINS(msps))
	{	CaptureMouse();
		wxPoint pos = ClientToScreen(msps);
		wxPoint origin = GetPosition();
		int dx =  pos.x - origin.x;
		int dy = pos.y - origin.y;
		m_delta = wxPoint(dx, dy);
	}
	
	if(m_bSizeable)
	{	int w,h;
		GetClientSize(&w,&h);
		wxRect rectWN(0, 0, 5, 5);
		wxRect rectEN(w-5, 0, 5, 5);
		wxRect rectES(w-5,h-5, 5, 5);
		wxRect rectWS(0, h-5, 5, 5);
#ifndef __WXMAC__
        wxRect rectWEST(0, 0, 5, h);
		wxRect rectEAST(w-5,0,5,h);
		wxRect rectSOUTH(0,h-5,w,5);
		wxRect rectNORTH(0, 0, w, 5);
#endif
		if(rectWN.CONTAINS(msps))
			m_sizeMode = SIZE_WN;
	    else if (rectEN.CONTAINS(msps))
			m_sizeMode = SIZE_EN;
	    else if (rectES.CONTAINS(msps))
			m_sizeMode = SIZE_ES;
	    else if (rectWS.CONTAINS(msps))
			m_sizeMode = SIZE_WS;
#ifndef __WXMAC__
		else if(rectWEST.CONTAINS(msps))
			m_sizeMode = SIZE_WEST;
		else if(rectEAST.CONTAINS(msps))
			m_sizeMode = SIZE_EAST;
		else if(rectNORTH.CONTAINS(msps))
			m_sizeMode = SIZE_NORTH;
		else if(rectSOUTH.CONTAINS(msps))
			m_sizeMode = SIZE_SOUTH;
#endif
	}

	//evt.Skip();
}

void wxSkinFrameBase::OnLeftUp(wxMouseEvent& evt)
{
	if(m_sizeMode != 0)
	{	m_sizeMode = 0;
		//SetClientSize(GetClientSize());
		Refresh();
	}

    if (HasCapture())
    {
        ReleaseMouse();
    }
	//evt.Skip();
}

void wxSkinFrameBase::OnMouseMove(wxMouseEvent& evt)
{
    wxPoint msps = evt.GetPosition(); // mouse position
    bool dragFlag = evt.Dragging();
    bool leftIsDown = evt.LeftIsDown();
	int w,h;
	GetClientSize(&w,&h);
    
	if(m_bSizeable)
	{			
		wxRect rectWN(0, 0, 5, 5);
		wxRect rectEN(w-5, 0, 5, 5);
		wxRect rectES(w-5,h-5,5,5);
		wxRect rectWS(0, h-5, 5, 5);
#ifndef __WXMAC__
        wxRect rectWEST(0, 0, 5, h);
		wxRect rectEAST(w-5,0,5,h);
		wxRect rectSOUTH(0,h-5,w,5);
		wxRect rectNORTH(0, 0, w, 5);
#endif
		if(rectWN.CONTAINS(msps) || rectES.CONTAINS(msps))
		{	
		    CaptureMouse();

			SetCursor(wxCursor(wxCURSOR_SIZENWSE ));
		}
		else if(rectEN.CONTAINS(msps) || rectWS.CONTAINS(msps))
		{	
		    CaptureMouse();
			SetCursor(wxCursor(wxCURSOR_SIZENESW ));
		}
#ifndef __WXMAC__
		else if(rectWEST.CONTAINS(msps) || rectEAST.CONTAINS(msps))
		{	



		    CaptureMouse();
			SetCursor(wxCursor(wxCURSOR_SIZEWE ));
		}
		else if(rectNORTH.CONTAINS(msps) || rectSOUTH.CONTAINS(msps))
		{	
		    CaptureMouse();
			SetCursor(wxCursor(wxCURSOR_SIZENS ));
		}
#endif
		else
		{
			SetCursor(wxNullCursor);
			if (!(HasCapture() && dragFlag && leftIsDown))
		        ReleaseMouse();
		}
			
	}

    wxPoint pt = evt.GetPosition();
	wxPoint pos = ClientToScreen(pt);
	wxPoint orignPos = this->GetPosition();

    if (HasCapture() && dragFlag && leftIsDown)
    {
		if(m_sizeMode == SIZE_ES)
		{	
			int newX = pt.x;
			int newY = pt.y;
			
			SetClientSize(newX,newY);
			
		}
		else if(m_sizeMode == SIZE_EAST)
		{
			int newX = pt.x;
			SetClientSize(newX,h);
		}
		else if(m_sizeMode == SIZE_SOUTH)
		{
			int newY = pt.y;
			SetClientSize(w,newY);
		}
		else if(m_sizeMode == SIZE_WEST)
		{
			int newW = w + orignPos.x - (pos.x - m_delta.x);
			//SetClientSize(newW, h);
			//Move((pos.x - m_delta.x), orignPos.y);
			SetSize(wxRect((pos.x - m_delta.x), orignPos.y, newW, h));
		}
		else if(m_sizeMode == SIZE_NORTH)
		{
			int newH = h + orignPos.y - (pos.y - m_delta.y);
			//SetClientSize(w, newH);
			//Move(orignPos.x, (pos.y - m_delta.y));
			SetSize(wxRect(orignPos.x, (pos.y - m_delta.y), w, newH));			
		}
		else if(m_sizeMode == SIZE_EN)
		{
			int newW = pt.x;
			int newH = h + orignPos.y - (pos.y - m_delta.y);
			//SetClientSize(newW, newH);
			//Move(orignPos.x, (pos.y - m_delta.y));
			SetSize(wxRect(orignPos.x, (pos.y - m_delta.y), newW, newH));			
		}
		else if(m_sizeMode == SIZE_WS)
		{
			int newW = w + orignPos.x - (pos.x - m_delta.x);
			int newH = pt.y;
			//SetClientSize(newW, newH);
			//Move((pos.x - m_delta.x), orignPos.y);
			SetSize(wxRect((pos.x - m_delta.x), orignPos.y, newW, newH));
		}
		else if(m_sizeMode == SIZE_WN)
		{
			int newW = w + orignPos.x - (pos.x - m_delta.x);
			int newH = h + orignPos.y - (pos.y - m_delta.y);
			//SetClientSize(newW, newH);
			//Move((pos.x - m_delta.x), (pos.y - m_delta.y));
			SetSize(wxRect((pos.x - m_delta.x), (pos.y - m_delta.y), newW, newH));
		}
		else
		{
			Move(wxPoint(pos.x - m_delta.x, pos.y - m_delta.y));
			
		}
	
		return;
    }
	//evt.Skip();
}
void wxSkinFrameBase::PopupSystemMenu(wxMouseEvent& evt)
{
	if( rect_title.CONTAINS(evt.GetPosition()) )
	{	
#ifdef __WXMSW__
		wxPoint pos = ClientToScreen(evt.GetPosition());
		HMENU hMenu = GetSystemMenu((HWND)GetHWND(),FALSE);
		int flag = ::TrackPopupMenu(hMenu, TPM_RIGHTBUTTON|TPM_RETURNCMD ,
			pos.x, pos.y,
			0, (HWND)GetHWND(), NULL);
		if(flag > 0)
			SendMessage((HWND)this->GetHandle(), WM_SYSCOMMAND, flag, 0);
#else
		wxMenu *menu = new wxMenu;
	
		if ( this->GetWindowStyle() & wxMAXIMIZE_BOX )
			menu->Append(wxID_RESTORE_FRAME , _("&Restore"));
		if ( this->GetWindowStyle() & wxRESIZE_BORDER )
			menu->Append(wxID_RESIZE_FRAME , _("&Size"));
		if( this->GetWindowStyle() & wxMAXIMIZE_BOX )
			menu->Append(wxID_ICONIZE_FRAME , _("Mi&nimize"));
		if ( this->GetWindowStyle() & wxMAXIMIZE_BOX )
			menu->Append(wxID_MAXIMIZE_FRAME , _("Ma&ximize"));
		
		if ( this->GetWindowStyle() & wxCLOSE_BOX )
		{	menu->AppendSeparator();
			menu->Append(wxID_CLOSE_FRAME, _("Close\tAlt-F4"));
		}

		if ( this->GetWindowStyle() & wxMAXIMIZE_BOX )
		{
			if ( this->IsMaximized() )
			{
				menu->Enable(wxID_MAXIMIZE_FRAME, false);
				menu->Enable(wxID_MOVE_FRAME, false);
				if ( this->GetWindowStyle() & wxRESIZE_BORDER )
					menu->Enable(wxID_RESIZE_FRAME, false);
			}
			else
				menu->Enable(wxID_RESTORE_FRAME, false);
		}

		this->PopupMenu(menu, evt.GetPosition());
		delete menu;
#endif
	}
}
void wxSkinFrameBase::OnCloseFrame(wxCommandEvent& e)
{
	Close();
}

void wxSkinFrameBase::OnMaximizeFrame(wxCommandEvent& e)
{
	Maximize(IsMaximized() ? false : true);
}
void wxSkinFrameBase::OnMinimizeFrame(wxCommandEvent& e)
{
	Iconize(true);
}

