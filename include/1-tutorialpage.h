/**	
	\page skin_tutorial wxSkin tutorial

	Creating skinnable applications with wxSkin is not a hard task, but I decided to create a small tutorial covering the most basic steps to create a skinned application.
	
	\section initial Initial considerations
	Before you start using wxSkin there are some things you'll have to think about.
	- Do you want to use wxSkin controls in a normal wxWidgets application?
	- Do you want to use skin files to control the look?
	- Do you want to use a simple frame?
	- Do you want to use a complex frame?
	
	\subsection controls Do you want to use wxSkin controls in a normal wxWidgets application?
	You can use each wxSkin control separately in a normal wxWidgets applicarton. If you only want to add a skinned button for example and kepp the rest "standard", there maybe is no need to create a skin file.
	The process is as simple as creating a "normal" GUI: Create the elements and place them on the GUI. The only addition is that you will have to assign images for different states manually.
	
	\subsection skinfiles Do you want to use skin files to control the look?
	This means that you will be using an XML file, formated as described here \ref skin_file and that you will be using wxSkinEngine to load and process the files.
	This should be the most common way to go, as it allows you or your users to switch skins, even during runtime.
	
	\subsection simpleframe Do you want to use a simple frame?
	A simple frame is a frame that is created by using a single image file. The image file is used to create a shaped frame that will be filled entirely with the image. A simple frame can have a client area defined
	where you can place other controls using a sizer. Mostly you'd define a simple frame where the GUI is not resizable. Other skinned controls are mostly placed on it using absolute positioning.
	
	\subsection complexframe Do you want to use a complex frame?
	Complex frames are a bit like standard frames. They can be resized and have a client area that can be filled with other controls. To create a complex frame you'll need seven images for a whole
	frame:
	- 4 images to create borders (left, right, top, bottom)
	- 4 images to create edges (top-left, top-right, bottom-left, bottom-right)
	- 1 image for the "body" (internal) area.
	
	wxSkin will place and resize the given images as needed. By default the client area is defined to match the "body". Note that this class is not yet perfect.

	\section create_sample Creating a simple example media player
	In this tutorial we will create a wxSkinSimpleFrame based application. The application will make use of different controls and have a wxMediaCtrl to playback videos.
	
	\subsection step_1 Step 1: The main frame
	First of all you'll need an image for your frame. I choose this one
	
	\image html fond.png
	
	The image is a PNG file that has transparent parts. The resulting frame using this image will adapt the transparency and be shaped like the image.
	
	In order to display our frame, we need to create an instance of wxSkinSimpleFrame. As we're going to use a skin file to control skins we don't need to pass an image to it right now.
	we simply create an instance of wxSkinFrame like this
\code
wxSkinSimpleFrame* frame = new wxSkinSimpleFrame(NULL,wxID_ANY,"Title goes here");
\endcode
	Showing the frame now might not yield the expected result, as it has no skin assigned yet.
	
	\subsection step_2 Step 2: The GUI elements
	In order to have a working media player, we need some controls to, well, control our application :-)
	
	To keep things simple our application uses mostly wxSkinButtons. There also is a wxSkinCheckBox for muting the volume and a wxSkinSlider to indicate the current position while playback.
	
	For example the "play" wxSkinButton
	\image html play.png "Standard skin"
	\image html play_down.png "Skin displayed when the button is clicked or the mouse moves over the button"
	
	The wxSkinCheckBox for muting is quite similar
	\image html mute.png "Standard skin"
	\image html mute_over.png Skin for "checked" state
	
	For the wxSkinSlider we use three images
	\image html position_back.png The "background"
	\image html position.png The slider "grip"
	\image html position_over.png The slider "grip" displayed when the mouse is over it
	
	In order to have the controls skinned later on, create instances of them like
	\code
wxSkinButton* btnPlay = new wxSkinButton(frame,ID_PLAY);
wxSkinSlider* sldPos = new wxSkinSlider(frame,ID_SLIDER);
wxSkinCheckBox* chkMute = new wxSkinCheckBox(frame,ID_MUTE);
	\endcode
	Great! Believe it or not, but we're already halfway-through.
	
	For simplicity I did not derive a frame from wxSkinSimpleFrame and add controls there, but you could have done that as well. Instead I created everthing directly in wxApp::OnInit().
	So far it should look like:
	\code
bool MyMediaApp::OnInit()
{
	wxSkinSimpleFrame* frame = new wxSkinSimpleFrame(NULL,wxID_ANY,"Title goes here");
	
	wxSkinButton* btnPlay = new wxSkinButton(frame,ID_PLAY);
	wxSkinSlider* sldPos = new wxSkinSlider(frame,ID_SLIDER);
	wxSkinCheckBox* chkMute = new wxSkinCheckBox(frame,ID_MUTE);
	
	return true;
}
	\endcode
	
	\subsection step_3 Step 3: Add code to load the skins
	As I said we're going to use an XML skin file to load and create out GUI. So far we've only created the needed instances of our controls, but didn't add skins to them. Let's do this now.
	
	In order to load, parse and process skin files we need wxSkinEngine. This class is a singleton accessible through wxSkinEngine::Get().
	We will need three methods of it: Load(), AssignControl() and InitialzeSkin().
	
	Each control defined in the skin file has (most likely) its own label by which it is identified. So far we only have the definition in the XML file (giving images to use, position size, etc.) and the "real"
	control that should use these settings. In order to connect the XML definition with the control, we have to use AssignControl().
	\code
wxSkinEngine::Get()->AssignControl("button_play",btnPlay);
	\endcode
	The above statement simply tells wxSkinEngine that there will be a label in the XML file called "button_play" and that the control to be used with the definitions found there will be btnPlay.
	Note that you should avoid to assign one control to many labels. wxSkinEngine will check if there are duplicate labels assigned, but it won't do that for duplicate control assignments.
	
	To load a skin file do something like
	\code
if(!wxSkinEngine::Get()->Load(wxT("PATH_TO_MY_SKIN/skin.xml")))
{
	wxMessageBox("Missing skin file or skin file is broken...");
}
	\endcode
	Load() gets the location of a skin file to load. This can either be a simple XML file, or a zip archive. wxSkinEngine will determine if the file passed is an archive or not. Note that when using an archive
	the XML file has to be located at the root of the archive and named skin.xml. The archive itself can have any name and extension.
	
	If the skin file was loaded successfully, all that is left to do is to initialize the skin.
	\code
wxSkinEngine::Get()->InitializeSkin();
	\endcode
	If everything worked you should now be able to show the frame and have a skinned application!
	
	OnInit() should look something like this:
	\code
bool MyMediaApp::OnInit()
{
	wxSkinSimpleFrame* frame = new wxSkinSimpleFrame(NULL,wxID_ANY,"Title goes here");
	
	wxSkinButton* btnPlay = new wxSkinButton(frame,ID_PLAY);
	wxSkinSlider* sldPos = new wxSkinSlider(frame,ID_SLIDER);
	wxSkinCheckBox* chkMute = new wxSkinCheckBox(frame,ID_MUTE);
	
	wxSkinEngine::Get()->AssignControl("button_play",btnPlay);
	wxSkinEngine::Get()->AssignControl("slider_position",sldPos);
	wxSkinEngine::Get()->AssignControl("check_mute",chkMute);
	
	if(!wxSkinEngine::Get()->Load(wxT("PATH_TO_MY_SKIN/skin.xml")))
	{
		wxMessageBox("Missing skin file or skin file is broken...");
		return false; //we exit the application here
	}
	
	wxSkinEngine::Get()->InitializeSkin();
	
	frame->Show();
	
	return true;
}
	\endcode
	
	\subsection step_4 Step 4: Add the missing wxMedaiCtrl
	Right, I told you this would be a media player, so we need to add some media playback control. In this case we use wxMediaCtrl.
	
	wxSkin frames can be assigned sizers that will be treated a bit special. For a wxSkinSimpleFrame the area that the sizer can fill is the client area defined in the skin file.
	To create a wxMediaCtrl that fits the client area of our skin, we simple do something like
	\code
wxMediaCtrl* mediaCtrl = new wxMediaCtrl(frame,wxID_ANY,"");
wxBoxSizer* box = new wxBoxSizer(wxVERTICAL);
box->Add(mediaCtrl,1,wxEXPAND,0);
frame->SetSizer(box);
	\endcode
	
	So, OnInit() would look like
	\code
bool MyMediaApp::OnInit()
{
	wxSkinSimpleFrame* frame = new wxSkinSimpleFrame(NULL,wxID_ANY,"Title goes here");
	
	wxSkinButton* btnPlay = new wxSkinButton(frame,ID_PLAY);
	wxSkinSlider* sldPos = new wxSkinSlider(frame,ID_SLIDER);
	wxSkinCheckBox* chkMute = new wxSkinCheckBox(frame,ID_MUTE);
	
	wxSkinEngine::Get()->AssignControl("button_play",btnPlay);
	wxSkinEngine::Get()->AssignControl("slider_position",sldPos);
	wxSkinEngine::Get()->AssignControl("check_mute",chkMute);
	
	if(!wxSkinEngine::Get()->Load(wxT("PATH_TO_MY_SKIN/skin.xml")))
	{
		wxMessageBox("Missing skin file or skin file is broken...");
		return false; //we exit the application here
	}
	
	wxSkinEngine::Get()->InitializeSkin();
	
	wxMediaCtrl* mediaCtrl = new wxMediaCtrl(frame,wxID_ANY,"");
	wxBoxSizer* box = new wxBoxSizer(wxVERTICAL);
	box->Add(mediaCtrl,1,wxEXPAND,0);
	frame->SetSizer(box);
	
	frame->Show();
	
	return true;
}
	\endcode
	Thats about all there is to do. For completeness here's an example skin file for this example. Information about the skin file format can be found in the \ref skin_file overview.
	
	\htmlonly
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<style type="text/css">
.S0 {
	font-family: 'Times New Roman';
	color: #000000;
	background: #FFFFFF;
	font-size: 11pt;
}
.S1 {
	color: #000080;
	background: #FFFFFF;
}
.S3 {
	color: #008080;
	background: #FFFFFF;
}
.S6 {
	color: #7F007F;
	background: #FFFFFF;
}
.S8 {
	color: #800080;
	background: #FFFFFF;
}
.S11 {
	color: #000080;
	background: #FFFFFF;
}
.S12 {
	font-weight: bold;
	color: #800080;
	background: #FFFFFF;
}
.S13 {
	font-weight: bold;
	color: #800080;
	background: #FFFFFF;
}
</style>
</head>
<body bgcolor="#FFFFFF">
<span><span class="S12">&lt;?</span><span class="S1">xml</span><span class="S8"> </span><span class="S3">version</span><span class="S8">=</span><span class="S6">"1.0"</span><span class="S8"> </span><span class="S3">encoding</span><span class="S8">=</span><span class="S6">"ISO-8859-1"</span><span class="S13">?&gt;</span><br />
<br />
<span class="S1">&lt;Skin</span><span class="S8"> </span><span class="S3">name</span><span class="S8">=</span><span class="S6">"Example skin"</span><span class="S8"> </span><span class="S3">author</span><span class="S8">=</span><span class="S6">"Rene aka upCASE"</span><br />
<span class="S8">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="S3">comment</span><span class="S8">=</span><span class="S6">"A simple mediaplayer skin\nCredits go to jix for creating the VLC skin"</span><br />
<span class="S8">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="S3">screenshot</span><span class="S8">=</span><span class="S6">"wxskin2.jpg"</span><span class="S8"> </span><span class="S3">application</span><span class="S8">=</span><span class="S6">"wxSkin Media Playe example"</span><br />
<span class="S8">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="S3">customInfo</span><span class="S8">=</span><span class="S6">""</span><span class="S1">&gt;</span><br />
<br />
<span class="S0">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="S1">&lt;Transparency</span><span class="S8"> </span><span class="S3">red</span><span class="S8">=</span><span class="S6">"255"</span><span class="S8"> </span><span class="S3">green</span><span class="S8">=</span><span class="S6">"0"</span><span class="S8"> </span><span class="S3">blue</span><span class="S8">=</span><span class="S6">"255"</span><span class="S8"> </span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><br />
<span class="S0">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="S1">&lt;Buttons&gt;</span><br />
<span class="S0">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="S1">&lt;button_play</span><span class="S8"> </span><span class="S3">normal</span><span class="S8">=</span><span class="S6">"play.png"</span><span class="S8"> </span><span class="S3">pressed</span><span class="S8">=</span><span class="S6">"play_down.png"</span><span class="S8"> </span><span class="S3">pos</span><span class="S8">=</span><span class="S6">"631,82"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="S1">&lt;/Buttons&gt;</span><br />
<span class="S0">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="S1">&lt;Checkboxes&gt;</span><br />
<span class="S0">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="S1">&lt;mute</span><span class="S8"> </span><span class="S3">normal</span><span class="S8">=</span><span class="S6">"mute.png"</span><span class="S8"> </span><span class="S3">checked</span><span class="S8">=</span><span class="S6">"mute_over.png"</span><span class="S8"> </span><span class="S3">pos</span><span class="S8">=</span><span class="S6">"41,249"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="S1">&lt;/Checkboxes&gt;</span><br />
<br />
<span class="S0">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="S1">&lt;Sliders&gt;</span><br />
<span class="S0">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="S1">&lt;progressslide</span><span class="S8"> </span><span class="S3">slider</span><span class="S8">=</span><span class="S6">"position.png"</span><span class="S8"> </span><span class="S3">background</span><span class="S8">=</span><span class="S6">"position_back.png"</span><span class="S8"> </span><span class="S3">over</span><span class="S8">=</span><span class="S6">"position_over.png"</span><span class="S8"> </span><span class="S3">pos</span><span class="S8">=</span><span class="S6">"177,297"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="S1">&lt;/Sliders&gt;</span><br />
<span class="S0">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><br />
<span class="S0">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="S1">&lt;SimpleFrame</span><span class="S8"> </span><span class="S3">name</span><span class="S8">=</span><span class="S6">"PlayerFrame"</span><span class="S8"> </span><span class="S3">skin</span><span class="S8">=</span><span class="S6">"fond.png"</span><span class="S8"> </span><span class="S3">movable</span><span class="S8">=</span><span class="S6">"yes"</span><span class="S8"> &nbsp;</span><span class="S3">guibuilder</span><span class="S8">=</span><span class="S6">""</span><span class="S1">&gt;</span><br />
<span class="S0">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="S1">&lt;client_area</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">"165,35,409,230"</span><span class="S8"> </span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="S1">&lt;closebox</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">"680,219,-1,-1"</span><span class="S8"> </span><span class="S3">normal</span><span class="S8">=</span><span class="S6">"close.png"</span><span class="S8"> </span><span class="S3">over</span><span class="S8">=</span><span class="S6">"close_over.png"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="S1">&lt;minimizebox</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">"660,252,-1,-1"</span><span class="S8"> </span><span class="S3">normal</span><span class="S8">=</span><span class="S6">"reduce.png"</span><span class="S8"> </span><span class="S3">over</span><span class="S8">=</span><span class="S6">"reduce_over.png"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="S1">&lt;maximizebox</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">"2,2,2,2"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="S1">&lt;titlebar</span><span class="S8"> </span><span class="S3">measure</span><span class="S8">=</span><span class="S6">"160,10,418,20"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="S1">&lt;titleposition</span><span class="S8"> </span><span class="S3">placement</span><span class="S8">=</span><span class="S6">"left"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="S1">&lt;titlefont</span><span class="S8"> </span><span class="S3">name</span><span class="S8">=</span><span class="S6">"Comic Sans MS"</span><span class="S8"> </span><span class="S3">size</span><span class="S8">=</span><span class="S6">"12"</span><span class="S8"> </span><span class="S3">weight</span><span class="S8">=</span><span class="S6">"normal"</span><span class="S8"> </span><span class="S3">underline</span><span class="S8">=</span><span class="S6">"false"</span><span class="S8"> </span><span class="S3">italic</span><span class="S8">=</span><span class="S6">"false"</span><span class="S11">/&gt;</span><br />
<span class="S0">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><span class="S1">&lt;/SimpleFrame&gt;</span><br />
<span class="S1">&lt;/Skin&gt;</span></span>
</body>
</html>
	\endhtmlonly
	
	Now is the time to add event handlers and processing to the application.
	In th end our example application should look something like this (I added more buttons):
	\image html wxskin2.jpg
	
	\subsection step_5 Step 5: And how do I change skins?
	Simple: Leave the assigned controls untouched, call wxSkinEngine::ClearSkin() and load a new XML file.
	Then call InitializeSkin() and the new skin should appear (given that the labels in the skin file were the same, meaning the skin file matches a skin format for your application).
	
	\code
	//clears the currently active skin, no "visual" changes yet
	wxSkinEngine::Get->ClearSkin(); 
	//Load the new skin, better add a check here
	wxSkinEngine::Get->Load(wxT("PATH_TO_MY/newSkin.skinfile")); 
	//the same as above: this assigns new skins to all mapped controls and "reloads" the GUI
	wxSkinEngine::Get->InitializeSkin(); 
	\endcode
	
	\section conclu Conclusion
	As you can see creating a simple skinned application is not a hard or timetaking task.
	
	However, I left out some "advanced" features of wxSkin, like complex frames and support for GUI creation using Jorgen Boddes "guibuilder". 
	Have a look at the examples that come with wxSkin. One is the wxSkinSimpleFrame example above, the other a simple example of a using wxSkinFrame.
	An empty XML skin file can be found in template/skin_template.xml.
	
	I you haven't done it yet, I'd advice you to have a look at the \ref skin_file. This document explains how skin files can be created and modified and what entities you  can create and control with it.
	
	Have fun skinning your apps with wxSkin! I hope you like this addon. 
*/