///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "pch.h"

#include "zrcolachrgrid.h"

#include "zrcolagui.h"

// Using the construction of a static object to ensure that the help provider is set
// wx Manages the most recent HelpProvider passed to ::Set, but not the previous ones
// If ::Set gets called more than once, the previous one is returned and should be deleted
class wxFBContextSensitiveHelpSetter
{
public:
wxFBContextSensitiveHelpSetter()
{
wxSimpleHelpProvider* help = new wxSimpleHelpProvider();
wxHelpProvider* old = wxHelpProvider::Set( help );
if (old != 0){
delete old;
}
}
};

static wxFBContextSensitiveHelpSetter s_wxFBSetTheHelpProvider;
///////////////////////////////////////////////////////////////////////////

wxZRColaFrameBase::wxZRColaFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, long style, const wxString& name ) : wxFrame( parent, id, title, pos, parent->FromDIP(wxSize(600, 400)), style, name )
{
	this->SetSizeHints( FromDIP(wxSize( 150,150 )), wxDefaultSize );
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
	m_menuItemEditCut->SetBitmaps( wxIcon( wxT("edit_cut.ico"), wxBITMAP_TYPE_ICO_RESOURCE, FromDIP(16), FromDIP(16) ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menuItemEditCut->SetBitmap( wxIcon( wxT("edit_cut.ico"), wxBITMAP_TYPE_ICO_RESOURCE, FromDIP(16), FromDIP(16) ) );
	#endif
	m_menuEdit->Append( m_menuItemEditCut );

	wxMenuItem* m_menuItemEditCopy;
	m_menuItemEditCopy = new wxMenuItem( m_menuEdit, wxID_COPY, wxString( wxEmptyString ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menuItemEditCopy->SetBitmaps( wxIcon( wxT("edit_copy.ico"), wxBITMAP_TYPE_ICO_RESOURCE, FromDIP(16), FromDIP(16) ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menuItemEditCopy->SetBitmap( wxIcon( wxT("edit_copy.ico"), wxBITMAP_TYPE_ICO_RESOURCE, FromDIP(16), FromDIP(16) ) );
	#endif
	m_menuEdit->Append( m_menuItemEditCopy );

	wxMenuItem* m_menuItemEditPaste;
	m_menuItemEditPaste = new wxMenuItem( m_menuEdit, wxID_PASTE, wxString( wxEmptyString ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menuItemEditPaste->SetBitmaps( wxIcon( wxT("edit_paste.ico"), wxBITMAP_TYPE_ICO_RESOURCE, FromDIP(16), FromDIP(16) ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menuItemEditPaste->SetBitmap( wxIcon( wxT("edit_paste.ico"), wxBITMAP_TYPE_ICO_RESOURCE, FromDIP(16), FromDIP(16) ) );
	#endif
	m_menuEdit->Append( m_menuItemEditPaste );

	m_menuEdit->AppendSeparator();

	wxMenuItem* m_menuItemSelectAll;
	m_menuItemSelectAll = new wxMenuItem( m_menuEdit, wxID_SELECTALL, wxString( _("Select &All") ) + wxT('\t') + wxT("Ctrl+A"), _("Select all text"), wxITEM_NORMAL );
	m_menuEdit->Append( m_menuItemSelectAll );

	m_menuEdit->AppendSeparator();

	wxMenuItem* m_menuItemCharSelect;
	m_menuItemCharSelect = new wxMenuItem( m_menuEdit, wxID_CHARACTER_SELECTOR, wxString( _("Find C&haracter...") ) + wxT('\t') + wxT("F8"), _("Toggle character search to select character to insert into text"), wxITEM_CHECK );
	m_menuEdit->Append( m_menuItemCharSelect );

	m_menuEdit->AppendSeparator();

	wxMenuItem* m_menuItemSendDestination;
	m_menuItemSendDestination = new wxMenuItem( m_menuEdit, wxID_SEND_DESTINATION, wxString( _("&Send Composed") ) + wxT('\t') + wxT("F5"), _("Send composed text to source window"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menuItemSendDestination->SetBitmaps( wxIcon( wxT("send_destination.ico"), wxBITMAP_TYPE_ICO_RESOURCE, FromDIP(16), FromDIP(16) ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menuItemSendDestination->SetBitmap( wxIcon( wxT("send_destination.ico"), wxBITMAP_TYPE_ICO_RESOURCE, FromDIP(16), FromDIP(16) ) );
	#endif
	m_menuEdit->Append( m_menuItemSendDestination );

	wxMenuItem* m_menuItemCopyDestinationAndReturn;
	m_menuItemCopyDestinationAndReturn = new wxMenuItem( m_menuEdit, wxID_COPY_DESTINATION_AND_RETURN, wxString( _("Copy Composed and &Return") ) + wxT('\t') + wxT("Ctrl+F5"), _("Copy composed text to clipboard and return focus to source window"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menuItemCopyDestinationAndReturn->SetBitmaps( wxIcon( wxT("copy_destination_and_return.ico"), wxBITMAP_TYPE_ICO_RESOURCE, FromDIP(16), FromDIP(16) ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menuItemCopyDestinationAndReturn->SetBitmap( wxIcon( wxT("copy_destination_and_return.ico"), wxBITMAP_TYPE_ICO_RESOURCE, FromDIP(16), FromDIP(16) ) );
	#endif
	m_menuEdit->Append( m_menuItemCopyDestinationAndReturn );

	wxMenuItem* m_menuItemSendSource;
	m_menuItemSendSource = new wxMenuItem( m_menuEdit, wxID_SEND_SOURCE, wxString( _("Send &Decomposed") ) + wxT('\t') + wxT("F6"), _("Send decomposed text to source window"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menuItemSendSource->SetBitmaps( wxIcon( wxT("send_source.ico"), wxBITMAP_TYPE_ICO_RESOURCE, FromDIP(16), FromDIP(16) ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menuItemSendSource->SetBitmap( wxIcon( wxT("send_source.ico"), wxBITMAP_TYPE_ICO_RESOURCE, FromDIP(16), FromDIP(16) ) );
	#endif
	m_menuEdit->Append( m_menuItemSendSource );

	wxMenuItem* m_menuItemCopySourceAndReturn;
	m_menuItemCopySourceAndReturn = new wxMenuItem( m_menuEdit, wxID_COPY_SOURCE_AND_RETURN, wxString( _("Copy Decomposed and Re&turn") ) + wxT('\t') + wxT("Ctrl+F6"), _("Copy decomposed text to clipboard and return focus to source window"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menuItemCopySourceAndReturn->SetBitmaps( wxIcon( wxT("copy_source_and_return.ico"), wxBITMAP_TYPE_ICO_RESOURCE, FromDIP(16), FromDIP(16) ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menuItemCopySourceAndReturn->SetBitmap( wxIcon( wxT("copy_source_and_return.ico"), wxBITMAP_TYPE_ICO_RESOURCE, FromDIP(16), FromDIP(16) ) );
	#endif
	m_menuEdit->Append( m_menuItemCopySourceAndReturn );

	wxMenuItem* m_menuItemSendAbort;
	m_menuItemSendAbort = new wxMenuItem( m_menuEdit, wxID_SEND_ABORT, wxString( _("Abort (De)composition") ) + wxT('\t') + wxT("Esc"), _("Abort composition and return focus to source window"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menuItemSendAbort->SetBitmaps( wxIcon( wxT("send_abort.ico"), wxBITMAP_TYPE_ICO_RESOURCE, FromDIP(16), FromDIP(16) ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menuItemSendAbort->SetBitmap( wxIcon( wxT("send_abort.ico"), wxBITMAP_TYPE_ICO_RESOURCE, FromDIP(16), FromDIP(16) ) );
	#endif
	m_menuEdit->Append( m_menuItemSendAbort );

	m_menuEdit->AppendSeparator();

	m_menuTranslationSeq = new wxMenu();
	wxMenuItem* m_menuTranslationSeqItem = new wxMenuItem( m_menuEdit, wxID_ANY, _("Tra&nslation"), wxEmptyString, wxITEM_NORMAL, m_menuTranslationSeq );
	m_menuEdit->Append( m_menuTranslationSeqItem );

	m_menuItemComposition = new wxMenuItem( m_menuEdit, wxID_COMPOSITION_MENU, wxString( _("&ZRCola (De)composition") ) , _("Toggle ZRCola character (De)composition"), wxITEM_CHECK );
	m_menuEdit->Append( m_menuItemComposition );

	m_menuItemWarnPUA = new wxMenuItem( m_menuEdit, wxID_WARN_PUA_MENU, wxString( _("&PUA Warning") ) , _("Toggle PUA warning"), wxITEM_CHECK );
	m_menuEdit->Append( m_menuItemWarnPUA );

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
	m_menuItemToolbarCompose = new wxMenuItem( m_menuView, wxID_TOOLBAR_TRANSLATE, wxString( _("&Compose Toolbar") ) , _("Toggle compose toolbar"), wxITEM_CHECK );
	m_menuView->Append( m_menuItemToolbarCompose );

	m_menuView->AppendSeparator();

	wxMenuItem* m_menuItemPanelChrGrps;
	m_menuItemPanelChrGrps = new wxMenuItem( m_menuView, wxID_PANEL_CHRGRPS, wxString( _("Character Catalo&g") ) , _("Toggle character catalog panel"), wxITEM_CHECK );
	m_menuView->Append( m_menuItemPanelChrGrps );

	m_menubar->Append( m_menuView, _("&View") );

	m_menuHelp = new wxMenu();
	wxMenuItem* m_menuHelpInstructions;
	m_menuHelpInstructions = new wxMenuItem( m_menuHelp, wxID_HELP_INSTRUCTIONS, wxString( _("&Instructions...") ) + wxT('\t') + wxT("F1"), _("Open a web site with product instructions"), wxITEM_NORMAL );
	m_menuHelp->Append( m_menuHelpInstructions );

	wxMenuItem* m_menuHelpShortcuts;
	m_menuHelpShortcuts = new wxMenuItem( m_menuHelp, wxID_HELP_SHORTCUTS, wxString( _("Keyboard &Shortcuts...") ) , _("Open a list of keyboard shortcuts"), wxITEM_NORMAL );
	m_menuHelp->Append( m_menuHelpShortcuts );

	m_menuHelp->AppendSeparator();

	wxMenuItem* m_menuHelpReqChar;
	m_menuHelpReqChar = new wxMenuItem( m_menuHelp, wxID_HELP_REQCHAR, wxString( _("&Request a new character...") ) , _("Submit a request to ZRC to add a new character"), wxITEM_NORMAL );
	m_menuHelp->Append( m_menuHelpReqChar );

	wxMenuItem* m_menuHelpUpdate;
	m_menuHelpUpdate = new wxMenuItem( m_menuHelp, wxID_HELP_UPDATE, wxString( _("Check for &Updates...") ) , _("Check online for product update"), wxITEM_NORMAL );
	m_menuHelp->Append( m_menuHelpUpdate );

	m_menuHelp->AppendSeparator();

	wxMenuItem* m_menuHelpAbout;
	m_menuHelpAbout = new wxMenuItem( m_menuHelp, wxID_ABOUT, wxString( _("&About...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuHelp->Append( m_menuHelpAbout );

	m_menubar->Append( m_menuHelp, _("&Help") );

	this->SetMenuBar( m_menubar );

	m_toolbarEdit = new wxAuiToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_HORZ_LAYOUT );
	m_toolEditCut = m_toolbarEdit->AddTool( wxID_CUT, _("Cut"), wxIcon( wxT("edit_cut.ico"), wxBITMAP_TYPE_ICO_RESOURCE, FromDIP(24), FromDIP(24) ), wxNullBitmap, wxITEM_NORMAL, _("Cut"), _("Cut selection"), NULL );

	m_toolEditCopy = m_toolbarEdit->AddTool( wxID_COPY, _("Copy"), wxIcon( wxT("edit_copy.ico"), wxBITMAP_TYPE_ICO_RESOURCE, FromDIP(24), FromDIP(24) ), wxNullBitmap, wxITEM_NORMAL, _("Copy"), _("Copy selection"), NULL );

	m_toolEditPaste = m_toolbarEdit->AddTool( wxID_PASTE, _("Paste"), wxIcon( wxT("edit_paste.ico"), wxBITMAP_TYPE_ICO_RESOURCE, FromDIP(24), FromDIP(24) ), wxNullBitmap, wxITEM_NORMAL, _("Paste"), _("Paste selection"), NULL );

	m_toolbarEdit->Realize();
	m_mgr.AddPane( m_toolbarEdit, wxAuiPaneInfo().Name( wxT("toolbarEdit") ).Top().Caption( _("Edit") ).PinButton( true ).Dock().Resizable().FloatingSize( wxSize( -1,-1 ) ).LeftDockable( false ).RightDockable( false ).Row( 0 ).Layer( 1 ).ToolbarPane() );

	m_toolbarTranslate = new wxAuiToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_HORZ_LAYOUT );
	m_toolCharSelect = m_toolbarTranslate->AddTool( wxID_CHARACTER_SELECTOR, _("Find Character"), wxIcon( wxT("char_select.ico"), wxBITMAP_TYPE_ICO_RESOURCE, FromDIP(24), FromDIP(24) ), wxNullBitmap, wxITEM_CHECK, _("Find Character"), _("Toggle character search to select character to insert into text"), NULL );

	m_toolSendDestination = m_toolbarTranslate->AddTool( wxID_SEND_DESTINATION, _("Send Composed"), wxIcon( wxT("send_destination.ico"), wxBITMAP_TYPE_ICO_RESOURCE, FromDIP(24), FromDIP(24) ), wxNullBitmap, wxITEM_NORMAL, _("Send Composed"), _("Send composed text to source window"), NULL );

	m_toolSendSource = m_toolbarTranslate->AddTool( wxID_SEND_SOURCE, _("Send Decomposed"), wxIcon( wxT("send_source.ico"), wxBITMAP_TYPE_ICO_RESOURCE, FromDIP(24), FromDIP(24) ), wxNullBitmap, wxITEM_NORMAL, _("Send Decomposed"), _("Send decomposed text to source window"), NULL );

	m_toolbarTranslate->AddSeparator();

	wxArrayString m_toolTranslationSeqChoices;
	m_toolTranslationSeq = new wxChoice( m_toolbarTranslate, wxID_ANY, wxDefaultPosition, FromDIP(wxSize( 240,-1 )), m_toolTranslationSeqChoices, 0 );
	m_toolTranslationSeq->SetSelection( 0 );
	m_toolbarTranslate->AddControl( m_toolTranslationSeq );
	m_toolComposition = m_toolbarTranslate->AddTool( wxID_COMPOSITION_TOOLBAR, _("ZRCola (De)composition"), wxIcon( wxT("composition.ico"), wxBITMAP_TYPE_ICO_RESOURCE, FromDIP(24), FromDIP(24) ), wxNullBitmap, wxITEM_CHECK, _("ZRCola (De)composition"), _("Toggle ZRCola character (De)composition"), NULL );

	m_toolWarnPUA = m_toolbarTranslate->AddTool( wxID_WARN_PUA_TOOLBAR, _("PUA Warning"), wxIcon( wxT("warn_pua.ico"), wxBITMAP_TYPE_ICO_RESOURCE, FromDIP(24), FromDIP(24) ), wxNullBitmap, wxITEM_CHECK, _("Highlight PUA Characters"), _("Toggle PUA warning"), NULL );

	m_toolbarTranslate->Realize();
	m_mgr.AddPane( m_toolbarTranslate, wxAuiPaneInfo().Name( wxT("toolbarCompose") ).Top().Caption( _("Compose") ).PinButton( true ).Dock().Resizable().FloatingSize( wxSize( -1,-1 ) ).LeftDockable( false ).RightDockable( false ).Row( 0 ).Layer( 1 ).ToolbarPane() );

	m_panelChrCat = new wxZRColaCharacterCatalogPanel( this );

	m_mgr.AddPane( m_panelChrCat, wxAuiPaneInfo() .Name( wxT("panelChrGrp") ).Left() .Caption( _("Character Catalog") ).PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ).Row( 1 ).BestSize(FromDIP(wxSize( 150,200 )) ).MinSize( FromDIP(wxSize( 100,100 )) ).Layer( 1 ) );

	m_panel = new wxZRColaComposerPanel( this );

	m_mgr.AddPane( m_panel, wxAuiPaneInfo() .Name( wxT("composerPanel") ).Center() .Caption( _("(De)Composer") ).CaptionVisible( false ).CloseButton( false ).PaneBorder( false ).Dock().Resizable().FloatingSize( wxDefaultSize ).Floatable( false ) );

	m_statusBar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );

	m_mgr.Update();
	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( wxZRColaFrameBase::OnClose ) );
	this->Connect( wxEVT_ICONIZE, wxIconizeEventHandler( wxZRColaFrameBase::OnIconize ) );
	this->Connect( wxEVT_IDLE, wxIdleEventHandler( wxZRColaFrameBase::OnIdle ) );
	m_toolTranslationSeq->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( wxZRColaFrameBase::OnTranslationSeqChoice ), NULL, this );
}

wxZRColaFrameBase::~wxZRColaFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( wxZRColaFrameBase::OnClose ) );
	this->Disconnect( wxEVT_ICONIZE, wxIconizeEventHandler( wxZRColaFrameBase::OnIconize ) );
	this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( wxZRColaFrameBase::OnIdle ) );
	m_toolTranslationSeq->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( wxZRColaFrameBase::OnTranslationSeqChoice ), NULL, this );

	m_mgr.UnInit();

}

wxZRColaComposerPanelBase::wxZRColaComposerPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizerMain;
	bSizerMain = new wxBoxSizer( wxVERTICAL );

	m_splitterSource = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D|wxSP_LIVE_UPDATE );
	m_splitterSource->SetSashGravity( 1 );
	m_splitterSource->Connect( wxEVT_IDLE, wxIdleEventHandler( wxZRColaComposerPanelBase::m_splitterSourceOnIdle ), NULL, this );
	m_splitterSource->SetMinimumPaneSize( FromDIP(5) );

	m_panelSourceEdit = new wxPanel( m_splitterSource, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerSourceEdit;
	bSizerSourceEdit = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* bSizerSourceEdit2;
	bSizerSourceEdit2 = new wxStaticBoxSizer( new wxStaticBox( m_panelSourceEdit, wxID_ANY, _("Decomposed Text") ), wxVERTICAL );

	m_source = new wxTextCtrl( bSizerSourceEdit2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_RICH );
	m_source->SetFont( wxFont( 20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("ZRCola") ) );
	m_source->SetMinSize( FromDIP(wxSize( 100,25 )) );

	bSizerSourceEdit2->Add( m_source, 1, wxEXPAND, FromDIP(5) );


	bSizerSourceEdit->Add( bSizerSourceEdit2, 1, wxEXPAND, FromDIP(5) );


	m_panelSourceEdit->SetSizer( bSizerSourceEdit );
	m_panelSourceEdit->Layout();
	bSizerSourceEdit->Fit( m_panelSourceEdit );
	m_panelSourceHex = new wxPanel( m_splitterSource, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerSourceHex;
	bSizerSourceHex = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* bSizerSourceHex2;
	bSizerSourceHex2 = new wxStaticBoxSizer( new wxStaticBox( m_panelSourceHex, wxID_ANY, _("Decomposed Unicode Dump") ), wxVERTICAL );

	m_sourceHex = new wxTextCtrl( bSizerSourceHex2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	m_sourceHex->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizerSourceHex2->Add( m_sourceHex, 1, wxEXPAND, FromDIP(5) );


	bSizerSourceHex->Add( bSizerSourceHex2, 1, wxEXPAND, FromDIP(5) );


	m_panelSourceHex->SetSizer( bSizerSourceHex );
	m_panelSourceHex->Layout();
	bSizerSourceHex->Fit( m_panelSourceHex );
	m_splitterSource->SplitVertically( m_panelSourceEdit, m_panelSourceHex, FromDIP(-5) );
	bSizerMain->Add( m_splitterSource, 50, wxALL|wxEXPAND, FromDIP(5) );

	m_splitterDestination = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D|wxSP_LIVE_UPDATE );
	m_splitterDestination->SetSashGravity( 1 );
	m_splitterDestination->Connect( wxEVT_IDLE, wxIdleEventHandler( wxZRColaComposerPanelBase::m_splitterDestinationOnIdle ), NULL, this );
	m_splitterDestination->SetMinimumPaneSize( FromDIP(5) );

	m_panelDestinationEdit = new wxPanel( m_splitterDestination, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerDestinationEdit;
	bSizerDestinationEdit = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* bSizerDestinationEdit2;
	bSizerDestinationEdit2 = new wxStaticBoxSizer( new wxStaticBox( m_panelDestinationEdit, wxID_ANY, _("Composed Text") ), wxVERTICAL );

	m_destination = new wxTextCtrl( bSizerDestinationEdit2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_RICH );
	m_destination->SetFont( wxFont( 20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("ZRCola") ) );
	m_destination->SetMinSize( FromDIP(wxSize( 100,25 )) );

	bSizerDestinationEdit2->Add( m_destination, 1, wxEXPAND, FromDIP(5) );


	bSizerDestinationEdit->Add( bSizerDestinationEdit2, 1, wxEXPAND, FromDIP(5) );


	m_panelDestinationEdit->SetSizer( bSizerDestinationEdit );
	m_panelDestinationEdit->Layout();
	bSizerDestinationEdit->Fit( m_panelDestinationEdit );
	m_panelDestinationHex = new wxPanel( m_splitterDestination, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerDestinationHex;
	bSizerDestinationHex = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* bSizerDestinationHex2;
	bSizerDestinationHex2 = new wxStaticBoxSizer( new wxStaticBox( m_panelDestinationHex, wxID_ANY, _("Composed Unicode Dump") ), wxVERTICAL );

	m_destinationHex = new wxTextCtrl( bSizerDestinationHex2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	m_destinationHex->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizerDestinationHex2->Add( m_destinationHex, 1, wxEXPAND, FromDIP(5) );


	bSizerDestinationHex->Add( bSizerDestinationHex2, 1, wxEXPAND, FromDIP(5) );


	m_panelDestinationHex->SetSizer( bSizerDestinationHex );
	m_panelDestinationHex->Layout();
	bSizerDestinationHex->Fit( m_panelDestinationHex );
	m_splitterDestination->SplitVertically( m_panelDestinationEdit, m_panelDestinationHex, FromDIP(-5) );
	bSizerMain->Add( m_splitterDestination, 50, wxALL|wxEXPAND, FromDIP(5) );


	this->SetSizer( bSizerMain );
	this->Layout();
	bSizerMain->Fit( this );
	m_timerSave.SetOwner( this, wxID_TIMER_SAVE );

	// Connect Events
	m_source->Connect( wxEVT_PAINT, wxPaintEventHandler( wxZRColaComposerPanelBase::OnSourcePaint ), NULL, this );
	m_source->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaComposerPanelBase::OnSourceText ), NULL, this );
	m_sourceHex->Connect( wxEVT_PAINT, wxPaintEventHandler( wxZRColaComposerPanelBase::OnSourceHexPaint ), NULL, this );
	m_destination->Connect( wxEVT_PAINT, wxPaintEventHandler( wxZRColaComposerPanelBase::OnDestinationPaint ), NULL, this );
	m_destination->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaComposerPanelBase::OnDestinationText ), NULL, this );
	m_destinationHex->Connect( wxEVT_PAINT, wxPaintEventHandler( wxZRColaComposerPanelBase::OnDestinationHexPaint ), NULL, this );
	this->Connect( wxID_TIMER_SAVE, wxEVT_TIMER, wxTimerEventHandler( wxZRColaComposerPanelBase::OnSaveTimer ) );
}

wxZRColaComposerPanelBase::~wxZRColaComposerPanelBase()
{
	// Disconnect Events
	m_source->Disconnect( wxEVT_PAINT, wxPaintEventHandler( wxZRColaComposerPanelBase::OnSourcePaint ), NULL, this );
	m_source->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaComposerPanelBase::OnSourceText ), NULL, this );
	m_sourceHex->Disconnect( wxEVT_PAINT, wxPaintEventHandler( wxZRColaComposerPanelBase::OnSourceHexPaint ), NULL, this );
	m_destination->Disconnect( wxEVT_PAINT, wxPaintEventHandler( wxZRColaComposerPanelBase::OnDestinationPaint ), NULL, this );
	m_destination->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaComposerPanelBase::OnDestinationText ), NULL, this );
	m_destinationHex->Disconnect( wxEVT_PAINT, wxPaintEventHandler( wxZRColaComposerPanelBase::OnDestinationHexPaint ), NULL, this );
	this->Disconnect( wxID_TIMER_SAVE, wxEVT_TIMER, wxTimerEventHandler( wxZRColaComposerPanelBase::OnSaveTimer ) );

}

wxZRColaCharacterCatalogPanelBase::wxZRColaCharacterCatalogPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer;
	bSizer = new wxBoxSizer( wxVERTICAL );

	wxArrayString m_choiceChoices;
	m_choice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceChoices, 0 );
	m_choice->SetSelection( 0 );
	m_choice->SetToolTip( _("Select category to display") );

	bSizer->Add( m_choice, 0, wxALL|wxEXPAND, FromDIP(5) );

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
	m_grid->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_grid->EnableDragRowSize( false );
	m_grid->SetRowLabelSize( 0 );
	m_grid->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_grid->SetDefaultCellBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	m_grid->SetDefaultCellFont( wxFont( 20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("ZRCola") ) );
	m_grid->SetDefaultCellAlignment( wxALIGN_CENTER, wxALIGN_CENTER );
	m_grid->SetMinSize( FromDIP(wxSize( 35,35 )) );

	bSizer->Add( m_grid, 1, wxALL|wxEXPAND, FromDIP(5) );

	m_show_all = new wxCheckBox( this, wxID_ANY, _("Show &All"), wxDefaultPosition, wxDefaultSize, 0 );
	m_show_all->SetToolTip( _("Toggle display of less frequent characters") );

	bSizer->Add( m_show_all, 0, wxALL|wxEXPAND, FromDIP(5) );


	this->SetSizer( bSizer );
	this->Layout();
	bSizer->Fit( this );

	// Connect Events
	m_choice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( wxZRColaCharacterCatalogPanelBase::OnChoice ), NULL, this );
	m_grid->Connect( wxEVT_GRID_CELL_LEFT_CLICK, wxGridEventHandler( wxZRColaCharacterCatalogPanelBase::OnGridClick ), NULL, this );
	m_grid->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( wxZRColaCharacterCatalogPanelBase::OnGridKeyDown ), NULL, this );
	m_show_all->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( wxZRColaCharacterCatalogPanelBase::OnShowAll ), NULL, this );
}

wxZRColaCharacterCatalogPanelBase::~wxZRColaCharacterCatalogPanelBase()
{
	// Disconnect Events
	m_choice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( wxZRColaCharacterCatalogPanelBase::OnChoice ), NULL, this );
	m_grid->Disconnect( wxEVT_GRID_CELL_LEFT_CLICK, wxGridEventHandler( wxZRColaCharacterCatalogPanelBase::OnGridClick ), NULL, this );
	m_grid->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( wxZRColaCharacterCatalogPanelBase::OnGridKeyDown ), NULL, this );
	m_show_all->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( wxZRColaCharacterCatalogPanelBase::OnShowAll ), NULL, this );

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
	m_search->SetToolTip( _("Full or partial terms from Unicode character description (in English) to search for") );

	sbSizerBrowse->Add( m_search, 0, wxALL|wxEXPAND, FromDIP(5) );

	m_search_more = new wxHyperlinkCtrl( sbSizerBrowse->GetStaticBox(), wxID_ANY, _("Search Options"), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	m_search_more->SetToolTip( _("Shows/hides additional search options") );

	sbSizerBrowse->Add( m_search_more, 0, wxALL|wxEXPAND, FromDIP(5) );

	m_search_panel = new wxPanel( sbSizerBrowse->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_search_panel->Hide();

	wxBoxSizer* sbSizerSearch;
	sbSizerSearch = new wxBoxSizer( wxVERTICAL );

	wxArrayString m_categoriesChoices;
	m_categories = new wxCheckListBox( m_search_panel, wxID_ANY, wxDefaultPosition, FromDIP(wxSize( -1,60 )), m_categoriesChoices, 0 );
	m_categories->SetToolTip( _("List of Unicode character categories to search in") );

	sbSizerSearch->Add( m_categories, 0, wxALL|wxEXPAND, FromDIP(5) );

	wxBoxSizer* bSizerCategoriesCtrl;
	bSizerCategoriesCtrl = new wxBoxSizer( wxHORIZONTAL );

	m_categoriesAll = new wxHyperlinkCtrl( m_search_panel, wxID_ANY, _("All"), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	m_categoriesAll->SetToolTip( _("Select all categories") );

	bSizerCategoriesCtrl->Add( m_categoriesAll, 0, wxALL, FromDIP(5) );

	m_categoriesNone = new wxHyperlinkCtrl( m_search_panel, wxID_ANY, _("None"), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	m_categoriesNone->SetToolTip( _("Clear category selection") );

	bSizerCategoriesCtrl->Add( m_categoriesNone, 0, wxALL, FromDIP(5) );

	m_categoriesInvert = new wxHyperlinkCtrl( m_search_panel, wxID_ANY, _("Invert"), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	m_categoriesInvert->SetToolTip( _("Invert category selection") );

	bSizerCategoriesCtrl->Add( m_categoriesInvert, 0, wxALL, FromDIP(5) );


	sbSizerSearch->Add( bSizerCategoriesCtrl, 0, wxALIGN_RIGHT, FromDIP(5) );


	m_search_panel->SetSizer( sbSizerSearch );
	m_search_panel->Layout();
	sbSizerSearch->Fit( m_search_panel );
	sbSizerBrowse->Add( m_search_panel, 0, wxEXPAND, FromDIP(5) );

	m_gridResults = new wxZRColaCharGrid( sbSizerBrowse->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxBORDER_STATIC );

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
	m_gridResults->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_gridResults->EnableDragRowSize( false );
	m_gridResults->SetRowLabelSize( 0 );
	m_gridResults->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_gridResults->SetDefaultCellFont( wxFont( 20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("ZRCola") ) );
	m_gridResults->SetDefaultCellAlignment( wxALIGN_CENTER, wxALIGN_CENTER );
	m_gridResults->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	m_gridResults->SetToolTip( _("Character search results") );
	m_gridResults->SetMinSize( FromDIP(wxSize( 560,35 )) );
	m_gridResults->SetMaxSize( FromDIP(wxSize( 560,-1 )) );

	sbSizerBrowse->Add( m_gridResults, 1, wxALL|wxEXPAND, FromDIP(5) );


	bSizerLeft->Add( sbSizerBrowse, 1, wxALL|wxEXPAND, FromDIP(5) );

	wxStaticBoxSizer* sbSizerRecent;
	sbSizerRecent = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Re&cently Used") ), wxVERTICAL );

	m_gridRecent = new wxZRColaCharGrid( sbSizerRecent->GetStaticBox(), wxID_ANY, wxDefaultPosition, FromDIP(wxSize( -1,35 )), wxBORDER_STATIC );

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
	m_gridRecent->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_gridRecent->EnableDragRowSize( false );
	m_gridRecent->SetRowLabelSize( 0 );
	m_gridRecent->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_gridRecent->SetDefaultCellFont( wxFont( 20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("ZRCola") ) );
	m_gridRecent->SetDefaultCellAlignment( wxALIGN_CENTER, wxALIGN_CENTER );
	m_gridRecent->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	m_gridRecent->SetToolTip( _("List of recently inserted characters") );

	sbSizerRecent->Add( m_gridRecent, 0, wxALL|wxEXPAND, FromDIP(5) );


	bSizerLeft->Add( sbSizerRecent, 0, wxALL|wxEXPAND, FromDIP(5) );


	bSizerColumns->Add( bSizerLeft, 1, wxEXPAND, FromDIP(5) );

	wxBoxSizer* bSizerRight;
	bSizerRight = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizerPreview;
	sbSizerPreview = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Preview") ), wxVERTICAL );

	wxBoxSizer* bSizerUnicode;
	bSizerUnicode = new wxBoxSizer( wxHORIZONTAL );

	m_labelUnicode = new wxStaticText( sbSizerPreview->GetStaticBox(), wxID_ANY, _("U+"), wxDefaultPosition, wxDefaultSize, 0 );
	m_labelUnicode->Wrap( -1 );
	bSizerUnicode->Add( m_labelUnicode, 0, wxALIGN_CENTER|wxBOTTOM|wxLEFT|wxTOP, FromDIP(5) );

	m_unicode = new wxTextCtrl( sbSizerPreview->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, FromDIP(wxSize( 50,-1 )), 0 );
	m_unicode->SetToolTip( _("Unicode hexadecimal code") );

	bSizerUnicode->Add( m_unicode, 0, wxALIGN_CENTER|wxBOTTOM|wxRIGHT|wxTOP, FromDIP(5) );

	m_shortcut = new wxTextCtrl( sbSizerPreview->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTER );
	m_shortcut->SetToolTip( _("Keyboard shortcut in Composer window") );

	bSizerUnicode->Add( m_shortcut, 1, wxALL|wxEXPAND, FromDIP(5) );


	sbSizerPreview->Add( bSizerUnicode, 0, wxEXPAND, FromDIP(5) );

	m_gridPreview = new wxGrid( sbSizerPreview->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_STATIC );

	// Grid
	m_gridPreview->CreateGrid( 1, 1 );
	m_gridPreview->EnableEditing( false );
	m_gridPreview->EnableGridLines( false );
	m_gridPreview->EnableDragGridSize( false );
	m_gridPreview->SetMargins( 0, 0 );

	// Columns
	m_gridPreview->SetColSize( 0, FromDIP(200) );
	m_gridPreview->EnableDragColMove( false );
	m_gridPreview->EnableDragColSize( false );
	m_gridPreview->SetColLabelSize( 0 );
	m_gridPreview->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_gridPreview->SetRowSize( 0, FromDIP(200) );
	m_gridPreview->EnableDragRowSize( false );
	m_gridPreview->SetRowLabelSize( 0 );
	m_gridPreview->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_gridPreview->SetDefaultCellFont( wxFont( 96, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("ZRCola") ) );
	m_gridPreview->SetDefaultCellAlignment( wxALIGN_CENTER, wxALIGN_CENTER );
	m_gridPreview->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	m_gridPreview->SetToolTip( _("Character preview") );

	sbSizerPreview->Add( m_gridPreview, 0, wxALL|wxEXPAND, FromDIP(5) );

	m_description = new wxTextCtrl( sbSizerPreview->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_CENTER );
	m_description->SetToolTip( _("Unicode character description") );

	sbSizerPreview->Add( m_description, 1, wxALL|wxEXPAND, FromDIP(5) );

	m_tags = new wxTextCtrl( sbSizerPreview->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_CENTER );
	m_tags->SetToolTip( _("Character tags") );

	sbSizerPreview->Add( m_tags, 1, wxALL|wxEXPAND, FromDIP(5) );

	m_category = new wxTextCtrl( sbSizerPreview->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTER );
	m_category->SetToolTip( _("Unicode character category") );

	sbSizerPreview->Add( m_category, 0, wxALL|wxEXPAND, FromDIP(5) );

	wxBoxSizer* bSizerNavigateButtons;
	bSizerNavigateButtons = new wxBoxSizer( wxHORIZONTAL );

	m_navigateBack = new wxHyperlinkCtrl( sbSizerPreview->GetStaticBox(), wxID_ANY, _("« Back"), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	m_navigateBack->Enable( false );
	m_navigateBack->SetToolTip( _("To previously viewed character") );

	bSizerNavigateButtons->Add( m_navigateBack, 0, wxALL, FromDIP(5) );

	m_navigateForward = new wxHyperlinkCtrl( sbSizerPreview->GetStaticBox(), wxID_ANY, _("Forward »"), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	m_navigateForward->Enable( false );
	m_navigateForward->SetToolTip( _("To following viewed character") );

	bSizerNavigateButtons->Add( m_navigateForward, 0, wxALL, FromDIP(5) );


	sbSizerPreview->Add( bSizerNavigateButtons, 0, wxALIGN_RIGHT, FromDIP(5) );


	bSizerRight->Add( sbSizerPreview, 70, wxALL|wxEXPAND, FromDIP(5) );

	wxStaticBoxSizer* sbSizerRelated;
	sbSizerRelated = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Re&lated") ), wxVERTICAL );

	m_gridRelated = new wxZRColaCharGrid( sbSizerRelated->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxBORDER_STATIC );

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
	m_gridRelated->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_gridRelated->EnableDragRowSize( false );
	m_gridRelated->SetRowLabelSize( 0 );
	m_gridRelated->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_gridRelated->SetDefaultCellFont( wxFont( 20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("ZRCola") ) );
	m_gridRelated->SetDefaultCellAlignment( wxALIGN_CENTER, wxALIGN_CENTER );
	m_gridRelated->SetToolTip( _("List of related characters") );

	sbSizerRelated->Add( m_gridRelated, 1, wxALL|wxEXPAND, FromDIP(5) );


	bSizerRight->Add( sbSizerRelated, 30, wxALL|wxEXPAND, FromDIP(5) );


	bSizerColumns->Add( bSizerRight, 0, wxEXPAND, FromDIP(5) );


	bSizerContent->Add( bSizerColumns, 1, wxALL|wxEXPAND, FromDIP(5) );


	bSizerContent->Add( FromDIP(5), FromDIP(5), 0, wxALL|wxEXPAND, FromDIP(5) );

	wxBoxSizer* bSizerButtons;
	bSizerButtons = new wxBoxSizer( wxHORIZONTAL );

	m_buttonInsert = new wxButton( this, wxID_ANY, _("&Insert"), wxDefaultPosition, wxDefaultSize, 0 );

	m_buttonInsert->SetDefault();
	m_buttonInsert->Enable( false );
	m_buttonInsert->SetToolTip( _("Insert character into decomposed text") );

	bSizerButtons->Add( m_buttonInsert, 0, wxALL, FromDIP(5) );


	bSizerContent->Add( bSizerButtons, 0, wxALIGN_RIGHT|wxALL, FromDIP(5) );


	this->SetSizer( bSizerContent );
	this->Layout();
	bSizerContent->Fit( this );

	// Connect Events
	this->Connect( wxEVT_IDLE, wxIdleEventHandler( wxZRColaCharSelectBase::OnIdle ) );
	m_search->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaCharSelectBase::OnSearchText ), NULL, this );
	m_search_more->Connect( wxEVT_COMMAND_HYPERLINK, wxHyperlinkEventHandler( wxZRColaCharSelectBase::OnSearchMore ), NULL, this );
	m_categories->Connect( wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, wxCommandEventHandler( wxZRColaCharSelectBase::OnCategoriesToggle ), NULL, this );
	m_categoriesAll->Connect( wxEVT_COMMAND_HYPERLINK, wxHyperlinkEventHandler( wxZRColaCharSelectBase::OnCategoriesAll ), NULL, this );
	m_categoriesNone->Connect( wxEVT_COMMAND_HYPERLINK, wxHyperlinkEventHandler( wxZRColaCharSelectBase::OnCategoriesNone ), NULL, this );
	m_categoriesInvert->Connect( wxEVT_COMMAND_HYPERLINK, wxHyperlinkEventHandler( wxZRColaCharSelectBase::OnCategoriesInvert ), NULL, this );
	m_gridResults->Connect( wxEVT_GRID_CELL_LEFT_DCLICK, wxGridEventHandler( wxZRColaCharSelectBase::OnResultCellDClick ), NULL, this );
	m_gridResults->Connect( wxEVT_GRID_SELECT_CELL, wxGridEventHandler( wxZRColaCharSelectBase::OnResultSelectCell ), NULL, this );
	m_gridResults->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( wxZRColaCharSelectBase::OnResultsKeyDown ), NULL, this );
	m_gridRecent->Connect( wxEVT_GRID_CELL_LEFT_DCLICK, wxGridEventHandler( wxZRColaCharSelectBase::OnRecentCellDClick ), NULL, this );
	m_gridRecent->Connect( wxEVT_GRID_SELECT_CELL, wxGridEventHandler( wxZRColaCharSelectBase::OnRecentSelectCell ), NULL, this );
	m_gridRecent->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( wxZRColaCharSelectBase::OnRecentKeyDown ), NULL, this );
	m_unicode->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaCharSelectBase::OnUnicodeText ), NULL, this );
	m_gridPreview->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( wxZRColaCharSelectBase::OnPreviewKeyDown ), NULL, this );
	m_navigateBack->Connect( wxEVT_COMMAND_HYPERLINK, wxHyperlinkEventHandler( wxZRColaCharSelectBase::OnNavigateBack ), NULL, this );
	m_navigateForward->Connect( wxEVT_COMMAND_HYPERLINK, wxHyperlinkEventHandler( wxZRColaCharSelectBase::OnNavigateForward ), NULL, this );
	m_gridRelated->Connect( wxEVT_GRID_SELECT_CELL, wxGridEventHandler( wxZRColaCharSelectBase::OnRelatedSelectCell ), NULL, this );
	m_buttonInsert->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxZRColaCharSelectBase::OnInsert ), NULL, this );
	m_buttonInsert->Connect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( wxZRColaCharSelectBase::OnInsertUpdateUI ), NULL, this );
}

wxZRColaCharSelectBase::~wxZRColaCharSelectBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( wxZRColaCharSelectBase::OnIdle ) );
	m_search->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaCharSelectBase::OnSearchText ), NULL, this );
	m_search_more->Disconnect( wxEVT_COMMAND_HYPERLINK, wxHyperlinkEventHandler( wxZRColaCharSelectBase::OnSearchMore ), NULL, this );
	m_categories->Disconnect( wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, wxCommandEventHandler( wxZRColaCharSelectBase::OnCategoriesToggle ), NULL, this );
	m_categoriesAll->Disconnect( wxEVT_COMMAND_HYPERLINK, wxHyperlinkEventHandler( wxZRColaCharSelectBase::OnCategoriesAll ), NULL, this );
	m_categoriesNone->Disconnect( wxEVT_COMMAND_HYPERLINK, wxHyperlinkEventHandler( wxZRColaCharSelectBase::OnCategoriesNone ), NULL, this );
	m_categoriesInvert->Disconnect( wxEVT_COMMAND_HYPERLINK, wxHyperlinkEventHandler( wxZRColaCharSelectBase::OnCategoriesInvert ), NULL, this );
	m_gridResults->Disconnect( wxEVT_GRID_CELL_LEFT_DCLICK, wxGridEventHandler( wxZRColaCharSelectBase::OnResultCellDClick ), NULL, this );
	m_gridResults->Disconnect( wxEVT_GRID_SELECT_CELL, wxGridEventHandler( wxZRColaCharSelectBase::OnResultSelectCell ), NULL, this );
	m_gridResults->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( wxZRColaCharSelectBase::OnResultsKeyDown ), NULL, this );
	m_gridRecent->Disconnect( wxEVT_GRID_CELL_LEFT_DCLICK, wxGridEventHandler( wxZRColaCharSelectBase::OnRecentCellDClick ), NULL, this );
	m_gridRecent->Disconnect( wxEVT_GRID_SELECT_CELL, wxGridEventHandler( wxZRColaCharSelectBase::OnRecentSelectCell ), NULL, this );
	m_gridRecent->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( wxZRColaCharSelectBase::OnRecentKeyDown ), NULL, this );
	m_unicode->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaCharSelectBase::OnUnicodeText ), NULL, this );
	m_gridPreview->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( wxZRColaCharSelectBase::OnPreviewKeyDown ), NULL, this );
	m_navigateBack->Disconnect( wxEVT_COMMAND_HYPERLINK, wxHyperlinkEventHandler( wxZRColaCharSelectBase::OnNavigateBack ), NULL, this );
	m_navigateForward->Disconnect( wxEVT_COMMAND_HYPERLINK, wxHyperlinkEventHandler( wxZRColaCharSelectBase::OnNavigateForward ), NULL, this );
	m_gridRelated->Disconnect( wxEVT_GRID_SELECT_CELL, wxGridEventHandler( wxZRColaCharSelectBase::OnRelatedSelectCell ), NULL, this );
	m_buttonInsert->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxZRColaCharSelectBase::OnInsert ), NULL, this );
	m_buttonInsert->Disconnect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( wxZRColaCharSelectBase::OnInsertUpdateUI ), NULL, this );

}

