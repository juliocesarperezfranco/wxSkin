// For compilers that support precompilation, includes "wx/wx.h"
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "guibuilder.h"

#include <wx/statline.h>

#ifndef _NO_DOXYGEN_

//s_idcounter=1;

typedef struct _gb_button_ids {
  /*const*/ wxChar title[15];
    long id;
} gb_button_ids;

#endif  // _NO_DOXYGEN_

// function to set the error from all classes that
// reference the sizer producer
void SetRealizeError(wxGuiBuilder &owner)
{
    owner.SetErrorFlag();
}

//----------------------------------------------------------------------
// wxGbParseBuffer
//----------------------------------------------------------------------

wxChar wxGbParseBuffer::PeekChar(size_t offset) const
{
    if(CState().pos + offset >= m_buffer.Len())
        return wxChar(0);
    else
        return m_buffer.GetChar(CState().pos + offset);
}

bool wxGbParseBuffer::IsDigit() const
{
    wxChar chr = PeekChar();
    return (chr >= '0' && chr <= '9');
}

bool wxGbParseBuffer::IsAlpha() const
{
    wxChar chr = PeekChar();
    return (chr >= 'a' && chr <= 'z') ||
           (chr >= 'A' && chr <= 'Z') ||
           (chr == '_');
}

bool wxGbParseBuffer::ToLong(size_t &value)
{
    wxString strval;
    value = 0;

    while(IsDigit())
        strval.Append(PopChar());

    if(strval.Len() > 0)
    {
        long tmpval;
        if(strval.ToLong(&tmpval))
        {
            value = (size_t)tmpval;
            return true;
        }
    }

    return false;
}

bool wxGbParseBuffer::ToLabel(wxString &value)
{
    wxString strval;
    value.Clear();

    // first char is always alpha
    if(!IsAlpha())
        return false;
    else
    {
        strval.Append(PopChar());

        // now digits are also allowed
        while(IsAlpha() || IsDigit())
            strval.Append(PopChar());
    }

    if(strval.Len() > 0)
    {
        value = strval;
        return true;
    }

    return false;
}

bool wxGbParseBuffer::ToString(wxString &value)
{
    wxString strval;
    value.Clear();

    // get string
    if(PeekChar() == wxChar(wxGB_WND_LABEL_START))
    {
        // pop the first quote
        PopChar();

        // go on until end quote or end
        while(PeekChar() != wxChar(wxGB_WND_LABEL_START))
        {
            // allow end quote to be used in string
            if(PeekChar() == wxGB_LABEL_ESCAPE)
                PopChar();

            if(PeekChar() != wxChar(0))
                strval.Append(PopChar());
            else
                return false;  // premature EOF
        }
    }

    // ok, now remove the last quote
    if(PeekChar() == wxChar(wxGB_WND_LABEL_START))
    {
        PopChar();
        value = strval;
        return true;
    }

    return false;
}
wxChar wxGbParseBuffer::PopChar(int times)
{
    if(State().pos < m_buffer.Len())
    {
        wxChar chr = 0;
        int count = (int)times;
        do
        {
            chr = m_buffer.GetChar(State().pos);

            State().col ++;
            State().pos ++;

            // update row and column
            if(chr == '\n' || chr == '\r')
            {
                State().col = 1;
                State().row ++;
            }

            count --;
        }
        while(count > 0 && chr != wxChar(0));
        return chr;
    }

    return wxChar(0);
}

void wxGbParseBuffer::EatWhites()
{
    while(1)
    {
        wxChar chr = PeekChar();
        // skip normal whites
        if(chr == wxChar('\t') || chr == wxChar(' ') || chr == wxChar('\n') || chr == wxChar('\r'))
            PopChar();
        else if(PeekChar(0) == wxChar('/') && PeekChar(1) == wxChar('*'))
        {
            PopChar(2);
            // skip till end of line
            while(PeekChar(0) != wxChar('*') && PeekChar(1) != wxChar('/'))
            {
                // eat one that belongs to the comment
                chr = PopChar();
                if(chr == wxChar(0))
                    break;
            }

            PopChar(2); // eat comment
        }
        else
            break;
    }
}

bool wxGbParseBuffer::ParseTuple(int &x, int &y)
{
    bool parsed = false;
    
    // get the X
    if(IsDigit())
    {
        size_t tmp;
        if(ToLong(tmp))
            x = (int)tmp;
        else
            return false;
        parsed = true;      
    }
    
    // expect comma, or nothing
    if(PeekChar() == wxChar(','))
    {
        // get Y
        PopChar();
        if(IsDigit())
        {
            size_t tmp;
            if(ToLong(tmp))
                y = (int)tmp;
            else
                return false;        
            parsed = true;      
        }
    }
    
    return parsed;
}

//----------------------------------------------------------------------
// wxGbBaseItem
//----------------------------------------------------------------------

int wxGbBaseItem::ParseCommonFlags(wxGbParseBuffer &buf, wxSizerFlags &flags,
                                   wxString &label)
{
    int border = -1, borders = 0;  // default indication not used

    // check for border marker
    if(buf.PeekChar() == wxChar(wxGB_BORDER_MARK))
    {
        buf.PopChar();

        // if we have a number, adjust border
        if(buf.IsDigit())
        {
            size_t tmpval;
            if(buf.ToLong(tmpval))
                border = (int)tmpval;
            else
                border = 0;
        }

        // go do the flags like 'a' - all, 'l' = left, 'r' = right, 't' = top
        bool bordering = true;
        while(bordering)
        {
            char chr = buf.PeekChar();
            switch(chr)
            {
            case wxGB_BORDER_ALL:
                borders = wxALL;
                break;
            case wxGB_BORDER_LEFT:
                borders |= wxLEFT;
                break;
            case wxGB_BORDER_RIGHT:
                borders |= wxRIGHT;
                break;
            case wxGB_BORDER_BOTTOM:
                borders |= wxBOTTOM;
                break;
            case wxGB_BORDER_TOP:
                borders |= wxTOP;
                break;

            default:
                bordering = false;
                break;
            }

            if(bordering)
                buf.PopChar();
            else
            {
                // we must have gotten something here
                // since the digit was there for the border flags
                if(borders == 0)
                {
                    // ::wxLogError(buf.GetAt() + wxT("Missing border flags!"));
                    return -1;
                }
            }
        }

        // set the border in the sizerflags
        if(borders != 0)
        {
            if(border < 0)
                flags.Border(borders);
            else
                flags.Border(borders, border);
        }
    }

    // get align flags
    bool aligning = true;
    while(aligning)
    {
        char align = (char)buf.PeekChar();
        switch(align)
        {
        case wxGB_EXPAND:
            // expand flag
            flags.Expand();
            break;
        case wxGB_ALIGN_BOTTOM:
            // align on bottom, clear top flag
            flags.Align(wxALIGN_BOTTOM);
            break;
        case wxGB_ALIGN_TOP:
            // align on top, clear top flag
            flags.Align(wxALIGN_TOP);
            break;
        case wxGB_ALIGN_LEFT:
            // align on left, clear right flag
            flags.Left();
            break;
        case wxGB_ALIGN_RIGHT:
            // align on right, clear left flag
            flags.Right();
            break;
        case wxGB_ALIGN_CENTER:
            // align center
            flags.Center();
            break;
        case wxGB_ALIGN_HOR:
            // align horizontal center
            flags.Align(wxALIGN_CENTER_HORIZONTAL);
            break;
        case wxGB_ALIGN_VER:
            // align vertical center
            flags.Align(wxALIGN_CENTER_VERTICAL);
            break;
        default:
            aligning = false;
            break;
        }

        // get rid of align char
        if(aligning)
            buf.PopChar();
    }

    // determine the proportional part
    if(buf.PeekChar() == wxGB_PROPORTION)
    {
        buf.PopChar();

        // if we have a number, adjust proportion
        if(buf.IsDigit())
        {
            size_t tmpval;
            if(buf.ToLong(tmpval))
                flags.Proportion((int)tmpval);
            else
                flags.Proportion(0);
        }
        else
        {
            // ::wxLogError(buf.GetAt() + wxT("Missing proportion number!"));
            return -1;
        }
    }

    // now determine if there is a label or not
    if(buf.PeekChar() == wxChar(wxGB_LABEL_MARK))
    {
        buf.PopChar();

        // get the label
        if(!buf.ToLabel(label))
        {
            // ::wxLogError(buf.GetAt() + wxT("Missing label at end of item"));
            return -1;
        }
    }

    return (int)buf.GetPos();
}

