/////////////////////////////////////////////////////////////////////////////////////////
// Name:		wxSkin
// Author:      René Kraus aka upCASE <himself@upcase.de>
// Copyright:   René Kraus (c)
// Licence:     wxWindows license <http://www.wxwidgets.org/licence3.txt>
//				with the addendum that you notify the author about projects using wxSkin
/////////////////////////////////////////////////////////////////////////////////////////
#ifndef __wxSkinSimpleFrame_H
#define __wxSkinSimpleFrame_H

#ifdef __GNUG__
	#pragma interface "wxSkinSimpleFrame.h"
#endif

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include <wxSkinFrameBase.h>
///Implements a simple frame using one image for skinning
/**Simple frames consist of one image for skinning and shaping. They can't be resized.
*/
class wxSkinSimpleFrame : public wxSkinFrameBase
{
private:
	wxImage m_bmpSkin;
	wxImage m_bmpSkinMaster;
	wxRect rect_clientarea;
public:
	wxSkinSimpleFrame();
	
	wxSkinSimpleFrame(wxWindow* parent,
 				int id,
 				const wxString& label = wxEmptyString,
				const wxImage& bmp_skin = wxNullImage,
  				const wxPoint& pos = wxDefaultPosition,
         		const wxSize& size = wxDefaultSize,
           		long style = wxDEFAULT_FRAME_STYLE | wxSIMPLE_BORDER,
				const wxString& name = wxT("wxSkinSimpleFrame"));

 	~wxSkinSimpleFrame();

	///Sets the skin to use for the frame.
	void SetSkin(const wxImage& img){
		m_bmpSkinMaster = img;
		m_bmpSkin = img;
		wxSize s(m_bmpSkin.GetWidth(), m_bmpSkin.GetHeight());
		SetClientSize(s);
		DoSize(s);
	}

	///Sets a client area where one can embedd a sizer and other widgets.
	void SetClientArea(wxRect& r){rect_clientarea = r;}

	///Set a sizer for the frame, containing other widgets. This sizer will be created in the client area.
	void SetSizer(wxSizer* sizer);
	///Recreate the sizer for the frame. Normaly not used directly.
	void RecreateSizer();

private:
	void DoSize(wxSize e);

	virtual void OnPaint(wxPaintEvent& WXUNUSED(event));
#ifdef __WXGTK__
	void OnWindowCreate(wxWindowCreateEvent& evt);
#endif
	
private:
	DECLARE_DYNAMIC_CLASS(wxSkinSimpleFrame)
	DECLARE_EVENT_TABLE()

};

#endif