wxZRColaSettingsBase::wxZRColaSettingsBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxDialog( parent, id, title, pos, size, style, name )
{
	this->SetSizeHints( wxDefaultSize, wxSize( -1,-1 ) );

	wxBoxSizer* bSizerContent;
	bSizerContent = new wxBoxSizer( wxVERTICAL );

	m_listbook = new wxListbook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLB_DEFAULT );
	wxListView *list = m_listbook->GetListView();
	list->SetColumnWidth(0, FromDIP(120));

	m_panelLanguage = new wxPanel( m_listbook, wxID_ANY, wxDefaultPosition, FromDIP(wxSize( 500,-1 )), wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerLanguage;
	bSizerLanguage = new wxBoxSizer( wxVERTICAL );

	m_langLabel = new wxStaticText( m_panelLanguage, wxID_ANY, _("Some character native to specific language you are working with should not decompose to primitives.\nFor optimal decomposition you should set the language correctly."), wxDefaultPosition, wxDefaultSize, 0 );
	m_langLabel->Wrap( FromDIP(490) );
	bSizerLanguage->Add( m_langLabel, 0, wxALL|wxEXPAND, FromDIP(5) );

	m_langAuto = new wxRadioButton( m_panelLanguage, wxID_ANY, _("Select language &automatically according to selected keyboard"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	bSizerLanguage->Add( m_langAuto, 0, wxALL|wxEXPAND, FromDIP(5) );

	m_langManual = new wxRadioButton( m_panelLanguage, wxID_ANY, _("&Manually select the language from the list below:"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerLanguage->Add( m_langManual, 0, wxALL|wxEXPAND, FromDIP(5) );

	m_languages = new wxListBox( m_panelLanguage, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	m_languages->SetMinSize( FromDIP(wxSize( -1,150 )) );

	bSizerLanguage->Add( m_languages, 1, wxALL|wxEXPAND, FromDIP(5) );


	m_panelLanguage->SetSizer( bSizerLanguage );
	m_panelLanguage->Layout();
	m_listbook->AddPage( m_panelLanguage, _("Text Language"), true );
	m_panelAutoStart = new wxPanel( m_listbook, wxID_ANY, wxDefaultPosition, FromDIP(wxSize( 500,-1 )), wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerAutoStart;
	bSizerAutoStart = new wxBoxSizer( wxVERTICAL );

	m_autoStartLabel = new wxStaticText( m_panelAutoStart, wxID_ANY, _("ZRCola can be launched every time you log in to your computer.\nIt will be available on the system tray and via registered shortcuts Win+F5 and Win+F6."), wxDefaultPosition, wxDefaultSize, 0 );
	m_autoStartLabel->Wrap( FromDIP(490) );
	bSizerAutoStart->Add( m_autoStartLabel, 0, wxALL|wxEXPAND, FromDIP(5) );

	m_autoStart = new wxCheckBox( m_panelAutoStart, wxID_ANY, _("Start ZRCola &automatically on logon"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerAutoStart->Add( m_autoStart, 0, wxALL|wxEXPAND, FromDIP(5) );


	m_panelAutoStart->SetSizer( bSizerAutoStart );
	m_panelAutoStart->Layout();
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

	bSizerContent->Add( m_listbook, 1, wxALL|wxEXPAND, FromDIP(5) );


	bSizerContent->Add( 0, 0, 0, wxALL|wxEXPAND, FromDIP(5) );

	m_sdbSizerButtons = new wxStdDialogButtonSizer();
	m_sdbSizerButtonsOK = new wxButton( this, wxID_OK );
	m_sdbSizerButtons->AddButton( m_sdbSizerButtonsOK );
	m_sdbSizerButtonsApply = new wxButton( this, wxID_APPLY );
	m_sdbSizerButtons->AddButton( m_sdbSizerButtonsApply );
	m_sdbSizerButtonsCancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizerButtons->AddButton( m_sdbSizerButtonsCancel );
	m_sdbSizerButtons->Realize();

	bSizerContent->Add( m_sdbSizerButtons, 0, wxALL|wxEXPAND, FromDIP(5) );


	this->SetSizer( bSizerContent );
	this->Layout();
	bSizerContent->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( wxZRColaSettingsBase::OnInitDialog ) );
	m_panelLanguage->Connect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( wxZRColaSettingsBase::OnLanguageUpdate ), NULL, this );
	m_sdbSizerButtonsApply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxZRColaSettingsBase::OnApplyButtonClick ), NULL, this );
	m_sdbSizerButtonsOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxZRColaSettingsBase::OnOKButtonClick ), NULL, this );
}

wxZRColaSettingsBase::~wxZRColaSettingsBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( wxZRColaSettingsBase::OnInitDialog ) );
	m_panelLanguage->Disconnect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( wxZRColaSettingsBase::OnLanguageUpdate ), NULL, this );
	m_sdbSizerButtonsApply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxZRColaSettingsBase::OnApplyButtonClick ), NULL, this );
	m_sdbSizerButtonsOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxZRColaSettingsBase::OnOKButtonClick ), NULL, this );

}