bool wxGbBaseItem::Verify(wxGbBaseItem *item)
{
    bool result = true;

    if(item != NULL)
    {
        // compare labels
        if(!m_label.IsEmpty() &&
            item->GetLabel().IsSameAs(m_label))
        {
            // ::wxLogError(wxT("Label '") + m_label + wxT("' is defined more then once!"));
            result = false;
        }
    }

    return result;
}


//----------------------------------------------------------------------
// wxGbSizerItem
//----------------------------------------------------------------------

size_t wxGbSizerItem::ScanSizerType(const wxGbParseBuffer &buf)
{
    char chr = (char)buf.PeekChar(0),
         bnd = (char)buf.PeekChar(1);

  // convert to upper case
  if(chr >= 'a' && chr <= 'z')
    chr -= 0x020;

  // check our sizer type
    switch(chr)
    {
        // horizontal sizer
        case wxGB_SIZER_HTYPE:
            if(bnd == wxGB_WND_LABEL_START)
                return wxGB_STATICBOXSIZER_H;
            else
                return wxGB_BOXSIZER_H;            
            
        // vertical sizer
        case wxGB_SIZER_VTYPE:
            if(bnd == wxGB_WND_LABEL_START)
                return wxGB_STATICBOXSIZER_V;
            else
                return wxGB_BOXSIZER_V;            
            break;
        
        // gridsizer
        case wxGB_SIZER_GTYPE:
            return wxGB_GRIDSIZER;
            
        // flexgridsizer
        case wxGB_SIZER_FTYPE:
            return wxGB_FLEXGRIDSIZER;
                   
    }

    // return default undefined one
  return wxGB_UNDEFINED;
}

int wxGbSizerItem::Parse(wxGbParseBuffer &buf)
{
  // skip whites and update row / col count
    buf.EatWhites();

    int result = 0;
    m_type = wxGbSizerItem::ScanSizerType(buf);
    
    // parse all types of sizers
    buf.Push();
    switch(m_type)
    {
    case wxGB_STATICBOXSIZER_H:
    case wxGB_STATICBOXSIZER_V:
        result = ParseStaticBoxSizer(buf, true);
        break;
    
    case wxGB_BOXSIZER_H:
    case wxGB_BOXSIZER_V:
        result = ParseStaticBoxSizer(buf, false);
        break;
    
    case wxGB_GRIDSIZER:
        result = ParseGridSizer(buf, false);
        break;
    
    case wxGB_FLEXGRIDSIZER:
        result = ParseGridSizer(buf, true);
        break;
    
    default:
        result = -1;
        break;
    }

    if(result < 0)
    {
        buf.Pop();
        // ::wxLogError(buf.GetAt() + wxT("Unknown type of sizer found!"));
        return -1;
    }
    else
        buf.Delegate();    

    return (int)buf.GetPos();
}

int wxGbSizerItem::ParseStaticBoxSizer(wxGbParseBuffer &buf, bool withlabel)
{
    // skip sizer type
    buf.PopChar();
    
    // get title
    if(withlabel && buf.PeekChar() == wxChar(wxGB_WND_LABEL_START))
    {
        wxString str; 
        if(buf.ToString(str))
            m_caption = str;
        else
        {
            wxLogError(buf.GetAt() + wxT("Error parsing label of static box sizer"));
            return -1;
        }
    }
    
    return ParseSizerContents(buf, wxGB_SIZER_BEGIN, wxGB_SIZER_END);
}

int wxGbSizerItem::ParseGridSizer(wxGbParseBuffer &buf, bool withgrow)
{
    // skip sizer type
    buf.PopChar();
    
    // get columns / rows for this sizer
    bool result = false;
    buf.Push();
    if(buf.PeekChar() == wxChar(wxGB_TUPLE_START))
    {
        buf.PopChar();    
        result = buf.ParseTuple(m_rows, m_cols);
        
        while(result && withgrow)
        {
            // growable columns or rows
            buf.EatWhites();                                    
            
            size_t val = 0;
            wxChar type = buf.PeekChar();
            
            if(type == wxChar(wxGB_GROW_ROW) || 
               type == wxChar(wxGB_GROW_COL))               
            {
                buf.PopChar();
                
                // get number
                size_t rowcol_idx, prop = 0;
                result = buf.ToLong(rowcol_idx);
                if(result)
                {
                    // we got the number, now get proportion info (if needed)
                    if(buf.PeekChar() == wxChar(',') || buf.PeekChar() == wxChar(':'))
                    {
                        buf.PopChar();
                        result = buf.ToLong(prop);                                             
                    }
                    else
                        break;                    
                }
                else
                    break;
            
                if(result)
                {
                    // we got all the stuff we need. Now check and add
                    wxSizerItemFlexInfo fi;
                    if(type == wxChar(wxGB_GROW_ROW))
                        fi.rowcol = 0;
                    else
                        fi.rowcol = 1;
                    
                    // add proportion info
                    fi.idx = rowcol_idx;
                    fi.prop = prop;
                    
                    // add this info
                    m_growinfo.push_back(fi);
                }                           
            }
            else
                break;         
        }
        
        // get end tuple
        result &= (buf.PeekChar() == wxChar(wxGB_TUPLE_END));
        if(result)
            buf.PopChar();
    }
    
    if(!result || (m_rows == 0 && m_cols == 0))
    {
        buf.Pop();
        // ::wxLogError(buf.GetAt() + wxT("Invalid or missing row / columns size for wx(flex)GridSizer"));
        return -1;
    }
    
    buf.Delegate();
        
    return ParseSizerContents(buf, wxGB_SIZER_BEGIN, wxGB_SIZER_END);
}

