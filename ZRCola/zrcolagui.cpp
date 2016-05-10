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
	wxMenuItem* m_menuItemAutoStart;
	m_menuItemAutoStart = new wxMenuItem( m_menuProgram, wxID_AUTOSTART, wxString( _("&Start on Logon") ) , _("Start this program automatically on logon"), wxITEM_CHECK );
	m_menuProgram->Append( m_menuItemAutoStart );
	
	m_menuProgram->AppendSeparator();
	
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
	wxMenuItem* m_menuDecompLanguageAuto;
	m_menuDecompLanguageAuto = new wxMenuItem( m_menuDecompLanguage, wxID_DECOMP_LANG_AUTO, wxString( _("&Automatic") ) , _("Set language according to keyboard layout automatically"), wxITEM_CHECK );
	m_menuDecompLanguage->Append( m_menuDecompLanguageAuto );
	
	m_menuDecompLanguage->AppendSeparator();
	
	m_menuEdit->Append( m_menuDecompLanguageItem );
	
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
	wxMenuItem* m_menuItemAbout;
	m_menuItemAbout = new wxMenuItem( m_menuHelp, wxID_ABOUT, wxString( wxEmptyString ) , wxEmptyString, wxITEM_NORMAL );
	m_menuHelp->Append( m_menuItemAbout );
	
	m_menubar->Append( m_menuHelp, _("&Help") ); 
	
	this->SetMenuBar( m_menubar );
	
	m_toolbarEdit = new wxAuiToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_HORZ_LAYOUT ); 
	m_toolEditCut = m_toolbarEdit->AddTool( wxID_CUT, _("Cut"), wxIcon( wxT("edit_cut.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 24, 24 ), wxNullBitmap, wxITEM_NORMAL, _("Cut"), _("Cut selection"), NULL ); 
	
	m_toolEditCopy = m_toolbarEdit->AddTool( wxID_COPY, _("Copy"), wxIcon( wxT("edit_copy.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 24, 24 ), wxNullBitmap, wxITEM_NORMAL, _("Copy"), _("Copy selection"), NULL ); 
	
	m_toolEditPaste = m_toolbarEdit->AddTool( wxID_PASTE, _("Paste"), wxIcon( wxT("edit_paste.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 24, 24 ), wxNullBitmap, wxITEM_NORMAL, _("Paste"), _("Paste selection"), NULL ); 
	
	m_toolbarEdit->Realize();
	m_mgr.AddPane( m_toolbarEdit, wxAuiPaneInfo().Name( wxT("toolbarEdit") ).Top().Caption( _("Edit") ).PinButton( true ).Dock().Resizable().FloatingSize( wxSize( -1,-1 ) ).LeftDockable( false ).RightDockable( false ).Row( 0 ).Layer( 1 ).ToolbarPane() );
	
	m_toolbarCompose = new wxAuiToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_HORZ_LAYOUT ); 
	m_toolSendComposed = m_toolbarCompose->AddTool( wxID_SEND_COMPOSED, _("Send Composed"), wxIcon( wxT("send_composed.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 24, 24 ), wxNullBitmap, wxITEM_NORMAL, _("Send Composed"), _("Send composed text to source window"), NULL ); 
	
	m_toolSendDecomposed = m_toolbarCompose->AddTool( wxID_SEND_DECOMPOSED, _("Send Decomposed"), wxIcon( wxT("send_decomposed.ico"), wxBITMAP_TYPE_ICO_RESOURCE, 24, 24 ), wxNullBitmap, wxITEM_NORMAL, _("Send Decomposed"), _("Send decomposed text to source window"), NULL ); 
	
	m_toolbarCompose->AddSeparator(); 
	
	wxArrayString m_toolDecompLanguageChoices;
	m_toolDecompLanguage = new wxChoice( m_toolbarCompose, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_toolDecompLanguageChoices, 0 );
	m_toolDecompLanguage->SetSelection( 0 );
	m_toolbarCompose->AddControl( m_toolDecompLanguage );
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
	m_toolDecompLanguage->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( wxZRColaFrameBase::OnDecompLanguageChoice ), NULL, this );
}

wxZRColaFrameBase::~wxZRColaFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_ICONIZE, wxIconizeEventHandler( wxZRColaFrameBase::OnIconize ) );
	this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( wxZRColaFrameBase::OnIdle ) );
	m_toolDecompLanguage->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( wxZRColaFrameBase::OnDecompLanguageChoice ), NULL, this );
	
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
	
	// Connect Events
	m_decomposed->Connect( wxEVT_PAINT, wxPaintEventHandler( wxZRColaComposerPanelBase::OnDecomposedPaint ), NULL, this );
	m_decomposed->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaComposerPanelBase::OnDecomposedText ), NULL, this );
	m_decomposedHex->Connect( wxEVT_PAINT, wxPaintEventHandler( wxZRColaComposerPanelBase::OnDecomposedHexPaint ), NULL, this );
	m_composed->Connect( wxEVT_PAINT, wxPaintEventHandler( wxZRColaComposerPanelBase::OnComposedPaint ), NULL, this );
	m_composed->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaComposerPanelBase::OnComposedText ), NULL, this );
	m_composedHex->Connect( wxEVT_PAINT, wxPaintEventHandler( wxZRColaComposerPanelBase::OnComposedHexPaint ), NULL, this );
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
