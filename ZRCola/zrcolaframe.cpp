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

wxBEGIN_EVENT_TABLE(ZRColaFrame, wxAppBar)
    EVT_MENU(ZRColaFrame::ID_Hello, ZRColaFrame::OnHello)
    EVT_MENU(wxID_EXIT,             ZRColaFrame::OnExit)
    EVT_MENU(wxID_ABOUT,            ZRColaFrame::OnAbout)
wxEND_EVENT_TABLE()


ZRColaFrame::ZRColaFrame(const wxString& title, const wxPoint& pos, const wxSize& size) : wxAppBar(NULL, wxID_ANY, title, wxSTATE_FLOAT, wxFLAG_ALLOWFLOAT | wxFLAG_ALLOWDOCKHORIZONTAL | wxFLAG_ALLOWSIZING | wxFLAG_HIDETASKBARTABWHENDOCKED, pos, size)
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ZRColaFrame::ID_Hello, _("&Hello...\tCtrl+H"), _("Help string shown in status bar for this menu item"));
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, _("&File"));
    menuBar->Append(menuHelp, _("&Help"));
    SetMenuBar(menuBar);

    //CreateStatusBar();
    //SetStatusText(_("Welcome to wxWidgets!"));
}


void ZRColaFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}


void ZRColaFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox(_("This is a wxWidgets' Hello world sample"), _("About Hello World"), wxOK | wxICON_INFORMATION);
}


void ZRColaFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage(_("Hello world from wxWidgets!"));
}
