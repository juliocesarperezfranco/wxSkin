#ifndef __WX__SIZERPROD__
#define __WX__SIZERPROD__

#define wxGB_VERSION_HIGH       "0"
#define wxGB_VERSION_LOW        "9"
#define wxGB_VERSION_RELEASE    "beta"

#include <wx/notebook.h>
//#include "wxSkinButton.h"
// no doxygen
#ifndef _NO_DOXYGEN_

#define wxGB_SIZER_BEGIN	    '{'			// begin symbol for sizer
#define wxGB_SIZER_END		    '}'			// end symbol for sizer
#define wxGB_SIZER_HTYPE        'H'			// symbol for horizontal sizer
#define wxGB_SIZER_VTYPE        'V'			// symbol for vertical sizer
#define wxGB_SIZER_GTYPE        'G'         // wxGridSizer
#define wxGB_SIZER_FTYPE        'F'         // wxFlexGridSizer
#define wxGB_TABSHEET_START     'T'         // symbol for tabsheet
#define wxGB_PANEL_ITEM         '#'         // indicates we need to create a panel for this
#define wxGB_ALIGN_BOTTOM       '_'         // align bottom
#define wxGB_ALIGN_TOP          '^'         // align top
#define wxGB_ALIGN_LEFT         '<'         // align left
#define wxGB_ALIGN_RIGHT        '>'         // align right
#define wxGB_ALIGN_CENTER       '|'         // align center
#define wxGB_ALIGN_HOR          '-'         // align horizontal
#define wxGB_ALIGN_VER          '/'         // align vertical
#define wxGB_EXPAND             '+'         // expand the element when needed
#define wxGB_PROPORTION         ','         // proportional
#define wxGB_WND_SUBSTITUTE     '$'         // window item substitute
#define wxGB_DEFAULT_FLAGS      '%'         // default sizer or window flag %w or %s
#define wxGB_WND_CHECK_START    '['         // start of a checkbox
#define wxGB_WND_CHECK_END      ']'         // end of a checkbox
#define wxGB_WND_RADIO_START    '('         // start of a radio button
#define wxGB_WND_RADIO_END      ')'         // end of a radio button
#define wxGB_WND_BUTTON_START   '['         // start of a button
#define wxGB_WND_BUTTON_END     ']'         // end of button
#define wxGB_WND_SPECIAL_START  '<'         // static line vertical
#define wxGB_WND_SPECIAL_END    '>'         // static line horizontal
#define wxGB_WND_TEXTCTL_START  '['         // start of a text ctrl
#define wxGB_WND_TEXTCTL_END    ']'         // end of a text ctrl
#define wxGB_WND_TEXTCTL_MULTI  '='         // end of a text ctrl
#define wxGB_WND_TEXTCTL_SINGLE '-'         // end of a text ctrl
#define wxGB_WND_CHOICE_START   '|'         // start of a choice (readonly combo)
#define wxGB_WND_COMBO_START    '|'         // start of a combobox
#define wxGB_WND_LISTCTRL_START '|'         // start of a listctrl
#define wxGB_WND_TREECTRL_START '|'         // start of a treectrl
#define wxGB_WND_LABEL_START    '\''        // start of a static text
#define wxGB_LABEL_ESCAPE       '\\'        // escape character
#define wxGB_BORDER_MARK        '.'         // border seperator
#define wxGB_LABEL_MARK         ':'         // label seperator
#define wxGB_BORDER_ALL         'a'         // border all
#define wxGB_BORDER_LEFT        'l'         // border left
#define wxGB_BORDER_RIGHT       'r'         // border right
#define wxGB_BORDER_BOTTOM      'b'         // border bottom
#define wxGB_BORDER_TOP         't'         // border top
#define wxGB_ASSIGN             '='         // assign
#define wxGB_WND_AUTOID_START   '_'         // button auto ID start 
#define wxGB_WND_AUTOID_END     '_'         // button auto ID end
#define wxGB_TUPLE_START        '('         // begin of a tuple
#define wxGB_TUPLE_END          ')'         // end of a tuple
#define wxGB_GROW_ROW           'r'         // growable row
#define wxGB_GROW_COL           'c'         // growable col

#include <vector>

enum
{
    wxGB_BOXSIZER_H = 0,        // create a horizontal boxsizer
    wxGB_BOXSIZER_V,            // create a vertical boxsizer
    wxGB_FLEXGRIDSIZER,         // create a flexgrid sizer
    wxGB_STATICBOXSIZER_H,      // create a static horizontal box sizer
    wxGB_STATICBOXSIZER_V,      // create a static vertical box sizer
    wxGB_GRIDSIZER,             // create a grid sizer
    //wxGB_GRIDBAGSIZER,        // create a grid bag sizer
    wxGB_UNDEFINED              // keep last in list
};

typedef struct _wxGbParseBufferState
{
    // position
    size_t pos;
    // row marker
    size_t row;
    // col marker
    size_t col;
} wxGbParseBufferState;

typedef struct _wxSizerItemFlexInfo
{
    char rowcol;        // 0 = row, 1 = col
    size_t idx;         // index of the row / col
    size_t prop;        // proportion factor
} wxSizerItemFlexInfo;

// this class will aid the other classes in parsing the contents of the
// sizer text. It contains the necessary helper functions to make
// parsing easier. SHOULD NOT BE USED DIRECTLY
class wxGbParseBuffer
{
public:
    wxGbParseBuffer(const wxString &buffer)
        : m_buffer(buffer)
    {
        // push the base state on the stack
        wxGbParseBufferState pb = {0, 1, 1};
        m_states.push_back(pb);
    };

    virtual ~wxGbParseBuffer() {
    };

    // push the buffer on the stack, so that we have a snapshot
    // of where we were
    void Push() {
        wxGbParseBufferState pb = m_states[m_states.size() - 1];
        m_states.push_back(pb);
    };

    // pop the state, or give error when we are at the base state
    void Pop() {
        wxCHECK2(m_states.size() > 1, return);
        m_states.pop_back();
    };

    // pop but keep the latest state
    void Delegate() {
        // store the latest
        wxGbParseBufferState gb = State();
        Pop();
        // store back to reference
        State() = gb;
    };

    // this relation has all the items that belong to this
    // peeks a character from the buffer, if none left,
    // a 0 char is returned
    wxChar PeekChar(size_t offset = 0) const;

    // gets a character from the buffer, if none is left
    // a 0 char is returned
    wxChar PopChar(int times = 1);

    // helper function to eat CR / LF and whitepaces until
    // we choke
    void EatWhites();

