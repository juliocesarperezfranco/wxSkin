/////////////////////////////////////////////////////////////////////////////////////////
// Name:		wxSkin
// Author:      René Kraus aka upCASE <himself@upcase.de>
// Copyright:   René Kraus (c)
// Licence:     wxWindows license <http://www.wxwidgets.org/licence3.txt>
//				with the addendum that you notify the author about projects using wxSkin
/////////////////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
  #pragma implementation "wxSkinEngine.h"
#endif

#include "wxSkinEngine.h"

#include "wxSkinSimpleFrame.h"
#include "wxSkinFrame.h"
#include "wxSkinButton.h"
#include "wxSkinStaticText.h"
#include "wxSkinWindow.h"
#include "wxSkinSplitterWindow.h"

//ifdef this
#include "guibuilder.h"
#include <wx/txtstrm.h>
#include <wx/settings.h>


wxSkinEngine* wxSkinEngine::s_skinInstance = NULL;

wxSkinEngine::wxSkinEngine()
{
  fsfile = NULL;

  //Disable logging for all builds
  //if wxInitAllImageHandlers was called before, calling it again leads to log messages in debug builds.
  {	wxLogNull noLog;
    wxInitAllImageHandlers();
  }
  wxFileSystem::AddHandler(new wxZipFSHandler);

  m_transR = 255;
  m_transG = 0;
  m_transB = 255;
}

void wxSkinEngine::Destroy()
{	
  if(s_skinInstance)
  {	/*s_skinInstance->m_controlMap.clear();
    s_skinInstance->m_frameSimpleMap.clear();
    s_skinInstance->m_frameComplexMap.clear();
    s_skinInstance->m_widgetMap.clear();
    s_skinInstance->m_assignedControls.clear();*/
    delete s_skinInstance;
    s_skinInstance = NULL;
  }
}
wxSkinEngine* wxSkinEngine::Get()
{
  if(!s_skinInstance)
    s_skinInstance = new wxSkinEngine;
  return s_skinInstance;
}
void wxSkinEngine::ClearSkin()
{
  m_skinfile.Empty();
  for(MapOfControls::iterator itControls = m_controlMap.begin();
    itControls != m_controlMap.end(); ++itControls)
  {
      ControlInfo *info = itControls->second;
    delete info;
  }
  m_controlMap.clear();
  m_frameSimpleMap.clear();
  m_frameComplexMap.clear();
  m_widgetMap.clear();

  m_transR = 255;
  m_transG = 0;
  m_transB = 255;

  m_skinAuthor.Empty();
  m_skinName.Empty();
  m_skinComment.Empty();
  m_skinScreenshot.Empty();
  m_skinApplication.Empty();
  m_skinInfo.Empty();
}
void wxSkinEngine::ClearAssignments()
{
  m_assignedControls.clear();
}
bool wxSkinEngine::AssignControl(const wxString& label, const wxWindow* ptr)
{
  if(m_assignedControls.find(label)==m_assignedControls.end())
  {	//&& m_assignedControls.count((wxWindow*)ptr) == 0
    m_assignedControls[label] = (wxWindow*)ptr;
    return true;
  }
  return false;
}

/*
bool wxSkinEngine::ReassignControlSkin(const wxWindow* ptr, const wxString& label)
{
  MapOfControls::iterator itControl = m_controlMap.find(label);
  if(itControl != m_controlMap.end())
  {
    ControlInfo info = itControl->second;
    wxSkinWindow* win = wxDynamicCast(ptr,wxSkinWindow);
    if(win)
    {	
      wxLogNull nullLog;

      win->SetNormalSkin(LoadSkinImage(m_skinfile+info.skinState1));
      win->SetState2Skin(LoadSkinImage(m_skinfile+info.skinState2));
      win->SetOverSkin(LoadSkinImage(m_skinfile+info.skinOver));
      win->SetDisabledSkin(LoadSkinImage(m_skinfile+info.skinDisabled));

      win->SetSize(info.measure);
      win->Show(info.shown);

      win->Refresh();

      return true;
    }
    
  }

  MapOfWidgets::iterator itWidgets = m_widgetMap.find(label);
  if(itWidgets != m_widgetMap.end())
  {
    wxWindow* win = wxDynamicCast(ptr,wxWindow);
    if(win)
    {
      CustomWidgetInfo inf = itWidgets->second;
      
      win->SetSize(inf.measure);
      win->Show(inf.shown);
      win->Refresh();
      return true;
    }
    
  }
  
  return false;
}
*/

