/*
    Copyright 2015-2016 Amebis

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
    EVT_TEXT     (wxZRColaFrame::wxID_COMPOSER, wxZRColaFrame::OnCompose   )
    EVT_UPDATE_UI(wxZRColaFrame::wxID_SEND    , wxZRColaFrame::OnSendUpdate)
    EVT_MENU     (wxZRColaFrame::wxID_SEND    , wxZRColaFrame::OnSend      )
    EVT_MENU     (               wxID_ABOUT   , wxZRColaFrame::OnAbout     )
wxEND_EVENT_TABLE()


wxZRColaFrame::wxZRColaFrame() :
    m_hWndSource(NULL),
    wxAppBarFrame()
{
}


bool wxZRColaFrame::Create()
{
    wxImage::AddHandler(new wxICOHandler);

    wxCHECK(wxAppBarFrame::Create(NULL, wxID_ANY, _("ZRCola"), wxABS_FLOAT, wxABF_ALLOWFLOAT | wxABF_ALLOWDOCKHORIZONTAL | wxABF_ALLOWSIZING | wxABF_HIDETASKBARTABWHENDOCKED, wxDefaultPosition, wxSize(400, 150)), false);

    wxFont fontZRCola(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("00 ZRCola"));

    wxCHECK(m_preview.Create(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxTE_READONLY), false);
    m_preview.SetFont(fontZRCola);

    wxCHECK(m_composer.Create(this, wxID_COMPOSER, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE), false);
    m_composer.SetFont(fontZRCola);

    wxBoxSizer
        *boxH = new wxBoxSizer(wxHORIZONTAL),
        *boxV = new wxBoxSizer(wxVERTICAL);

    boxV->Add(&m_preview,  1, wxEXPAND, 5);
    boxV->Add(&m_composer, 1, wxEXPAND, 5);
    boxH->Add(boxV, 1, wxEXPAND, 5);

    m_toolBar.Create(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL);
    m_toolBar.AddTool(wxID_SEND, _("Send"), wxBitmap(wxIcon(wxICON(send.ico))));
    m_toolBar.Realize();

    boxH->Add(&m_toolBar, 0, wxEXPAND, 5);

    SetSizer(boxH);
    Layout();

    // Register global hotkey(s).
    if (!RegisterHotKey(wxZRColaHKID_INVOKE, MOD_ALT | MOD_CONTROL, 'Z'))
        wxMessageBox(_("ZRCola keyboard shortcut Ctrl+Alt+Z could not be registered. Some functionality will not be available."), _("Warning"), wxOK | wxICON_WARNING);

    // Register frame specific hotkey(s).
    {
        wxAcceleratorEntry entries[1];
        entries[0].Set(wxACCEL_NORMAL, WXK_RETURN, wxID_SEND);
        SetAcceleratorTable(wxAcceleratorTable(_countof(entries), entries));
    }

    return true;
}


bool wxZRColaFrame::Destroy()
{
    // Unregister global hotkey(s).
    UnregisterHotKey(wxZRColaHKID_INVOKE);

    return wxAppBarFrame::Destroy();
}


void wxZRColaFrame::OnCompose(wxCommandEvent& event)
{
    // TODO: Do the real ZRCola composition here.
    m_preview.SetValue(m_composer.GetValue());

    event.Skip();
}


void wxZRColaFrame::OnSendUpdate(wxUpdateUIEvent& event)
{
    event.Enable(m_hWndSource ? true : false);
}


void wxZRColaFrame::OnSend(wxCommandEvent& event)
{
    if (m_hWndSource) {
        // Get text and its length (in Unicode characters). Prepare the INPUT table.
        wxString text = m_preview.GetValue();
        std::vector<INPUT>::size_type i = 0, n = text.length();
        wxString::const_iterator i_text = text.begin();
        std::vector<INPUT> input(n);
        for (; i < n; i++, i_text++) {
            INPUT &inp = input[i];
            inp.type           = INPUT_KEYBOARD;
            inp.ki.wVk         = 0;
            inp.ki.wScan       = *i_text;
            inp.ki.dwFlags     = KEYEVENTF_UNICODE;
            inp.ki.time        = 0;
            inp.ki.dwExtraInfo = 0;
        }

        // Return focus to the source window and send the input.
        ::SetActiveWindow(m_hWndSource);
        ::SetForegroundWindow(m_hWndSource);
        ::SendInput(n, &input[0], sizeof(INPUT));
        m_hWndSource = NULL;

        // Select all input in composer to prepare for the overwrite next time.
        m_composer.SelectAll();
    }

    event.Skip();
}


void wxZRColaFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox(wxString::Format(_("ZRCola v%s\nCopyright 2015-%s Amebis"), wxT(ZRCOLA_VERSION_STR), wxT(ZRCOLA_BUILD_YEAR_STR)), _("About ZRCola"), wxOK | wxICON_INFORMATION);
}


WXLRESULT wxZRColaFrame::MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{
    if (message == WM_HOTKEY && wParam == wxZRColaHKID_INVOKE) {
        // ZRCola hotkey was pressed. Remember the source window and focus ours.
        m_hWndSource = ::GetForegroundWindow();

        m_composer.SetFocus();
        if (m_state == wxABS_FLOAT) {
            if (IsIconized()) {
                ::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
            } else {
                ::SetActiveWindow(m_hWnd);
                ::SetForegroundWindow(m_hWnd);
            }
        } else if (wxAppBarIsDocked(m_state)) {
            // ZRCola window is currently docked.

            if (GetAutoHidden()) {
                // Unhide it.
                ShowAutoHideAppBar();
            }

            ::SetActiveWindow(m_hWnd);
            ::SetForegroundWindow(m_hWnd);
        } else
            wxFAIL_MSG(wxT("unsupported application bar state"));

        return 0;
    } else
        return wxAppBarFrame::MSWWindowProc(message, wParam, lParam);
}
