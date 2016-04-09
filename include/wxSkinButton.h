/////////////////////////////////////////////////////////////////////////////////////////
// Name:		wxSkin
// Author:      René Kraus aka upCASE <himself@upcase.de>
// Copyright:   René Kraus (c)
// Licence:     wxWindows license <http://www.wxwidgets.org/licence3.txt>
//				with the addendum that you notify the author about projects using wxSkin
/////////////////////////////////////////////////////////////////////////////////////////
#ifndef __WXSKINBUTTON_H
#define __WXSKINBUTTON_H

#ifdef __GNUG__
	#pragma interface "wxSkinButton.h"
#endif

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include <wx/image.h>
#include "wxSkinWindow.h"
///wxSkinButton is a simple skinned button class
/**wxSkinButton is a button created from different skins. To catch its events use the standard EVT_BUTTON() macro.
*/
class wxSkinButton : public wxSkinWindow
{

public:
	
	wxSkinButton();
	
	wxSkinButton(wxWindow* parent,
 				int id,
				const wxString& label = wxEmptyString,
  				const wxPoint& pos = wxDefaultPosition,
         		const wxSize& size = wxDefaultSize,
           		long style = wxNO_BORDER,
				const wxString& name = wxT("wxSkinButton"));
	
 	~wxSkinButton();
	///Set an optional text label. The label will be rendered inside the skin.
	void SetLabel(const wxString& label);
	
	virtual void DrawCustom(wxDC& dc);
	virtual void SetCustomSkin(const ControlInfo* info){}
protected:
	virtual wxSize DoGetBestSize() const;

private:
	void OnLeftClick(wxMouseEvent& WXUNUSED(event));
	void OnLeftRelease(wxMouseEvent& WXUNUSED(event));

	DECLARE_DYNAMIC_CLASS(wxSkinButton);
	DECLARE_EVENT_TABLE()

};


#endif