    // convert the next number to a size_t value. If
    // false is returned, there is no number
    bool ToLong(size_t &value);

    // returns true if the current pos is a digit
    bool IsDigit() const;

    // returns true if the current pos is alpha
    bool IsAlpha() const;

    // gets the next quoted string at this location
    bool ToString(wxString &value);

    // returns a standard line like 'At Row, Col (pos) - '
    wxString GetAt() const {
        return wxString::Format(wxT("At Row %i, Col %i (Pos %i) - "), CState().row, CState().col, CState().pos);
    };

    // returns position
    size_t GetPos() const {
        return CState().pos;
    };

    // returns the label at the current pos (if true)
    bool ToLabel(wxString &value);

    // get the last state
    wxGbParseBufferState &State() {
        return m_states[m_states.size() - 1];
    };

    // get the last state (const) read only object
    const wxGbParseBufferState &CState() const {
        return m_states[m_states.size() - 1];
    };

    // parses x,y where the method also allows the absence
    // of one or the other, which will remain the default
    // value given.
    bool ParseTuple(int &x, int &y);


private:
    // the buffer of the parser
    wxString m_buffer;
    // the pushed states
    std::vector<wxGbParseBufferState> m_states;
};


// The base item is the abstract class that can either be a sizer
// or a window item. When it is a sizer item, multiple window or sizer
// items can be added. SHOULD NOT BE USED DIRECTLY
class wxGbSizerItem;
class wxGuiBuilder;
class wxGbBaseItem
{
public:
    wxGbBaseItem(wxGbSizerItem *parent, const wxSizerFlags &defwnd,
                 const wxSizerFlags &defsizer)
        : m_parent(parent)
        , m_flags(0)
        , m_defWindowFlags(defwnd)
        , m_defSizerFlags(defsizer)
    {
    };

    virtual ~wxGbBaseItem() {
    };

    // parse the common flags which belong to every item
    // something like :{label} or .{border}{align}:{label}
    int ParseCommonFlags(wxGbParseBuffer &buf,wxSizerFlags &flags, wxString &label);

    // returns label reference of this object
    const wxString &GetLabel() const {
        return m_label;
    };

    // return the parent of this sizer
    wxGbSizerItem *GetParent() {
        return m_parent;
    };

    // return the sizer flags
    const wxSizerFlags &GetSizerFlags() const {
        return m_flags;
    };

    // a method to verify the contents if
    // everything is set up allright
    virtual bool Verify(wxGbBaseItem *item);

    // method to get the window elements for in the
    // lookup list. This methid needs to be overridden
    // and the class should handle the population of the 
    // lookup list
    virtual void AddToLookup(std::vector<wxGbBaseItem *> &list) = 0;

    // returns window caption
    const wxString &GetCaption() const {
        return m_caption;
    };


protected:
    // our parent
    wxGbSizerItem *m_parent;
    // the flags belonging to this item (window or sizer)
    wxSizerFlags m_flags;
    // label of any control
    wxString m_label;
    // default flags for any window item
    wxSizerFlags m_defWindowFlags;
    // default flags for any sizer item
    wxSizerFlags m_defSizerFlags;
    // a caption that belongs to this window item, like a static text,
    // check box, button, etc
    wxString m_caption;
};

// The sizer item holds a list of window / sizer items in a recursive
// way so that a tree structure can exist with sizers and window items
// SHOULD NOT BE USED DIRECTLY
class wxGbSizerItem : public wxGbBaseItem
{
public:
    wxGbSizerItem(wxGbSizerItem *parent, const wxSizerFlags &defwnd,
                 const wxSizerFlags &defsizer)
        : wxGbBaseItem(parent, defwnd, defsizer)
        , m_type(wxGB_UNDEFINED)
        , m_sizer(0)
        , m_cols(0)
        , m_rows(0)
    {
    };

    virtual ~wxGbSizerItem() {
        // mass cleanup!
        for(size_t n = 0; n < m_items.size(); n++)
            delete m_items[n];
    };

    // parses the sizer, returns -1 when an error occured, otherwise
    // the offset where the next action can take place. The wxString
    // should contain the sizer text to be parsed. The structures are
    // populated as the sizer is parsed.
    int Parse(wxGbParseBuffer &buf);

    // scans for valid sizer type begin, and returns the type
    static size_t ScanSizerType(const wxGbParseBuffer &buf);

    // return count of all sub items
    size_t GetCount() const {
        return m_items.size();
    };

    // return sub item as base class
    wxGbBaseItem *GetItem(size_t n) const {
        wxCHECK(n < m_items.size(), 0);
        return m_items[n];
    };

    // create sizer structure from this node on
    wxSizer *RealizeSizer(wxGuiBuilder &owner, wxWindow *parent);

    // a method to verify the contents if
    // everything is set up allright
    virtual bool Verify(wxGbBaseItem *item);

    // add to lookup list 
    virtual void AddToLookup(std::vector<wxGbBaseItem *> &list);

private:
  void DoCreateSizer(wxWindow *parent);
    // method that parses a BoxSizer and StaticBoxSizer depending on
    // what the type is
    int ParseStaticBoxSizer(wxGbParseBuffer &buf, bool withlabel);
    // parse grid box sizer
    int ParseGridSizer(wxGbParseBuffer &buf, bool withgrow);
    // parse all controls between the sizer
    int ParseWindowControls(wxGbParseBuffer &buf, wxChar sizer_end);
    // parse sizer contents
    int ParseSizerContents(wxGbParseBuffer &buf, wxChar szbegin, wxChar szend);	
    
    // sizer which can be sizers as well as window items
    std::vector<wxGbBaseItem *> m_items;
    // growable rows, columns
    std::vector<wxSizerItemFlexInfo> m_growinfo;
    // the type of sizer
    size_t m_type;
  // parent sizer
  wxGbSizerItem *m_parent;
  // the instantiated sizer
  wxSizer *m_sizer;
    // column size
    int m_cols;
    // row size
    int m_rows;   
};

// The window item
class wxGbWindowItem : public wxGbBaseItem
{
public:
    wxGbWindowItem(wxGbSizerItem *parent,const wxSizerFlags &defwnd,
                 const wxSizerFlags &defsizer)
        : wxGbBaseItem(parent, defwnd, defsizer)
        , m_number(0)
        , m_windowType(WI_UNKNOWN)
        , m_wnd(0)
        , m_sizer(0)
        , m_id(wxID_ANY)
        , m_checked(false)
        , m_multiLine(false)
        , m_width(5)
        , m_height(5)
    {
    };