int wxGbSizerItem::ParseSizerContents(wxGbParseBuffer &buf, wxChar szbegin, wxChar szend)
{
    // now get the sizer start and accumulate the items inside
    if(buf.PeekChar() == szbegin)
    {
        buf.PopChar();
        
        buf.Push();
        if(ParseWindowControls(buf, szend) < 0)
        {
            buf.Pop();
            // ::wxLogError(buf.GetAt() + wxT("Error parsing sizer contents"));
            return -1;
        }
        else
            buf.Delegate();
            
        buf.EatWhites();
    }
    else
    {
        // ::wxLogError(buf.GetAt() + wxT("Expected begin sizer element '") +
        //   wxString(szbegin) + wxT("'"));
        return -1;
    }
    
    return buf.GetPos();
}


int wxGbSizerItem::ParseWindowControls(wxGbParseBuffer &buf, wxChar sizer_end)
{
    // go into parse loop
    while(1)
    {
        // ignore all whitespaces
        buf.EatWhites();

        // if we have a default window / sizer flag we will take that from
        // here so all items in this sizer will get the default flags
        if(buf.PeekChar() == wxChar(wxGB_DEFAULT_FLAGS))
        {
            buf.PopChar();

            // check WHICH of the two we are supposed to make default
            // if none matches, we do both
            wxSizerFlags flags;
            bool window = false, sizer = false;
            if(buf.PeekChar() == wxChar('w'))
            {
                window = true;
                flags = m_defWindowFlags;
                buf.PopChar();
            }
            else if(buf.PeekChar() == wxChar('s'))
            {
                sizer = true;
                flags = m_defSizerFlags;
                buf.PopChar();
            }
            else
                sizer = window = true; // no specification, both

            wxString label;
            if(ParseCommonFlags(buf, flags, label) != -1)
            {
                // apply the default flags
                if(sizer)
                    m_defSizerFlags = flags;
                if(window)
                    m_defWindowFlags = flags;
            }
            else
            {
                // ::wxLogError(wxT("Error parsing default flags!"));
                return -1;
            }

            buf.EatWhites();
        }

        // now parse items in between, see if this is a possible
        // window, if so let's parse it
        if(wxGbWindowItem::IsWindowQualifier(buf))
        {
            // store window item
            //wxGbParseBuffer tmpbuf(buf);
            buf.Push();
            wxGbWindowItem *wi = new wxGbWindowItem(this, m_defWindowFlags,
                                                    m_defSizerFlags);
            if(wi->Parse(buf) > 0)
            {
                // copy buffer back and store
                buf.Delegate();
                m_items.push_back(wi);
            }
            else
            {
                buf.Pop();

                delete wi;
                wxLogError(buf.GetAt() + wxT("Unable to parse window item"));

                return -1;
            }
        }
        else if(wxGbSizerItem::ScanSizerType(buf) != wxGB_UNDEFINED)
        {
            // if we have a sizer type here, that means
            // we have to go recursive
            //wxGbParseBuffer tmpbuf(buf);
            buf.Push();
            wxGbSizerItem *si = new wxGbSizerItem(this, m_defWindowFlags,
                                                  m_defSizerFlags);
            if(si->Parse(buf) > 0)
            {
                // copy buffer back and store
                buf.Delegate();
                m_items.push_back(si);
            }
            else
            {
                buf.Pop();

                delete si;
                wxLogError(buf.GetAt() + wxT("Unable to parse sizer item"));

                return -1;
            }
        }
        else if(buf.PeekChar() == wxChar(sizer_end))
        {
            buf.PopChar();

            // set default flags in m_flags for sizer
            m_flags = m_defSizerFlags;
            if(ParseCommonFlags(buf, m_flags, m_label) == -1)
                return -1;

            break;
        }
        else
        {
            if(buf.PeekChar() == wxChar(0))
                wxLogError(buf.GetAt() + wxT("Reached premature EOF (no sizer end found)"));
            else
                wxLogError(buf.GetAt() + wxString::Format(wxT("Unknown item '%c' to parse"), buf.PeekChar()));
            return -1;
        }
    }
    
    return buf.GetPos();
}

bool wxGbSizerItem::Verify(wxGbBaseItem *item)
{
    bool result = wxGbBaseItem::Verify(item);

    if(item == NULL)
    {
        // self diagnostics
        if(m_type == wxGB_UNDEFINED)
        {
            result = false;
            // ::wxLogError(wxT("No sizer type specified!"));
        }
    }

    return result;
}

void wxGbSizerItem::AddToLookup(std::vector<wxGbBaseItem *> &list)
{
    // push ourselves on it
    list.push_back(this);

    // go iterate our children
    for(size_t i = 0; i < m_items.size(); i++)
        m_items[i]->AddToLookup(list);
} 

wxSizer *wxGbSizerItem::RealizeSizer(wxGuiBuilder &owner, wxWindow *parent)
{
    // ok let's start with creating stuff
    DoCreateSizer(parent);

    // now go and add elements we have
    if(m_sizer != NULL)
    {
        wxGbSizerItem *s;
        wxGbWindowItem *w;
        for(size_t i = 0; i < m_items.size(); i++)
        {
            // for other sizers, go and realize them
            s = dynamic_cast<wxGbSizerItem *>(m_items[i]);
            if(s != NULL)
            {
                wxSizer *sz = s->RealizeSizer(owner, parent);
                if(sz == NULL)
                    SetRealizeError(owner);

                wxCHECK_MSG(sz != NULL, m_sizer, wxT("Error realizing subsizer!"));
                m_sizer->Add(sz, s->GetSizerFlags());
            }
            else
            {
                w = dynamic_cast<wxGbWindowItem *>(m_items[i]);
                if(w != NULL)
                {                 
                    if(w->GetWindowType() == wxGbWindowItem::WI_SPACER)
                        m_sizer->Add(w->GetWidth(), w->GetHeight(), 
                                     w->GetSizerFlags().GetProportion(), 
                                     w->GetSizerFlags().GetFlags(),
                                     w->GetSizerFlags().GetBorderInPixels());
                    else
                    {                    
                        wxWindow *wnd = w->RealizeWindow(owner, parent);
                        if(wnd == NULL)
                            SetRealizeError(owner);
    
                        wxCHECK_MSG(wnd != NULL, m_sizer, wxT("Error creating window!"));
                        m_sizer->Add(wnd, w->GetSizerFlags());
                    }
                }
            }
        }
    }

    return m_sizer;
}

void wxGbSizerItem::DoCreateSizer(wxWindow *parent)
{
    // instantiate the type of sizer we need to create with the
    // proper flags
    m_sizer = NULL;

    switch(m_type)
    {
        case wxGB_BOXSIZER_H:
            m_sizer = new wxBoxSizer(wxHORIZONTAL);
            break;

        case wxGB_BOXSIZER_V:
            m_sizer = new wxBoxSizer(wxVERTICAL);
            break;
            
        case wxGB_STATICBOXSIZER_H:
            m_sizer = new wxStaticBoxSizer(wxHORIZONTAL, parent, m_caption);
            break;
            
        case wxGB_STATICBOXSIZER_V:
            m_sizer = new wxStaticBoxSizer(wxVERTICAL, parent, m_caption);
            break;

        case wxGB_GRIDSIZER:
            if(m_rows == 0 && m_cols > 0)
                m_sizer = new wxGridSizer(m_cols);
            else
                m_sizer = new wxGridSizer(m_rows, m_cols, 0, 0);
            break;
        
        case wxGB_FLEXGRIDSIZER:
            {
                wxFlexGridSizer *f;
                if(m_rows == 0 && m_cols > 0)
                    f = new wxFlexGridSizer(m_cols);
                else
                    f = new wxFlexGridSizer(m_rows, m_cols, 0, 0);
                
                m_sizer = f;
                
                // add the columns and rows to grow
                for(size_t i = 0; i < m_growinfo.size(); i++)
                {
                    if(!m_growinfo[i].rowcol)
                        f->AddGrowableRow(m_growinfo[i].idx, m_growinfo[i].prop);
                    else
                        f->AddGrowableCol(m_growinfo[i].idx, m_growinfo[i].prop);     
                }
            }
            break;
        
        default:
            break;
    }
}