void wxSkinEngine::InitializeSkin()
{
  wxFileSystem system;

  for(MapOfAssignments::iterator itAssigned = m_assignedControls.begin();
    itAssigned != m_assignedControls.end(); ++itAssigned)
  {
    wxString label = itAssigned->first;	

    MapOfSimpleFrames::iterator itFrame = m_frameSimpleMap.find(label);
    if(itFrame != m_frameSimpleMap.end())
    {
      SimpleFrameInfo info = itFrame->second;
      wxSkinSimpleFrame* frame = wxDynamicCast(itAssigned->second,wxSkinSimpleFrame);
      if(frame)
      {	
        bool wasShown = false;
        if(frame->IsShown())
        {	
          frame->Hide();
          //frame->Freeze();
          wasShown = true;
        }
        
        frame->SetSkin(LoadSkinImage(info.skinFile));
        
        frame->SetMovable(info.movable);
        frame->SetClientArea(info.clientarea);

        ////////CLOSEBOX
        frame->SetClose(info.closebox,LoadSkinImage(info.skin_closebox));
        frame->SetCloseOver(LoadSkinImage(info.skin_closebox_over));
        frame->SetClosePressed(LoadSkinImage(info.skin_closebox_press));
        ////////////////
        ////////MINIMIZEBOX
        frame->SetMinimize(info.minimizebox,LoadSkinImage(info.skin_minimizebox));
        frame->SetMinimizeOver(LoadSkinImage(info.skin_minimizebox_over));
        frame->SetMinimizePressed(LoadSkinImage(info.skin_minimizebox_press));
        ////////////////
        ////////MAXIMIZEBOX
        frame->SetMaximize(info.minimizebox,LoadSkinImage(info.skin_maximizebox));
        frame->SetMaximizeOver(LoadSkinImage(info.skin_maximizebox_over));
        frame->SetMaximizePressed(LoadSkinImage(info.skin_maximizebox_press));
        ////////////////

        frame->SetTitleRect(info.titlebar);
        frame->SetTitlePlacement(info.titleposition);
        
        if(info.font.ISOK())
          frame->SetFont(info.font);
        else
          frame->SetFont(wxSystemSettings::GetFont(wxSYS_SYSTEM_FONT));

        frame->RecreateSizer();

        if(!info.guibuilderFile.IsEmpty())
        {
          wxFileSystem system;
          wxFSFile* thefile = system.OpenFile(m_skinfile+info.guibuilderFile);
          if(thefile)
          {	wxInputStream* stream = thefile->GetStream();
            long long size = stream->GetLength();
            wxString content;
            stream->Read(wxStringBuffer(content,size),size);
            wxGuiBuilder gb(content);
            gb.Setup(frame,wxID_ANY,true);

            for(MapOfAssignments::iterator itAssignedWnd = m_assignedControls.begin();
              itAssignedWnd != m_assignedControls.end(); ++itAssignedWnd)
            {
              wxString label = itAssignedWnd->first;
              if(gb.CheckWindowByLabel(label))
              {	wxWindow* wnd = wxDynamicCast(itAssignedWnd->second,wxWindow);
                if(wnd)
                  gb << wxGbWindowStub(label,wnd);
              }
              
            }

            if(gb.Build(frame, wxID_ANY))
              frame->SetSizer(gb.GetSizer());

            delete thefile;
          }
        }

        if(wasShown)
        {
          //frame->Thaw();
          frame->Show();
        }
        frame->Restore();
        
      }
      continue;
    }

    MapOfComplexFrames::iterator itCFrame = m_frameComplexMap.find(label);
    if(itCFrame != m_frameComplexMap.end())
    {
      ComplexFrameInfo info = itCFrame->second;
      wxSkinFrame* frame = wxDynamicCast(itAssigned->second,wxSkinFrame);
      if(frame)
      {
        bool wasShown = false;
        if(frame->IsShown())
        {	
          frame->Hide();
          wasShown = true;
        }

        int initWidth = 0, initHeight = 0;

        wxImage img = LoadSkinImage(info.skin_top);
        initWidth += img.GetWidth();
        frame->SetTopSkin(img);

        img = LoadSkinImage(info.skin_topleft);
        initWidth += img.GetWidth();
        initHeight += img.GetHeight();
        frame->SetTopLeftSkin(img);

        img = LoadSkinImage(info.skin_topright);
        initWidth += img.GetWidth();
        frame->SetTopRightSkin(img);

        img = LoadSkinImage(info.skin_bottom);
        initHeight += img.GetHeight();
        frame->SetBottomSkin(img);

        frame->SetBottomLeftSkin(LoadSkinImage(info.skin_bottomleft));
        frame->SetBottomRightSkin(LoadSkinImage(info.skin_bottomright));

        img = LoadSkinImage(info.skin_left);
        initHeight += img.GetHeight();
        frame->SetLeftSkin(img);

        frame->SetRightSkin(LoadSkinImage(info.skin_right));
        frame->SetBodySkin(LoadSkinImage(info.skin_body));

        frame->SetMovable(info.movable);
        ////////CLOSEBOX
        frame->SetClose(info.closebox,LoadSkinImage(info.skin_closebox));
        frame->SetCloseOver(LoadSkinImage(info.skin_closebox_over));
        frame->SetClosePressed(LoadSkinImage(info.skin_closebox_press));
        ////////////////
        ////////MINIMIZEBOX
        frame->SetMinimize(info.minimizebox,LoadSkinImage(info.skin_minimizebox));
        frame->SetMinimizeOver(LoadSkinImage(info.skin_minimizebox_over));
        frame->SetMinimizePressed(LoadSkinImage(info.skin_minimizebox_press));
        ////////////////
        ////////MAXIMIZEBOX
        frame->SetMaximize(info.minimizebox,LoadSkinImage(info.skin_maximizebox));
        frame->SetMaximizeOver(LoadSkinImage(info.skin_maximizebox_over));
        frame->SetMaximizePressed(LoadSkinImage(info.skin_maximizebox_press));
        ////////////////		

        frame->SetCloseBoxRelation(info.closebox_relateto);
        frame->SetMinimizeBoxRelation(info.minimizebox_relateto);

        frame->SetTitleRect(wxRect(0,0,0,info.titlebarheight));
        frame->SetTitlePlacement(info.titleposition);

        if(info.font.ISOK())
          frame->SetFont(info.font);
        else
          frame->SetFont(wxSystemSettings::GetFont(wxSYS_SYSTEM_FONT));

        //frame->SetWindowStyle(flags | info.titleposition);
        frame->SetSize(wxSize(initWidth,initHeight));
        frame->RecreateSizer();

        if(!info.guibuilderFile.IsEmpty())
        {
          wxFileSystem system;
          wxFSFile* thefile = system.OpenFile(m_skinfile+info.guibuilderFile);
          if(thefile)
          {	wxInputStream* stream = thefile->GetStream();
            long long size = stream->GetLength();
            wxString content;
            stream->Read(wxStringBuffer(content,size),size);
            wxGuiBuilder gb(content);
            gb.Setup(frame,wxID_ANY,true);

            for(MapOfAssignments::iterator itAssignedWnd = m_assignedControls.begin();
              itAssignedWnd != m_assignedControls.end(); ++itAssignedWnd)
            {
              wxString label = itAssignedWnd->first;
              if(gb.CheckWindowByLabel(label))
              {	wxWindow* wnd = wxDynamicCast(itAssignedWnd->second,wxWindow);
                if(wnd)
                  gb << wxGbWindowStub(label,wnd);
              }
              
            }

            if(gb.Build(frame, wxID_ANY))
              frame->SetSizer(gb.GetSizer());

            delete thefile;
          }
        }

        if(wasShown)
        {
          frame->Show();
        }
      }
      continue;
    }

    MapOfControls::iterator itControl = m_controlMap.find(label);
    if(itControl != m_controlMap.end())
    {
      ControlInfo *info = itControl->second;
      wxSkinWindow* win = wxDynamicCast(itAssigned->second,wxSkinWindow);
      if(win)
      {
        wxLogNull nullLog;
                win->SetSkin(info);
                continue;
      }
      
      wxSkinStaticText *win1 = wxDynamicCast(itAssigned->second,wxSkinStaticText);
      if (win1) 
      {
          wxLogNull nullLog;
                win1->SetSkin(info);
                continue;
      }
      
      wxSkinSplitterWindow *win2 = wxDynamicCast(itAssigned->second,wxSkinSplitterWindow);
      if (win2) 
      {
          wxLogNull nullLog;
                win2->SetSkin(info);
                continue;
      }
      continue;
    }

    MapOfWidgets::iterator itWidgets = m_widgetMap.find(label);
    if(itWidgets != m_widgetMap.end())
    {
      wxWindow* win = wxDynamicCast(itAssigned->second,wxWindow);
      if(win)
      {
        CustomWidgetInfo inf = itWidgets->second;
        //win->SetPosition(inf.position);
        win->SetSize(inf.measure);
        win->Show(inf.shown);
      }
      continue;
    }
    
  }

  /*for(MapOfAssignments::iterator itAssigned = m_assignedControls.begin();
    itAssigned != m_assignedControls.end(); ++itAssigned)
  {
    itAssigned->second->Refresh();
  }*/
}
wxImage wxSkinEngine::LoadSkinImage(const wxString& file)
{
  wxFileSystem system;
  wxFSFile* thefile = system.OpenFile(m_skinfile+file);
  if(thefile)
  {	wxImage img(*thefile->GetStream(),wxBITMAP_TYPE_ANY);
    delete thefile;
    if(img.HasAlpha())
    {	img.ConvertAlphaToMask();
    }
    else
      img.SetMaskColour(m_transR,m_transG,m_transB);
    if(img.ISOK())
    {	return img;
    }
  }
  
  return wxNullImage;
}
void wxSkinEngine::GetDefaultSkin(wxSkinWindow* win)
{	
  wxFileSystem system;

  int type = win->GetSkinWindowType();
  wxString nodeName = wxT("default");

  switch(type)
  {
    case TYPE_BUTTON:
    {	
      nodeName.Append(wxT("BUTTON"));
    }	
    break;
    case TYPE_CHECKBOX:
    {	
      nodeName.Append(wxT("CHECKBOX"));
    }	
    break;
    case TYPE_RADIOBUTTON:
    {	nodeName.Append(wxT("RADIOBUTTON"));
    }	
    break;
    case TYPE_GAUGE:
    {	
      nodeName.Append(wxT("GAUGE"));
    }	
    break;
    case TYPE_SLIDER:
    {	
      nodeName.Append(wxT("SLIDER"));
    }	
    break;
    case TYPE_PANEL:
    {	
      nodeName.Append(wxT("PANEL"));
    }
    break;
    case TYPE_STATICTEXT:
    {	
      nodeName.Append(wxT("STATICTEXT"));
    }
    break;
    case TYPE_SPLITTERWINDOW:
    {	
      nodeName.Append(wxT("SPLITTERWINDOW"));
    }
    break;
  }

  MapOfControls::iterator itControl = m_controlMap.find(nodeName);

  if(itControl != m_controlMap.end())
  {
    ControlInfo* info = itControl->second;

    wxLogNull nullLog;
        win->SetSkin(info);
  }
  else
  {	
        win->SetSkin(NULL);
  }
}
bool wxSkinEngine::LoadInfo(const wxString& file,wxString& name, wxString& author, wxString& comment, wxString& application, wxString& custInfo, wxImage& screenshot)
{
  wxFileSystem system;

  wxString filePrefix;

  {
    wxLogNull logNo;
    filePrefix = file+wxT("#zip:");
    fsfile = system.OpenFile(filePrefix+wxT("skin.xml"));

    if(!fsfile)
    {	fsfile = system.OpenFile(file);
      wxFileName filename(file);
      filePrefix = filename.GetPath()+ wxT("/");
    }
  }

  //m_skinfile = filePrefix;
  
  if(fsfile==NULL)
  {	wxLogError(wxT("Couldn't open the specified skin file."));
    return false;
  }

  wxXmlDocument configFile(*fsfile->GetStream());

  wxXmlNode* root = configFile.GetRoot();
  if(!root)
  {	wxLogError(wxT("Couldn't get XML root."));
    delete fsfile;
    return false;
  }
  if(root->GetName().Lower() != wxT("skin"))
  {	delete fsfile;
    return false;
  }

  name = root->GetAttribute(wxT("name"),wxT(""));
  author = root->GetAttribute(wxT("author"),wxT(""));
  comment = root->GetAttribute(wxT("comment"),wxT(""));
  comment.Replace(wxT("\\n"),wxT("\n"));

  wxString screenloc = root->GetAttribute(wxT("screenshot"),wxT(""));
  application = root->GetAttribute(wxT("application"),wxT(""));

  custInfo = root->GetAttribute(wxT("customInfo"),wxT(""));

  delete fsfile;
  fsfile = system.OpenFile(filePrefix+screenloc);
  if(fsfile)
  {	wxImage img(*fsfile->GetStream(),wxBITMAP_TYPE_ANY);
    img.SetMaskColour(m_transR,m_transG,m_transB);
    if(img.ISOK())
    {	
      int w = img.GetWidth();
      int h = img.GetHeight();

            if(w <= 300 && h <= 300)
      {	screenshot = img;
        delete fsfile;
        return true;
      }	
      else if( w >= h )
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
      screenshot = img;
    }
    delete fsfile;
  }
  else
    screenshot = wxImage(300,300);

  return true;
}
bool wxSkinEngine::Load(const wxString& file)
{
    wxFileSystem system;

    wxString filePrefix;

    {
      wxLogNull logNo;
      filePrefix = file+wxT("#zip:");
      fsfile = system.OpenFile(filePrefix+wxT("skin.xml"));

      if(!fsfile)
      {	fsfile = system.OpenFile(file);
        wxFileName filename(file);
        filePrefix = filename.GetPath()+ wxT("/");
      }
    }

    m_skinfile = filePrefix;
    
    if(fsfile==NULL)
    {	wxLogError(wxT("Couldn't open the specified skin file."));
      return false;
    }

    wxXmlDocument configFile(*fsfile->GetStream());

    wxXmlNode* root = configFile.GetRoot();
    if(!root)
    {	wxLogError(wxT("Couldn't get XML root."));
      delete fsfile;
      return false;
    }

    m_skinName = root->GetAttribute(wxT("name"),wxT(""));
    m_skinAuthor = root->GetAttribute(wxT("author"),wxT(""));
    m_skinComment = root->GetAttribute(wxT("comment"),wxT(""));
    m_skinComment.Replace(wxT("\\n"),wxT("\n"));

    m_skinScreenshot = root->GetAttribute(wxT("screenshot"),wxT(""));
    m_skinApplication = root->GetAttribute(wxT("application"),wxT(""));

    m_skinInfo = root->GetAttribute(wxT("customInfo"),wxT(""));
  
    wxXmlNode* node = root->GetChildren();
    while(node)
    {
      if( node->GetType() != wxXML_ELEMENT_NODE )
      {	node = node->GetNext();
        continue;
      }

      wxString nodeName = node->GetName();
      if( nodeName == wxT("SimpleFrame") )
        ParseSimpleFrameAttributes(node);
      else if( nodeName == wxT("Frame") )
        ParseFrameAttributes(node);
      else if( nodeName == wxT("Buttons") )
        ParseInnerAttributes(TYPE_BUTTON,node);	
      else if( nodeName == wxT("Checkboxes") )
        ParseInnerAttributes(TYPE_CHECKBOX,node);
      else if( nodeName == wxT("Radiobuttons") )
        ParseInnerAttributes(TYPE_RADIOBUTTON,node);
      else if( nodeName == wxT("Gauges") )
        ParseInnerAttributes(TYPE_GAUGE,node);
      else if( nodeName == wxT("Sliders") )
        ParseInnerAttributes(TYPE_SLIDER,node);
      else if( nodeName == wxT("Panels") )
        ParseInnerAttributes(TYPE_PANEL,node);
      else if( nodeName == wxT("StaticTexts") )
        ParseInnerAttributes(TYPE_STATICTEXT,node);
      else if( nodeName == wxT("SplitterWindows") )
        ParseInnerAttributes(TYPE_SPLITTERWINDOW,node);
      else if( nodeName == wxT("Widgets") )
      {	
        wxXmlNode* thisNode = node->GetChildren();
        while(thisNode)
        {
          if( thisNode->GetType() != wxXML_ELEMENT_NODE )
          {	thisNode = thisNode->GetNext();
            continue;
          }
          wxString nodeNameInner = thisNode->GetName();

          CustomWidgetInfo info;
          info.measure.x = info.measure.y = 
            info.measure.width = info.measure.height = -1;
          wxString isShown = thisNode->GetAttribute(wxT("shown"),wxT("true"));
          info.shown = (isShown.Lower() == wxT("true")) ? true : false;

          //wxRect widgetRect;
          wxStringTokenizer tkz(thisNode->GetAttribute(wxT("pos"),wxT("")),wxT(","));
          if(tkz.HasMoreTokens())
          {	tkz.GetNextToken().ToLong((long*)&info.measure.x);
            tkz.GetNextToken().ToLong((long*)&info.measure.y);
            //tkz.GetNextToken().ToLong((long*)&info.rect.width);
            //tkz.GetNextToken().ToLong((long*)&info.rect.height);
          }
          wxStringTokenizer tkzsize(thisNode->GetAttribute(wxT("size"),wxT("")),wxT(","));
          if(tkzsize.HasMoreTokens())
          {	tkzsize.GetNextToken().ToLong((long*)&info.measure.width);
            tkzsize.GetNextToken().ToLong((long*)&info.measure.height);
            //tkz.GetNextToken().ToLong((long*)&info.rect.width);
            //tkz.GetNextToken().ToLong((long*)&info.rect.height);
          }
          m_widgetMap[nodeNameInner] = info;

          thisNode = thisNode->GetNext();
        }

      }
      else if( nodeName == wxT("Transparency") )
      {
        node->GetAttribute(wxT("red"),wxT("")).ToULong(&m_transR);
        node->GetAttribute(wxT("green"),wxT("")).ToULong(&m_transG);
        node->GetAttribute(wxT("blue"),wxT("")).ToULong(&m_transB);
      }

      node = node->GetNext();
    }

    delete fsfile;
    fsfile = NULL;
/*		
    for(MapOfAssignments::iterator itAssigned = m_assignedControls.begin();
      itAssigned != m_assignedControls.end(); ++itAssigned)
    {
      wxString label = itAssigned->first;	
      if(m_frameSimpleMap.find(label) == m_frameSimpleMap.end() 
        && m_controlMap.find(label) == m_controlMap.end()
        && m_widgetMap.find(label) == m_widgetMap.end() )
      {	
        wxLogWarning("Missing definition for \"%s\" in skinfile",label.c_str());
      }
    }
*/
    return true;
}
void wxSkinEngine::ParseSimpleFrameAttributes(wxXmlNode* node)
{
  if( node == NULL )
    return;

  SimpleFrameInfo frameInfo;
  wxFileSystem system;
  
  wxString frameName = node->GetAttribute(wxT("name"),wxT(""));

  if(m_frameSimpleMap.find(frameName) != m_frameSimpleMap.end())
  {
    wxLogError(wxT("Frame named \"%s\" allready in use"),frameName.c_str());
    return;
  }

  wxString frameMove = node->GetAttribute(wxT("movable"),wxT("no"));
  frameInfo.movable = frameMove == wxT("yes") ? true : false;

  wxString value = node->GetAttribute(wxT("skin"),wxT(""));
  frameInfo.skinFile = value;

  wxString guival = node->GetAttribute(wxT("guibuilder"),wxT(""));
  frameInfo.guibuilderFile = guival;
      
  wxXmlNode* thisNode = node->GetChildren();

  while(thisNode)
  {	if( thisNode->GetType() != wxXML_ELEMENT_NODE )
    {	thisNode = thisNode->GetNext();
      continue;
    }

    wxString nodeName = thisNode->GetName();

    if(nodeName == wxT("titleposition"))
    {
      wxString placement = thisNode->GetAttribute(wxT("placement"),wxT(""));
      if(placement.Lower() == wxT("left"))
        frameInfo.titleposition = wxTITLE_LEFT;
      else if(placement.Lower() == wxT("middle"))
        frameInfo.titleposition = wxTITLE_MIDDLE;
      else if(placement.Lower() == wxT("right"))
        frameInfo.titleposition = wxTITLE_RIGHT;
      else
        frameInfo.titleposition = wxTITLE_LEFT;

      thisNode = thisNode->GetNext();
      continue;
    }

    if(nodeName == wxT("titlefont"))
    {	
      wxString name = thisNode->GetAttribute(wxT("name"),wxT(""));
      wxString size = thisNode->GetAttribute(wxT("size"),wxT(""));
      wxString weight = thisNode->GetAttribute(wxT("weight"),wxT(""));
      wxString underline = thisNode->GetAttribute(wxT("underline"),wxT("false"));
      wxString italic = thisNode->GetAttribute(wxT("italic"),wxT("false"));
      
      wxFont font;
      font.SetFaceName(name);

      long psize;
      size.ToLong(&psize);
      font.SetPointSize(psize);

      if(underline == wxT("true"))
        font.SetUnderlined(true);

      if(italic == wxT("true"))
        font.SetStyle(wxFONTSTYLE_ITALIC);

      if(weight == wxT("bold"))
        font.SetWeight(wxFONTWEIGHT_BOLD);
      else if(weight == wxT("light"))
        font.SetWeight(wxFONTWEIGHT_LIGHT);

      frameInfo.font = font;

      thisNode = thisNode->GetNext();
      continue;
    }
    wxStringTokenizer tkz(thisNode->GetAttribute(wxT("measure"),wxT("")),wxT(","));

    if(nodeName == wxT("closebox"))
    {
      tkz.GetNextToken().ToLong((long*)&frameInfo.closebox.x);
      tkz.GetNextToken().ToLong((long*)&frameInfo.closebox.y);
      tkz.GetNextToken().ToLong((long*)&frameInfo.closebox.width);
      tkz.GetNextToken().ToLong((long*)&frameInfo.closebox.height);

      frameInfo.skin_closebox = thisNode->GetAttribute(wxT("normal"),wxT(""));
      frameInfo.skin_closebox_over = thisNode->GetAttribute(wxT("over"),wxT(""));
      frameInfo.skin_closebox_press = thisNode->GetAttribute(wxT("pressed"),wxT(""));
    }
    else if(nodeName == wxT("client_area"))
    {
      tkz.GetNextToken().ToLong((long*)&frameInfo.clientarea.x);
      tkz.GetNextToken().ToLong((long*)&frameInfo.clientarea.y);
      tkz.GetNextToken().ToLong((long*)&frameInfo.clientarea.width);
      tkz.GetNextToken().ToLong((long*)&frameInfo.clientarea.height);
    }
    else if(nodeName == wxT("minimizebox"))
    {
      tkz.GetNextToken().ToLong((long*)&frameInfo.minimizebox.x);
      tkz.GetNextToken().ToLong((long*)&frameInfo.minimizebox.y);
      tkz.GetNextToken().ToLong((long*)&frameInfo.minimizebox.width);
      tkz.GetNextToken().ToLong((long*)&frameInfo.minimizebox.height);

      frameInfo.skin_minimizebox = thisNode->GetAttribute(wxT("normal"),wxT(""));
      frameInfo.skin_minimizebox_over = thisNode->GetAttribute(wxT("over"),wxT(""));
      frameInfo.skin_minimizebox_press = thisNode->GetAttribute(wxT("pressed"),wxT(""));
    }
    else if(nodeName == wxT("maximizebox"))
    {
      tkz.GetNextToken().ToLong((long*)&frameInfo.maximizebox.x);
      tkz.GetNextToken().ToLong((long*)&frameInfo.maximizebox.y);
      tkz.GetNextToken().ToLong((long*)&frameInfo.maximizebox.width);
      tkz.GetNextToken().ToLong((long*)&frameInfo.maximizebox.height);

      frameInfo.skin_maximizebox = thisNode->GetAttribute(wxT("normal"),wxT(""));
      frameInfo.skin_maximizebox_over = thisNode->GetAttribute(wxT("over"),wxT(""));
      frameInfo.skin_maximizebox_press = thisNode->GetAttribute(wxT("pressed"),wxT(""));
    }
    else if(nodeName == wxT("titlebar"))
    {
      tkz.GetNextToken().ToLong((long*)&frameInfo.titlebar.x);
      tkz.GetNextToken().ToLong((long*)&frameInfo.titlebar.y);
      tkz.GetNextToken().ToLong((long*)&frameInfo.titlebar.width);
      tkz.GetNextToken().ToLong((long*)&frameInfo.titlebar.height);

    }

    thisNode = thisNode->GetNext();
  }

  m_frameSimpleMap[frameName] = frameInfo;

}
void wxSkinEngine::ParseFrameAttributes(wxXmlNode* node)
{
  if( node == NULL )
    return;

  ComplexFrameInfo frameInfo;
  wxFileSystem system;
  
  wxString frameName = node->GetAttribute(wxT("name"),wxT(""));

  if(m_frameComplexMap.find(frameName) != m_frameComplexMap.end())
  {
    wxLogError(wxT("Frame named \"%s\" allready in use"),frameName.c_str());
    return;
  }

  wxString frameMove = node->GetAttribute(wxT("movable"),wxT("no"));
  frameInfo.movable = frameMove == wxT("yes") ? true : false;

  wxString guival = node->GetAttribute(wxT("guibuilder"),wxT(""));
  frameInfo.guibuilderFile = guival;
      
  wxXmlNode* thisNode = node->GetChildren();

  while(thisNode)
  {	if( thisNode->GetType() != wxXML_ELEMENT_NODE )
    {	thisNode = thisNode->GetNext();
      continue;
    }

    wxString nodeName = thisNode->GetName();

    if(nodeName == wxT("titleposition"))
    {
      wxString placement = thisNode->GetAttribute(wxT("placement"),wxT(""));
      if(placement.Lower() == wxT("left"))
        frameInfo.titleposition = wxTITLE_LEFT;
      else if(placement.Lower() == wxT("middle"))
        frameInfo.titleposition = wxTITLE_MIDDLE;
      else if(placement.Lower() == wxT("right"))
        frameInfo.titleposition = wxTITLE_RIGHT;
      else
        frameInfo.titleposition = wxTITLE_LEFT;

      thisNode = thisNode->GetNext();
      continue;
    }

    if(nodeName == wxT("titlefont"))
    {	
      wxString name = thisNode->GetAttribute(wxT("name"),wxT(""));
      wxString size = thisNode->GetAttribute(wxT("size"),wxT(""));
      wxString weight = thisNode->GetAttribute(wxT("weight"),wxT(""));
      wxString underline = thisNode->GetAttribute(wxT("underline"),wxT("false"));
      wxString italic = thisNode->GetAttribute(wxT("italic"),wxT("false"));
      
      wxFont font;
      font.SetFaceName(name);

      long psize;
      size.ToLong(&psize);
      font.SetPointSize(psize);

      if(underline == wxT("true"))
        font.SetUnderlined(true);

      if(italic == wxT("true"))
        font.SetStyle(wxFONTSTYLE_ITALIC);

      if(weight == wxT("bold"))
        font.SetWeight(wxFONTWEIGHT_BOLD);
      else if(weight == wxT("light"))
        font.SetWeight(wxFONTWEIGHT_LIGHT);

      frameInfo.font = font;

      thisNode = thisNode->GetNext();
      continue;
    }

    wxStringTokenizer tkz(thisNode->GetAttribute(wxT("measure"),wxT("")),wxT(","));

    if(nodeName == wxT("top"))
      frameInfo.skin_top = thisNode->GetAttribute(wxT("skin"),wxT(""));
    else if(nodeName == wxT("top_left"))
      frameInfo.skin_topleft = thisNode->GetAttribute(wxT("skin"),wxT(""));
    else if(nodeName == wxT("top_right"))
      frameInfo.skin_topright = thisNode->GetAttribute(wxT("skin"),wxT(""));
    else if(nodeName == wxT("bottom"))
      frameInfo.skin_bottom = thisNode->GetAttribute(wxT("skin"),wxT(""));
    else if(nodeName == wxT("bottom_left"))
      frameInfo.skin_bottomleft = thisNode->GetAttribute(wxT("skin"),wxT(""));
    else if(nodeName == wxT("bottom_right"))
      frameInfo.skin_bottomright = thisNode->GetAttribute(wxT("skin"),wxT(""));
    else if(nodeName == wxT("left"))
      frameInfo.skin_left = thisNode->GetAttribute(wxT("skin"),wxT(""));
    else if(nodeName == wxT("right"))
      frameInfo.skin_right = thisNode->GetAttribute(wxT("skin"),wxT(""));
    else if(nodeName == wxT("body"))
      frameInfo.skin_body = thisNode->GetAttribute(wxT("skin"),wxT(""));
    else if(nodeName == wxT("closebox"))
    {
      tkz.GetNextToken().ToLong((long*)&frameInfo.closebox.x);
      tkz.GetNextToken().ToLong((long*)&frameInfo.closebox.y);
      tkz.GetNextToken().ToLong((long*)&frameInfo.closebox.width);
      tkz.GetNextToken().ToLong((long*)&frameInfo.closebox.height);

      frameInfo.skin_closebox = thisNode->GetAttribute(wxT("normal"),wxT(""));
      frameInfo.skin_closebox_over = thisNode->GetAttribute(wxT("over"),wxT(""));
      frameInfo.skin_closebox_press = thisNode->GetAttribute(wxT("pressed"),wxT(""));

      wxString relation = thisNode->GetAttribute(wxT("relate_to"),wxT("absolute"));
      if(relation == wxT("absolute"))
        frameInfo.closebox_relateto = RELATE_ABSOLUTE;
      else if(relation == wxT("top"))
        frameInfo.closebox_relateto = RELATE_TOP;
      else if(relation == wxT("top_left"))
        frameInfo.closebox_relateto = RELATE_TOP_LEFT;
      else if(relation == wxT("top_right"))
        frameInfo.closebox_relateto = RELATE_TOP_RIGHT;
    }
    else if(nodeName == wxT("minimizebox"))
    {
      tkz.GetNextToken().ToLong((long*)&frameInfo.minimizebox.x);
      tkz.GetNextToken().ToLong((long*)&frameInfo.minimizebox.y);
      tkz.GetNextToken().ToLong((long*)&frameInfo.minimizebox.width);
      tkz.GetNextToken().ToLong((long*)&frameInfo.minimizebox.height);
      
      frameInfo.skin_minimizebox = thisNode->GetAttribute(wxT("normal"),wxT(""));
      frameInfo.skin_minimizebox_over = thisNode->GetAttribute(wxT("over"),wxT(""));
      frameInfo.skin_minimizebox_press = thisNode->GetAttribute(wxT("pressed"),wxT(""));

      wxString relation = thisNode->GetAttribute(wxT("relate_to"),wxT("absolute"));
      if(relation == wxT("absolute"))
        frameInfo.minimizebox_relateto = RELATE_ABSOLUTE;
      else if(relation == wxT("top"))
        frameInfo.minimizebox_relateto = RELATE_TOP;
      else if(relation == wxT("top_left"))
        frameInfo.minimizebox_relateto = RELATE_TOP_LEFT;
      else if(relation == wxT("top_right"))
        frameInfo.minimizebox_relateto = RELATE_TOP_RIGHT;
    }
    else if(nodeName == wxT("maximizebox"))
    {
      tkz.GetNextToken().ToLong((long*)&frameInfo.maximizebox.x);
      tkz.GetNextToken().ToLong((long*)&frameInfo.maximizebox.y);
      tkz.GetNextToken().ToLong((long*)&frameInfo.maximizebox.width);
      tkz.GetNextToken().ToLong((long*)&frameInfo.maximizebox.height);

      frameInfo.skin_maximizebox = thisNode->GetAttribute(wxT("normal"),wxT(""));
      frameInfo.skin_maximizebox_over = thisNode->GetAttribute(wxT("over"),wxT(""));
      frameInfo.skin_maximizebox_press = thisNode->GetAttribute(wxT("pressed"),wxT(""));
      
    }
    else if(nodeName == wxT("titlebar"))
    {
      /*tkz.GetNextToken().ToLong((long*)&frameInfo.titlebar.x);
      tkz.GetNextToken().ToLong((long*)&frameInfo.titlebar.y);
      tkz.GetNextToken().ToLong((long*)&frameInfo.titlebar.width);
      tkz.GetNextToken().ToLong((long*)&frameInfo.titlebar.height);*/
      wxString height = thisNode->GetAttribute(wxT("height"),wxT("10"));
      height.ToLong((long*)&frameInfo.titlebarheight);
    }

    thisNode = thisNode->GetNext();
  }

  m_frameComplexMap[frameName] = frameInfo;

}