    enum {
        WI_UNKNOWN = -1,        // undefined, should never happen!
        WI_SUBSTITUTE = 0,      // substitute, we need a handle to a wxWindow*
        WI_PANEL,               // a panel, which means we have one sizer
                                // that is assigned to this window item
        WI_TABSHEET,            // a tabsheet, which contains multiple window elements
        WI_BUTTON,              // button element
        WI_RADIO,               // radio button
        WI_TEXT,                // text ctrl
        WI_CHECK,               // check box
        WI_LINE_H,              // static line h
        WI_LINE_V,              // static line v
        WI_STATICTEXT,          // static text
        WI_SPACER               // spacer item (not really a window)
    };

    virtual ~wxGbWindowItem() {
        // delete sizer item
        if(m_sizer != NULL)
            delete m_sizer;

        // delete window items
        for(size_t i = 0; i < m_windowItems.size(); i++)
            delete m_windowItems[i];
    };

    // parses the window, returns -1 when an error occured, otherwise
    // the offset where the next action can take place.
    int Parse(wxGbParseBuffer &buf);

    // static method that determines ifthe next item in the buffer
    // is actually a qualifier for a window item
    static bool IsWindowQualifier(const wxGbParseBuffer &buf);

    // return Number
    size_t GetNumber() const {
        return m_number;
    };

    // return the sizer that might be assigned to this window item
    wxGbSizerItem *GetAssignedSizer() {
        return m_sizer;
    };

    // returns type of the window
    int GetWindowType() const {
        return m_windowType;
    };

    // returns window handle
    wxWindow *GetWindow() const {
        return m_wnd;
    };

    // assigns the window element
    void AssignWindow(wxWindow *wnd) {
        m_wnd = wnd;
    };

    // get height of the window which should be the value of m_height
    int GetHeight() const {
        return m_height;
    };
    
    // get width of the window which should be the value of m_width
    int GetWidth() const {
        return m_width;
    };

    // a method to verify the contents if
    // everything is set up allright
    virtual bool Verify(wxGbBaseItem *item);

    // realize function to get a wxWindow object back for adding it to the sizer
    wxWindow *RealizeWindow(wxGuiBuilder &owner, wxWindow *parent);

    // add to lookup list 
    virtual void AddToLookup(std::vector<wxGbBaseItem *> &list); 

private:
    void DoCreateWindow(wxGuiBuilder &owner, wxWindow *parent);

    // attempts a parsing of a substitute element
    int ParseSubstitute(wxGbParseBuffer &buf);

    // attempts a parsing of a static text element
    int ParseStaticText(wxGbParseBuffer &buf);

    // attempts a parsing of a panel assign element
    int ParsePanel(wxGbParseBuffer &buf);

    // attempts to parse a button
    int ParseButton(wxGbParseBuffer &buf);

    // attempts to parse a radio
    int ParseRadioButton(wxGbParseBuffer &buf);

    // attempt to parse the checkbox
    int ParseCheckBox(wxGbParseBuffer &buf);

    // attempt to parse the text ctrl (multi or single line)
    int ParseTextCtrl(wxGbParseBuffer &buf);

    // attempt to parse a static line
    int ParseSpecial(wxGbParseBuffer &buf);

    // attempt to parse a tabsheet
    int ParseTabsheet(wxGbParseBuffer &buf);

    // reset type in case parsing of type
    // fails we know about it
    void ResetType() {
        m_windowType = WI_UNKNOWN;

        // we still set to zero here, also non owned control
        // pointers should be reset in this stage
        m_id = wxID_ANY;
        m_wnd = 0;
        m_sizer = 0;
        m_caption.Clear();
        m_checked = false;
        m_multiLine = false; // single text ctrl
    };

    // the ID of the item
    size_t m_number;
    // window type
    int m_windowType;
    // pointer that holds the class, if m_ownMade is true, we created this
    // class. If not, it is assigned
    wxWindow *m_wnd;
    // the only sizer allowed to be added to this panel
    wxGbSizerItem *m_sizer;
    // this is a container class for a wxTabSheet. All elements need to be
    // of a wxSpWindowItem type (preferrably a panel)
    std::vector<wxGbWindowItem *> m_windowItems;
    // the ID assigned to this window, if none wxID_ANY is used
    long m_id;
    // checked state of the radio or checkbox
    bool m_checked;
    // single text ctrl or multi text ctrl
    bool m_multiLine;
    // width and height of the entity, only used for spacers at the moment
    int m_width, m_height; 
};

#endif // _NO_DOXYGEN_

/** \brief Internal class. Do not use this class unless you wish to extend wxGuiBuilder with extra classes to register. 
     
    This class is used by wxGuiBuilder to inherit classes from that are used to assign window properties to the substitute windows. 
*/
class wxGbRegWindowBase
{
public:
    /** Copy operator to make a deep copy of the regwindow class that is passed along */
    void operator=(const wxGbRegWindowBase &rw) {
        m_label = rw.m_label;
        m_size = rw.m_size;
        m_style = rw.m_style;
        m_ptr = rw.m_ptr;
        m_id = rw.m_id;
    };

    /** Destructor */
    virtual ~wxGbRegWindowBase() {
    };

    /** Returns assigned label. The label is given to link a GUI element from wxGuiBuilder to a 
        pointer-pointer, window styles, position or ID  */
    const wxString &GetLabel() const {
        return m_label;
    };

    /** returns the ID of this item. The ID is the window ID that is used to assign to the created 
        GUI control. */
    long GetId() const {
        return m_id;
    };

    /** Returns the window style assigned to this item. The style is a long which is totally 
        independent of the wxWindow it is assigned to. If the style remains 0, wxGuiBuilder 
        will not include this style in window creation, but instead will take the default 
        style that is used if the style parameter is omitted in the GUI control creation */
    long GetStyle() const {
        return m_style;
    };

    /** Returns the associated size. By default the wxDefaultSize is taken, which is passing
        a parameter of (-1,-1) to let the control determine the size. If another size is given 
        then the default size, the control that is created by wxGuiBuilder will inform the 
        sizer system that the minimal initial size is different then the default size */
    const wxSize &GetSize() const {
        return m_size;
    };

#ifndef _NO_DOXYGEN_
    /** Needs to be overridden to set the pointer to the internal wxGbWindowItem which is 
        created by wxGuiBuilder */
    virtual bool SetPointer(wxGbWindowItem *w) = 0;

protected:
    wxGbRegWindowBase(const wxString &label, void **ptr, long id, const wxSize &size, long style)
        : m_label(label)
        , m_size(size)
        , m_style(style)
        , m_id(id)
        , m_ptr(ptr)
    {
        if(ptr != NULL)
            (*ptr) = NULL;
    };

