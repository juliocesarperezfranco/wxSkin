/////////////////////////////////////////////////////////////////////////////////////////
// Name:		wxSkin
// Author:      René Kraus aka upCASE <himself@upcase.de>
// Copyright:   René Kraus (c)
// Licence:     wxWindows license <http://www.wxwidgets.org/licence3.txt>
//				with the addendum that you notify the author about projects using wxSkin
/////////////////////////////////////////////////////////////////////////////////////////
#include "wxSkinChooser.h"
#include <wxSkinEngine.h>
#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(wxArraySkins);

wxSkinChooser::wxSkinChooser(wxWindow* parent,const wxString& dirname,const wxString mask, const wxString application)
:wxDialog(parent, -1, wxT("Choose a skin"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER)
{
    splitterWnd = new wxSplitterWindow(this, -1, wxDefaultPosition, wxDefaultSize, wxSP_3D|wxSP_BORDER);
    panel = new wxPanel(splitterWnd, -1);
   
    listSkins = new wxListBox(splitterWnd, ID_LIST, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE);
    bmpScreenshot = new wxStaticBitmap(panel, -1, wxNullBitmap);
    bmpScreenshot->SetBackgroundColour(*wxRED);
    static_line_1 = new wxStaticLine(panel, -1);
    txtComment = new wxTextCtrl(panel, -1, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY);
    static_line_2 = new wxStaticLine(panel, -1);
    lbAuthor = new wxStaticText(panel, -1, wxT("Author name"));
    btnSelect = new wxButton(this, wxID_OK, wxT("Select skin"));
    btnCancel = new wxButton(this, wxID_CANCEL, wxT("Cancel"));

  splitterWnd->SetMinimumPaneSize(50);

    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* splitterSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* panelSizer = new wxBoxSizer(wxVERTICAL);
    panelSizer->Add(bmpScreenshot, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 2);
    panelSizer->Add(static_line_1, 0, wxEXPAND, 0);
    panelSizer->Add(txtComment, 1, wxALL|wxEXPAND|wxFIXED_MINSIZE, 5);
    panelSizer->Add(static_line_2, 0, wxEXPAND, 0);
    panelSizer->Add(lbAuthor, 0, wxALL|wxFIXED_MINSIZE, 5);
    panel->SetAutoLayout(true);
    panel->SetSizer(panelSizer);
    panelSizer->Fit(panel);
    panelSizer->SetSizeHints(panel);
    splitterWnd->SplitVertically(listSkins, panel);
    splitterSizer->Add(splitterWnd, 1, wxEXPAND, 0);
    topSizer->Add(splitterSizer, 1, wxEXPAND, 0);
    buttonSizer->Add(btnSelect, 0, wxRIGHT|wxFIXED_MINSIZE, 10);
    buttonSizer->Add(btnCancel, 0, wxFIXED_MINSIZE, 0);
    topSizer->Add(buttonSizer, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5);
    SetAutoLayout(true);
    SetSizer(topSizer);
    topSizer->Fit(this);
    topSizer->SetSizeHints(this);
    Layout();

  SetSize(wxSize(580,450));

  wxString currentSkin = wxSkinEngine::Get()->GetSkinName();

  wxArrayString skinFiles;
  wxDir dir(dirname);
  wxSkinTraverser skinTravers(skinFiles);
  dir.Traverse(skinTravers,mask);

  skinFiles.Sort();

  wxArrayString skinNames;

  for(int i=0; i < (int)skinFiles.GetCount(); i++)
  {	wxString name, author, comment,inf,app;
    wxImage screen;
    if(wxSkinEngine::Get()->LoadInfo(skinFiles[i],name,author,comment,app,inf,screen))
    {
      if(application.IsEmpty())
      {	SkinEntry entry;	
        entry.author = author;
        entry.comment = comment;
        entry.path = skinFiles[i];
        entry.screenshot = screen;
          
        skinNames.Add(name);
        m_skins.Add(entry);
      }
      else if( app == application)
      {
        SkinEntry entry;	
        entry.author = author;
        entry.comment = comment;
        entry.path = skinFiles[i];
        entry.screenshot = screen;
          
        skinNames.Add(name);
        m_skins.Add(entry);
      }

    }
  }
  listSkins->Set(skinNames);

  if(listSkins->GetCount() != 0)
  {
    int selItem = -1;
    if(listSkins->SetStringSelection(currentSkin))
    {	selItem = listSkins->GetSelection();
    }
    else if(listSkins->GetCount() != 0)
    {	listSkins->SetSelection(0);
      selItem = 0;
    }

    if(selItem != -1)
    {
      SkinEntry entry = m_skins[selItem];
      txtComment->SetValue(entry.comment);
      lbAuthor->SetLabel(entry.author);
      bmpScreenshot->SetBitmap(entry.screenshot);
    }
  }

}


BEGIN_EVENT_TABLE(wxSkinChooser, wxDialog)
    EVT_LISTBOX(ID_LIST, wxSkinChooser::OnListSelect)
    EVT_BUTTON(wxID_OK, wxSkinChooser::OnSelectSkin)
    EVT_BUTTON(wxID_CANCEL, wxSkinChooser::OnCancel)
END_EVENT_TABLE();


void wxSkinChooser::OnListSelect(wxCommandEvent &event)
{
  int sel = listSkins->GetSelection();
  if(sel != wxNOT_FOUND && sel < (int)m_skins.GetCount())
  {
    SkinEntry entry = m_skins[sel];
    txtComment->SetValue(entry.comment);
    lbAuthor->SetLabel(entry.author);
    bmpScreenshot->SetBitmap(entry.screenshot);
    
    panel->Layout();
    panel->Refresh();
  }
}


void wxSkinChooser::OnSelectSkin(wxCommandEvent &event)
{
  int sel = listSkins->GetSelection();
  if(sel != wxNOT_FOUND && sel < (int)m_skins.GetCount())
  {	
    //wxStopWatch sw;

    wxSkinEngine::Get()->ClearSkin();
    wxSkinEngine::Get()->Load(m_skins[sel].path);
    wxSkinEngine::Get()->InitializeSkin();

    //wxLogMessage("Loading the skin took %ldms to execute",
         //        sw.Time());


    m_skins.Clear();
    event.Skip();
  }
  else
    wxMessageBox(wxT("You didn't select a skin :-)"));
}


void wxSkinChooser::OnCancel(wxCommandEvent &event)
{
  m_skins.Clear();
    event.Skip();
}

