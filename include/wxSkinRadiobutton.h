/////////////////////////////////////////////////////////////////////////////////////////
// Name:		wxSkin
// Author:      René Kraus aka upCASE <himself@upcase.de>
// Copyright:   René Kraus (c)
// Licence:     wxWindows license <http://www.wxwidgets.org/licence3.txt>
//				with the addendum that you notify the author about projects using wxSkin
/////////////////////////////////////////////////////////////////////////////////////////
#ifndef __WXSKINRADIOBUTTON_H
#define __WXSKINRADIOBUTTON_H

#ifdef __GNUG__
	#pragma interface "wxSkinRadiobutton.h"
#endif

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include <wx/image.h>
#include "wxSkinWindow.h"
///wxSkinRadiobutton implements a simple radio button class. Emits wxRadioButton events.
class wxSkinRadiobutton : public wxSkinWindow
{
private:
	static bool s_checkdone;
	bool m_checked;			//!<State of the checkbox
public:
	
	wxSkinRadiobutton();
	
	wxSkinRadiobutton(wxWindow* parent,
 				int id,
				const wxString& label = wxEmptyString,
  				const wxPoint& pos = wxDefaultPosition,
         		const wxSize& size = wxDefaultSize,
           		long style = wxNO_BORDER,
             	const wxString& name = wxT("wxSkinCheckBox"));

	~wxSkinRadiobutton();
	///Returns the current state.
	virtual bool GetValue() const;
	///Sets the current state.
	virtual void SetValue(const bool state);
	///Sets an optional text label.
	void SetLabel(const wxString& label);
	virtual void DrawCustom(wxDC& dc);
	virtual void SetCustomSkin(const ControlInfo* info){}

	void OnCheck();
	
protected:
	virtual wxSize DoGetBestSize() const;

private:

	void OnLeftClick(wxMouseEvent& WXUNUSED(event));

	DECLARE_DYNAMIC_CLASS(wxSkinCheckBox)
	DECLARE_EVENT_TABLE()
};

#endif
