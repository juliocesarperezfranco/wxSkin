/////////////////////////////////////////////////////////////////////////////////////////
// Name:		wxSkin
// Author:      René Kraus aka upCASE <himself@upcase.de>
// Copyright:   René Kraus (c)
// Licence:     wxWindows license <http://www.wxwidgets.org/licence3.txt>
//				with the addendum that you notify the author about projects using wxSkin
/////////////////////////////////////////////////////////////////////////////////////////
#ifndef __wxSkinFrameBase_H
#define __wxSkinFrameBase_H

#ifdef __GNUG__
	#pragma interface "wxSkinFrameBase.h"
#endif

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include <wx/image.h>

#include "wxSkinButton.h"

#define wxTITLE_LEFT	0x10000
#define wxTITLE_MIDDLE	0x20000
#define wxTITLE_RIGHT	0x40000

#define SIZE_NORTH 1
#define SIZE_EN    2
#define SIZE_EAST  3
#define SIZE_ES    4
#define SIZE_SOUTH 5
#define SIZE_WS    6
#define SIZE_WEST  7
#define SIZE_WN    8 

///Base class for all skinned frames
/**This class manages skinned frames. Normally you won't have to use it directly. Use one of the frame classes
(wxSkinSimpleFrame and wxSkinFrame) instead.
*/
class wxSkinFrameBase : public wxFrame
{
protected:
	bool m_bSizeable;
	int m_sizeMode;
	int m_titlePlacement;
	
	wxRect rect_title;

	wxSkinButton* m_bClose;
	wxSkinButton* m_bMin;
	wxSkinButton* m_bMax;

	bool m_isMovable;
	wxPoint m_delta;
public:
	wxSkinFrameBase();
	
	wxSkinFrameBase(wxWindow* parent,
 				int id,
 				const wxString& label = wxEmptyString,
  				const wxPoint& pos = wxDefaultPosition,
         		const wxSize& size = wxDefaultSize,
           		long style = wxDEFAULT_FRAME_STYLE | wxSIMPLE_BORDER,
				const wxString& name = wxT("wxSkinFrame"));


 	~wxSkinFrameBase();

	///Set the frame to "movable" state. This means that the frame can be moved by clicking anywhere in the skin.
	void SetMovable(bool b){ m_isMovable = b; }
	///Set the Titlebar measures.
	void SetTitleRect(wxRect r){ rect_title = r;}
	///Set the placement for the title caption. Can be wxTITLE_LEFT, wxTITLE_MIDDLE, or wxTITLE_RIGHT.
	void SetTitlePlacement(int p){m_titlePlacement = p;}
	///Set the title caption text.
	void SetTitle(const wxString& title);

	///Set the measure for the closebox with an optional image to display.
	void SetClose(wxRect& r, const wxImage& img = wxNullImage);
	///Set the image for the closebox in over state
	void SetCloseOver(const wxImage& imgover = wxNullImage);
	///Set the image for the closebox in pressed state
	void SetClosePressed(const wxImage& imgover = wxNullImage);

	///Set the measure for the minimizebox with an optional image to display.
	void SetMinimize(wxRect& r, const wxImage& img = wxNullImage);
	///Set the image for the minimizebox in over state
	void SetMinimizeOver(const wxImage& img = wxNullImage);
	///Set the image for the minimizebox in pressed state
	void SetMinimizePressed(const wxImage& img = wxNullImage);

	///Set the measure for the maximizebox with an optional image to display.
	void SetMaximize(wxRect& r, const wxImage& img = wxNullImage);
	///Set the image for the maximizebox in over state
	void SetMaximizeOver(const wxImage& img = wxNullImage);
	///Set the image for the maximizebox in pressed state
	void SetMaximizePressed(const wxImage& img = wxNullImage);
	
#if defined(__WXGTK__)
    virtual bool SetTransparent(wxByte alpha);
#endif

protected:
	void SetWindowShape(wxImage& img);
private:
	void OnErase(wxEraseEvent& e);
	void OnLeftDown(wxMouseEvent& evt);
	void PopupSystemMenu(wxMouseEvent& evt);
	void OnLeftUp(wxMouseEvent& evt);
	void OnMouseMove(wxMouseEvent& evt);

	void OnCloseFrame(wxCommandEvent& e);
	void OnMaximizeFrame(wxCommandEvent& e);
	void OnMinimizeFrame(wxCommandEvent& e);


private:
	DECLARE_DYNAMIC_CLASS(wxSkinFrame)
	DECLARE_EVENT_TABLE()

};

#endif
