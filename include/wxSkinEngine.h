/////////////////////////////////////////////////////////////////////////////////////////
// Name:		wxSkin
// Author:      René Kraus aka upCASE <himself@upcase.de>
// Copyright:   René Kraus (c)
// Licence:     wxWindows license <http://www.wxwidgets.org/licence3.txt>
//				with the addendum that you notify the author about projects using wxSkin
/////////////////////////////////////////////////////////////////////////////////////////


#ifndef __wxSkinEngine_H
#define __wxSkinEngine_H

#ifdef __GNUG__
	#pragma interface "wxSkinEngine.h"
#endif

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include <wx/image.h>

#include <wx/zipstrm.h>
#include <wx/fs_mem.h>
#include <wx/fs_zip.h>
#include <wx/tokenzr.h>
#include <wx/wfstream.h>
#include <wx/filename.h>

#include <wx/xml/xml.h>

#include "wxSkinWindow.h"

#if wxCHECK_VERSION(2, 7, 0)
	#define ISOK IsOk
#else
	#define ISOK Ok
#endif


///A class for loading, parsing and assigning skins
/**wxSkinEngine loads and parses skin files in the wxSkin XML format. It also maintains assocciations between
identifiers and windows and can assign (new) skins to existing window controls derived from wxSkinWindow. wxSkinEngine
is a signleton and you should not store pointers to it.
*/
class wxSkinEngine
{
	MapOfSimpleFrames m_frameSimpleMap;
	MapOfComplexFrames m_frameComplexMap;
	MapOfControls m_controlMap;
	MapOfWidgets m_widgetMap;
	MapOfAssignments m_assignedControls;
	
	wxFSFile* fsfile;
	wxString m_skinfile;

	unsigned long m_transR, m_transG, m_transB;

	wxString m_skinAuthor, m_skinName, m_skinComment,m_skinScreenshot,m_skinApplication, m_skinInfo;

	static wxSkinEngine* s_skinInstance;
public:
	/**As wxSkinEngine is a singleton, use Get() to access the running instance.
	*/
	static wxSkinEngine* Get();
	/**Destroys the currently active instance.
	*/
	static void Destroy();

	/**Load a wxSkin file from the given location. Files can either be zipped or plain XML files.
	Load() will determine the format automatically.
	\param file the location of the file to load
	\returns true on success, false if loading or parsing failed.
	*/
	bool Load(const wxString& file);
	/**Basically the same as Load(), but it only loads the "info" fields for skin files from the "skin" XML tag and returns the info. 
	\param file the location of the file to load
	\param name the name of the skin
	\param author the skins author
	\param comment the skins comment
	\param application the skins application property
	\param custInfo the skins custom info
	\param screenshot the skins screenshot
	\returns true on success, false if loading or parsing failed.
	*/
	bool LoadInfo(const wxString& file,wxString& name, wxString& author, wxString& comment, wxString& application, wxString& custInfo, wxImage& screenshot);
	/**Map an instance of a wxWindow or wxSkinWindow to a label. If the label occurs in a loaded
	wxSkin file, the skin and properties will be assigned to the control when calling InitializeSkin().
	*/
	bool AssignControl(const wxString& label, const wxWindow* ptr);
	/**Calling this initializes the loaded skin. This will assign the correct skin images and properties to all
	assigned/mapped windows.
	*/
	void InitializeSkin();
	/**Used internaly. Gets the "default" skin for the specified window.
	*/
	void GetDefaultSkin(wxSkinWindow* win);
	/**Clears the currently loaded skin. This does not reset the GUI, it only clears internal assignments.
	The map of assigned controls is not altered.
	*/
	void ClearSkin();
	/**Clears the map of assigned controls. Call this if you need to create a new label <-> control mapping.
	*/
	void ClearAssignments();

	/**Gets the tranparency colour set through the skin file. Standard colour is
	R=255, G=0, B=255.
	*/
	wxColour GetTransparencyColour(){ return wxColour(m_transR,m_transG,m_transB); }
	/**Returns the skin author property.
	*/
	wxString GetSkinAuthor(){ return m_skinAuthor; }
	/**Returns the skin name property.
	*/
	wxString GetSkinName(){ return m_skinName; }
	/**Returns the skin comment property.
	*/
	wxString GetSkinComment(){ return m_skinComment; }
	/**Returns the skin application property.
	*/
	wxString GetSkinApplication(){ return m_skinApplication; }
	/**Returns the custom info property.
	*/
	wxString GetCustomSkinInfo(){ return m_skinInfo; }
	/**Loads and returns a screenshot image for the skin if one was given.
	Note that screenshots will be resized if they are bigger than 300*300 px.
	*/
	wxImage GetSkinScreenshot()
	{
		wxFileSystem system;
		fsfile = system.OpenFile(m_skinfile+m_skinScreenshot);
		if(fsfile)
		{	wxImage img(*fsfile->GetStream(),wxBITMAP_TYPE_ANY);
			img.SetMaskColour(m_transR,m_transG,m_transB);
			if(img.ISOK())
			{	
				int w = img.GetWidth();
				int h = img.GetHeight();

                if(w <= 300 && h <= 300)
					return img;

				if( w >= h )
				{
					w = 300;
					h = (int)(img.GetHeight()*((float)300/(float)img.GetWidth()));
				}
				else
				{
					h = 300;
					w = (int)(img.GetWidth()*((float)300/(float)img.GetHeight()));
				}
			
				img.Rescale(w,h);
				return img;
			}
			delete fsfile;
		}
		return wxImage(300,300);
	}
	
	wxImage LoadSkinImage(const wxString& file);
private:
	wxSkinEngine();

	void ParseInnerAttributes(int type,wxXmlNode* node);
	void ParseSimpleFrameAttributes(wxXmlNode* node);
	void ParseFrameAttributes(wxXmlNode* node);
};

#endif