//----------------------------------------------------------------------
// wxGbWindowItem
//----------------------------------------------------------------------

int wxGbWindowItem::Parse(wxGbParseBuffer &buf)
{
    int result = 0;

    // skip whitespaces
    buf.EatWhites();

    int kind = 0;
    while(result == 0)
    {
        //wxGbParseBuffer tb(buf);
        buf.Push();
        switch(kind)
        {
            // attempt a subst window item
            case 0:
                result = ParseSubstitute(buf);
                break;
            case 1:
                result = ParseStaticText(buf);
                break;
            case 2:
                result = ParsePanel(buf);
                break;
            case 3:
                result = ParseButton(buf);
                break;
            case 4:
                result = ParseRadioButton(buf);
                break;
            case 5:
                result = ParseCheckBox(buf);
                break;
            case 6:
                result = ParseTextCtrl(buf);
                break;
            case 7:
                result = ParseSpecial(buf);
                break;
            case 8:
                result = ParseTabsheet(buf);
                break;
            default:
                result = -1;
                break;
        }

        // when we are lucky
        if(result > 0)
        {
            buf.Delegate();

            // parse common flags
            m_flags = m_defWindowFlags;
            if(ParseCommonFlags(buf, m_flags, m_label) == -1)
                return -1;

            return (int)buf.GetPos();
        }
        else
        {
            buf.Pop();
            kind++;
        }
    }

    return -1;
}

bool wxGbWindowItem::IsWindowQualifier(const wxGbParseBuffer &buf)
{
    char chr = (char)buf.PeekChar();

    return (chr == wxGB_WND_SUBSTITUTE) ||
           (chr == wxGB_WND_CHECK_START) ||
           (chr == wxGB_WND_RADIO_START) ||
           (chr == wxGB_WND_BUTTON_START) ||
           (chr == wxGB_WND_SPECIAL_START) ||
           //(chr == wxGB_WND_TEXTCTL_START) ||
           //(chr == wxGB_WND_CHOICE_START) ||
           //(chr == wxGB_WND_COMBO_START) ||
           //(chr == wxGB_WND_LISTCTRL_START) ||
           //(chr == wxGB_WND_TREECTRL_START) ||
           (chr == wxGB_TABSHEET_START) ||
           (chr == wxGB_WND_LABEL_START) ||
           (chr == wxGB_PANEL_ITEM);
}

int wxGbWindowItem::ParseSubstitute(wxGbParseBuffer &buf)
{
    size_t nr;

    ResetType();

    // verify if we are parsing the right thing
    if(buf.PeekChar() == wxChar(wxGB_WND_SUBSTITUTE))
    {
        // remove subst marker
        buf.PopChar();

    wxString strVal;
        // get number
        if(buf.ToLong(nr))
        {
            m_number = nr;

            // set type
            m_windowType = WI_SUBSTITUTE;
        }
    else if(buf.ToLabel(strVal))
    {
      m_number = 0;
      for(int i=0; i < (int)strVal.Length(); i++)
        m_number += (char)strVal[i];
      m_windowType = WI_SUBSTITUTE;
      m_label = strVal;
    }
        else
        {
            // ::wxLogError(buf.GetAt() + wxT("Error while parsing substitute number"));
            return -1;
        }
    }
    else
        return 0;   // no scan

    return buf.GetPos();
}

int wxGbWindowItem::ParseStaticText(wxGbParseBuffer &buf)
{
    ResetType();

    // verify if we are parsing the right thing
    if(buf.PeekChar() == wxChar(wxGB_WND_LABEL_START))
    {
        wxString strval;

        // get string
        if(buf.ToString(strval))
        {
            m_caption = strval;

            m_windowType = WI_STATICTEXT;
        }
        else
        {
            // ::wxLogError(buf.GetAt() + wxT("Error while parsing static text label"));
            return -1;
        }
    }
    else
        return 0;   // no scan

    return buf.GetPos();
}

int wxGbWindowItem::ParseButton(wxGbParseBuffer &buf)
{
     static gb_button_ids gb_ids[] = { { wxT("OK"), wxID_OK },                    { wxT("CANCEL"), wxID_CANCEL }, 
                                      { wxT("APPLY"), wxID_APPLY },              { wxT("YES"), wxID_YES },
                                      { wxT("NO"), wxID_NO },                    { wxT("STATIC"), wxID_STATIC },
                                      { wxT("FORWARD"), wxID_FORWARD },          { wxT("BACKWARD"), wxID_BACKWARD },
                                      { wxT("DEFAULT"), wxID_DEFAULT },          { wxT("MORE"), wxID_MORE },
                                      { wxT("SETUP"), wxID_SETUP },              { wxT("RESET"), wxID_RESET },
                                      { wxT("CONTEXTHELP"), wxID_CONTEXT_HELP }, { wxT("YESTOALL"), wxID_YESTOALL },  
                                      { wxT("NOTOALL"), wxID_NOTOALL },          { wxT("ABORT"), wxID_ABORT },
                                      { wxT("RETRY"), wxID_RETRY },              { wxT("IGNORE"), wxID_IGNORE },
                                      { wxT("ADD"), wxID_ADD },                  { wxT("REMOVE"), wxID_REMOVE },
                                      { wxT("UP"), wxID_UP },                    { wxT("DOWN"), wxID_DOWN },
                                      { wxT("HOME"), wxID_HOME },                { wxT("REFRESH"), wxID_REFRESH },
                                      { wxT("STOP"), wxID_STOP },                { wxT("INDEX"), wxID_INDEX },                          
                    { wxT(""), wxID_ANY } };              /* keep this last! */


    ResetType();

    // verify if we are parsing the right thing
    if(buf.PeekChar() == wxChar(wxGB_WND_BUTTON_START))
    {
        // get button start
        buf.PopChar();

        bool valid_button = false;
        if(buf.PeekChar() == wxChar(wxGB_WND_AUTOID_START))
        {            
            // we are going to auto ID first, which means the id of the button
            // will be defaulted to a standard ID based upon a match 
            buf.PopChar();
                
            buf.Push();
            wxString ourlabel;
            while(buf.PeekChar() != wxChar(0) && buf.PeekChar() != wxChar(wxGB_WND_AUTOID_END))
                ourlabel.Append(buf.PopChar());
            
            if(buf.PeekChar() == wxChar(wxGB_WND_AUTOID_END))
            {
                buf.Delegate();
                buf.PopChar();
                
                // check which face we have
                size_t pos = 0;
                while(gb_ids[pos].id != wxID_ANY)
                {
                    if(ourlabel.IsSameAs(gb_ids[pos].title, false))
                    {
                        m_id = gb_ids[pos].id;
                        break;
                    } 
                    pos++;  
                }
                
                if(gb_ids[pos].id == wxID_ANY)
                {
                    // ::wxLogError(buf.GetAt() + wxT("Unknown auto ID for button!"));
                    return -1;                            
                }
                
                // check for semicolon, we skip that for a label
                // we can do without, but it looks nicer
                if(buf.PeekChar() == wxChar(wxGB_LABEL_MARK))
                    buf.PopChar();
                    
                valid_button = true;
            }
            else
            {
                buf.Pop();
                // ::wxLogError(buf.GetAt() + wxT("Premature end for button auto ID (missing end marker)"));
                return -1;            
            }
        }        
        
        
        if(buf.PeekChar() == wxChar(wxGB_WND_LABEL_START))
        {
            // get string
            wxString strval;
            if(buf.ToString(strval))
                m_caption = strval;
            else
            {
                // ::wxLogError(buf.GetAt() + wxT("Error while parsing button text"));
                return -1;
            }
            
            valid_button = true;
        }
        
        // parse the end of the button
        if(valid_button)
        {
            // end button marker
            if(buf.PeekChar() == wxChar(wxGB_WND_BUTTON_END))
            {
                // remove end marker
                buf.PopChar();
    
                m_windowType = WI_BUTTON;
            }
        }
        else
            return 0;
    }
    else
        return 0;   // no scan

    return buf.GetPos();
}

