/////////////////////////////////////////////////////////////////////////////////////////
// Name:		wxSkin
// Author:      René Kraus aka upCASE <himself@upcase.de>
// Copyright:   René Kraus (c)
// Licence:     wxWindows license <http://www.wxwidgets.org/licence3.txt>
//				with the addendum that you notify the author about projects using wxSkin
/////////////////////////////////////////////////////////////////////////////////////////
#ifdef __GNUG__
	#pragma implementation "wxSkinGauge.h"
#endif

#include "wxSkinGauge.h"
#include "wxSkinEngine.h"


BEGIN_EVENT_TABLE(wxSkinGauge,wxSkinWindow)
//	EVT_PAINT(wxSkinGauge::OnPaint)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(wxSkinGauge,wxSkinWindow)

wxSkinGauge::wxSkinGauge()
	: wxSkinWindow()
{
	m_currentvalue = 0;
	m_maxvalue = 0;
}

wxSkinGauge::wxSkinGauge(wxWindow* parent,
 				int id,
 				int max,
   				const wxPoint& pos,
         		const wxSize& size,
           		long style,
				const wxString& name)
	:wxSkinWindow(parent,id,pos,size,style,name,TYPE_GAUGE), m_maxvalue(max)
{
	m_currentvalue = 0;

	m_isvertical = false;

}

wxSkinGauge::~wxSkinGauge()
{
}
wxSize wxSkinGauge::DoGetBestSize() const
{
	if(bmp_normal.ISOK())
		return wxSize(bmp_normal.GetWidth(),bmp_normal.GetHeight());
	return wxSize(70,30);
}

void wxSkinGauge::DrawCustom(wxDC& dc)
{
	if(bmp_state2.ISOK())
	{
		int h, w;
		GetSize(&w,&h);
		
		wxBitmap temp;

		float scale = ((float)w/(float)m_maxvalue)*m_currentvalue+1;
		temp = wxBitmap(bmp_state2.Scale((int)scale,h));
		dc.DrawBitmap( temp, 0 , 0 );
	}

}
int wxSkinGauge::GetRange() const
{
	return m_maxvalue;
}
int wxSkinGauge::GetValue() const
{
	return m_currentvalue;
}
void wxSkinGauge::SetRange(int range)
{
	m_maxvalue = range;
}

void wxSkinGauge::SetValue(int value)
{
	m_currentvalue = value;
	Refresh();
}