    wxGbRegWindowBase(const wxGbRegWindowBase &rwb) {
        (*this) = rwb;
    };

    wxString m_label;
    wxSize m_size;
    long m_style;
    long m_id;
    void **m_ptr;
#endif
};

#define WX_GB_DECLARE_REGWINDOW(gbwin, wxctrl)                          \
    class gbwin : public wxGbRegWindowBase                              \
    {                                                                   \
public:                                                                 \
    gbwin(const wxString &label, wxctrl **item, long id = wxID_ANY,     \
               const wxSize &size = wxDefaultSize, long style = 0)      \
        : wxGbRegWindowBase(label, (void **)item, id, size, style)      \
    {                                                                   \
    };                                                                  \
                                                                        \
    gbwin(const gbwin &item)                                            \
        : wxGbRegWindowBase(item)                                       \
    {                                                                   \
    };                                                                  \
                                                                        \
    virtual bool SetPointer(wxGbWindowItem *w) {                        \
        if(m_ptr != NULL)                                               \
        {                                                               \
            wxctrl *item = dynamic_cast<wxctrl *>(w->GetWindow());      \
            (*m_ptr) = item;                                            \
            return item != NULL;                                        \
        }                                                               \
        return true;                                                    \
    };                                                                  \
}

/** \brief The class that needs to be passed to wxGuiBuilder when you used substitute windows. 

    The wxGbWindowStub class needs to be passed to wxGuiBuilder via the register command or 
    with the operator<<(const wxGbWindowStub &) when you are using $1 ... $x references in your 
    code. Window stubs are needed when the wxWindow derived class is not supported by wxGuiBuilder 
    or you created an own foreign window. Consider the following example:
    
    \code
        wxGuiBuilder gb("H{ 'Select your data' $1 $2 }");
        
        wxChoice *my_choices = new wxChoice(this, -1);
        my_choices->Append("Choice 1");
        my_choices->Append("Choice 2");
                
        // now tell wxGuiBuilder that the wxChoice needs to be substituted
        gb << wxGbWindowStub(1, my_choices);
        
        // add a button
        gb << wxGbWindowStub(2, new wxButton(this, ID_CLICKME, "Click Me"));
        
        if(gb.Build(this))
        {
            // now your dialog is realized with the use of 
            // a wxChoice and a text in a horizontal sizer.
            this->SetSizerAndFit(gb.GetSizer());
        
        }
    \endcode
    
    The substitute class needs an ID which needs to match the number following the $ (in this case 1) 
    and a wxWindow pointer. The wxWindow needs to be created as child of the parent window the sizer
    will be assigned to. If a sub panel or notebook will become the new parent of the substitute
    it will be reparented internally.
    
    See also \ref grm_substitute .
 */


class wxGbWindowStub : public wxGbRegWindowBase
{
public:
    /** Use this class construction to pass the ID of the substitute in question to wxGuiBuilder. The ID 
        is the number following the $, like $1 or $2. The wxWindow will be positioned and reparented 
        if needed to fit in the GUI. This class is copied internally so it is safe to pass it on 
        the stack, no need to create it on the heap */
    wxGbWindowStub(int id, wxWindow *window)
        : wxGbRegWindowBase(wxEmptyString, NULL, id, wxDefaultSize, 0)
        , m_wnd(window)
    {
    };

  wxGbWindowStub(wxString label, wxWindow *window)
        : wxGbRegWindowBase(label, NULL, 1, wxDefaultSize, 0)
        , m_wnd(window)
    {
    m_id = 0;
    for(int i=0; i < (int)label.Length(); i++)
      m_id += (char)label[i];
    };
#ifndef _NO_DOXYGEN_
    /** Copy constructor, only for internal use */
    wxGbWindowStub(const wxGbWindowStub &src)
        : wxGbRegWindowBase(src)
    {
        m_wnd = src.m_wnd;
    };

    ~wxGbWindowStub() {
    };

    void operator=(const wxGbWindowStub &src) {
        (*(wxGbRegWindowBase *)this) = src;
        m_wnd = src.m_wnd;
    };

    virtual bool SetPointer(wxGbWindowItem *w) {
        return true;
    };

    int GetId() const {
        return m_id;
    };

    wxWindow *GetWindow() const {
        return m_wnd;
    };

private:
    wxWindow *m_wnd;
#endif
};

WX_GB_DECLARE_REGWINDOW(wxGbSomeWindow, wxWindow);
//WX_GB_DECLARE_REGWINDOW(wxGbSkinButton, wxSkinButton);

/** \class wxGbButton 
    \brief Pass this class to wxGuiBuilder to retrieve the pointer of the created wxButton, assign it a fixed ID, style or size.
    
    The wxGbButton class needs to be passed to wxGuiBuilder to retrieve a pointer to your wxButton, or when you wish to assign
    it extra button styles, or define a fixed size. 
    
    Consider the following example: 
    
\code
    
    wxGuiBuilder gb("H{ 'Press button A' ['A']:but_a }");
            
    gb << wxGbButton(wxT("but_a"), &m_button_a, ID_BUTTON_A, wxSize(10,20), wxNO_BORDER);
    
    if(gb.Build(this))
    {
        SetSizerAndFit(gb.GetSizer());
    
        // TODO:
    }
    
\endcode

    See also \ref gb_refs and \ref grm_button
*/
WX_GB_DECLARE_REGWINDOW(wxGbButton, wxButton);

/** \class wxGbTextCtrl 
    \brief Pass this class to wxGuiBuilder to retrieve the pointer of the created wxTextCtr, assign it a fixed ID, style or size.
    
    The wxGbTextCtrl class needs to be passed to wxGuiBuilder to retrieve a pointer to your wxTextCtrl, or when you wish to assign it extra button styles, or define a fixed size. 
    
    Consider the following example: 
    
\code
    
    wxGuiBuilder gb("H{ 'Enter text' [--------------]:text }");
            
    gb << wxGbTextCtrl(wxT("text"), &m_myText, ID_TEXT_CTRL, wxDefaultSize, wxTE_READONLY);
    
    if(gb.Build(this))
    {
        SetSizerAndFit(gb.GetSizer());
    
        // set the text
        m_myText->SetValue("Hello there!");
    }
    
\endcode

    See also \ref gb_refs and \ref grm_textctrl
*/
WX_GB_DECLARE_REGWINDOW(wxGbTextCtrl, wxTextCtrl);