int wxGbWindowItem::ParseTabsheet(wxGbParseBuffer &buf)
{
    ResetType();

    wxGbWindowItem *wi;
    if(buf.PeekChar(0) == wxChar(wxGB_TABSHEET_START) && 
       buf.PeekChar(1) == wxChar(wxGB_SIZER_BEGIN))
    {
        // let's go parse
        buf.PopChar(2);

        while(1)
        {
            // in a while loop we allow only panel items which will 
            // become the base of the sizer
            buf.EatWhites();

            // scan for panel
            if(buf.PeekChar() == wxChar(wxGB_PANEL_ITEM))
            {
                // create a new window item and attempt
                // a parse
                wi = new wxGbWindowItem(0, m_defWindowFlags, m_defSizerFlags);
                buf.Push();
                if(wi->Parse(buf) > 0)
                {
                    // add to our collection
                    buf.Delegate();
                    m_windowItems.push_back(wi);

                    // if no tab name is given, force a title
                    if(wi->m_caption.IsEmpty())
                        wi->m_caption = wxString::Format(wxT("Tab %i"), (int)m_windowItems.size());
                }
                else
                {
                    delete wi;
                    buf.Pop();

                    // ::wxLogError(buf.GetAt() + wxT("Error while parsing tabsheet item"));
                    return -1;
                }

            }
            else if(buf.PeekChar() == wxChar(wxGB_SIZER_END))
            {
                // end of the panel
                buf.PopChar();
                m_windowType = WI_TABSHEET;

                return buf.GetPos();
            }
            else
            {
                // ::wxLogError(buf.GetAt() + wxT("Only panel items allowed in a wxNotebook item"));
                return -1;
            }
        }
    }

    return 0;
}

int wxGbWindowItem::ParseTextCtrl(wxGbParseBuffer &buf)
{
    ResetType();

    // verify if we are parsing the right thing
    if(buf.PeekChar() == wxChar(wxGB_WND_TEXTCTL_START))
    {
        // get start
        buf.PopChar();

        // check if we are multi or single line
        wxChar chr = buf.PeekChar();
        if(chr == wxChar(wxGB_WND_TEXTCTL_SINGLE) ||
           chr == wxChar(wxGB_WND_TEXTCTL_MULTI))
        {
            // set flag for text ctrl type
            m_multiLine = (chr == wxChar(wxGB_WND_TEXTCTL_MULTI));

            m_windowType = WI_TEXT;

            // now scan until all -- or == chars are gone
            while(buf.PeekChar() == chr)
                buf.PopChar();

            // multi line label scanning (or simply appending multiple
            // text blocks to one, since they are encapsulated in a text ctrl
            bool labelscanning = true;
            while(labelscanning)
            {
                // if we have a label, scan this one
                buf.EatWhites();
                if(buf.PeekChar() == wxChar(wxGB_WND_LABEL_START))
                {
                    // get string
                    wxString strval;
                    if(buf.ToString(strval))
                    {
                        if(m_multiLine && !m_caption.IsEmpty())
                            m_caption << wxT("\n");
                        m_caption << strval;
                    }
                    else
                    {
                        // ::wxLogError(buf.GetAt() + wxT("Error while parsing text"));
                        return -1;
                    }
                }
                else
                    labelscanning = false;
            }

            // now scan until all -- or == chars are gone
            // they must match, to enforce the multi or single line text
            while(buf.PeekChar() == chr)
                buf.PopChar();

            // end button marker
            if(buf.PeekChar() == wxChar(wxGB_WND_TEXTCTL_END))
                buf.PopChar();
            else
            {
                // ::wxLogError(buf.GetAt() + wxT("End of text ctrl element expected"));
                return -1;
            }
        }
        else
            return 0; // no match
    }
    else
        return 0;   // no scan

    return buf.GetPos();
}

int wxGbWindowItem::ParseSpecial(wxGbParseBuffer &buf)
{
    ResetType();

    // verify if we are parsing the right thing
    if(buf.PeekChar() == wxChar(wxGB_WND_SPECIAL_START))
    {
        // get start
        buf.PopChar();

        wxString label;
        if(buf.ToLabel(label))
        {
            if(label.IsSameAs(wxT("hdiv")))
                m_windowType = WI_LINE_H;
            else if(label.IsSameAs(wxT("vdiv")))
                m_windowType = WI_LINE_V;
            else if(label.IsSameAs(wxT("spc")))
            {
                m_windowType = WI_SPACER;
                
                // we can expect a semi colon and one or two values. One value
                // means the width and height are the same, two values means
                // seperate values
                if(buf.PeekChar() == wxChar(wxGB_LABEL_MARK))
                {
                    buf.PopChar();
                    int width = -1, height = -1;
                    
                    if(buf.ParseTuple(width, height))
                    {
                        // only assign when we have at least a valid value
                        if(width != -1 && height != -1)
                        {
                            m_height = height;
                            m_width = width;
                        }
                        else if(width != -1)
                            m_height = m_width = width;
                    }
                    else
                    {
                        // ::wxLogError(buf.GetAt() + wxT("Error parsing spacer width / height values!"));
                        return -1;                    
                    }
                }
            }
            
            if(buf.PeekChar() != wxChar(wxGB_WND_SPECIAL_END))
                return 0;

            buf.PopChar();

        }
        else
            return 0;
    }
    else
        return 0;   // no scan

    return buf.GetPos();
}

