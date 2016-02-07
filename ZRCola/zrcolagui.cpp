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

wxZRColaDialogBase::wxZRColaDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizerEditor;
	bSizerEditor = new wxBoxSizer( wxVERTICAL );
	
	bSizerEditor->SetMinSize( wxSize( 100,100 ) ); 
	m_decomposed = new wxTextCtrl( this, wxID_DECOMPOSED, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE );
	m_decomposed->SetFont( wxFont( 20, 70, 90, 90, false, wxT("00 ZRCola") ) );
	
	bSizerEditor->Add( m_decomposed, 50, wxALL|wxEXPAND, 5 );
	
	m_composed = new wxTextCtrl( this, wxID_COMPOSED, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE );
	m_composed->SetFont( wxFont( 20, 70, 90, 90, false, wxT("00 ZRCola") ) );
	
	bSizerEditor->Add( m_composed, 50, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( bSizerEditor );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( wxZRColaDialogBase::OnClose ) );
	m_decomposed->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaDialogBase::OnDecomposedText ), NULL, this );
	m_composed->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaDialogBase::OnComposedText ), NULL, this );
}

wxZRColaDialogBase::~wxZRColaDialogBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( wxZRColaDialogBase::OnClose ) );
	m_decomposed->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaDialogBase::OnDecomposedText ), NULL, this );
	m_composed->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( wxZRColaDialogBase::OnComposedText ), NULL, this );
	
}
