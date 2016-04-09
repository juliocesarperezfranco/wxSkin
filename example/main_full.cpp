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

#include <wx/mediactrl.h>

#if !wxUSE_MEDIACTRL
	#error "You'll need wxMediaCtrl to compile this example!"
#else

class MyApp: public wxApp {


	wxMediaCtrl* mediaWidget;

	enum{
	ID_CHANGESKIN = wxID_HIGHEST +1000,
	ID_OPEN,
	ID_PLAY,
	ID_STOP,
        ID_NEXT,
        ID_PREV,
        ID_SUP,
	ID_SDOWN,
        ID_PLIST,
	ID_MUTE,
        ID_SLIDER,
        ID_TIMER,
	};
	wxSkinSimpleFrame* frame;
	wxSkinSlider* slider;
	wxSkinStaticText* stat;
	long m_mediaLength;
	wxString m_skindir;
	wxTimer m_timer;
public:
    bool OnInit();
    virtual int OnExit();
private:
	void OnOpen(wxCommandEvent& e);
	void OnPlay(wxCommandEvent& e);
	void OnStop(wxCommandEvent& e);
	void OnSup(wxCommandEvent& e);
	void OnSdown(wxCommandEvent& e);

	void OnMute(wxCommandEvent& e);

	void OnNotImplemented(wxCommandEvent& e);

	void OnTimer(wxTimerEvent& e);
	void OnSlider(wxCommandEvent& e);

	void OnMediaLoaded(wxMediaEvent& e);

	void OnChangeSkin(wxCommandEvent& e);
	DECLARE_EVENT_TABLE()
};

IMPLEMENT_APP(MyApp)

BEGIN_EVENT_TABLE(MyApp,wxApp)
	EVT_BUTTON(ID_CHANGESKIN, MyApp::OnChangeSkin)

	EVT_BUTTON(ID_OPEN,MyApp::OnOpen)
	EVT_BUTTON(ID_PLAY,MyApp::OnPlay)
	EVT_BUTTON(ID_STOP,MyApp::OnStop)
	EVT_BUTTON(ID_SUP,MyApp::OnSup)
	EVT_BUTTON(ID_SDOWN,MyApp::OnSdown)
	EVT_CHECKBOX(ID_MUTE, MyApp::OnMute)

	EVT_BUTTON(ID_PLIST,MyApp::OnNotImplemented)	
	EVT_BUTTON(ID_NEXT,MyApp::OnNotImplemented)	
	EVT_BUTTON(ID_PREV,MyApp::OnNotImplemented)	

	EVT_TIMER(ID_TIMER, MyApp::OnTimer)
	EVT_SLIDER(ID_SLIDER,MyApp::OnSlider)

	EVT_MEDIA_LOADED(123,MyApp::OnMediaLoaded)
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
	wxSkinButton* btnOpen = new wxSkinButton(frame,ID_OPEN);
	wxSkinButton* btnNext = new wxSkinButton(frame,ID_NEXT);
	wxSkinButton* btnPrev = new wxSkinButton(frame,ID_PREV);
	wxSkinButton* btnStop = new wxSkinButton(frame,ID_STOP);
	wxSkinButton* btnPlay = new wxSkinButton(frame,ID_PLAY);
	wxSkinButton* btnSup = new wxSkinButton(frame,ID_SUP);
	wxSkinButton* btnSdown = new wxSkinButton(frame,ID_SDOWN);
	wxSkinButton* btnPlist = new wxSkinButton(frame,ID_PLIST);
	wxSkinButton* btnPrefs = new wxSkinButton(frame,ID_CHANGESKIN);

	slider = new wxSkinSlider(frame,ID_SLIDER);

	wxSkinCheckBox* chkMute = new wxSkinCheckBox(frame,ID_MUTE);
	chkMute->SetToolTip("Mute");

	stat = new wxSkinStaticText(frame,-1,wxT("00:00 - 00:00"));
	
	//create some guibuilder test widgets
#ifdef __WXMSW__
	mediaWidget =  new wxMediaCtrl(frame,123,"",wxDefaultPosition,wxDefaultSize,0,wxMEDIABACKEND_DIRECTSHOW );
#else
	mediaWidget = new wxMediaCtrl(frame,-1);
#endif
	
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
	wxSkinEngine::Get()->AssignControl(wxT("progressslide"),slider);
	
	wxSkinEngine::Get()->AssignControl(wxT("status"),stat);

	//assign the guibuilder widgets as well, as we want to use guilbuilder to layout these widgets
	wxSkinEngine::Get()->AssignControl(wxT("mediaplayer"),mediaWidget);
	
	//initialize the skin
	wxSkinEngine::Get()->InitializeSkin();

	m_timer.SetOwner(this,ID_TIMER);

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
void MyApp::OnOpen(wxCommandEvent& e)
{
	wxString fname = ::wxFileSelector(wxT("Open a media file"));
	if(!fname.IsEmpty())
	{	if(!mediaWidget->Load(fname))
			wxMessageBox(wxT("File could not be opened"));
		slider->SetValue(0);
	}
}
void MyApp::OnMediaLoaded(wxMediaEvent& e)
{
	m_mediaLength = mediaWidget->Length()/1000;
	stat->SetLabel(wxString::Format(wxT("00:00 - %02d:%02d"),m_mediaLength/60,m_mediaLength%60));
}
void MyApp::OnTimer(wxTimerEvent& e)
{
	if(mediaWidget->GetState() == wxMEDIASTATE_PLAYING )
	{	slider->SetValue(mediaWidget->Tell()/100);
		long t = mediaWidget->Tell()/1000;
		stat->SetLabel(wxString::Format(wxT("%02d:%02d - %02d:%02d"),t/60,t%60,m_mediaLength/60,m_mediaLength%60));
	}
	else
	{	m_timer.Stop();
		slider->SetValue(0);
		stat->SetLabel(wxString::Format(wxT("00:00 - %02d:%02d"),m_mediaLength/60,m_mediaLength%60));
	}
}
void MyApp::OnPlay(wxCommandEvent& e)
{
	mediaWidget->Play();
	slider->SetRange(mediaWidget->Length()/100);
	m_timer.Start(100);
}
void MyApp::OnStop(wxCommandEvent& e)
{
	mediaWidget->Stop();
	slider->SetValue(0);
}
void MyApp::OnSup(wxCommandEvent& e)
{
	if(mediaWidget->GetState() == wxMEDIASTATE_PLAYING )
		mediaWidget->SetPlaybackRate(mediaWidget->GetPlaybackRate()*2);
}
void MyApp::OnSdown(wxCommandEvent& e)
{
	if(mediaWidget->GetState() == wxMEDIASTATE_PLAYING )
		mediaWidget->SetPlaybackRate(mediaWidget->GetPlaybackRate()/2);
}
void MyApp::OnMute(wxCommandEvent& e)
{
	if(e.GetInt() == 1)
		mediaWidget->SetVolume(0);
	else
		mediaWidget->SetVolume(1);
}
void MyApp::OnSlider(wxCommandEvent& e)
{
	int pos = e.GetInt();
	mediaWidget->Seek(pos*100);
}
void MyApp::OnNotImplemented(wxCommandEvent& e)
{
	wxMessageBox(wxT("Sorry, but this feature was not yet implemented..."));
}
#endif