int wxGbWindowItem::ParseRadioButton(wxGbParseBuffer &buf)
{
    ResetType();

    // verify if we are parsing the right thing
    if(buf.PeekChar() == wxChar(wxGB_WND_RADIO_START))
    {
        // get radio start
        buf.PopChar();

        // get a space or a '.' or '*' for default checked state
        wxChar chr = buf.PeekChar();
        if(chr == wxChar(' ') || chr == wxChar('.') || chr == wxChar('*'))
        {
            // get entry
            buf.PopChar();

            // checked state all but space
            m_checked = !(chr == wxChar(' '));

            // see if we have a ending radio
            if(buf.PeekChar() != wxChar(wxGB_WND_RADIO_END))
                return 0;   // nope still not enough

            // now we have enough
            buf.PopChar();
            m_windowType = WI_RADIO;

            buf.EatWhites();

            // check for a caption. This is optional
            if(buf.PeekChar() == wxChar(wxGB_WND_LABEL_START))
            {
                // get string
                wxString strval;
                if(buf.ToString(strval))
                    m_caption = strval;
                else
                {
                    // ::wxLogError(buf.GetAt() + wxT("Error while parsing checkbox text"));
                    return -1;
                }
            }
        }
        else
            return 0; // probably not what we need
    }
    else
        return 0;   // no scan

    return buf.GetPos();
}

int wxGbWindowItem::ParseCheckBox(wxGbParseBuffer &buf)
{
    ResetType();

    // verify if we are parsing the right thing
    if(buf.PeekChar() == wxChar(wxGB_WND_CHECK_START))
    {
        // get check start
        buf.PopChar();

        // get a space or a 'x' or 'X' for default checked state
        wxChar chr = buf.PeekChar();
        if(chr == wxChar(' ') || chr == wxChar('x') || chr == wxChar('X'))
        {
            // get entry
            buf.PopChar();

            // checked state all but space
            m_checked = !(chr == wxChar(' '));

            // see if we have a ending radio
            if(buf.PeekChar() != wxChar(wxGB_WND_CHECK_END))
                return 0;   // nope still not enough

            // now we have enough
            buf.PopChar();
            m_windowType = WI_CHECK;

            buf.EatWhites();

            // check for a caption. This is optional
            if(buf.PeekChar() == wxChar(wxGB_WND_LABEL_START))
            {
                // get string
                wxString strval;
                if(buf.ToString(strval))
                    m_caption = strval;
                else
                {
                    // ::wxLogError(buf.GetAt() + wxT("Error while parsing checkbox text"));
                    return -1;
                }
            }
        }
        else
            return 0; // probably not what we need
    }
    else
        return 0;   // no scan

    return buf.GetPos();
}

int wxGbWindowItem::ParsePanel(wxGbParseBuffer &buf)
{
    ResetType();

    // verify if we are parsing the right thing
    if(buf.PeekChar() == wxChar(wxGB_PANEL_ITEM))
    {
        // skip panel element
        buf.PopChar();

        // if we have a label, parse this first
        if(buf.PeekChar() == wxChar(wxGB_WND_LABEL_START))
        {
            wxString label;
            if(buf.ToString(label))
                m_caption = label;
            else
                return 0;   // no panel
        }

        // ok it is a panel, get common flags
        // and parse the assigned sizer
        m_flags = m_defWindowFlags;
        if(ParseCommonFlags(buf, m_flags, m_label) == -1)
            return -1;

        // set the type
        m_windowType = WI_PANEL;

        // get the assign char
        if(buf.PeekChar() == wxGB_ASSIGN)
        {
            // now set the type and assign the sizer
            buf.PopChar();

        if(wxGbSizerItem::ScanSizerType(buf) != wxGB_UNDEFINED)
        {
          // if we have a sizer type here, that means
          // we have to go recursive
          //wxGbParseBuffer tmpbuf(buf);
          buf.Push();
          wxGbSizerItem *si = new wxGbSizerItem(GetParent(),
                                                m_defWindowFlags,
                                                      m_defSizerFlags);

                if(si->Parse(buf) > 0)
          {
            // copy buffer back and store
            buf.Delegate();
            m_sizer = si;
          }
          else
          {
            buf.Pop();

            delete si;
            wxLogError(buf.GetAt() + wxT("Unable to parse and assign sizer item"));

            return -1;
          }
            }
        }
    }
    else
        return 0; // no match

    return buf.GetPos();
}


void wxGbWindowItem::AddToLookup(std::vector<wxGbBaseItem *> &list)
{
    // push ourselves on it
    list.push_back(this);
    
    // if this window has a sizer as well, go recursive from there
    if(m_sizer != NULL) 
        m_sizer->AddToLookup(list);

    // if we have more window items
    for(size_t i = 0; i < m_windowItems.size(); i++)
        m_windowItems[i]->AddToLookup(list);
}

bool wxGbWindowItem::Verify(wxGbBaseItem *item)
{
    bool result = wxGbBaseItem::Verify(item);

    if(item == NULL)
    {
        // self diagnostics
        if(m_windowType == WI_UNKNOWN)
        {
            result = false;
            // ::wxLogError(wxT("No window type specified!"));
        }

        // if we have a substituted window, but no assignee
        // then we have a problem
        if(m_windowType == WI_SUBSTITUTE)
        {
            if(m_number == 0)
            {
                result = false;
                // ::wxLogError(wxT("No substitute number found!"));
            }
            else if(m_wnd == NULL)
            {
                result = false;
                // ::wxLogError(wxString::Format(wxT("No assigned window for element $%i"), m_number));
            }
        }
    }
    else
    {
        wxGbWindowItem *w = dynamic_cast<wxGbWindowItem *>(item);
        if(w != NULL)
        {
            // if both windows are substitutes, then we check if they are cross defined
            if(w->GetWindowType() == WI_SUBSTITUTE && m_windowType == WI_SUBSTITUTE)
            {
                if(w->GetNumber() == m_number)
                {
                    result = false;
                    // ::wxLogError(wxString::Format(wxT("Substitute element $%i is defined more then once!"),
                    //              m_number));
                }
            }
        }
    }

    return result;
}

wxWindow *wxGbWindowItem::RealizeWindow(wxGuiBuilder &owner, wxWindow *parent)
{
    // if we have an assigned window, we are done easily
    if(m_windowType == WI_SUBSTITUTE)
    {
        wxCHECK_MSG(m_wnd != NULL, NULL, wxT("Assigned window is not present!"));

        // see if we need to reparent the window this is needed for example
        // when controls are created in a wxPanel for a wxNotebook etc, like;
        // #=H{ $1 $2 $3}
        if(m_wnd->GetParent() != parent)
            m_wnd->Reparent(parent);

        return m_wnd;
    }
    else
    {
        // we are supposed to create one of our own
        DoCreateWindow(owner, parent);
        if(m_wnd == NULL)
            SetRealizeError(owner);

        wxCHECK_MSG(m_wnd != 0, NULL, wxT("Error creating window item!"));

        // now, if we are a panel, and we have an assigned sizer,
        // go and realize a sizer for this window too
        if(m_windowType == WI_PANEL)
        {
            wxPanel *p = wxDynamicCast(m_wnd, wxPanel);
            if(p != NULL && m_sizer != NULL)
            {
                // ok, now realize the sizer with the panel as new parent
                wxSizer *sz = m_sizer->RealizeSizer(owner, p);
                if(sz == NULL)
                    SetRealizeError(owner);

                // assign sizer to panel
                wxCHECK_MSG(sz != NULL, m_wnd, wxT("Error creating wxPanel->wxSizer!"));
                p->SetSizer(sz);
            }
        }
#ifndef __SMARTPHONE__
        else if(m_windowType == WI_TABSHEET)
        {
            wxNotebook *n = wxDynamicCast(m_wnd, wxNotebook);
            if(n != NULL && m_windowItems.size() > 0)
            {
                for(size_t i = 0; i < m_windowItems.size(); i++)
                {
                    // now realize all windows that we refer
                    wxWindow *w = m_windowItems[i]->RealizeWindow(owner, n);
                    wxCHECK_MSG(w != NULL, m_wnd, wxT("Error creating wxNotebook item"));

                    // add this as a page
                    n->AddPage(w, m_windowItems[i]->GetCaption());
                }
            }
        }
#endif
    }

    return m_wnd;
}

