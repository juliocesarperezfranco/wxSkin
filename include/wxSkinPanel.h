/////////////////////////////////////////////////////////////////////////////////////////
// Name:		wxSkin
// Author:      René Kraus aka upCASE <himself@upcase.de>
// Copyright:   René Kraus (c)
// Licence:     wxWindows license <http://www.wxwidgets.org/licence3.txt>
//				with the addendum that you notify the author about projects using wxSkin
/////////////////////////////////////////////////////////////////////////////////////////
#ifndef __WXSKINPANEL_H
#define __WXSKINPANEL_H

#ifdef __GNUG__
	#pragma interface "wxSkinPanel.h"
#endif

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include <wx/image.h>
#include "wxSkinWindow.h"

///Create a simple panel that will be shaped using a background image.
class wxSkinPanel : public wxSkinWindow
{
public:
	
	wxSkinPanel();
	
	wxSkinPanel(wxWindow* parent,
 				int id,
  				const wxPoint& pos = wxDefaultPosition,
         		const wxSize& size = wxDefaultSize,
           		long style = wxNO_BORDER,
				const wxString& name = wxT("wxSkinPanel"));
	
 	~wxSkinPanel();
	///Set the image to used for skinning and shaping the panel.
	void SetBackground(const wxImage& img);
	virtual void DrawCustom(wxDC& dc);
	virtual void SetCustomSkin(const ControlInfo* info){}
protected:
	
	virtual wxSize DoGetBestSize() const;

private:

	DECLARE_DYNAMIC_CLASS(wxSkinPanel);

};


#endif

