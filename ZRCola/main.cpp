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


class MyFrame: public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    enum {
        ID_Hello = 1
    };

private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(MyFrame::ID_Hello, MyFrame::OnHello)
    EVT_MENU(wxID_EXIT,         MyFrame::OnExit)
    EVT_MENU(wxID_ABOUT,        MyFrame::OnAbout)
wxEND_EVENT_TABLE()

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size) : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(MyFrame::ID_Hello, _("&Hello...\tShift+H"), _("Help string shown in status bar for this menu item"));
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, _("&File"));
    menuBar->Append(menuHelp, _("&Help"));
    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText(_("Welcome to wxWidgets!"));
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox(_("This is a wxWidgets' Hello world sample"), _("About Hello World"), wxOK | wxICON_INFORMATION);
}

void MyFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage(_("Hello world from wxWidgets!"));
}




class MyApp: public wxApp
{
public:
    virtual bool OnInit();
    wxLocale m_locale;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    if (wxLocale::IsAvailable(wxLANGUAGE_SLOVENIAN)) {
        wxString sPath(wxPathOnly(argv[0]));
        sPath << wxT("\\..\\locale");
        m_locale.AddCatalogLookupPathPrefix(sPath);
        averify(m_locale.Init(wxLANGUAGE_SLOVENIAN));
        averify(m_locale.AddCatalog(wxT("ZRCola")));
    }

    MyFrame *frame = new MyFrame(_("Hello World"), wxPoint(50, 50), wxSize(450, 340));
    frame->Show(true);
    return true;
}



//int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
//{
//    UNREFERENCED_PARAMETER(hInstance);
//    UNREFERENCED_PARAMETER(hPrevInstance);
//    UNREFERENCED_PARAMETER(lpCmdLine);
//    UNREFERENCED_PARAMETER(nCmdShow);
//
//    Sleep(10000);
//
//    {
//        INPUT input[2];
//
//        input[0].type           = INPUT_KEYBOARD;
//        input[0].ki.wVk         = 0;
//        input[0].ki.wScan       = 0x00d2;
//        input[0].ki.dwFlags     = KEYEVENTF_UNICODE;
//        input[0].ki.time        = 0;
//        input[0].ki.dwExtraInfo = 0;
//
//        input[1].type           = INPUT_KEYBOARD;
//        input[1].ki.wVk         = 0;
//        input[1].ki.wScan       = 0x00d2;
//        input[1].ki.dwFlags     = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
//        input[1].ki.time        = 0;
//        input[1].ki.dwExtraInfo = 0;
//
//        SendInput(_countof(input), input, sizeof(INPUT));
//    }
//
//    return 0;
//}