void wxGbWindowItem::DoCreateWindow(wxGuiBuilder &owner, wxWindow *parent)
{
    // maintain assigned window
    if(m_windowType == WI_SUBSTITUTE)
        return;

    m_wnd = NULL;

    // get the ID, leave assigned one if there
    if(m_id == wxID_ANY)
        m_id = owner.RequestID(GetLabel());

    // get the size and the style of the window to create
    // if they do not exist, they are left default
    wxSize size = wxDefaultSize;
    long style = 0;

    bool gotstyle = owner.RequestWindowParams(GetLabel(), style, size);
    gotstyle &= (style != 0);

    // we create a window with the default style, or the style given by the
    // registered item. If that style is also zero, the default style is taken
    // again.
    // TODO: What happens when the user explicitly wants a style of zero? If
    // it is set to zero, the default style is taken again.
    switch(m_windowType)
    {
        case WI_PANEL:
            if(!gotstyle)
                m_wnd = new wxPanel(parent, m_id, wxDefaultPosition, size);
            else
                m_wnd = new wxPanel(parent, m_id, wxDefaultPosition, size, style);
            break;

        case WI_STATICTEXT:
            if(!gotstyle)
                m_wnd = new wxStaticText(parent, m_id, m_caption, wxDefaultPosition, size);
            else
                m_wnd = new wxStaticText(parent, m_id, m_caption, wxDefaultPosition, size, style);
            break;

        case WI_BUTTON:
            if(!gotstyle)
                m_wnd = new wxButton(parent, m_id, m_caption, wxDefaultPosition, size);
            else
                m_wnd = new wxButton(parent, m_id, m_caption, wxDefaultPosition, size, style);
            break;

        case WI_RADIO:
            {
                wxRadioButton *r;
                if(!gotstyle)
                    r = new wxRadioButton(parent, m_id, m_caption, wxDefaultPosition, size);
                else
                    r = new wxRadioButton(parent, m_id, m_caption, wxDefaultPosition, size, style);

                r->SetValue(m_checked);
                m_wnd = r;
            }
            break;

        case WI_CHECK:
            {
                wxCheckBox *c;
                if(!gotstyle)
                    c = new wxCheckBox(parent, m_id, m_caption, wxDefaultPosition, size);
                else
                    c = new wxCheckBox(parent, m_id, m_caption, wxDefaultPosition, size, style);

                c->SetValue(m_checked);
                m_wnd = c;
            }
            break;

        case WI_TEXT:
            if(!gotstyle)
                m_wnd = new wxTextCtrl(parent, m_id, m_caption, wxDefaultPosition,
                                       size, ~wxTE_PROCESS_ENTER & (m_multiLine ? wxTE_MULTILINE : 0));
            else
                m_wnd = new wxTextCtrl(parent, m_id, m_caption, wxDefaultPosition,
                                       size, ~wxTE_PROCESS_ENTER & (style | (m_multiLine ? wxTE_MULTILINE : 0)));
            break;

        case WI_LINE_H:
        case WI_LINE_V:
            m_wnd = new wxStaticLine(parent, m_id, wxDefaultPosition, wxDefaultSize,
                                    (m_windowType == WI_LINE_H ? wxLI_HORIZONTAL : wxLI_VERTICAL));
            break;
#ifndef __SMARTPHONE__
        case WI_TABSHEET:
            if(!gotstyle)
                m_wnd = new wxNotebook(parent, m_id, wxDefaultPosition, size);
            else
                m_wnd = new wxNotebook(parent, m_id, wxDefaultPosition, size, style);
            break;                
#endif
        default:
            break;
    }
}

//----------------------------------------------------------------------
// wxGuiBuilder
//----------------------------------------------------------------------
bool wxGuiBuilder::Setup(wxWindow *parent, int startID, bool recompile)
{
    m_realizeError = false;
    m_workID = startID;
    m_sizer = NULL;
    
    // we need to have a parent window to assign our
    // controls to
    if(parent == NULL)
    {
        // ::wxLogError(wxT("Please assign a topwindow"));
        return 0;
    }

    // recompile the structure
    if(m_topSizer == NULL || recompile)
    {
        if(!Compile())
            return 0;
    }

    if(m_topSizer != NULL)
    {
    // set up relations for quick lookup
    m_lookup.clear();
    FillLookupList(m_topSizer);
  }
  return true;
}
bool wxGuiBuilder::Build(wxWindow *parent, int startID, bool recompile)
{
    m_realizeError = false;
    m_workID = startID;
    m_sizer = NULL;
    
    // we need to have a parent window to assign our
    // controls to
    if(parent == NULL)
    {
        // ::wxLogError(wxT("Please assign a topwindow"));
        return 0;
    }

    // recompile the structure
    if(m_topSizer == NULL || recompile)
    {
        if(!Compile())
            return 0;
    }

    if(m_topSizer != NULL)
    {
    // set up relations for quick lookup
    m_lookup.clear();
    FillLookupList(m_topSizer);

        // if we are in demo mode, we substitute all windows ourselves
        if(m_demoMode)
            DoDemoMode(parent);
        else
        {
            // fill all substitutes, if we have ID's that are not filled
            // the verification phase will nag us about it
            wxGbWindowStub *st;
            wxGbWindowItem *w;
            for(size_t i = 0; i < m_regitems.size(); i++)
            {
                st = dynamic_cast<wxGbWindowStub *>(m_regitems[i]);
                if(st != NULL)
                {
                    for(size_t n = 0; n < m_lookup.size(); n++)
                    {
                        w = dynamic_cast<wxGbWindowItem *>(m_lookup[n]);
                        if(w != NULL)
                        {
                            if((size_t)st->GetId() == w->GetNumber() &&
                            w->GetWindowType() == wxGbWindowItem::WI_SUBSTITUTE)
                            {
                                w->AssignWindow(st->GetWindow());
                                break;
                            }
                        }
                    }
                }
            }
        }
        
        // verify double entries, valid entries etc
        if(!VerifyLookupList())
        {
            // delete all and give error
            // ::wxLogError(wxT("Verification error(s) encountered"));
            Reset();
            return 0;
        }

        // let's start the show
        m_sizer = m_topSizer->RealizeSizer((*this), parent);
        if(m_sizer == NULL)
        {
            // ::wxLogError(wxT("Error realizing sizer structure"));
            Reset();
            return 0;
        }
        else
        {
            if(m_realizeError)
            {
                // ::wxLogError(wxT("Not all elements have been created succesfully"));
            }
        }

    }
    else
        // ::wxLogError(wxT("Cannot realize sizer, no topsizer window"));

    // map our controls, this will abort when one or
    // more cannot be mapped
    if(m_sizer != NULL)
    {
        MapControlPointers();
        return true;
    }
        
    return false;
}