void wxSkinEngine::ParseInnerAttributes(int type,wxXmlNode* node)
{
  if(node == NULL )
    return;

  wxXmlNode* thisNode = node->GetChildren();
  //wxFileSystem system;
  wxString value;
  while(thisNode)
  {
    if( thisNode->GetType() != wxXML_ELEMENT_NODE )
    {	thisNode = thisNode->GetNext();
      continue;
    }

    wxString nodeName = thisNode->GetName();
    if(nodeName == wxT("default"))
    {
      switch(type)
      {
        case TYPE_BUTTON:
        {	
          nodeName.Append(wxT("BUTTON"));
        }	
        break;
        case TYPE_CHECKBOX:
        {	
          nodeName.Append(wxT("CHECKBOX"));
        }	
        break;
        case TYPE_RADIOBUTTON:
        {	nodeName.Append(wxT("RADIOBUTTON"));
        }	
        break;
        case TYPE_GAUGE:
        {	
          nodeName.Append(wxT("GAUGE"));
        }	
        break;
        case TYPE_SLIDER:
        {	
          nodeName.Append(wxT("SLIDER"));
        }	
        break;
        case TYPE_PANEL:
        {	
          nodeName.Append(wxT("PANEL"));
        }
        break;
        case TYPE_STATICTEXT:
        {	
          nodeName.Append(wxT("STATICTEXT"));
        }
        case TYPE_SPLITTERWINDOW:
        {	
          nodeName.Append(wxT("SPLITTERWINDOW"));
        }
        break;
      }
    }


    if(m_controlMap.find(nodeName) != m_controlMap.end())
    {
      wxLogError(wxT("Group %s: Label \"%s\" allready in use"),thisNode->GetParent()->GetName().c_str(),nodeName.c_str());
      thisNode = thisNode->GetNext();
      continue;
    }

        ControlInfo *controlInfo;
        
    switch(type)
    {
      case TYPE_BUTTON:
      {	
          controlInfo = new ControlInfo();
        controlInfo->skinState1 = thisNode->GetAttribute(wxT("normal"),wxT(""));
        controlInfo->skinState2 = thisNode->GetAttribute(wxT("pressed"),wxT(""));
        controlInfo->skinOver = thisNode->GetAttribute(wxT("over"),wxT(""));
        controlInfo->skinDisabled = thisNode->GetAttribute(wxT("disabled"),wxT(""));
        int i=0;
      }	
      break;
      case TYPE_CHECKBOX:
      {	
          controlInfo = new ControlInfo();
        controlInfo->skinState1 = thisNode->GetAttribute(wxT("normal"),wxT(""));
        controlInfo->skinState2 = thisNode->GetAttribute(wxT("checked"),wxT(""));
        controlInfo->skinOver = thisNode->GetAttribute(wxT("over"),wxT(""));
        controlInfo->skinDisabled = thisNode->GetAttribute(wxT("disabled"),wxT(""));
        
      }	
      break;
      case TYPE_RADIOBUTTON:
      {	
          controlInfo = new ControlInfo();
          controlInfo->skinState1 = thisNode->GetAttribute(wxT("normal"),wxT(""));
        controlInfo->skinState2 = thisNode->GetAttribute(wxT("checked"),wxT(""));
        controlInfo->skinOver = thisNode->GetAttribute(wxT("over"),wxT(""));
        controlInfo->skinDisabled = thisNode->GetAttribute(wxT("disabled"),wxT(""));
        
      }	
      break;
      case TYPE_GAUGE:
      {	
          controlInfo = new ControlInfo();
        controlInfo->skinState1 = thisNode->GetAttribute(wxT("foreground"),wxT(""));
        controlInfo->skinState2 = thisNode->GetAttribute(wxT("background"),wxT(""));
        controlInfo->skinDisabled = thisNode->GetAttribute(wxT("disabled"),wxT(""));
      }	
      break;
      case TYPE_SLIDER:
      {	
          controlInfo = new SliderControlInfo();
        controlInfo->skinState1 = thisNode->GetAttribute(wxT("background"),wxT(""));
        controlInfo->skinState2 = thisNode->GetAttribute(wxT("slider"),wxT(""));
        controlInfo->skinOver = thisNode->GetAttribute(wxT("over"),wxT(""));
        controlInfo->skinDisabled = thisNode->GetAttribute(wxT("disabled"),wxT(""));
        ((SliderControlInfo*)controlInfo)->skinFill = thisNode->GetAttribute(wxT("fill"), wxT(""));
        ((SliderControlInfo*)controlInfo)->isVertical = false;
            wxString isVertical = thisNode->GetAttribute(wxT("isVertical"),wxT("false"));
            if(isVertical == wxT("true"))
              ((SliderControlInfo*)controlInfo)->isVertical = true;
      }	
      break;
      case TYPE_PANEL:
      {	
          ControlInfo* controlInfo = new ControlInfo();
        controlInfo->skinState1 = thisNode->GetAttribute(wxT("background"),wxT(""));
        controlInfo->skinDisabled = thisNode->GetAttribute(wxT("disabled"),wxT(""));
      }
      break;
      case TYPE_STATICTEXT:
      {	
          StaticTextControlInfo* info = new StaticTextControlInfo();
          controlInfo = info;
          
          info->underlined = false;
          wxString underlined = thisNode->GetAttribute(wxT("underlined"),wxT("false"));
          if (underlined == wxT("true"))
              info->underlined = true;
              
          info->weight = wxFONTWEIGHT_NORMAL;
          wxString weight = thisNode->GetAttribute(wxT("weight"),wxT("normal"));
          if (weight == wxT("light"))
              info->weight = wxFONTWEIGHT_LIGHT;
          else if (weight == wxT("bold"))
              info->weight = wxFONTWEIGHT_BOLD;
              
          info->style = wxFONTSTYLE_NORMAL;
          wxString style = thisNode->GetAttribute(wxT("style"),wxT("normal"));
          if (style == wxT("slant"))
              info->style = wxFONTSTYLE_SLANT;
          else if (style == wxT("italic"))
              info->style = wxFONTSTYLE_ITALIC;
              
          info->faceName = thisNode->GetAttribute(wxT("faceName"),wxT(""));
          
          info->fontFamily = wxFONTFAMILY_DEFAULT;
          wxString fontFamily = thisNode->GetAttribute(wxT("fontFamily"),wxT("sans-serif"));
          if (fontFamily == wxT("decorative"))
              info->fontFamily = wxFONTFAMILY_DECORATIVE;
          else if (fontFamily == wxT("formal-serif"))
              info->fontFamily = wxFONTFAMILY_ROMAN;
          else if (fontFamily == wxT("handwriting"))
              info->fontFamily = wxFONTFAMILY_SCRIPT;
          else if (fontFamily == wxT("sans-serif"))
              info->fontFamily = wxFONTFAMILY_SWISS;
          else if (fontFamily == wxT("fixed-pitch"))
              info->fontFamily = wxFONTFAMILY_MODERN;
          else if (fontFamily == wxT("teletype"))
              info->fontFamily = wxFONTFAMILY_TELETYPE;
              
              int fontSize = 10;
              thisNode->GetAttribute(wxT("fontSize"),wxT("10")).ToLong((long*)&fontSize);
              info->fontSize = fontSize;
              info->colour.Set(thisNode->GetAttribute(wxT("colour"),wxT("#FFFFFF")));

      }
      break;
      case TYPE_SPLITTERWINDOW:
      {	
          SplitterWindowControlInfo* info = new SplitterWindowControlInfo();
          controlInfo = info;
          
        controlInfo->skinState1 = thisNode->GetAttribute(wxT("btn_left_normal"),wxT(""));
        controlInfo->skinState2 = thisNode->GetAttribute(wxT("btn_left_pressed"),wxT(""));				
        controlInfo->skinOver = thisNode->GetAttribute(wxT("btn_left_over"),wxT(""));				
        
        controlInfo->skinDisabled = thisNode->GetAttribute(wxT("sash"),wxT(""));
        
        info->skinExtra1 = thisNode->GetAttribute(wxT("btn_right_normal"),wxT(""));
        info->skinExtra2 = thisNode->GetAttribute(wxT("btn_right_pressed"),wxT(""));				
        info->skinExtra3 = thisNode->GetAttribute(wxT("btn_right_over"),wxT(""));	
          
              info->direct = true;
          wxString direct = thisNode->GetAttribute(wxT("direct"),wxT("true"));
          if (direct == wxT("false"))
              info->direct = false;
      }
      break;
    }
    

    
    controlInfo->measure.x = controlInfo->measure.y = -1;
    controlInfo->measure.width = controlInfo->measure.height = -1;

    controlInfo->shown = true;
    wxString shown = thisNode->GetAttribute(wxT("shown"),wxT("true"));
    if(shown == wxT("false"))
      controlInfo->shown = false;


    if(!nodeName.StartsWith(wxT("default")))
    {	wxStringTokenizer tkz(thisNode->GetAttribute(wxT("pos"),wxT("")),wxT(","));
      if(tkz.HasMoreTokens())
      {	tkz.GetNextToken().ToLong((long*)&controlInfo->measure.x);
        tkz.GetNextToken().ToLong((long*)&controlInfo->measure.y);
      }
      wxStringTokenizer tkzsize(thisNode->GetAttribute(wxT("size"),wxT("")),wxT(","));
      if(tkzsize.HasMoreTokens())
      {	tkzsize.GetNextToken().ToLong((long*)&controlInfo->measure.width);
        tkzsize.GetNextToken().ToLong((long*)&controlInfo->measure.height);
      }
    }

    m_controlMap[nodeName] = controlInfo;

    thisNode = thisNode->GetNext();
  }
}
