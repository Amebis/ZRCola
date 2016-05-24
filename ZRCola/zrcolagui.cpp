///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "zrcolachrgrid.h"

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
	m_mgr.SetManagedWindow(this);
	m_mgr.SetFlags(wxAUI_MGR_DEFAULT);
	
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
	
	wxMenuItem* m_menuCharSelect;
	m_menuCharSelect = new wxMenuItem( m_menuEdit, wxID_CHARACTER_SELECTOR, wxString( _("C&haracter Selector...") ) + wxT('\t') + wxT("F8"), _("Display character selector to select character to insert into text"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menuCharSelect->SetBitmaps( wxIcon( wxT("char_select.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16 ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menuCharSelect->SetBitmap( wxIcon( wxT("char_select.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16 ) );
	#endif
	m_menuEdit->Append( m_menuCharSelect );
	
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
	
	m_menuEdit->AppendSeparator();
	
	wxMenuItem* m_menuSettings;
	m_menuSettings = new wxMenuItem( m_menuEdit, wxID_SETTINGS, wxString( _("&Settings...") ) , _("Open program configuration dialog"), wxITEM_NORMAL );
	m_menuEdit->Append( m_menuSettings );
	
	m_menubar->Append( m_menuEdit, _("&Edit") ); 
	
	m_menuView = new wxMenu();
	wxMenuItem* m_menuItemToolbarEdit;
	m_menuItemToolbarEdit = new wxMenuItem( m_menuView, wxID_TOOLBAR_EDIT, wxString( _("&Edit Toolbar") ) , _("Toggle edit toolbar"), wxITEM_CHECK );
	m_menuView->Append( m_menuItemToolbarEdit );
	
	wxMenuItem* m_menuItemToolbarCompose;
	m_menuItemToolbarCompose = new wxMenuItem( m_menuView, wxID_TOOLBAR_COMPOSE, wxString( _("&Compose Toolbar") ) , _("Toggle compose toolbar"), wxITEM_CHECK );
	m_menuView->Append( m_menuItemToolbarCompose );
	
	m_menuView->AppendSeparator();
	
	wxMenuItem* m_menuItemPanelChrGrps;
	m_menuItemPanelChrGrps = new wxMenuItem( m_menuView, wxID_PANEL_CHRGRPS, wxString( _("Character Catalo&g") ) , _("Toggle character catalog panel"), wxITEM_CHECK );
	m_menuView->Append( m_menuItemPanelChrGrps );
	
	m_menubar->Append( m_menuView, _("&View") ); 
	
	m_menuHelp = new wxMenu();
	wxMenuItem* m_menuHelpReqChar;
	m_menuHelpReqChar = new wxMenuItem( m_menuHelp, wxID_HELP_REQCHAR, wxString( _("&Request a new character...") ) , _("Submit a request to ZRC to add a new character"), wxITEM_NORMAL );
	m_menuHelp->Append( m_menuHelpReqChar );
	
	m_menuHelp->AppendSeparator();
	
	wxMenuItem* m_menuHelpAbout;
	m_menuHelpAbout = new wxMenuItem( m_menuHelp, wxID_ABOUT, wxString( wxEmptyString ) , wxEmptyString, wxITEM_NORMAL );
	m_menuHelp->Append( m_menuHelpAbout );
	
	m_menubar->Append( m_menuHelp, _("&Help") ); 
	
	this->SetMenuBar( m_menubar );
	
	m_toolbarEdit = new wxAuiToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_HORZ_LAYOUT ); 
	m_toolEditCut = m_toolbarEdit->AddTool( wxID_CUT, _("Cut"), wxIcon( wxT("edit_cut.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 24, 24 ), wxNullBitmap, wxITEM_NORMAL, _("Cut"), _("Cut selection"), NULL ); 
	
	m_toolEditCopy = m_toolbarEdit->AddTool( wxID_COPY, _("Copy"), wxIcon( wxT("edit_copy.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 24, 24 ), wxNullBitmap, wxITEM_NORMAL, _("Copy"), _("Copy selection"), NULL ); 
	
	m_toolEditPaste = m_toolbarEdit->AddTool( wxID_PASTE, _("Paste"), wxIcon( wxT("edit_paste.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 24, 24 ), wxNullBitmap, wxITEM_NORMAL, _("Paste"), _("Paste selection"), NULL ); 
	
	m_toolbarEdit->Realize();
	m_mgr.AddPane( m_toolbarEdit, wxAuiPaneInfo().Name( wxT("toolbarEdit") ).Top().Caption( _("Edit") ).PinButton( true ).Dock().Resizable().FloatingSize( wxSize( -1,-1 ) ).LeftDockable( false ).RightDockable( false ).Row( 0 ).Layer( 1 ).ToolbarPane() );
	
	m_toolbarCompose = new wxAuiToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_HORZ_LAYOUT ); 
	m_toolCharSelect = m_toolbarCompose->AddTool( wxID_CHARACTER_SELECTOR, _("Character Selector"), wxIcon( wxT("char_select.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 24, 24 ), wxNullBitmap, wxITEM_NORMAL, _("Character Selector"), _("Display character selector to select character to insert into text"), NULL ); 
	
	m_toolSendComposed = m_toolbarCompose->AddTool( wxID_SEND_COMPOSED, _("Send Composed"), wxIcon( wxT("send_composed.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 24, 24 ), wxNullBitmap, wxITEM_NORMAL, _("Send Composed"), _("Send composed text to source window"), NULL ); 
	
	m_toolSendDecomposed = m_toolbarCompose->AddTool( wxID_SEND_DECOMPOSED, _("Send Decomposed"), wxIcon( wxT("send_decomposed.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 24, 24 ), wxNullBitmap, wxITEM_NORMAL, _("Send Decomposed"), _("Send decomposed text to source window"), NULL ); 
	
	m_toolbarCompose->Realize();
	m_mgr.AddPane( m_toolbarCompose, wxAuiPaneInfo().Name( wxT("toolbarCompose") ).Top().Caption( _("Compose") ).PinButton( true ).Dock().Resizable().FloatingSize( wxSize( -1,-1 ) ).LeftDockable( false ).RightDockable( false ).Row( 0 ).Layer( 1 ).ToolbarPane() );
	
	m_panelChrCat = new wxZRColaCharacterCatalogPanel( this );
	
	m_mgr.AddPane( m_panelChrCat, wxAuiPaneInfo() .Name( wxT("panelChrGrp") ).Left() .Caption( _("Character Catalog") ).PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ).Row( 1 ).BestSize( wxSize( 150,200 ) ).MinSize( wxSize( 100,100 ) ).Layer( 1 ) );
	
	m_panel = new wxZRColaComposerPanel( this );
	
	m_mgr.AddPane( m_panel, wxAuiPaneInfo() .Name( wxT("composerPanel") ).Center() .Caption( _("(De)Composer") ).CaptionVisible( false ).CloseButton( false ).PaneBorder( false ).Dock().Resizable().FloatingSize( wxDefaultSize ).Floatable( false ) );
	
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	m_mgr.Update();
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_ICONIZE, wxIconizeEventHandler( wxZRColaFrameBase::OnIconize ) );
	this->Connect( wxEVT_IDLE, wxIdleEventHandler( wxZRColaFrameBase::OnIdle ) );
}

wxZRColaFrameBase::~wxZRColaFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_ICONIZE, wxIconizeEventHandler( wxZRColaFrameBase::OnIconize ) );
	this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( wxZRColaFrameBase::OnIdle ) );
	
	m_mgr.UnInit();
	
}

wxZRColaComposerPanelBase::wxZRColaComposerPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizerMain;
	bSizerMain = new wxBoxSizer( wxVERTICAL );
	
	m_splitterDecomposed = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D|wxSP_LIVE_UPDATE );
	m_splitterDecomposed->SetSashGravity( 1 );
	m_splitterDecomposed->Connect( wxEVT_IDLE, wxIdleEventHandler( wxZRColaComposerPanelBase::m_splitterDecomposedOnIdle ), NULL, this );
	m_splitterDecomposed->SetMinimumPaneSize( 5 );
	
	m_panelDecomposedEdit = new wxPanel( m_splitterDecomposed, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerDecomposedEdit;
	bSizerDecomposedEdit = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* bSizerDecomposedEdit2;
	bSizerDecomposedEdit2 = new wxStaticBoxSizer( new wxStaticBox( m_panelDecomposedEdit, wxID_ANY, _("Decomposed Text") ), wxVERTICAL );
	
	m_decomposed = new wxTextCtrl( bSizerDecomposedEdit2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	m_decomposed->SetFont( wxFont( 20, 70, 90, 90, false, wxT("00 ZRCola") ) );
	m_decomposed->SetMinSize( wxSize( 100,25 ) );
	
	bSizerDecomposedEdit2->Add( m_decomposed, 1, wxEXPAND, 5 );
	
	
	bSizerDecomposedEdit->Add( bSizerDecomposedEdit2, 1, wxEXPAND, 5 );
	
	
	m_panelDecomposedEdit->SetSizer( bSizerDecomposedEdit );
	m_panelDecomposedEdit->Layout();
	bSizerDecomposedEdit->Fit( m_panelDecomposedEdit );
	m_panelDecomposedHex = new wxPanel( m_splitterDecomposed, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerDecomposedHex;
	bSizerDecomposedHex = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* bSizerDecomposedHex2;
	bSizerDecomposedHex2 = new wxStaticBoxSizer( new wxStaticBox( m_panelDecomposedHex, wxID_ANY, _("Decomposed Unicode Dump") ), wxVERTICAL );
	
	m_decomposedHex = new wxTextCtrl( bSizerDecomposedHex2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	m_decomposedHex->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 76, 90, 90, false, wxEmptyString ) );
	
	bSizerDecomposedHex2->Add( m_decomposedHex, 1, wxEXPAND, 5 );
	
	
	bSizerDecomposedHex->Add( bSizerDecomposedHex2, 1, wxEXPAND, 5 );
	
	
	m_panelDecomposedHex->SetSizer( bSizerDecomposedHex );
	m_panelDecomposedHex->Layout();
	bSizerDecomposedHex->Fit( m_panelDecomposedHex );
	m_splitterDecomposed->SplitVertically( m_panelDecomposedEdit, m_panelDecomposedHex, -5 );
	bSizerMain->Add( m_splitterDecomposed, 50, wxALL|wxEXPAND, 5 );
	
	m_splitterComposed = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D|wxSP_LIVE_UPDATE );
	m_splitterComposed->SetSashGravity( 1 );
	m_splitterComposed->Connect( wxEVT_IDLE, wxIdleEventHandler( wxZRColaComposerPanelBase::m_splitterComposedOnIdle ), NULL, this );
	m_splitterComposed->SetMinimumPaneSize( 5 );
	
	m_panelComposedEdit = new wxPanel( m_splitterComposed, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerComposedEdit;
	bSizerComposedEdit = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* bSizerComposedEdit2;
	bSizerComposedEdit2 = new wxStaticBoxSizer( new wxStaticBox( m_panelComposedEdit, wxID_ANY, _("Composed Text") ), wxVERTICAL );
	
	m_composed = new wxTextCtrl( bSizerComposedEdit2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	m_composed->SetFont( wxFont( 20, 70, 90, 90, false, wxT("00 ZRCola") ) );
	m_composed->SetMinSize( wxSize( 100,25 ) );
	
	bSizerComposedEdit2->Add( m_composed, 1, wxEXPAND, 5 );
	
	
	bSizerComposedEdit->Add( bSizerComposedEdit2, 1, wxEXPAND, 5 );
	
	
	m_panelComposedEdit->SetSizer( bSizerComposedEdit );
	m_panelComposedEdit->Layout();
	bSizerComposedEdit->Fit( m_panelComposedEdit );
	m_panelComposedHex = new wxPanel( m_splitterComposed, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerComposedHex;
	bSizerComposedHex = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* bSizerComposedHex2;
	bSizerComposedHex2 = new wxStaticBoxSizer( new wxStaticBox( m_panelComposedHex, wxID_ANY, _("Composed Unicode Dump") ), wxVERTICAL );
	
	m_composedHex = new wxTextCtrl( bSizerComposedHex2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	m_composedHex->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 76, 90, 90, false, wxEmptyString ) );
	
	bSizerComposedHex2->Add( m_composedHex, 1, wxEXPAND, 5 );
	
	
	bSizerComposedHex->Add( bSizerComposedHex2, 1, wxEXPAND, 5 );
	
	
	m_panelComposedHex->SetSizer( bSizerComposedHex );
	m_panelComposedHex->Layout();
	bSizerComposedHex->Fit( m_panelComposedHex );
	m_splitterComposed->SplitVertically( m_panelComposedEdit, m_panelComposedHex, -5 );
	bSizerMain->Add( m_splitterComposed, 50, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( bSizerMain );
	this->Layout();
	bSizerMain->Fit( this );
	m_timerSave.SetOwner( this, wxID_TIMER_SAVE );
	
	// Connect Events
	m_decomposed->Connect( wxEVT_PAINT, wxPaintEventHandler( wxZRColaComposerPanelBase::OnDecomposedPaint ), NULL, this );
	m_decomposed->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaComposerPanelBase::OnDecomposedText ), NULL, this );
	m_decomposedHex->Connect( wxEVT_PAINT, wxPaintEventHandler( wxZRColaComposerPanelBase::OnDecomposedHexPaint ), NULL, this );
	m_composed->Connect( wxEVT_PAINT, wxPaintEventHandler( wxZRColaComposerPanelBase::OnComposedPaint ), NULL, this );
	m_composed->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaComposerPanelBase::OnComposedText ), NULL, this );
	m_composedHex->Connect( wxEVT_PAINT, wxPaintEventHandler( wxZRColaComposerPanelBase::OnComposedHexPaint ), NULL, this );
	this->Connect( wxID_TIMER_SAVE, wxEVT_TIMER, wxTimerEventHandler( wxZRColaComposerPanelBase::OnSaveTimer ) );
}

wxZRColaComposerPanelBase::~wxZRColaComposerPanelBase()
{
	// Disconnect Events
	m_decomposed->Disconnect( wxEVT_PAINT, wxPaintEventHandler( wxZRColaComposerPanelBase::OnDecomposedPaint ), NULL, this );
	m_decomposed->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaComposerPanelBase::OnDecomposedText ), NULL, this );
	m_decomposedHex->Disconnect( wxEVT_PAINT, wxPaintEventHandler( wxZRColaComposerPanelBase::OnDecomposedHexPaint ), NULL, this );
	m_composed->Disconnect( wxEVT_PAINT, wxPaintEventHandler( wxZRColaComposerPanelBase::OnComposedPaint ), NULL, this );
	m_composed->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaComposerPanelBase::OnComposedText ), NULL, this );
	m_composedHex->Disconnect( wxEVT_PAINT, wxPaintEventHandler( wxZRColaComposerPanelBase::OnComposedHexPaint ), NULL, this );
	this->Disconnect( wxID_TIMER_SAVE, wxEVT_TIMER, wxTimerEventHandler( wxZRColaComposerPanelBase::OnSaveTimer ) );
	
}

wxZRColaCharacterCatalogPanelBase::wxZRColaCharacterCatalogPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer;
	bSizer = new wxBoxSizer( wxVERTICAL );
	
	wxArrayString m_choiceChoices;
	m_choice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceChoices, 0 );
	m_choice->SetSelection( 0 );
	bSizer->Add( m_choice, 0, wxALL|wxEXPAND, 5 );
	
	m_grid = new wxZRColaCharGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	
	// Grid
	m_grid->CreateGrid( 0, 0 );
	m_grid->EnableEditing( false );
	m_grid->EnableGridLines( false );
	m_grid->EnableDragGridSize( false );
	m_grid->SetMargins( 0, 0 );
	
	// Columns
	m_grid->EnableDragColMove( false );
	m_grid->EnableDragColSize( false );
	m_grid->SetColLabelSize( 0 );
	m_grid->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_grid->EnableDragRowSize( false );
	m_grid->SetRowLabelSize( 0 );
	m_grid->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	m_grid->SetDefaultCellBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	m_grid->SetDefaultCellFont( wxFont( 20, 70, 90, 90, false, wxT("00 ZRCola") ) );
	m_grid->SetDefaultCellAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	m_grid->SetMinSize( wxSize( 35,35 ) );
	
	bSizer->Add( m_grid, 1, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer );
	this->Layout();
	bSizer->Fit( this );
	
	// Connect Events
	m_choice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( wxZRColaCharacterCatalogPanelBase::OnChoice ), NULL, this );
	m_grid->Connect( wxEVT_GRID_CELL_LEFT_CLICK, wxGridEventHandler( wxZRColaCharacterCatalogPanelBase::OnGridClick ), NULL, this );
	m_grid->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( wxZRColaCharacterCatalogPanelBase::OnGridKeyDown ), NULL, this );
}

wxZRColaCharacterCatalogPanelBase::~wxZRColaCharacterCatalogPanelBase()
{
	// Disconnect Events
	m_choice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( wxZRColaCharacterCatalogPanelBase::OnChoice ), NULL, this );
	m_grid->Disconnect( wxEVT_GRID_CELL_LEFT_CLICK, wxGridEventHandler( wxZRColaCharacterCatalogPanelBase::OnGridClick ), NULL, this );
	m_grid->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( wxZRColaCharacterCatalogPanelBase::OnGridKeyDown ), NULL, this );
	
}

wxZRColaCharSelectBase::wxZRColaCharSelectBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxDialog( parent, id, title, pos, size, style, name )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizerContent;
	bSizerContent = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizerColumns;
	bSizerColumns = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizerLeft;
	bSizerLeft = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizerBrowse;
	sbSizerBrowse = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("&Browse") ), wxVERTICAL );
	
	m_search = new wxSearchCtrl( sbSizerBrowse->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	#ifndef __WXMAC__
	m_search->ShowSearchButton( true );
	#endif
	m_search->ShowCancelButton( true );
	sbSizerBrowse->Add( m_search, 0, wxALL|wxEXPAND, 5 );
	
	wxArrayString m_categoriesChoices;
	m_categories = new wxCheckListBox( sbSizerBrowse->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize( -1,60 ), m_categoriesChoices, 0 );
	sbSizerBrowse->Add( m_categories, 0, wxALL|wxEXPAND, 5 );
	
	m_gridResults = new wxZRColaCharGrid( sbSizerBrowse->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxSTATIC_BORDER );
	
	// Grid
	m_gridResults->CreateGrid( 0, 0 );
	m_gridResults->EnableEditing( false );
	m_gridResults->EnableGridLines( false );
	m_gridResults->EnableDragGridSize( false );
	m_gridResults->SetMargins( 0, 0 );
	
	// Columns
	m_gridResults->EnableDragColMove( false );
	m_gridResults->EnableDragColSize( false );
	m_gridResults->SetColLabelSize( 0 );
	m_gridResults->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_gridResults->EnableDragRowSize( false );
	m_gridResults->SetRowLabelSize( 0 );
	m_gridResults->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	m_gridResults->SetDefaultCellFont( wxFont( 20, 70, 90, 90, false, wxT("00 ZRCola") ) );
	m_gridResults->SetDefaultCellAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	m_gridResults->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	m_gridResults->SetMinSize( wxSize( 560,35 ) );
	m_gridResults->SetMaxSize( wxSize( 560,-1 ) );
	
	sbSizerBrowse->Add( m_gridResults, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizerLeft->Add( sbSizerBrowse, 1, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizerRecent;
	sbSizerRecent = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Re&cently Used") ), wxVERTICAL );
	
	m_gridRecent = new wxZRColaCharGrid( sbSizerRecent->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize( -1,35 ), wxSTATIC_BORDER );
	
	// Grid
	m_gridRecent->CreateGrid( 0, 0 );
	m_gridRecent->EnableEditing( false );
	m_gridRecent->EnableGridLines( false );
	m_gridRecent->EnableDragGridSize( false );
	m_gridRecent->SetMargins( 0, 0 );
	
	// Columns
	m_gridRecent->EnableDragColMove( false );
	m_gridRecent->EnableDragColSize( false );
	m_gridRecent->SetColLabelSize( 0 );
	m_gridRecent->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_gridRecent->EnableDragRowSize( false );
	m_gridRecent->SetRowLabelSize( 0 );
	m_gridRecent->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	m_gridRecent->SetDefaultCellFont( wxFont( 20, 70, 90, 90, false, wxT("00 ZRCola") ) );
	m_gridRecent->SetDefaultCellAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	m_gridRecent->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	sbSizerRecent->Add( m_gridRecent, 0, wxALL|wxEXPAND, 5 );
	
	
	bSizerLeft->Add( sbSizerRecent, 0, wxALL|wxEXPAND, 5 );
	
	
	bSizerColumns->Add( bSizerLeft, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizerRight;
	bSizerRight = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizerPreview;
	sbSizerPreview = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Preview") ), wxVERTICAL );
	
	wxBoxSizer* bSizerUnicode;
	bSizerUnicode = new wxBoxSizer( wxHORIZONTAL );
	
	m_labelUnicode = new wxStaticText( sbSizerPreview->GetStaticBox(), wxID_ANY, _("U+"), wxDefaultPosition, wxDefaultSize, 0 );
	m_labelUnicode->Wrap( -1 );
	bSizerUnicode->Add( m_labelUnicode, 0, wxALIGN_CENTER, 5 );
	
	m_unicode = new wxTextCtrl( sbSizerPreview->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), 0 );
	bSizerUnicode->Add( m_unicode, 0, wxALIGN_CENTER, 5 );
	
	
	sbSizerPreview->Add( bSizerUnicode, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_gridPreview = new wxGrid( sbSizerPreview->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSTATIC_BORDER );
	
	// Grid
	m_gridPreview->CreateGrid( 1, 1 );
	m_gridPreview->EnableEditing( false );
	m_gridPreview->EnableGridLines( false );
	m_gridPreview->EnableDragGridSize( false );
	m_gridPreview->SetMargins( 0, 0 );
	
	// Columns
	m_gridPreview->SetColSize( 0, 200 );
	m_gridPreview->EnableDragColMove( false );
	m_gridPreview->EnableDragColSize( false );
	m_gridPreview->SetColLabelSize( 0 );
	m_gridPreview->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_gridPreview->SetRowSize( 0, 200 );
	m_gridPreview->EnableDragRowSize( false );
	m_gridPreview->SetRowLabelSize( 0 );
	m_gridPreview->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	m_gridPreview->SetDefaultCellFont( wxFont( 96, 70, 90, 90, false, wxT("00 ZRCola") ) );
	m_gridPreview->SetDefaultCellAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	m_gridPreview->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	sbSizerPreview->Add( m_gridPreview, 0, wxALL|wxEXPAND, 5 );
	
	m_description = new wxTextCtrl( sbSizerPreview->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_MULTILINE|wxTE_READONLY );
	sbSizerPreview->Add( m_description, 1, wxALL|wxEXPAND, 5 );
	
	m_category = new wxTextCtrl( sbSizerPreview->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY );
	sbSizerPreview->Add( m_category, 0, wxALL|wxEXPAND, 5 );
	
	
	bSizerRight->Add( sbSizerPreview, 70, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizerRelated;
	sbSizerRelated = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Re&lated") ), wxVERTICAL );
	
	m_gridRelated = new wxZRColaCharGrid( sbSizerRelated->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxSTATIC_BORDER );
	
	// Grid
	m_gridRelated->CreateGrid( 0, 0 );
	m_gridRelated->EnableEditing( false );
	m_gridRelated->EnableGridLines( false );
	m_gridRelated->EnableDragGridSize( false );
	m_gridRelated->SetMargins( 0, 0 );
	
	// Columns
	m_gridRelated->EnableDragColMove( false );
	m_gridRelated->EnableDragColSize( false );
	m_gridRelated->SetColLabelSize( 0 );
	m_gridRelated->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_gridRelated->EnableDragRowSize( false );
	m_gridRelated->SetRowLabelSize( 0 );
	m_gridRelated->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	m_gridRelated->SetDefaultCellFont( wxFont( 20, 70, 90, 90, false, wxT("00 ZRCola") ) );
	m_gridRelated->SetDefaultCellAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	sbSizerRelated->Add( m_gridRelated, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizerRight->Add( sbSizerRelated, 30, wxALL|wxEXPAND, 5 );
	
	
	bSizerColumns->Add( bSizerRight, 0, wxEXPAND, 5 );
	
	
	bSizerContent->Add( bSizerColumns, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizerContent->Add( 5, 5, 0, wxALL|wxEXPAND, 5 );
	
	m_sdbSizerButtons = new wxStdDialogButtonSizer();
	m_sdbSizerButtonsOK = new wxButton( this, wxID_OK );
	m_sdbSizerButtons->AddButton( m_sdbSizerButtonsOK );
	m_sdbSizerButtonsCancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizerButtons->AddButton( m_sdbSizerButtonsCancel );
	m_sdbSizerButtons->Realize();
	
	bSizerContent->Add( m_sdbSizerButtons, 0, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( bSizerContent );
	this->Layout();
	bSizerContent->Fit( this );
	
	// Connect Events
	this->Connect( wxEVT_IDLE, wxIdleEventHandler( wxZRColaCharSelectBase::OnIdle ) );
	m_search->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaCharSelectBase::OnSearchText ), NULL, this );
	m_categories->Connect( wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, wxCommandEventHandler( wxZRColaCharSelectBase::OnCategoriesToggle ), NULL, this );
	m_gridResults->Connect( wxEVT_GRID_CELL_LEFT_DCLICK, wxGridEventHandler( wxZRColaCharSelectBase::OnResultCellDClick ), NULL, this );
	m_gridResults->Connect( wxEVT_GRID_SELECT_CELL, wxGridEventHandler( wxZRColaCharSelectBase::OnResultSelectCell ), NULL, this );
	m_gridResults->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( wxZRColaCharSelectBase::OnResultsKeyDown ), NULL, this );
	m_gridRecent->Connect( wxEVT_GRID_CELL_LEFT_DCLICK, wxGridEventHandler( wxZRColaCharSelectBase::OnRecentCellDClick ), NULL, this );
	m_gridRecent->Connect( wxEVT_GRID_SELECT_CELL, wxGridEventHandler( wxZRColaCharSelectBase::OnRecentSelectCell ), NULL, this );
	m_gridRecent->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( wxZRColaCharSelectBase::OnRecentKeyDown ), NULL, this );
	m_unicode->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaCharSelectBase::OnUnicodeText ), NULL, this );
	m_gridRelated->Connect( wxEVT_GRID_SELECT_CELL, wxGridEventHandler( wxZRColaCharSelectBase::OnRelatedSelectCell ), NULL, this );
	m_sdbSizerButtonsOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxZRColaCharSelectBase::OnOKButtonClick ), NULL, this );
}

