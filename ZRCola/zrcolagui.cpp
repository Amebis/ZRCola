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

wxZRColaFrameBase::wxZRColaFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 150,150 ), wxDefaultSize );
	
	m_menubar = new wxMenuBar( 0 );
	m_menuFile = new wxMenu();
	wxMenuItem* m_menuItemExit;
	m_menuItemExit = new wxMenuItem( m_menuFile, wxID_EXIT, wxString( _("E&xit") ) + wxT('\t') + wxT("Alt+F4"), wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuItemExit );
	
	m_menubar->Append( m_menuFile, _("&File") ); 
	
	m_menuHelp = new wxMenu();
	wxMenuItem* m_menuItemAbout;
	m_menuItemAbout = new wxMenuItem( m_menuHelp, wxID_ABOUT, wxString( _("&About") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuHelp->Append( m_menuItemAbout );
	
	m_menubar->Append( m_menuHelp, _("&Help") ); 
	
	this->SetMenuBar( m_menubar );
	
	wxBoxSizer* bSizerMain;
	bSizerMain = new wxBoxSizer( wxVERTICAL );
	
	m_panel = new wxZRColaComposerPanel( this );
	
	bSizerMain->Add( m_panel, 100, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizerMain );
	this->Layout();
	
	this->Centre( wxBOTH );
}

wxZRColaFrameBase::~wxZRColaFrameBase()
{
}

wxZRColaComposerPanelBase::wxZRColaComposerPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizerEditor;
	bSizerEditor = new wxBoxSizer( wxVERTICAL );
	
	m_decomposed = new wxTextCtrl( this, wxID_DECOMPOSED, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_MULTILINE );
	m_decomposed->SetFont( wxFont( 20, 70, 90, 90, false, wxT("00 ZRCola") ) );
	m_decomposed->SetMinSize( wxSize( 100,25 ) );
	
	bSizerEditor->Add( m_decomposed, 50, wxALL|wxEXPAND, 5 );
	
	m_composed = new wxTextCtrl( this, wxID_COMPOSED, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_MULTILINE );
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