/** \class wxGbStaticText 
    \brief Pass this class to wxGuiBuilder to retrieve the pointer of the created wxStaticText, assign it a fixed ID, style or size.
    
    The wxGbStaticText class needs to be passed to wxGuiBuilder to retrieve a pointer to your wxStaticText, or when you wish to assign it extra styles, or define a fixed size. 
    
    Consider the following example: 
    
\code
    
    wxGuiBuilder gb("H{ 'Status:' '-status-':stat }");
            
    gb << wxGbTextCtrl(wxT("stat"), &m_myStatus, wxID_ANY, wxDefaultSize, wxSIMPLE_BORDER);
    
    if(gb.Build(this))
    {
        SetSizerAndFit(gb.GetSizer());
    
        // set status
        m_myStatus->SetLabel(wxT("Status READY!"));
    }
    
\endcode

    See also \ref gb_refs and \ref grm_statictext
*/
WX_GB_DECLARE_REGWINDOW(wxGbStaticText, wxStaticText);

/** \class wxGbRadioButton 
    \brief Pass this class to wxGuiBuilder to retrieve the pointer of the created wxRadioButton, assign it a fixed ID, style or size.
    
    The wxGbRadioButton class needs to be passed to wxGuiBuilder to retrieve a pointer to your wxRadioButton, or when you wish to assign it extra styles, or define a fixed size. 
    
    Consider the following example: 
    
\code
    
    wxGuiBuilder gb("H{ 'Choose:' ( ) 'Option 1':opt1  "
                    "             (*) 'Option 2':opt2 }");
            
    gb << wxGbRadioButton(wxT("opt1"), &m_opt1, ID_OPTION1);
    gb << wxGbRadioButton(wxT("opt2"), &m_opt2, ID_OPTION2);
    
    if(gb.Build(this))
    {
        SetSizerAndFit(gb.GetSizer());
    
        // hook up events
        this->Connect(ID_OPTION1, wxEVT_COMMAND_RADIOBOX_SELECTED, 
                      wxCommandEventHandler(ThisDlg::OnRadio));
        this->Connect(ID_OPTION2, wxEVT_COMMAND_RADIOBOX_SELECTED, 
                      wxCommandEventHandler(ThisDlg::OnRadio));
    }
    
\endcode

    For the event handler:
    
\code
void ThisDlg::OnRadio(wxCommandEvent &event)
{
    // toggle radio buttons opposite
    if(event.GetId() == ID_OPTION1)
        m_opt2->SetValue(true);
    else if(event.GetId() == ID_OPTION2)
        m_opt1->SetValue(true);
}

\endcode

    See also \ref gb_refs and \ref grm_radiobutton
*/
WX_GB_DECLARE_REGWINDOW(wxGbRadioButton, wxRadioButton);

/** \class wxGbCheckBox 
    \brief Pass this class to wxGuiBuilder to retrieve the pointer of the created wxCheckBox, assign it a fixed ID, style or size.
    
    The wxGbCheckBox class needs to be passed to wxGuiBuilder to retrieve a pointer to your wxCheckBox, or when you wish to assign it extra styles, or define a fixed size. 
    
    Consider the following example: 
    
\code
    
    wxGuiBuilder gb("H{ 'Choose:' [ ] 'Option 1':opt1  "
                    "             [X] 'Option 2':opt2 }");
            
    gb << wxGbCheckBox(wxT("opt1"), &m_opt1, ID_OPTION1);
    gb << wxGbCheckBox(wxT("opt2"), &m_opt2, ID_OPTION2);
    
    if(gb.Build(this))
    {
        SetSizerAndFit(gb.GetSizer());
    
        // hook up events
        this->Connect(ID_OPTION1, wxEVT_COMMAND_CHECKBOX_CLICKED, 
                      wxCommandEventHandler(ThisDlg::OnCheck));
        this->Connect(ID_OPTION2, wxEVT_COMMAND_CHECKBOX_CLICKED, 
                      wxCommandEventHandler(ThisDlg::OnCheck));
    }
    
\endcode

    For the event handler:
    
\code
void ThisDlg::OnCheck(wxCommandEvent &event)
{
    // toggle check boxes opposite
    if(event.GetId() == ID_OPTION1)
    {
        m_opt1->SetValue(false);
        m_opt2->SetValue(true);
    }
    else if(event.GetId() == ID_OPTION2)
    {
        m_opt2->SetValue(false);
        m_opt1->SetValue(true);
    }
}
\endcode

    See also \ref gb_refs and \ref grm_checkbox
*/
WX_GB_DECLARE_REGWINDOW(wxGbCheckBox, wxCheckBox);

/** \class wxGbPanel 
    \brief Pass this class to wxGuiBuilder to retrieve the pointer of the created wxPanel, assign it a fixed ID, style or size.
    
    The wxGbPanel class needs to be passed to wxGuiBuilder to retrieve a pointer to your wxPanel, or when you wish to assign it extra styles, or define a fixed size. 
    
    Consider the following example: 
    
\code
    
    wxGuiBuilder gb("H{                                              "
                    "    #:pnl=V{                                    "
                    "          'Set background color of wxPanel'     "
                    "           H{   ['Black']:blk ['White']:wht   } "
                    "       }                                        "
                    " }                                              ");
            
    // give panel a border, and a fixed size of 300,300
    gb << wxGbPanel("pnl", &m_myPanel, wxID_ANY, wxSize(300,300), wxSIMPLE_BORDER);
    
    // get the buttons to assign events
    gb << wxGbButton("blk", &m_blackBut);
    gb << wxGbButton("wht", &m_whiteBut);
    
    if(gb.Build(this))
    {
        SetSizerAndFit(gb.GetSizer());
    
        // hook up events
        this->Connect(m_blackBut->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
                      wxCommandEventHandler(MyDialog::OnBlackClick));       
        this->Connect(m_whiteBut->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
                      wxCommandEventHandler(MyDialog::OnWhiteClick));       
    }
    
\endcode

    For the event handlers:
    
\code
void MyDialog::OnBlackClick(wxCommandEvent &event)
{
    // make the panel black
    m_myPanel->SetBackgroundColour(*wxBLACK);
}

void MyDialog::OnWhiteClick(wxCommandEvent &event)
{
    // make the panel white
    m_myPanel->SetBackgroundColour(*wxWHITE);
}

\endcode

    See also \ref gb_refs and \ref grm_panel
*/
WX_GB_DECLARE_REGWINDOW(wxGbPanel, wxPanel);