wxZRColaCharSelectBase::~wxZRColaCharSelectBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( wxZRColaCharSelectBase::OnIdle ) );
	m_search->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaCharSelectBase::OnSearchText ), NULL, this );
	m_categories->Disconnect( wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, wxCommandEventHandler( wxZRColaCharSelectBase::OnCategoriesToggle ), NULL, this );
	m_gridResults->Disconnect( wxEVT_GRID_CELL_LEFT_DCLICK, wxGridEventHandler( wxZRColaCharSelectBase::OnResultCellDClick ), NULL, this );
	m_gridResults->Disconnect( wxEVT_GRID_SELECT_CELL, wxGridEventHandler( wxZRColaCharSelectBase::OnResultSelectCell ), NULL, this );
	m_gridResults->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( wxZRColaCharSelectBase::OnResultsKeyDown ), NULL, this );
	m_gridRecent->Disconnect( wxEVT_GRID_CELL_LEFT_DCLICK, wxGridEventHandler( wxZRColaCharSelectBase::OnRecentCellDClick ), NULL, this );
	m_gridRecent->Disconnect( wxEVT_GRID_SELECT_CELL, wxGridEventHandler( wxZRColaCharSelectBase::OnRecentSelectCell ), NULL, this );
	m_gridRecent->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( wxZRColaCharSelectBase::OnRecentKeyDown ), NULL, this );
	m_unicode->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaCharSelectBase::OnUnicodeText ), NULL, this );
	m_gridRelated->Disconnect( wxEVT_GRID_SELECT_CELL, wxGridEventHandler( wxZRColaCharSelectBase::OnRelatedSelectCell ), NULL, this );
	m_sdbSizerButtonsOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxZRColaCharSelectBase::OnOKButtonClick ), NULL, this );
	
}

