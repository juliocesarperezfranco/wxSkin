/////////////////////////////////////////////////////////////////////////////////////////
// Name:		wxSkin
// Author:      René Kraus aka upCASE <himself@upcase.de>
// Copyright:   René Kraus (c)
// Licence:     wxWindows license <http://www.wxwidgets.org/licence3.txt>
//				with the addendum that you notify the author about projects using wxSkin
/////////////////////////////////////////////////////////////////////////////////////////
#ifndef __WXSKINGAUGE_H
#define __WXSKINGAUGE_H

#ifdef __GNUG__
	#pragma interface "wxSkinGauge.h"
#endif

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include <wx/image.h>
#include <wxSkinWindow.h>
///wxSkinGauge implements a simple gauge using skins
class wxSkinGauge : public wxSkinWindow
{
private:
	bool m_isvertical;
	int m_maxvalue;
	int m_currentvalue;
public:
	wxSkinGauge();

	wxSkinGauge(wxWindow* parent,
 				int id,
 				int max=100,
  				const wxPoint& pos = wxDefaultPosition,
         		const wxSize& size = wxDefaultSize,
           		long style = wxNO_BORDER,
				const wxString& name = wxT("wxSkinGauge"));

	~wxSkinGauge();
 	
 	wxSize DoGetBestSize() const;
	///Returns the current maximum range.
	int GetRange() const;
	///Returns the current value.
	int GetValue() const;
	///Sets the maximum range.
	void SetRange(int);
	///Sets the current value.
	void SetValue(int);
	
	virtual void DrawCustom(wxDC& dc);
	virtual void SetCustomSkin(const ControlInfo* info){}
private:
	DECLARE_DYNAMIC_CLASS(wxSkinGauge)
	DECLARE_EVENT_TABLE()
};

#endif
