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

#define ID_CHANGESKIN 1000

class MyApp: public wxApp {

	wxSkinSimpleFrame* frame;
	wxString m_skindir;
public:
    bool OnInit();
    virtual int OnExit();

private:
	void OnChangeSkin(wxCommandEvent& e);
	DECLARE_EVENT_TABLE()
};

IMPLEMENT_APP(MyApp)

BEGIN_EVENT_TABLE(MyApp,wxApp)
	EVT_BUTTON(ID_CHANGESKIN, MyApp::OnChangeSkin)
END_EVENT_TABLE()

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


	//Get the skin engine, the master of process
	//load a skin file
	//a skin file is either a valid xml file
	//or a zip archive. archives may contain a folder structure for subgrouping elements
	if(!wxSkinEngine::Get()->Load(m_skindir+wxT("/skinfiles/skin2.zip")))
	{
		wxMessageBox(wxT("Missing skin file or skin file is broken..."));
		return false;
	}

	//create a "simple" frame
	//a simple frame only uses one image as a skin
	//there are also "complex" frame that have 4 borders, 4 window edges and a body
	frame = new wxSkinSimpleFrame(NULL,-1,wxSkinEngine::Get()->GetSkinName());

	//create some buttons and a checkbox
	wxSkinButton* btnOpen = new wxSkinButton(frame,-1);
	wxSkinButton* btnNext = new wxSkinButton(frame,-1);
	wxSkinButton* btnPrev = new wxSkinButton(frame,-1);
	wxSkinButton* btnStop = new wxSkinButton(frame,-1);
	wxSkinButton* btnPlay = new wxSkinButton(frame,-1);
	wxSkinButton* btnSup = new wxSkinButton(frame,-1);
	wxSkinButton* btnSdown = new wxSkinButton(frame,-1);
	wxSkinButton* btnPlist = new wxSkinButton(frame,-1);
	wxSkinButton* btnPrefs = new wxSkinButton(frame,ID_CHANGESKIN);

	wxSkinSlider* testgauge = new wxSkinSlider(frame,-1);

	wxSkinCheckBox* chkMute = new wxSkinCheckBox(frame,-1);
	chkMute->SetToolTip("Mute");

	wxSkinStaticText* stat = new wxSkinStaticText(frame,-1,wxT("00:00 - 00:00"));
	
	//create some guibuilder test widgets
	wxStaticBitmap* mediaWidget = new wxStaticBitmap(frame,-1,wxBitmap(wxT("example.jpg"),wxBITMAP_TYPE_JPEG));
	
	//assign the elements to skin with names used as identifiers in the skin file
	wxSkinEngine::Get()->AssignControl(wxT("PlayerFrame"),frame);
	wxSkinEngine::Get()->AssignControl(wxT("button_open"),btnOpen);
	wxSkinEngine::Get()->AssignControl(wxT("button_next"),btnNext);
	wxSkinEngine::Get()->AssignControl(wxT("button_prev"),btnPrev);
	wxSkinEngine::Get()->AssignControl(wxT("button_stop"),btnStop);
	wxSkinEngine::Get()->AssignControl(wxT("button_play"),btnPlay);
	wxSkinEngine::Get()->AssignControl(wxT("button_speedup"),btnSup);
	wxSkinEngine::Get()->AssignControl(wxT("button_speeddown"),btnSdown);
	wxSkinEngine::Get()->AssignControl(wxT("button_playlist"),btnPlist);
	wxSkinEngine::Get()->AssignControl(wxT("button_prefs"),btnPrefs);
	wxSkinEngine::Get()->AssignControl(wxT("mute"),chkMute);
	wxSkinEngine::Get()->AssignControl(wxT("progressslide"),testgauge);
	
	wxSkinEngine::Get()->AssignControl(wxT("status"),stat);

	//assign the guibuilder widgets as well, as we want to use guilbuilder to layout these widgets
	wxSkinEngine::Get()->AssignControl(wxT("mediaplayer"),mediaWidget);
	
	//initialize the skin
	wxSkinEngine::Get()->InitializeSkin();


	frame->Show();
	frame->Refresh();

    return true;
}
int MyApp::OnExit()
{
	wxSkinEngine::Get()->Destroy();
	return(wxApp::OnExit());
} 

void MyApp::OnChangeSkin(wxCommandEvent& e)
{

	wxSkinChooser* dlg = new wxSkinChooser(frame,m_skindir,wxT("skin*.zip"),wxT("wxSkin Media Player example"));
	dlg->ShowModal();
	frame->SetTitle(wxSkinEngine::Get()->GetSkinName());
}