wxZRColaSettingsBase::wxZRColaSettingsBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxDialog( parent, id, title, pos, size, style, name )
{
	this->SetSizeHints( wxDefaultSize, wxSize( -1,-1 ) );
	
	wxBoxSizer* bSizerContent;
	bSizerContent = new wxBoxSizer( wxVERTICAL );
	
	m_listbook = new wxListbook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLB_DEFAULT );
	m_panelLanguage = new wxPanel( m_listbook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerLanguage;
	bSizerLanguage = new wxBoxSizer( wxVERTICAL );
	
	m_langLabel = new wxStaticText( m_panelLanguage, wxID_ANY, _("Some character native to specific language you are working with should not decompose to primitives.\nFor optimal decomposition you should set the language correctly."), wxDefaultPosition, wxDefaultSize, 0 );
	m_langLabel->Wrap( -1 );
	bSizerLanguage->Add( m_langLabel, 0, wxALL|wxEXPAND, 5 );
	
	m_langAuto = new wxRadioButton( m_panelLanguage, wxID_ANY, _("Select language &automatically according to selected keyboard"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	bSizerLanguage->Add( m_langAuto, 0, wxALL|wxEXPAND, 5 );
	
	m_langManual = new wxRadioButton( m_panelLanguage, wxID_ANY, _("&Manually select the language from the list below:"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerLanguage->Add( m_langManual, 0, wxALL|wxEXPAND, 5 );
	
	m_languages = new wxListBox( m_panelLanguage, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	m_languages->SetMinSize( wxSize( -1,150 ) );
	
	bSizerLanguage->Add( m_languages, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panelLanguage->SetSizer( bSizerLanguage );
	m_panelLanguage->Layout();
	bSizerLanguage->Fit( m_panelLanguage );
	m_listbook->AddPage( m_panelLanguage, _("Text Language"), true );
	m_panelAutoStart = new wxPanel( m_listbook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerAutoStart;
	bSizerAutoStart = new wxBoxSizer( wxVERTICAL );
	
	m_autoStartLabel = new wxStaticText( m_panelAutoStart, wxID_ANY, _("ZRCola can be launched every time you log in to your computer.\nIt will be available on the system tray and via registered shortcuts Win+F5 and Win+F6."), wxDefaultPosition, wxDefaultSize, 0 );
	m_autoStartLabel->Wrap( -1 );
	bSizerAutoStart->Add( m_autoStartLabel, 0, wxALL|wxEXPAND, 5 );
	
	m_autoStart = new wxCheckBox( m_panelAutoStart, wxID_ANY, _("Start ZRCola &automatically on logon"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerAutoStart->Add( m_autoStart, 0, wxALL|wxEXPAND, 5 );
	
	
	m_panelAutoStart->SetSizer( bSizerAutoStart );
	m_panelAutoStart->Layout();
	bSizerAutoStart->Fit( m_panelAutoStart );
	m_listbook->AddPage( m_panelAutoStart, _("Startup"), false );
	#ifdef __WXGTK__ // Small icon style not supported in GTK
	wxListView* m_listbookListView = m_listbook->GetListView();
	long m_listbookFlags = m_listbookListView->GetWindowStyleFlag();
	if( m_listbookFlags & wxLC_SMALL_ICON )
	{
		m_listbookFlags = ( m_listbookFlags & ~wxLC_SMALL_ICON ) | wxLC_ICON;
	}
	m_listbookListView->SetWindowStyleFlag( m_listbookFlags );
	#endif
	
	bSizerContent->Add( m_listbook, 1, wxEXPAND | wxALL, 5 );
	
	
	bSizerContent->Add( 0, 0, 0, wxALL|wxEXPAND, 5 );
	
	m_sdbSizerButtons = new wxStdDialogButtonSizer();
	m_sdbSizerButtonsOK = new wxButton( this, wxID_OK );
	m_sdbSizerButtons->AddButton( m_sdbSizerButtonsOK );
	m_sdbSizerButtonsApply = new wxButton( this, wxID_APPLY );
	m_sdbSizerButtons->AddButton( m_sdbSizerButtonsApply );
	m_sdbSizerButtonsCancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizerButtons->AddButton( m_sdbSizerButtonsCancel );
	m_sdbSizerButtons->Realize();
	
	bSizerContent->Add( m_sdbSizerButtons, 0, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( bSizerContent );
	this->Layout();
	bSizerContent->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( wxZRColaSettingsBase::OnInitDialog ) );
	m_langAuto->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( wxZRColaSettingsBase::OnLangAuto ), NULL, this );
	m_langManual->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( wxZRColaSettingsBase::OnLangManual ), NULL, this );
	m_sdbSizerButtonsApply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxZRColaSettingsBase::OnApplyButtonClick ), NULL, this );
	m_sdbSizerButtonsOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxZRColaSettingsBase::OnOKButtonClick ), NULL, this );
}

wxZRColaSettingsBase::~wxZRColaSettingsBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( wxZRColaSettingsBase::OnInitDialog ) );
	m_langAuto->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( wxZRColaSettingsBase::OnLangAuto ), NULL, this );
	m_langManual->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( wxZRColaSettingsBase::OnLangManual ), NULL, this );
	m_sdbSizerButtonsApply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxZRColaSettingsBase::OnApplyButtonClick ), NULL, this );
	m_sdbSizerButtonsOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxZRColaSettingsBase::OnOKButtonClick ), NULL, this );
	
}

