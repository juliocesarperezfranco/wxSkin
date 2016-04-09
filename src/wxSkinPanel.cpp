/////////////////////////////////////////////////////////////////////////////////////////
// Name:		wxSkin
// Author:      René Kraus aka upCASE <himself@upcase.de>
// Copyright:   René Kraus (c)
// Licence:     wxWindows license <http://www.wxwidgets.org/licence3.txt>
//				with the addendum that you notify the author about projects using wxSkin
/////////////////////////////////////////////////////////////////////////////////////////
#ifdef __GNUG__
	#pragma implementation "wxSkinPanel.h"
#endif

#include <wxSkinPanel.h>


#if wxCHECK_VERSION(2, 7, 0)
	#define ISOK IsOk
#else
	#define ISOK Ok
#endif

IMPLEMENT_DYNAMIC_CLASS(wxSkinPanel,wxSkinWindow)

//Default
wxSkinPanel::wxSkinPanel()
	: wxSkinWindow()
{
}
//Passing images
wxSkinPanel::wxSkinPanel(wxWindow* parent,
 				int id,
  				const wxPoint& pos,
         		const wxSize& size,
           		long style,
				const wxString& name)
	:wxSkinWindow(parent,id,pos,size,style,name,TYPE_PANEL)
{
	SetSize(bmp_normal.GetWidth(),bmp_normal.GetHeight());
}

wxSkinPanel::~wxSkinPanel()
{
}
void wxSkinPanel::SetBackground(const wxImage& img)
{
	bmp_normal = img;
	if(img.ISOK())
	{	SetSize(img.GetWidth(),img.GetHeight());
		SetShape(img);
	}
}
wxSize wxSkinPanel::DoGetBestSize() const
{	
	return wxSize(bmp_normal.GetWidth(),bmp_normal.GetHeight());

}


void wxSkinPanel::DrawCustom(wxDC& dc)
{	
}

