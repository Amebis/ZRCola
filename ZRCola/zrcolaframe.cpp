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
// wxZRColaFrame
//////////////////////////////////////////////////////////////////////////

wxBEGIN_EVENT_TABLE(wxZRColaFrame, wxAppBarFrame)
    EVT_MENU(wxID_ABOUT, wxZRColaFrame::OnAbout)
wxEND_EVENT_TABLE()


wxZRColaFrame::wxZRColaFrame() : wxAppBarFrame()
{
}


bool wxZRColaFrame::Create()
{
    wxImage::AddHandler(new wxICOHandler);

    wxCHECK(wxAppBarFrame::Create(NULL, wxID_ANY, _("ZRCola"), wxABS_FLOAT, wxABF_ALLOWFLOAT | wxABF_ALLOWDOCKHORIZONTAL | wxABF_ALLOWSIZING | wxABF_HIDETASKBARTABWHENDOCKED, wxDefaultPosition, wxSize(400, 100)), false);

    wxFont fontZRCola(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("00 ZRCola"));

    wxCHECK(m_preview.Create(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE), false);
    m_preview.Wrap(-1);
    m_preview.SetFont(fontZRCola);

    wxCHECK(m_composer.Create(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_MULTILINE), false);
    m_composer.SetFont(fontZRCola);

    wxBoxSizer
        //*boxH = new wxBoxSizer(wxHORIZONTAL),
        *boxV = new wxBoxSizer(wxVERTICAL);

    boxV->Add(&m_preview,  1, wxEXPAND, 5);
    boxV->Add(&m_composer, 1, wxEXPAND, 5);
    //boxH->Add(boxV, 1, wxEXPAND, 5);

    //m_toolBar.Create(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL);
    //m_toolBar.AddTool(wxID_ABOUT, _("About"), wxBitmap(wxIcon(wxICON(send.ico))));
    //m_toolBar.AddTool(wxID_ABOUT, _("About"), wxBitmap(wxIcon(wxICON(send.ico))));
    //m_toolBar.AddTool(wxID_ABOUT, _("About"), wxBitmap(wxIcon(wxICON(send.ico))));
    //m_toolBar.AddTool(wxID_ABOUT, _("About"), wxBitmap(wxIcon(wxICON(send.ico))));
    //m_toolBar.Realize();
    //Connect(wxID_ABOUT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(wxZRColaFrame::OnAbout));

    //boxH->Add(&m_toolBar, 0, wxEXPAND, 5);

    SetSizer(boxV);
    //SetSizer(boxH);
    Layout();

    return true;
}


void wxZRColaFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox(wxString::Format(_("ZRCola v%s\nCopyright 2016 Amebis"), wxT(ZRCOLA_VERSION_STR)), _("About ZRCola"), wxOK | wxICON_INFORMATION);
}