void wxGuiBuilder::DoDemoMode(wxWindow *parent)
{
    wxGbWindowItem *w;
    for(size_t n = 0; n < m_lookup.size(); n++)
    {
        w = dynamic_cast<wxGbWindowItem *>(m_lookup[n]);
        if(w != NULL)
        {
            if(w->GetWindowType() == wxGbWindowItem::WI_SUBSTITUTE)
            {
                wxPanel *pnl = new wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER);
                new wxStaticText(pnl, -1, wxString::Format(wxT(" $%i"), w->GetNumber()));
                
                w->AssignWindow(pnl);
            }
        }
    }
}

bool wxGuiBuilder::Compile()
{
    wxGbParseBuffer pb(m_buffer);

    // setup a parse buffer
    Reset();

    // seek for a start sizer entry
    pb.EatWhites();

    // create a sizer and set some defaults
    // this is only useful once at the beginning
    m_topSizer = new wxGbSizerItem(0, m_defWindowFlags,
                                      m_defSizerFlags);

    // go parsing
    //wxGbParseBuffer tmpbuf(m_buffer);
    pb.Push();
    if(m_topSizer->Parse(pb) > 0)
  {
    // write state of buffer back maybe for
    // last position in string, and chained sizers
  }
  else
  {
        // ::wxLogError(pb.GetAt() + wxT("Error parsing the top sizer"));
    Reset();

    return false;
  }

    pb.Pop();
    return true;
}


void wxGuiBuilder::FillLookupList(wxGbSizerItem *si)
{
    if(si != NULL)
        si->AddToLookup(m_lookup);
}

bool wxGuiBuilder::VerifyLookupList()
{
    bool status = true;

    if(m_lookup.size() < 2)
    {
        // ::wxLogError(wxT("There are not enough window items or sizers to realize!"));
        return false;
    }

    // check for element problems
    for(size_t i = 0; i < m_lookup.size(); i++)
    {
        // perform verification on item, and remember false status
        status &= m_lookup[i]->Verify(NULL);

        // if we have errors, do not bother with
        // more errors, as the element is not ok to begin with
        if(!status)
            continue;

        for(size_t n = 0; n < m_lookup.size(); n++)
        {
            // ignore ourselves, and cross compare items
            if(i != n)
                status &= m_lookup[i]->Verify(m_lookup[n]);
        }
    }

    return status;
}

// method that returns a new ID to be used. If the work ID is wxID_ANY
// we define one ourselves, if the ID has another value, we return and
// increment
long wxGuiBuilder::RequestID(const wxString &label)
{
    if(!label.IsEmpty())
    {
        // get label from our registered item list
        // if the label is found and the id is not wxID_ANY
        for(size_t i = 0; i < m_regitems.size(); i++)
        {
            if(m_regitems[i]->GetLabel().IsSameAs(label))
            {
                // found, now return in case it is a valid ID
                if(m_regitems[i]->GetId() != wxID_ANY)
                    return m_regitems[i]->GetId();
            }
        }
    }

    // if we come here there was no predefined label
    // and we either create a new one, or iterate a work ID
    long value;
    if(m_workID == wxID_ANY)
        value = wxNewId();
    else
    {
        value = m_workID;
        m_workID++;
    }

    return value;
}

bool wxGuiBuilder::RequestWindowParams(const wxString &label, long &style, wxSize &size)
{
    if(!label.IsEmpty())
    {
        // get label from our registered item list
        // if the label is found and the id is not wxID_ANY
        for(size_t i = 0; i < m_regitems.size(); i++)
        {
            if(m_regitems[i]->GetLabel().IsSameAs(label))
            {
                style = m_regitems[i]->GetStyle();
                size = m_regitems[i]->GetSize();
                return true;
            }
        }
    }

    return false;
}
bool wxGuiBuilder::CheckWindowByLabel(const wxString &label)
{
  for(size_t i = 0; i < m_lookup.size(); i++)
    {
        if(m_lookup[i]->GetLabel().IsSameAs(label))
        {
      return true;
        }
    }
  return false;
}
wxGbWindowItem *wxGuiBuilder::DoFindWindowByLabel(const wxString &label)
{
    wxGbWindowItem *w;
    for(size_t i = 0; i < m_lookup.size(); i++)
    {
        if(m_lookup[i]->GetLabel().IsSameAs(label))
        {
            w = dynamic_cast<wxGbWindowItem *>(m_lookup[i]);
            if(w != NULL)
                return w;
        }
    }

    return NULL;
}

void wxGuiBuilder::Reset()
{
    m_lookup.clear();
    m_lastIndex = 1;

    if(m_topSizer != NULL)
    {
        delete m_topSizer;
        m_topSizer = 0;
    }
}

bool wxGuiBuilder::CanRegister(const wxGbRegWindowBase &item)
{
    const wxGbWindowStub *st = dynamic_cast<const wxGbWindowStub *>(&item);
    if(st != NULL)
    {
        if(st->GetId() < 1)
        {
            // ::wxLogError(wxT("Illegal stub ID!"));
            return false;
        }
    }
    else if(item.GetLabel().IsEmpty())
    {
        // ::wxLogError(wxT("Label is missing, no registering possible!"));
        return false;
    }

    wxGbWindowStub *tt;
    for(size_t i = 0; i < m_regitems.size(); i++)
    {
        if(st != NULL)
        {
            // if we have a stub, check for existence
            tt = dynamic_cast<wxGbWindowStub *>(m_regitems[i]);
            if(tt != NULL)
            {
                if(tt->GetId() == st->GetId())
                {
                    // ::wxLogError(wxT("Stub window ID already registered!"));
                    return false;
                }
            }
        }
        else if(m_regitems[i]->GetLabel().IsSameAs(item.GetLabel()))
        {
            // ::wxLogError(wxT("Label '") + item.GetLabel() + wxT("' already registered!"));
            return false;
        }
    }

    return true;
}

void wxGuiBuilder::MapControlPointers()
{
    wxString str;
    
    wxGbWindowItem *w;
    for(size_t i = 0; i < m_regitems.size(); i++)
    {
        w = DoFindWindowByLabel(m_regitems[i]->GetLabel());
        if(w)
        {
            if(!m_regitems[i]->SetPointer(w))
            {
                if(!str.IsEmpty())
                    str << wxT("\n");
               
                str << wxT("Unable to set pointer reference for label '") 
                    << m_regitems[i]->GetLabel() << wxT("'");
            }
        }
        else
        {
            if(!str.IsEmpty())
                str << wxT("\n");
            
            str << wxT("Registered label '") << m_regitems[i]->GetLabel() 
                << wxT("' did not reference any control!");
        }
    }
    
    // if we have fatal errors, show user and give the possibility to abort
    if(!str.IsEmpty())
    {
        wxString msg = wxT("Not all window pointers can be assigned! The program\nwill most likely become unstable if you continue. \nWould you like to abort the application?\n\n") + str;
        int res = ::wxMessageBox(msg, wxT("Fatal errors!"), wxICON_HAND | wxYES_NO);
        if(res == wxYES)
        {
            ::wxLogFatalError(str);
        }
    }
}

