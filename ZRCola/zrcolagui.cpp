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
	this->SetSizeHints( wxSize( 100,100 ), wxDefaultSize );
	
	
	this->Centre( wxBOTH );
}

wxZRColaFrameBase::~wxZRColaFrameBase()
{
}

wxZRColaComposerPanelBase::wxZRColaComposerPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizerEditor;
	bSizerEditor = new wxBoxSizer( wxVERTICAL );
	
	m_decomposed = new wxTextCtrl( this, wxID_DECOMPOSED, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE );
	m_decomposed->SetFont( wxFont( 20, 70, 90, 90, false, wxT("00 ZRCola") ) );
	
	bSizerEditor->Add( m_decomposed, 50, wxALL|wxEXPAND, 5 );
	
	m_composed = new wxTextCtrl( this, wxID_COMPOSED, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE );
	m_composed->SetFont( wxFont( 20, 70, 90, 90, false, wxT("00 ZRCola") ) );
	
	bSizerEditor->Add( m_composed, 50, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( bSizerEditor );
	this->Layout();
	bSizerEditor->Fit( this );
	
	// Connect Events
	m_decomposed->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaComposerPanelBase::OnDecomposedText ), NULL, this );
	m_composed->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaComposerPanelBase::OnComposedText ), NULL, this );
}

wxZRColaComposerPanelBase::~wxZRColaComposerPanelBase()
{
	// Disconnect Events
	m_decomposed->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaComposerPanelBase::OnDecomposedText ), NULL, this );
	m_composed->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaComposerPanelBase::OnComposedText ), NULL, this );
	
}
