/*
    Copyright 2016 Amebis

    This file is part of ZRCola.

    ZRCola is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ZRCola is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ZRCola. If not, see <http://www.gnu.org/licenses/>.
*/

#include "stdafx.h"


//////////////////////////////////////////////////////////////////////////
// ZRColaFrame
//////////////////////////////////////////////////////////////////////////

wxBEGIN_EVENT_TABLE(ZRColaFrame, wxAppBarFrame)
    EVT_MENU(wxID_EXIT,  ZRColaFrame::OnExit)
    EVT_MENU(wxID_ABOUT, ZRColaFrame::OnAbout)
wxEND_EVENT_TABLE()


ZRColaFrame::ZRColaFrame() : wxAppBarFrame(NULL, wxID_ANY, _("ZRCola"), wxABS_FLOAT, wxABF_ALLOWFLOAT | wxABF_ALLOWDOCKHORIZONTAL | wxABF_ALLOWSIZING | wxABF_HIDETASKBARTABWHENDOCKED, wxDefaultPosition, wxSize(400, 100))
{
    wxBoxSizer* bSizer1;
    bSizer1 = new wxBoxSizer( wxHORIZONTAL );

    m_preview = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_MULTILINE|wxTE_WORDWRAP );
    m_preview->SetFont( wxFont( 20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("00 ZRCola") ) );
    bSizer1->Add( m_preview, 1, wxEXPAND, 5 );

    m_btnSend = new wxButton( this, wxID_ANY, wxT("&Send"), wxDefaultPosition, wxDefaultSize, 0 );
    m_btnSend->SetDefault(); 
    bSizer1->Add( m_btnSend, 0, wxEXPAND, 5 );

    SetSizer( bSizer1 );
    Layout();
}


void ZRColaFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}


void ZRColaFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox(wxString::Format(_("ZRCola v%s\nCopyright 2016 Amebis"), wxT(ZRCOLA_VERSION_STR)), _("About ZRCola"), wxOK | wxICON_INFORMATION);
}
