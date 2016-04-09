/////////////////////////////////////////////////////////////////////////////////////////
// Name:		wxSkin
// Author:      René Kraus aka upCASE <himself@upcase.de>
// Copyright:   René Kraus (c)
// Licence:     wxWindows license <http://www.wxwidgets.org/licence3.txt>
//				with the addendum that you notify the author about projects using wxSkin
/////////////////////////////////////////////////////////////////////////////////////////
#ifndef __wxSkinFrame_H
#define __wxSkinFrame_H

#ifdef __GNUG__
	#pragma interface "wxSkinFrame.h"
#endif

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include <wxSkinFrameBase.h>

#define RELATE_ABSOLUTE		0 
#define RELATE_TOP			1
#define RELATE_TOP_LEFT		2
#define RELATE_TOP_RIGHT	3
///A "complex" frame class
/**A "complex" skinned frame that behaves like a normal frame.
\todo SetMaximizeBoxRelation
*/
class wxSkinFrame : public wxSkinFrameBase
{
private:
	wxImage m_bmpSkin;
	
	wxImage skin_top;
	wxImage skin_topleft;
	wxImage skin_topright;

	wxImage skin_bottom;
	wxImage skin_bottomleft;
	wxImage skin_bottomright;

	wxImage skin_left;
	wxImage skin_right;

	wxImage skin_body;

	int m_initial_close_x;
	int closebox_relateto;
	int m_initial_min_x;
	int minimizebox_relateto;
public:
	wxSkinFrame();
	
	wxSkinFrame(wxWindow* parent,
 				int id,
 				const wxString& label = wxEmptyString,
				const wxImage& bmp_skin = wxNullImage,
  				const wxPoint& pos = wxDefaultPosition,
         		const wxSize& size = wxDefaultSize,
           		long style = wxDEFAULT_FRAME_STYLE | wxSIMPLE_BORDER,
				const wxString& name = wxT("wxSkinFrame"));

 	~wxSkinFrame();

	///Set the skin for the top border (title)
	void SetTopSkin(const wxImage& img);
	///Set the skin for the top left edge
	void SetTopLeftSkin(const wxImage& img);
	///Set the skin for the top right edge
	void SetTopRightSkin(const wxImage& img);

	///Set the skin for the bottom border
	void SetBottomSkin(const wxImage& img);
	///Set the skin for the bottom left edge
	void SetBottomLeftSkin(const wxImage& img);
	///Set the skin for the bottom right edge
	void SetBottomRightSkin(const wxImage& img);

	///Set the skin for the left side border
	void SetLeftSkin(const wxImage& img);
	///Set the skin for the right side border
	void SetRightSkin(const wxImage& img);

	///Set the skin for the body (client) area
	void SetBodySkin(const wxImage& img);
	
	///Set a sizer for the frame, containing other widgets. This sizer will be created in the client area.
	bool SetSizer(wxSizer* sizer);
	///Set a sizer to be place on the top border area
	bool SetTopSizer(wxSizer* sizer);
	///Set a sizer to be place on the bottom border area
	bool SetBottomSizer(wxSizer* sizer);
	///Recreate the sizer for the frame. Normaly not used directly.
	void RecreateSizer();

	/**Set the relation for the closebox. RELATE_ABSOLUTE means the closebox doesn't move, 
	RELATE_TOP means that it will be moved when the top border changes in size.
	*/
	void SetCloseBoxRelation(int rel = RELATE_ABSOLUTE){ closebox_relateto = rel;}
	///Same as SetCloseBoxRelation() for the minimize box
	void SetMinimizeBoxRelation(int rel = RELATE_ABSOLUTE){ minimizebox_relateto = rel;}
private:
	bool CreateFrame(wxSize s);

	virtual void OnPaint(wxPaintEvent& WXUNUSED(event));
#ifdef __WXGTK__
	void OnWindowCreate(wxWindowCreateEvent& evt);
#endif
	
	void OnSize(wxSizeEvent& e);
private:
	DECLARE_DYNAMIC_CLASS(wxSkinFrame)
	DECLARE_EVENT_TABLE()

};

#endif
