#include <wx/wx.h>
#include <wx/image.h>


#include <wxSkinEngine.h>
#include <wxSkinSimpleFrame.h>
#include <wxSkinFrame.h>
#include <wxSkinButton.h>
#include <wxSkinCheckBox.h>
#include <wxSkinGauge.h>
#include <wxSkinSlider.h>
#include <wxSkinRadiobutton.h>
#include <wxSkinStaticText.h>
#include <wxSkinPanel.h>

#include <wxSkinChooser.h>

#include <wx/filedlg.h>
#include <wx/filename.h>


class MyApp: public wxApp {

	wxString m_skindir;
public:
    bool OnInit();
    virtual int OnExit();
};

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	wxInitAllImageHandlers();

#ifndef __WXMAC__
	wxFileName fname (wxApp::argv[0]);
	fname.MakeAbsolute();
	m_skindir = fname.GetPath();
#else
	m_skindir = wxFileName::GetCwd();
#endif

	wxSkinEngine::Get()->Load(m_skindir+wxT("/skinfiles/skin_complex.zip"));

	wxSkinFrame* frame = new wxSkinFrame(NULL,-1,wxT("Long title string"));

	wxStaticBitmap* mediaWidget = new wxStaticBitmap(frame,-1,wxBitmap(wxT("example.jpg"),wxBITMAP_TYPE_JPEG));


	wxSkinButton* btn = new wxSkinButton(frame,-1);
	btn->SetPosition(wxPoint(25,390));
	
	wxSkinEngine::Get()->AssignControl(wxT("ComplexFrame"),frame);
	wxSkinEngine::Get()->AssignControl(wxT("button_1"),btn);
	wxSkinEngine::Get()->AssignControl(wxT("videoplayer"),mediaWidget);
	wxSkinEngine::Get()->InitializeSkin();


	SetTopWindow(frame);
	frame->SetSize(450,450);
	//frame2->Maximize(true);
	frame->Centre();
	frame->Show();

	wxBoxSizer* b = new wxBoxSizer(wxVERTICAL);
	b->Add(mediaWidget,1,wxEXPAND,0);
	frame->SetSizer(b);

	wxBoxSizer* b2 = new wxBoxSizer(wxHORIZONTAL);
	b2->AddStretchSpacer(1);
	b2->Add(btn,0,wxALIGN_CENTER,0);
	b2->AddStretchSpacer(1);
	frame->SetBottomSizer(b2);

    return true;
}
int MyApp::OnExit()
{
	wxSkinEngine::Get()->Destroy();
	return(wxApp::OnExit());
} 