wxZRColaAboutBase::wxZRColaAboutBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxDialog( parent, id, title, pos, size, style, name )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizerContent;
	bSizerContent = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizerColumns;
	bSizerColumns = new wxBoxSizer( wxHORIZONTAL );
	
	m_bitmapIcon = new wxStaticBitmap( this, wxID_ANY, wxIcon( wxT("00_zrcola.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 32, 32 ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerColumns->Add( m_bitmapIcon, 0, wxALL, 5 );
	
	wxBoxSizer* bSizerText;
	bSizerText = new wxBoxSizer( wxVERTICAL );
	
	m_staticTextZRCola = new wxStaticText( this, wxID_ANY, _("ZRCola"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextZRCola->Wrap( -1 );
	m_staticTextZRCola->SetFont( wxFont( 20, 70, 90, 90, false, wxT("00 ZRCola") ) );
	
	bSizerText->Add( m_staticTextZRCola, 0, wxALL|wxEXPAND, 5 );
	
	m_staticTextVersion = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextVersion->Wrap( -1 );
	bSizerText->Add( m_staticTextVersion, 0, wxALL|wxEXPAND, 5 );
	
	m_staticTextColophone = new wxStaticText( this, wxID_ANY, _("Editor: Janoš Ježovnik\nDevelopment: Amebis d. o. o.\nDevelopment and maintenance (2004-2015): dr. Peter Weiss"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextColophone->Wrap( -1 );
	bSizerText->Add( m_staticTextColophone, 0, wxALL|wxEXPAND, 5 );
	
	m_staticTextCopyright = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextCopyright->Wrap( -1 );
	bSizerText->Add( m_staticTextCopyright, 0, wxALL|wxEXPAND, 5 );
	
	m_hyperlink = new wxHyperlinkCtrl( this, wxID_ANY, _("www.zrc-sazu.si"), wxT("http://www.zrc-sazu.si/"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	bSizerText->Add( m_hyperlink, 0, wxALL|wxEXPAND, 5 );
	
	
	bSizerColumns->Add( bSizerText, 1, wxEXPAND, 5 );
	
	
	bSizerContent->Add( bSizerColumns, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizerContent->Add( 0, 0, 0, wxALL|wxEXPAND, 5 );
	
	m_sdbSizerButtons = new wxStdDialogButtonSizer();
	m_sdbSizerButtonsOK = new wxButton( this, wxID_OK );
	m_sdbSizerButtons->AddButton( m_sdbSizerButtonsOK );
	m_sdbSizerButtons->Realize();
	
	bSizerContent->Add( m_sdbSizerButtons, 0, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( bSizerContent );
	this->Layout();
	bSizerContent->Fit( this );
	
	this->Centre( wxBOTH );
}

wxZRColaAboutBase::~wxZRColaAboutBase()
{
}
