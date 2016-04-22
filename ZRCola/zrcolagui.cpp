///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "zrcolagui.h"

// Using the construction of a static object to ensure that the help provider is set
class wxFBContextSensitiveHelpSetter
{
public:
wxFBContextSensitiveHelpSetter()
{
wxHelpProvider::Set( new wxHelpControllerHelpProvider );
}
};

static wxFBContextSensitiveHelpSetter s_wxFBSetTheHelpProvider;
///////////////////////////////////////////////////////////////////////////

wxZRColaFrameBase::wxZRColaFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxFrame( parent, id, title, pos, size, style, name )
{
	this->SetSizeHints( wxSize( 150,150 ), wxDefaultSize );
	
	m_menubar = new wxMenuBar( 0 );
	m_menuProgram = new wxMenu();
	wxMenuItem* m_menuItemExit;
	m_menuItemExit = new wxMenuItem( m_menuProgram, wxID_EXIT, wxString( _("E&xit") ) + wxT('\t') + wxT("Alt+F4"), _("Quit this program"), wxITEM_NORMAL );
	m_menuProgram->Append( m_menuItemExit );
	
	m_menubar->Append( m_menuProgram, _("&Program") ); 
	
	m_menuEdit = new wxMenu();
	wxMenuItem* m_menuItemEditCut;
	m_menuItemEditCut = new wxMenuItem( m_menuEdit, wxID_CUT, wxString( wxEmptyString ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menuItemEditCut->SetBitmaps( wxIcon( wxT("edit_cut.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16 ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menuItemEditCut->SetBitmap( wxIcon( wxT("edit_cut.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16 ) );
	#endif
	m_menuEdit->Append( m_menuItemEditCut );
	
	wxMenuItem* m_menuItemEditCopy;
	m_menuItemEditCopy = new wxMenuItem( m_menuEdit, wxID_COPY, wxString( wxEmptyString ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menuItemEditCopy->SetBitmaps( wxIcon( wxT("edit_copy.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16 ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menuItemEditCopy->SetBitmap( wxIcon( wxT("edit_copy.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16 ) );
	#endif
	m_menuEdit->Append( m_menuItemEditCopy );
	
	wxMenuItem* m_menuItemEditPaste;
	m_menuItemEditPaste = new wxMenuItem( m_menuEdit, wxID_PASTE, wxString( wxEmptyString ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menuItemEditPaste->SetBitmaps( wxIcon( wxT("edit_paste.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16 ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menuItemEditPaste->SetBitmap( wxIcon( wxT("edit_paste.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16 ) );
	#endif
	m_menuEdit->Append( m_menuItemEditPaste );
	
	m_menuEdit->AppendSeparator();
	
	wxMenuItem* m_menuSelectAll;
	m_menuSelectAll = new wxMenuItem( m_menuEdit, wxID_SELECTALL, wxString( _("Select &All") ) + wxT('\t') + wxT("Ctrl+A"), _("Select all text"), wxITEM_NORMAL );
	m_menuEdit->Append( m_menuSelectAll );
	
	m_menuEdit->AppendSeparator();
	
	wxMenuItem* m_menuItemSendComposed;
	m_menuItemSendComposed = new wxMenuItem( m_menuEdit, wxID_SEND_COMPOSED, wxString( _("&Send Composed") ) + wxT('\t') + wxT("F5"), _("Send composed text to source window"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menuItemSendComposed->SetBitmaps( wxIcon( wxT("send_composed.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16 ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menuItemSendComposed->SetBitmap( wxIcon( wxT("send_composed.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16 ) );
	#endif
	m_menuEdit->Append( m_menuItemSendComposed );
	
	wxMenuItem* m_menuItemSendDecomposed;
	m_menuItemSendDecomposed = new wxMenuItem( m_menuEdit, wxID_SEND_DECOMPOSED, wxString( _("Send &Decomposed") ) + wxT('\t') + wxT("F6"), _("Send decomposed text to source window"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menuItemSendDecomposed->SetBitmaps( wxIcon( wxT("send_decomposed.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16 ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menuItemSendDecomposed->SetBitmap( wxIcon( wxT("send_decomposed.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16 ) );
	#endif
	m_menuEdit->Append( m_menuItemSendDecomposed );
	
	wxMenuItem* m_menuItemSendAbort;
	m_menuItemSendAbort = new wxMenuItem( m_menuEdit, wxID_SEND_ABORT, wxString( _("Abort (De)composition") ) + wxT('\t') + wxT("Esc"), _("Abort composition and return focus to source window"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menuItemSendAbort->SetBitmaps( wxIcon( wxT("send_abort.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16 ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menuItemSendAbort->SetBitmap( wxIcon( wxT("send_abort.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16 ) );
	#endif
	m_menuEdit->Append( m_menuItemSendAbort );
	
	m_menuDecompLanguage = new wxMenu();
	wxMenuItem* m_menuDecompLanguageItem = new wxMenuItem( m_menuEdit, wxID_ANY, _("&Language"), wxEmptyString, wxITEM_NORMAL, m_menuDecompLanguage );
	m_menuEdit->Append( m_menuDecompLanguageItem );
	
	m_menubar->Append( m_menuEdit, _("&Edit") ); 
	
	m_menuHelp = new wxMenu();
	wxMenuItem* m_menuItemAbout;
	m_menuItemAbout = new wxMenuItem( m_menuHelp, wxID_ABOUT, wxString( wxEmptyString ) , wxEmptyString, wxITEM_NORMAL );
	m_menuHelp->Append( m_menuItemAbout );
	
	m_menubar->Append( m_menuHelp, _("&Help") ); 
	
	this->SetMenuBar( m_menubar );
	
	m_toolbar = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY );
	m_toolbar->SetToolBitmapSize( wxSize( 16,16 ) );
	m_toolEditCut = m_toolbar->AddTool( wxID_CUT, _("Cut"), wxIcon( wxT("edit_cut.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 24, 24 ), wxNullBitmap, wxITEM_NORMAL, _("Cut"), _("Cut selection"), NULL ); 
	
	m_toolEditCopy = m_toolbar->AddTool( wxID_COPY, _("Copy"), wxIcon( wxT("edit_copy.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 24, 24 ), wxNullBitmap, wxITEM_NORMAL, _("Copy"), _("Copy selection"), NULL ); 
	
	m_toolEditPaste = m_toolbar->AddTool( wxID_PASTE, _("Paste"), wxIcon( wxT("edit_paste.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 24, 24 ), wxNullBitmap, wxITEM_NORMAL, _("Paste"), _("Paste selection"), NULL ); 
	
	m_toolbar->AddSeparator(); 
	
	m_toolSendComposed = m_toolbar->AddTool( wxID_SEND_COMPOSED, _("Send Composed"), wxIcon( wxT("send_composed.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 24, 24 ), wxNullBitmap, wxITEM_NORMAL, _("Send Composed"), _("Send composed text to source window"), NULL ); 
	
	m_toolSendDecomposed = m_toolbar->AddTool( wxID_SEND_DECOMPOSED, _("Send Decomposed"), wxIcon( wxT("send_decomposed.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 24, 24 ), wxNullBitmap, wxITEM_NORMAL, _("Send Decomposed"), _("Send decomposed text to source window"), NULL ); 
	
	m_toolDecompLanguageLbl = new wxStaticText( m_toolbar, wxID_ANY, _("Language:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_toolDecompLanguageLbl->Wrap( -1 );
	m_toolbar->AddControl( m_toolDecompLanguageLbl );
	wxArrayString m_toolDecompLanguageChoices;
	m_toolDecompLanguage = new wxChoice( m_toolbar, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_toolDecompLanguageChoices, 0 );
	m_toolDecompLanguage->SetSelection( 0 );
	m_toolbar->AddControl( m_toolDecompLanguage );
	m_toolbar->Realize(); 
	
	wxBoxSizer* bSizerMain;
	bSizerMain = new wxBoxSizer( wxVERTICAL );
	
	m_panel = new wxZRColaComposerPanel( this );
	
	bSizerMain->Add( m_panel, 100, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizerMain );
	this->Layout();
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_toolDecompLanguage->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( wxZRColaFrameBase::OnDecompLanguageChoice ), NULL, this );
}

wxZRColaFrameBase::~wxZRColaFrameBase()
{
	// Disconnect Events
	m_toolDecompLanguage->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( wxZRColaFrameBase::OnDecompLanguageChoice ), NULL, this );
	
}

wxZRColaComposerPanelBase::wxZRColaComposerPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizerEditor;
	bSizerEditor = new wxBoxSizer( wxVERTICAL );
	
	m_decomposed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_MULTILINE );
	m_decomposed->SetFont( wxFont( 20, 70, 90, 90, false, wxT("00 ZRCola") ) );
	m_decomposed->SetMinSize( wxSize( 100,25 ) );
	
	bSizerEditor->Add( m_decomposed, 50, wxALL|wxEXPAND, 5 );
	
	m_composed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_MULTILINE );
	m_composed->SetFont( wxFont( 20, 70, 90, 90, false, wxT("00 ZRCola") ) );
	m_composed->SetMinSize( wxSize( 100,25 ) );
	
	bSizerEditor->Add( m_composed, 50, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( bSizerEditor );
	this->Layout();
	bSizerEditor->Fit( this );
	
	// Connect Events
	m_decomposed->Connect( wxEVT_PAINT, wxPaintEventHandler( wxZRColaComposerPanelBase::OnDecomposedPaint ), NULL, this );
	m_decomposed->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaComposerPanelBase::OnDecomposedText ), NULL, this );
	m_composed->Connect( wxEVT_PAINT, wxPaintEventHandler( wxZRColaComposerPanelBase::OnComposedPaint ), NULL, this );
	m_composed->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaComposerPanelBase::OnComposedText ), NULL, this );
}

wxZRColaComposerPanelBase::~wxZRColaComposerPanelBase()
{
	// Disconnect Events
	m_decomposed->Disconnect( wxEVT_PAINT, wxPaintEventHandler( wxZRColaComposerPanelBase::OnDecomposedPaint ), NULL, this );
	m_decomposed->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaComposerPanelBase::OnDecomposedText ), NULL, this );
	m_composed->Disconnect( wxEVT_PAINT, wxPaintEventHandler( wxZRColaComposerPanelBase::OnComposedPaint ), NULL, this );
	m_composed->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaComposerPanelBase::OnComposedText ), NULL, this );
	
}