/** \class wxGbNotebook 
    \brief Pass this class to wxGuiBuilder to retrieve the pointer of the created wxNotebook, assign it a fixed ID, style or size.
    
    The wxGbNotebook class needs to be passed to wxGuiBuilder to retrieve a pointer to your wxNotebook, or when you wish to assign it extra styles, or define a fixed size. 
    
    Consider the following example: 
    
\code
    
    // simple example how the GUI builder should be used. The
    // contents is parsed, and all you have to do is set the sizer
    wxGuiBuilder gb("H{                                          "
                    "    T{                                      "
                    "       #'Page 1'=V{                         "
                    "                   ( ) 'Some option'        "
                    "                   (.) 'Some other option'  "
                    "                  }                         "
                    "                                            "
                    "                                            "
                    "    }+,1:nbook                              "
                    " }                                          ");
                                
    // get the notebook pointer to eventually add a few pages                               
    gb << wxGbNotebook("nbook", &m_notebook);
    
    if(gb.Build(this))
    {
        SetSizerAndFit(gb.GetSizer());
            
        // add a page to the notebook
        wxNotebookPage *page = GetPage();
        if(page != NULL)
            m_notebook->AddPage(page, "Another Page");
    }
    
\endcode

    For the page add code:
    
\code
wxNotebookPage *MyDialog::GetPage()
{
    wxPanel *pnl = new wxPanel(m_notebook, -1);
    
    // another guibuilder instance
    wxGuiBuilder gb("V{                "
                    "    'Name'        "
                    "    [------------]"
                    "    'Age'         "
                    "    [------------]"
                    " }                ");
    
    if(gb.Build(pnl, wxID_ANY))
        pnl->SetSizerAndFit(gb.GetSizer()); 

    return dynamic_cast<wxNotebookPage *>(pnl);
}
\endcode

    See also \ref gb_refs and \ref grm_notebook
*/
WX_GB_DECLARE_REGWINDOW(wxGbNotebook, wxNotebook);

/** \class wxGuiBuilder
    \brief The main object that parses the given text and creates a GUI with sizers from it

    This class produces a wxSizer object after parsing a text. This should provide an alternative to using 
    sizers directly, but provide at least as much flexibility to allow users to semi-visually design their 
    forms / panels real quickly using sizers.
    
    It is recommended to read the \ref welcome_gb "main page" for the general purpose of wxGuiBuilder, and 
    after that the \ref wxgb_controls "wxGuiGuilder grammar" and the \ref gb_refs "substitutes section" to 
    learn how to change the default looks and flags of the controls that are created.
*/
class wxGuiBuilder
{
public:
    /** Creates a wxGuiBuilder object. This object should most likely be created on the stack. Once the 
        object loses scope, only the internal structure is deleted, the generated sizers and controls 
        will not be deleted and stay assigned to the parent that is given during wxGuiBuilder::Build. 
        
        Pass the sizer string structure in the \a sizer argument. In UNICODE this must be a multi byte 
        string, in ANSI it is single byte. The string can be read from disk, or simply copied from a 
        char array if needed. 
        
        See \ref what_gb "a small example" of how this is used.
        */
    wxGuiBuilder(const wxString &sizer)
        : m_buffer(sizer)
        , m_topSizer(0)
        , m_startID(0)
        , m_lastIndex(0)
        , m_flags(0)
        , m_sizerType(wxGB_BOXSIZER_H)
        , m_workID(wxID_ANY)
        , m_realizeError(false)
        , m_sizer(NULL)
        , m_demoMode(false)
    {
        // set default flags
        m_defWindowFlags.Center().Border(wxALL, 5);
        m_defSizerFlags.Center().Border(wxALL, 5);
    };

#ifndef _NO_DOXYGEN_
    friend void SetRealizeError(wxGuiBuilder &owner);

    // use this method to change what sizer is used per default if there is no
    // sizer specification
    void SetSizerDefaults(size_t sizerType = wxGB_BOXSIZER_H,
                          const wxSizerFlags &flags = wxSizerFlags(0)) {
        m_sizerType = sizerType;
        m_flags = flags;
    };
#endif

    /** The main method. After you created the wxGuiBuilder object, and used the \ref gb_refs "reference classes" to
        add extra information on how the windows should be created, call this Build() method to initiate the building 
        of the GUI. 
        
        If you wanted to start the whole sizer with different default window flags or sizer flags, before calling buils
        call wxGuiBuilder::SetWindowFlags() or wxGuiBuilder::SetSizerFlags() to influence the start values of the default
        sizer flags.
        
        Arguments:
        - \a parent is the window the controls that are created will become a child of. This is the same dialog / panel or
          other container object you assign the sizer (retrieved with wxGuiBuilder::GetSizer()) to after a succesful build.
          It is not certain that all child GUI objects or substitute wxWindow classes will keep the same parent. For 
          example when a wxNotebook or wxPanel is created inside the wxGuiBuilder, the wxWindow items used inside this 
          container object will be reparented to belong to the proper parent. This is done automatically.
        - \a startID is the ID from which all auto created controls should get their ID from. If wxID_ANY is used, the 
          internal wxWidgets method ::wxNewId() is used to obtain a new ID for every control. If an ID is given as argument,
          all successive classes will get an ID equal to the startID + increment. 
        - \a recompile can be used to re-use the same wxGuiBuilder string that is passed. The internal data structure is 
          thrown away, <B>PLEASE BE AWARE</b> that all classes that are NOT auto created are also thrown away. This means
          all substitute windows need to be passed again. This is necessary to make sure the same window control is not
          re-used between sessions. It is not recommended to recompile. It is more safe to simply recreate a wxGuiBuilder 
          object and start from scratch.
    
        An example piece of code:
        
\code
wxGuiBuilder gb("                                                                         "
                "H{                                                                       "
                "    V'Select some options'{                                              "
                "        %w.0a<                                                           "
                "        ( ) 'Generate code on the fly'                                   "
                "        (.) 'Generate code after saving'                                 "
                "                                                                         "
                "        [ ] 'Use original file'                                          "
                "        [ ] 'Use backup file'                                            "
                "                                                                         "
                "        %w.5lr|                                                          "
                "        H{ <spc:15,0> 'Select your backup file' [----------],1 ['...'] }+"
                "    }+,1                                                                 "
                "}                                                                        ");

if(gb.Build(this, wxID_ANY))     
{     
    SetSizerAndFit(gb.GetSizer());      

    // TODO: Initialize your dialog further     
}         
\endcode
        
        Will result in this dialog:
    
        \image html dialog_example.png
    
        The error handling of this Build() command goes through wxLogError messages. The messages are very verbose, 
        and also pinpoint the exact row, column and position inside the text that a syntax error or problem occured.
        If this method returns <b>false</b> the build failed. Most usual there are three situations occuring:
        
        <h4>Syntax errors occured</h4>
        When syntax errors occur, you are usually presented with a dialog like this:
        
        \image html gb_29.png
        
        The errors propagate. The top error is the most important one, and it goes all the way down to the top sizer 
        that could not be parsed. It is not recommended to suppress these errors as they are useful in debugging the
        GUI markup code.
        
        <h4>Verification errors occured</h4>         
        This is more serious. There is either something wrong in the code, or in the sizer text. For example when you 
        define two substitute windows of ID 1 like <b>H{ $1 $1 }</b>. The verification procedure will make sure that 
        substitute windows are not defined double, if all substitutes have an assigned pointer, if labels are not
        defined more then once, if the top sizer has at least one element, etc. The following window will be shown 
        when verification errors occur:
        
        \image html gb_30.png
        
        <h4>Fatal errors occured</h4> 
        This is dead serious! When this occurs it is wise to abort your application or close it ASAP. Fatal errors occur 
        when you passed pointer-pointers to GUI controls like;
        
        \code
            gb << wxGbButton("my_button", &m_button);
        \endcode
        
        And you rely on the fact that m_button (which is a wxButton *) is filled with a pointer. The fatal error is 
        presented to you when one or more of those pointers are not set, risking NULL pointer assignments. Fatal errors
        look like;
        
        \image html gb_25.png
        
        If YES is chosen, the methog wxLogFatalError() is called and the application is aborted.
    */   
    bool Build(wxWindow *parent, int startID = wxID_ANY, bool recompile = false);

