/////////////////////////////////////////////////////////////////////////////////////////
// Name:		wxSkin
// Author:      René Kraus aka upCASE <himself@upcase.de>
// Copyright:   René Kraus (c)
// Licence:     wxWindows license <http://www.wxwidgets.org/licence3.txt>
//				with the addendum that you notify the author about projects using wxSkin
/////////////////////////////////////////////////////////////////////////////////////////
#include <wx/wx.h>
#include <wx/image.h>
#include <wx/dir.h>
#ifndef WXSKINCHOOSER_H
#define WXSKINCHOOSER_H

#include <wx/splitter.h>
#include <wx/statline.h>


typedef struct TAGENTRY{
	wxImage screenshot;
	wxString author;
	wxString comment;
	wxString path;
}SkinEntry;

WX_DECLARE_OBJARRAY(SkinEntry, wxArraySkins);

class wxSkinChooser: public wxDialog {
public:
    
    enum {
        ID_LIST = wxID_HIGHEST + 1000
    };
  
	wxSkinChooser(wxWindow* parent,const wxString& dirname,const wxString mask, const wxString application=wxEmptyString);
private:
  
	wxArraySkins m_skins;
	//int m_initialSkin;
    
    wxListBox* listSkins;
    wxStaticBitmap* bmpScreenshot;
    wxStaticLine* static_line_1;
    wxTextCtrl* txtComment;
    wxStaticLine* static_line_2;
    wxStaticText* lbAuthor;
    wxPanel* panel;
    wxSplitterWindow* splitterWnd;
    wxButton* btnSelect;
    wxButton* btnCancel;
    
    DECLARE_EVENT_TABLE();

public:
    void OnListSelect(wxCommandEvent &event);
    void OnSelectSkin(wxCommandEvent &event);
    void OnCancel(wxCommandEvent &event);
};

class wxSkinTraverser : public wxDirTraverser
{
public:
    wxSkinTraverser(wxArrayString& files) : m_files(files) { }

    virtual wxDirTraverseResult OnFile(const wxString& filename)
    {
        m_files.Add(filename);
        return wxDIR_CONTINUE;
    }

    virtual wxDirTraverseResult OnDir(const wxString& WXUNUSED(dirname))
    {
        return wxDIR_CONTINUE;
    }

private:
    wxArrayString& m_files;
};


#endif // WXSKINCHOOSER_H