wxZRColaAboutBase::wxZRColaAboutBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxDialog( parent, id, title, pos, size, style, name )
{
	this->SetSizeHints( wxDefaultSize, wxSize( -1,-1 ) );

	wxBoxSizer* bSizerContent;
	bSizerContent = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizerColumns;
	bSizerColumns = new wxBoxSizer( wxHORIZONTAL );

	m_bitmapIcon = new wxStaticBitmap( this, wxID_ANY, wxIcon( wxT("00_zrcola.ico"), wxBITMAP_TYPE_ICO_RESOURCE, FromDIP(32), FromDIP(32) ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerColumns->Add( m_bitmapIcon, 0, wxALL, FromDIP(5) );

	wxBoxSizer* bSizerText;
	bSizerText = new wxBoxSizer( wxVERTICAL );

	m_staticTextZRCola = new wxStaticText( this, wxID_ANY, _("ZRCola"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextZRCola->Wrap( -1 );
	m_staticTextZRCola->SetFont( wxFont( 20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("ZRCola") ) );
	m_staticTextZRCola->SetMaxSize( FromDIP(wxSize( 400,-1 )) );

	bSizerText->Add( m_staticTextZRCola, 0, wxALL|wxEXPAND, FromDIP(5) );

	m_staticTextVersion = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextVersion->Wrap( -1 );
	bSizerText->Add( m_staticTextVersion, 0, wxALL|wxEXPAND, FromDIP(5) );

	m_staticTextColophone = new wxStaticText( this, wxID_ANY, _("Program and Website Editor: Janoš Ježovnik\nDevelopment: Amebis, d. o. o., Kamnik\nTranslation into English: Janoš Ježovnik\nTranslation into Russian language: Domen Krvina, Silvo Torkar, Anastasia Plotnikova\nDevelopment and maintenance of the original program (2004–2015): Peter Weiss"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextColophone->Wrap( FromDIP(400) );
	bSizerText->Add( m_staticTextColophone, 0, wxALL|wxEXPAND, FromDIP(5) );

	m_staticTextCopyright = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextCopyright->Wrap( -1 );
	bSizerText->Add( m_staticTextCopyright, 0, wxALL|wxEXPAND, FromDIP(5) );

	m_hyperlink = new wxHyperlinkCtrl( this, wxID_ANY, _("zrcola.zrc-sazu.si"), wxT("http://zrcola.zrc-sazu.si/"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	bSizerText->Add( m_hyperlink, 0, wxALL|wxEXPAND, FromDIP(5) );


	bSizerText->Add( 0, 0, 0, wxALL|wxEXPAND, FromDIP(5) );

	m_staticTextDeclaration = new wxStaticText( this, wxID_ANY, _("Texts made using ZRCola have to include in a footnote or some other appropriate part of the publication the note below:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDeclaration->Wrap( FromDIP(400) );
	bSizerText->Add( m_staticTextDeclaration, 0, wxALL|wxEXPAND, FromDIP(5) );

	m_declaration = new wxTextCtrl( this, wxID_ANY, _("This text was written using the ZRCola input system (http://zrcola.zrc-sazu.si), developed at the Science and Research Centre of SAZU in Ljubljana (http://www.zrc-sazu.si) by Peter Weiss."), wxDefaultPosition, FromDIP(wxSize( -1,80 )), wxTE_MULTILINE|wxTE_READONLY );
	m_declaration->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizerText->Add( m_declaration, 0, wxALL|wxEXPAND, FromDIP(5) );


	bSizerColumns->Add( bSizerText, 1, wxEXPAND, FromDIP(5) );


	bSizerContent->Add( bSizerColumns, 1, wxALL|wxEXPAND, FromDIP(5) );


	bSizerContent->Add( 0, 0, 0, wxALL|wxEXPAND, FromDIP(5) );

	m_sdbSizerButtons = new wxStdDialogButtonSizer();
	m_sdbSizerButtonsOK = new wxButton( this, wxID_OK );
	m_sdbSizerButtons->AddButton( m_sdbSizerButtonsOK );
	m_sdbSizerButtons->Realize();

	bSizerContent->Add( m_sdbSizerButtons, 0, wxALL|wxEXPAND, FromDIP(5) );


	this->SetSizer( bSizerContent );
	this->Layout();
	bSizerContent->Fit( this );

	this->Centre( wxBOTH );
}

wxZRColaAboutBase::~wxZRColaAboutBase()
{
}

wxZRColaUpdaterBase::wxZRColaUpdaterBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxDialog( parent, id, title, pos, size, style, name )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizerContent;
	bSizerContent = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizerLog;
	sbSizerLog = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Log") ), wxVERTICAL );

	m_log = new wxTextCtrl( sbSizerLog->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	m_log->SetMinSize( FromDIP(wxSize( 450,150 )) );

	sbSizerLog->Add( m_log, 1, wxEXPAND, FromDIP(5) );


	bSizerContent->Add( sbSizerLog, 1, wxALL|wxEXPAND, FromDIP(5) );


	bSizerContent->Add( 0, 0, 0, wxEXPAND, FromDIP(5) );

	wxBoxSizer* bSizerButtons;
	bSizerButtons = new wxBoxSizer( wxHORIZONTAL );

	m_buttonUpdate = new wxButton( this, wxID_ANY, _("Quit and &Update..."), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonUpdate->Enable( false );
	m_buttonUpdate->SetToolTip( _("Exit this program and launch product update") );

	bSizerButtons->Add( m_buttonUpdate, 0, wxALL, FromDIP(5) );

	m_buttonClose = new wxButton( this, wxID_OK, _("&Close"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonClose->SetToolTip( _("Close this window") );

	bSizerButtons->Add( m_buttonClose, 0, wxALL, FromDIP(5) );


	bSizerContent->Add( bSizerButtons, 0, wxALIGN_RIGHT, FromDIP(5) );


	this->SetSizer( bSizerContent );
	this->Layout();
	bSizerContent->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_IDLE, wxIdleEventHandler( wxZRColaUpdaterBase::OnIdle ) );
	m_buttonUpdate->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxZRColaUpdaterBase::OnUpdate ), NULL, this );
	m_buttonClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxZRColaUpdaterBase::OnClose ), NULL, this );
}

wxZRColaUpdaterBase::~wxZRColaUpdaterBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( wxZRColaUpdaterBase::OnIdle ) );
	m_buttonUpdate->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxZRColaUpdaterBase::OnUpdate ), NULL, this );
	m_buttonClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxZRColaUpdaterBase::OnClose ), NULL, this );

}

wxZRColaCharRequestBase::wxZRColaCharRequestBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxDialog( parent, id, title, pos, size, style, name )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizerContent;
	bSizerContent = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizerCharacter;
	sbSizerCharacter = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Character") ), wxVERTICAL );

	m_characterLbl = new wxStaticText( sbSizerCharacter->GetStaticBox(), wxID_ANY, _("Enter the &character you would like to request:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_characterLbl->Wrap( FromDIP(400) );
	sbSizerCharacter->Add( m_characterLbl, 0, wxALL|wxEXPAND, FromDIP(5) );

	m_character = new wxTextCtrl( sbSizerCharacter->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	#ifdef __WXGTK__
	if ( !m_character->HasFlag( wxTE_MULTILINE ) )
	{
	m_character->SetMaxLength( 20 );
	}
	#else
	m_character->SetMaxLength( 20 );
	#endif
	m_character->SetFont( wxFont( 20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("ZRCola") ) );
	m_character->SetToolTip( _("Decomposed character to request") );

	sbSizerCharacter->Add( m_character, 0, wxALL|wxEXPAND, FromDIP(5) );

	m_characterNote = new wxStaticText( sbSizerCharacter->GetStaticBox(), wxID_ANY, _("Please, use the decomposed form.\nYou can use ZRCola keyboard shortcuts to enter the character or Copy&&Paste it from the Decomposed window."), wxDefaultPosition, wxDefaultSize, 0 );
	m_characterNote->Wrap( FromDIP(400) );
	sbSizerCharacter->Add( m_characterNote, 0, wxALL|wxEXPAND, FromDIP(5) );


	bSizerContent->Add( sbSizerCharacter, 40, wxALL|wxEXPAND, FromDIP(5) );

	wxStaticBoxSizer* sbSizerContext;
	sbSizerContext = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Context") ), wxVERTICAL );

	m_contextLbl = new wxStaticText( sbSizerContext->GetStaticBox(), wxID_ANY, _("The &context, examples or description why and where the character is required:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_contextLbl->Wrap( FromDIP(400) );
	sbSizerContext->Add( m_contextLbl, 0, wxALL|wxEXPAND, FromDIP(5) );

	m_context = new wxTextCtrl( sbSizerContext->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	m_context->SetFont( wxFont( 16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("ZRCola") ) );
	m_context->SetToolTip( _("Additional notes for character request") );

	sbSizerContext->Add( m_context, 1, wxALL|wxEXPAND, FromDIP(5) );


	bSizerContent->Add( sbSizerContext, 60, wxALL|wxEXPAND, FromDIP(5) );

	m_note = new wxStaticText( this, wxID_ANY, _("After clicking OK button, your e-mail application should open allowing you to submit the new character request to ZRCola Editor.\nYour e-mail application might not display all the characters correctly, but the Editor will be able to read them correctly anyway."), wxDefaultPosition, wxDefaultSize, 0 );
	m_note->Wrap( FromDIP(400) );
	bSizerContent->Add( m_note, 0, wxALL|wxEXPAND, FromDIP(5) );


	bSizerContent->Add( 0, 0, 0, wxALL|wxEXPAND, FromDIP(5) );

	m_sdbSizerButtons = new wxStdDialogButtonSizer();
	m_sdbSizerButtonsOK = new wxButton( this, wxID_OK );
	m_sdbSizerButtons->AddButton( m_sdbSizerButtonsOK );
	m_sdbSizerButtonsCancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizerButtons->AddButton( m_sdbSizerButtonsCancel );
	m_sdbSizerButtons->Realize();

	bSizerContent->Add( m_sdbSizerButtons, 0, wxALL|wxEXPAND, FromDIP(5) );


	this->SetSizer( bSizerContent );
	this->Layout();
	bSizerContent->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	m_sdbSizerButtonsOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxZRColaCharRequestBase::OnOKButtonClick ), NULL, this );
}

wxZRColaCharRequestBase::~wxZRColaCharRequestBase()
{
	// Disconnect Events
	m_sdbSizerButtonsOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxZRColaCharRequestBase::OnOKButtonClick ), NULL, this );

}

wxZRColaTranslationSeqBase::wxZRColaTranslationSeqBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxDialog( parent, id, title, pos, size, style, name )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizerContent;
	bSizerContent = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizerTranslationSet;
	sbSizerTranslationSet = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Translation") ), wxVERTICAL );

	m_transLbl = new wxStaticText( sbSizerTranslationSet->GetStaticBox(), wxID_ANY, _("ZRCola offers multiple text translations that can be arranged in a sequence.\nPlease select desired translations and the order they are applied."), wxDefaultPosition, wxDefaultSize, 0 );
	m_transLbl->Wrap( FromDIP(452) );
	sbSizerTranslationSet->Add( m_transLbl, 0, wxALL|wxEXPAND, FromDIP(5) );

	wxBoxSizer* bSizerTranslation;
	bSizerTranslation = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizerTransAvailable;
	bSizerTransAvailable = new wxBoxSizer( wxVERTICAL );

	m_availableLbl = new wxStaticText( sbSizerTranslationSet->GetStaticBox(), wxID_ANY, _("A&vailable:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_availableLbl->Wrap( -1 );
	bSizerTransAvailable->Add( m_availableLbl, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, FromDIP(5) );

	m_available = new wxListBox( sbSizerTranslationSet->GetStaticBox(), wxID_ANY, wxDefaultPosition, FromDIP(wxSize( 200,-1 )), 0, NULL, wxLB_SINGLE|wxLB_SORT );
	m_available->SetMinSize( FromDIP(wxSize( 200,150 )) );

	bSizerTransAvailable->Add( m_available, 1, wxALL|wxEXPAND, FromDIP(5) );


	bSizerTranslation->Add( bSizerTransAvailable, 1, wxEXPAND, FromDIP(5) );

	wxBoxSizer* bSizerTransSelect;
	bSizerTransSelect = new wxBoxSizer( wxVERTICAL );

	m_add = new wxButton( sbSizerTranslationSet->GetStaticBox(), wxID_ANY, _(">"), wxDefaultPosition, FromDIP(wxSize( 32,32 )), 0 );
	bSizerTransSelect->Add( m_add, 0, wxALL, FromDIP(5) );

	m_remove = new wxButton( sbSizerTranslationSet->GetStaticBox(), wxID_ANY, _("<"), wxDefaultPosition, FromDIP(wxSize( 32,32 )), 0 );
	bSizerTransSelect->Add( m_remove, 0, wxALL, FromDIP(5) );


	bSizerTranslation->Add( bSizerTransSelect, 0, wxALIGN_CENTER_VERTICAL, FromDIP(5) );

	wxBoxSizer* bSizerTransSelected;
	bSizerTransSelected = new wxBoxSizer( wxVERTICAL );

	m_selectedLbl = new wxStaticText( sbSizerTranslationSet->GetStaticBox(), wxID_ANY, _("&Selected:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_selectedLbl->Wrap( -1 );
	bSizerTransSelected->Add( m_selectedLbl, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, FromDIP(5) );

	m_selected = new wxListBox( sbSizerTranslationSet->GetStaticBox(), wxID_ANY, wxDefaultPosition, FromDIP(wxSize( 200,-1 )), 0, NULL, wxLB_SINGLE );
	m_selected->SetMinSize( FromDIP(wxSize( 200,150 )) );

	bSizerTransSelected->Add( m_selected, 1, wxALL|wxEXPAND, FromDIP(5) );


	bSizerTranslation->Add( bSizerTransSelected, 1, wxEXPAND, FromDIP(5) );


	sbSizerTranslationSet->Add( bSizerTranslation, 1, wxEXPAND, FromDIP(5) );

	wxBoxSizer* bSizerTransActiveReorder;
	bSizerTransActiveReorder = new wxBoxSizer( wxHORIZONTAL );

	m_selectedUp = new wxButton( sbSizerTranslationSet->GetStaticBox(), wxID_ANY, _("Up"), wxDefaultPosition, FromDIP(wxSize( 70,-1 )), 0 );
	bSizerTransActiveReorder->Add( m_selectedUp, 0, wxALL, FromDIP(5) );

	m_selectedDown = new wxButton( sbSizerTranslationSet->GetStaticBox(), wxID_ANY, _("Down"), wxDefaultPosition, FromDIP(wxSize( 70,-1 )), 0 );
	bSizerTransActiveReorder->Add( m_selectedDown, 0, wxALL, FromDIP(5) );


	sbSizerTranslationSet->Add( bSizerTransActiveReorder, 0, wxALIGN_RIGHT, FromDIP(5) );


	bSizerContent->Add( sbSizerTranslationSet, 1, wxEXPAND|wxALL, FromDIP(5) );


	bSizerContent->Add( 0, 0, 0, wxEXPAND|wxALL, FromDIP(5) );

	m_sdbSizerButtons = new wxStdDialogButtonSizer();
	m_sdbSizerButtonsOK = new wxButton( this, wxID_OK );
	m_sdbSizerButtons->AddButton( m_sdbSizerButtonsOK );
	m_sdbSizerButtonsCancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizerButtons->AddButton( m_sdbSizerButtonsCancel );
	m_sdbSizerButtons->Realize();

	bSizerContent->Add( m_sdbSizerButtons, 0, wxEXPAND|wxALL, FromDIP(5) );


	this->SetSizer( bSizerContent );
	this->Layout();
	bSizerContent->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( wxZRColaTranslationSeqBase::OnInitDialog ) );
	this->Connect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( wxZRColaTranslationSeqBase::OnUpdate ) );
	m_available->Connect( wxEVT_CHAR, wxKeyEventHandler( wxZRColaTranslationSeqBase::OnAvailableChar ), NULL, this );
	m_available->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( wxZRColaTranslationSeqBase::OnAvailableDClick ), NULL, this );
	m_add->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxZRColaTranslationSeqBase::OnAdd ), NULL, this );
	m_remove->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxZRColaTranslationSeqBase::OnRemove ), NULL, this );
	m_selected->Connect( wxEVT_CHAR, wxKeyEventHandler( wxZRColaTranslationSeqBase::OnSelectedChar ), NULL, this );
	m_selected->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( wxZRColaTranslationSeqBase::OnSelectedDClick ), NULL, this );
	m_selectedUp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxZRColaTranslationSeqBase::OnSelectedUp ), NULL, this );
	m_selectedDown->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxZRColaTranslationSeqBase::OnSelectedDown ), NULL, this );
	m_sdbSizerButtonsOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxZRColaTranslationSeqBase::OnOKButtonClick ), NULL, this );
}

