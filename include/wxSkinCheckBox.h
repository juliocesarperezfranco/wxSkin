/////////////////////////////////////////////////////////////////////////////////////////
// Name:		wxSkin
// Author:      René Kraus aka upCASE <himself@upcase.de>
// Copyright:   René Kraus (c)
// Licence:     wxWindows license <http://www.wxwidgets.org/licence3.txt>
//				with the addendum that you notify the author about projects using wxSkin
/////////////////////////////////////////////////////////////////////////////////////////
#ifndef __WXSKINCHECKBOX_H
#define __WXSKINCHECKBOX_H

#ifdef __GNUG__
	#pragma interface "wxSkinCheckBox.h"
#endif

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include <wx/image.h>
#include "wxSkinWindow.h"
///wxSkinCheckBox is a checkbox created through a skin
/**wxSkinCheckBox has the states checked/unchecked and emits the same events wxCheckBox does.
*/
class wxSkinCheckBox : public wxSkinWindow
{
private:
	bool m_checked;
public:
	
	wxSkinCheckBox();
	
	wxSkinCheckBox(wxWindow* parent,
 				int id,
				const wxString& label = wxEmptyString,
  				const wxPoint& pos = wxDefaultPosition,
         		const wxSize& size = wxDefaultSize,
           		long style = wxNO_BORDER,
             	const wxString& name = wxT("wxSkinCheckBox"));

	~wxSkinCheckBox();

	///Returns the current state (true = checked, false = unchecked).
	virtual bool GetValue() const;
	///Sets the current state.
	virtual void SetValue(const bool state);
	///Checks wether the checkbox is checked.
	virtual bool IsChecked() const;
	///Sets an opitional text label to be placed next to the control.
	void SetLabel(const wxString& label);
	virtual void DrawCustom(wxDC& dc);
	virtual void SetCustomSkin(const ControlInfo* info){}
protected:
	
	virtual wxSize DoGetBestSize() const;

private:
	void OnLeftClick(wxMouseEvent& WXUNUSED(event));

	DECLARE_DYNAMIC_CLASS(wxSkinCheckBox)
	DECLARE_EVENT_TABLE()
};

#endif