    /** Destructor. Everything is deleted, except all the wxWindow controls that are created or used as substitutes. These 
        windows are now child of one or more panels and are destroyed when the GUI is destroyed. 
        
        <B>IT IS NOT RECOMMENDED</B> to keep wxGuiBuilder as a member variable in your class, as it keeps an internal data 
        structure which keeps memory occupied for no reason, and re-using it is not recommended to begin with. Always use
        this object on the stack 
    */    
    virtual ~wxGuiBuilder() {
        Reset();

        for(size_t i = 0; i < m_regitems.size(); i++)
            delete m_regitems[i];
        m_regitems.clear();
    };

    /** After the wxGuiBuilder::Build() action returns <b>true</b> the wxSizer that needs to be assigned can be retrieved 
        from this method. When <b>false</b> is returned, this method returns a NULL sizer.  */
    wxSizer *GetSizer() {
        return m_sizer;
    };

#ifndef _NO_DOXYGEN_
    // method that returns a new ID to be used. If the work ID is wxID_ANY
    // we define one ourselves, if the ID has another value, we return and
    // increment
    long RequestID(const wxString &label);

    // request style and size
    bool RequestWindowParams(const wxString &label, long &style, wxSize &size);
#endif

    /** Sets default window flags. Set this function BEFORE you call the Build function. All windows created 
        will carry at least these flags, until they are overridden by another flags set. The default 
        window flags are:
        
        - Border of 5
        - Centered in the area given
        
        See wxSizerFlags in the wxWidgets documentation for more information.
    */
    void SetWindowFlags(wxSizerFlags &flags) {
        m_defWindowFlags = flags;
    };
    /** Sets default sizer flags. Set this function BEFORE you call the Build function. All sizers created will 
        carry at least these flags, until they are overridden by another flags set. The default sizer flags are:
        
        - Border of 5
        - Centred in the area given
    */
    void SetSizerFlags(wxSizerFlags &flags) {
        m_defSizerFlags = flags;
    };

   bool Register(const wxGbSomeWindow &button) {
        if(!CanRegister(button))
            return false;
        m_regitems.push_back(new wxGbSomeWindow(button));
        return true;
    };
    /** Registers a wxGbButton class that carries either a pointer-pointer, wxWindow ID, size or flags to the wxButton to 
        be created. By using a label in the GUI text, the wxGbButton reference is linked to the control to be created. 
        
        Also see operator<<(const wxGbButton &) which is a convenience operator and works easier.
        
        \sa wxGbButton
    */
    bool Register(const wxGbButton &button) {
        if(!CanRegister(button))
            return false;
        m_regitems.push_back(new wxGbButton(button));
        return true;
    };

    /** Registers a wxGbTextCtrl class that carries either a pointer-pointer, wxWindow ID, size or flags to the wxTextCtrl to 
        be created. By using a label in the GUI text, the wxGbTextCtrl reference is linked to the control to be created. 
        
        Also see operator<<(const wxGbTextCtrl &) which is a convenience operator and works easier.
        
        \sa wxGbTextCtrl
    */
    bool Register(const wxGbTextCtrl &text) {
        if(!CanRegister(text))
            return false;
        m_regitems.push_back(new wxGbTextCtrl(text));
        return true;
    };

    /** Registers a wxGbRadioButton class that carries either a pointer-pointer, wxWindow ID, size or flags to the 
        wxRadioButton to be created. By using a label in the GUI text, the wxGbRadioButton reference is linked to 
        the control to be created. 
        
        Also see operator<<(const wxGbRadioButton &) which is a convenience operator and works easier.
        
        \sa wxGbRadioButton
    */
    bool Register(const wxGbRadioButton &rb) {
        if(!CanRegister(rb))
            return false;
        m_regitems.push_back(new wxGbRadioButton(rb));
        return true;
    };

    /** Registers a wxGbCheckBox class that carries either a pointer-pointer, wxWindow ID, size or flags to the 
        wxCheckBox to be created. By using a label in the GUI text, the wxGbCheckBox reference is linked to 
        the control to be created. 
        
        Also see operator<<(const wxGbCheckBox &) which is a convenience operator and works easier.
        
        \sa wxGbCheckBox
    */
    bool Register(const wxGbCheckBox &cb) {
        if(!CanRegister(cb))
            return false;
        m_regitems.push_back(new wxGbCheckBox(cb));
        return true;
    };

    /** Registers a wxGbStaticText class that carries either a pointer-pointer, wxWindow ID, size or flags to the 
        wxStaticText to be created. By using a label in the GUI text, the wxGbStaticText reference is linked to 
        the control to be created. 
        
        Also see operator<<(const wxGbStaticText &) which is a convenience operator and works easier.
        
        \sa wxGbStaticText
    */
    bool Register(const wxGbStaticText &st) {
        if(!CanRegister(st))
            return false;
        m_regitems.push_back(new wxGbStaticText(st));
        return true;
    };