wxZRColaTranslationSeqBase::~wxZRColaTranslationSeqBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( wxZRColaTranslationSeqBase::OnInitDialog ) );
	this->Disconnect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( wxZRColaTranslationSeqBase::OnUpdate ) );
	m_available->Disconnect( wxEVT_CHAR, wxKeyEventHandler( wxZRColaTranslationSeqBase::OnAvailableChar ), NULL, this );
	m_available->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( wxZRColaTranslationSeqBase::OnAvailableDClick ), NULL, this );
	m_add->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxZRColaTranslationSeqBase::OnAdd ), NULL, this );
	m_remove->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxZRColaTranslationSeqBase::OnRemove ), NULL, this );
	m_selected->Disconnect( wxEVT_CHAR, wxKeyEventHandler( wxZRColaTranslationSeqBase::OnSelectedChar ), NULL, this );
	m_selected->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( wxZRColaTranslationSeqBase::OnSelectedDClick ), NULL, this );
	m_selectedUp->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxZRColaTranslationSeqBase::OnSelectedUp ), NULL, this );
	m_selectedDown->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxZRColaTranslationSeqBase::OnSelectedDown ), NULL, this );
	m_sdbSizerButtonsOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxZRColaTranslationSeqBase::OnOKButtonClick ), NULL, this );

}