    /** Registers a wxGbPanel class that carries either a pointer-pointer, wxWindow ID, size or flags to the 
        wxPanel to be created. By using a label in the GUI text, the wxGbPanel reference is linked to 
        the control to be created. 
        
        Also see operator<<(const wxGbPanel &) which is a convenience operator and works easier.
        
        \sa wxGbPanel
    */
    bool Register(const wxGbPanel &st) {
        if(!CanRegister(st))
            return false;
        m_regitems.push_back(new wxGbPanel(st));
        return true;
    };

    /** Registers a wxGbNotebook class that carries either a pointer-pointer, wxWindow ID, size or flags to the 
        wxNotebook to be created. By using a label in the GUI text, the wxGbNotebook reference is linked to 
        the control to be created. 
        
        Also see operator<<(const wxGbNotebook &) which is a convenience operator and works easier.
        
        \sa wxGbNotebook
    */
    bool Register(const wxGbNotebook &st) {
        if(!CanRegister(st))
            return false;
        m_regitems.push_back(new wxGbNotebook(st));
        return true;
    };
    
    /** Registers an existing wxWindow derived class to take the place in the stub assigned
        with the ID. If the window ID already exists an error is thrown. The stubs will
        be filled in and reparented (if needed) once the GUI is built by wxGuiBuilder
         
         \sa wxGbWindowStub 
    */
    bool Register(const wxGbWindowStub &item) {
        if(!CanRegister(item))
            return false;
        m_regitems.push_back(new wxGbWindowStub(item));
        return true;
    };

    /** Registers an existing wxWindow derived class to take the place in the stub assigned
        with the ID. If the window ID already exists an error is thrown. The stubs will
        be filled in and reparented (if needed) once the GUI is built by wxGuiBuilder
    
        This version does not use a class but only an \a id and \a window pointer.
        See more information at \ref gb_refs . 
    */
    bool Register(int id, wxWindow *window) {
        wxGbWindowStub st(id, window);
        return Register(st);
    };

    /** Convenience operator for wxGbPanel. See wxGuiBuilder::Register(const wxGbPanel &)
        for more information. */
    wxGuiBuilder &operator<<(const wxGbPanel &item) {
        Register(item);
        return (*this);
    };

    /** Convenience operator for wxGbButton. See wxGuiBuilder::Register(const wxGbButton &)
        for more information. */
    wxGuiBuilder &operator<<(const wxGbButton &item) {
        Register(item);
        return (*this);
    };

    /** Convenience operator for wxGbTextCtrl. See wxGuiBuilder::Register(const wxGbTextCtrl &)
        for more information. */
    wxGuiBuilder &operator<<(const wxGbTextCtrl &item) {
        Register(item);
        return (*this);
    };

    /** Convenience operator for wxGbCheckBox. See wxGuiBuilder::Register(const wxGbCheckBox &)
        for more information. */
    wxGuiBuilder &operator<<(const wxGbCheckBox &item) {
        Register(item);
        return (*this);
    };

    /** Convenience operator for wxGbRadioButton. See wxGuiBuilder::Register(const wxGbRadioButton &)
        for more information. */
    wxGuiBuilder &operator<<(const wxGbRadioButton &item) {
        Register(item);
        return (*this);
    };

    /** Convenience operator for wxGbStaticText. See wxGuiBuilder::Register(const wxGbStaticText &)
        for more information. */
    wxGuiBuilder &operator<<(const wxGbStaticText &item) {
        Register(item);
        return (*this);
    };
    
    /** Convenience operator for wxGbNotebook. See wxGuiBuilder::Register(const wxGbNotebook &)
        for more information. */
    wxGuiBuilder &operator<<(const wxGbNotebook &item) {
        Register(item);
        return (*this);
    };

    /** Convenience operator for wxGbWindowStub. See wxGuiBuilder::Register(const wxGbWindowStub &)
        for more information. */
    wxGuiBuilder &operator<<(const wxGbWindowStub &item) {
        Register(item);
        return (*this);
    };
    
    /** This method will auto create substitute windows ($1, $2 etc) so that the verification after the 
        build will not fail. This is only for demo purposes when code that is shown does not have the 
        ability to set substitute windows 
    */
    void SetDemoMode() {
        m_demoMode = true;
    };

  bool Setup(wxWindow *parent, int startID, bool recompile);
  bool CheckWindowByLabel(const wxString &label);

#ifndef _USE_DOXYGEN_
private:
    /** Maps all registered controls to the proper pointers. If after mapping some pointers
        are still NULL, false is returned. This will allow the user to take measures like
        raising an assertion. */
    void MapControlPointers();
    
    // demo mode lookup assigning
    void DoDemoMode(wxWindow *parent);
    
    // sets error flag, so the Realize method knows something
    // went wrong (if not shown in release mode)
    void SetErrorFlag() {
        m_realizeError = true;
    };

    // clean up sizer and unassign it
    void Reset();
//public:
    // method that parses and compiles the sizer to give back the sizer
  // pointer of the top sizer containing the layout
    bool Compile();
//public:
    // creates a flat list of window and sizer items, easy to quickly
    // scan through
    void FillLookupList(wxGbSizerItem *si);

    // A function to check for the list integrity, which means
    // no double labels, or double subsititute ID's, window ID's etc
    bool VerifyLookupList();

    wxGbWindowItem *DoFindWindowByLabel(const wxString &label);

    bool CanRegister(const wxGbRegWindowBase &item);

  // lookup table with all added window items, for quick ID lookup
    // it will be one time composed, and when the window items should be
    // added this list can help determining ID's and if there is already
    // a window attached to it.
    std::vector<wxGbBaseItem *> m_lookup;
    // items that are registered to be written to in the method MapControls
    // which will write to all given pointer pointers.
    std::vector<wxGbRegWindowBase *> m_regitems;
    // buffer of the sizer
    wxString m_buffer;
    // the starting sizer
    wxGbSizerItem *m_topSizer;
    // start ID
    long m_startID;
    // the index last used, which means when operator<< is used to slide in
    // ready created controls, the last count is used to look up that particular
    // item
    size_t m_lastIndex;
    // default sizer flags
    wxSizerFlags m_flags;
    // default sizer type to take if none is defined
    size_t m_sizerType;
    // ID for next control
    long m_workID;
    // error in realize procedure
    bool m_realizeError;
    // default window flags, e.g. with a border or alligned a certain way
    wxSizerFlags m_defWindowFlags;
    // default sizer flags, e.g. with a border or alligned a certain way
    wxSizerFlags m_defSizerFlags;
    // the sizer that is returned
    wxSizer *m_sizer;
    // the demo mode
    bool m_demoMode;
#endif // _USE_DOXYGEN_
};

#endif

