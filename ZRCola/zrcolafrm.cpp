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

wxBEGIN_EVENT_TABLE(wxZRColaFrame, wxZRColaFrameBase)
    EVT_UPDATE_UI_RANGE(wxZRColaFrame::wxID_SEND, wxZRColaFrame::wxID_SEND_DECOMPOSED, wxZRColaFrame::OnSendUpdate)

    EVT_MENU(wxZRColaFrame::wxID_SEND                  , wxZRColaFrame::OnSend                    )
    EVT_MENU(wxZRColaFrame::wxID_SEND_COMPOSED         , wxZRColaFrame::OnSendComposed            )
    EVT_MENU(wxZRColaFrame::wxID_SEND_DECOMPOSED       , wxZRColaFrame::OnSendDecomposed          )
    EVT_MENU(wxZRColaFrame::wxID_SEND_ABORT            , wxZRColaFrame::OnSendAbort               )
    EVT_MENU(               wxID_EXIT                  , wxZRColaFrame::OnExit                    )
    EVT_MENU(               wxID_INSERT_GRAVE          , wxZRColaFrame::OnInsertGrave             )
    EVT_MENU(               wxID_INSERT_ACUTE          , wxZRColaFrame::OnInsertAcute             )
    EVT_MENU(               wxID_INSERT_CIRCUMFLEX     , wxZRColaFrame::OnInsertCircumflex        )
    EVT_MENU(               wxID_INSERT_INV_BREVE_BELOW, wxZRColaFrame::OnInsertInvertedBreveBelow)
    EVT_MENU(               wxID_ABOUT                 , wxZRColaFrame::OnAbout                   )
wxEND_EVENT_TABLE()


wxZRColaFrame::wxZRColaFrame() :
    m_hWndSource(NULL),
    m_hotkey(-1),
    wxZRColaFrameBase(NULL)
{
    // Load main window icons.
#ifdef __WINDOWS__
    wxIconBundle icons;
    icons.AddIcon(wxIcon(wxT("00_zrcola.ico"), wxBITMAP_TYPE_ICO_RESOURCE, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON)));
    icons.AddIcon(wxIcon(wxT("00_zrcola.ico"), wxBITMAP_TYPE_ICO_RESOURCE, ::GetSystemMetrics(SM_CXICON  ), ::GetSystemMetrics(SM_CYICON  )));
    SetIcons(icons);
#else
    SetIcon(wxICON(00_zrcola.ico));
#endif

    m_panel->m_decomposed->SetFocus();

    // Register global hotkey(s).
    if (!RegisterHotKey(wxZRColaHKID_INVOKE_COMPOSE, MOD_WIN, 'Z'))
        wxMessageBox(_("ZRCola keyboard shortcut Win+Z could not be registered. Some functionality will not be available."), _("Warning"), wxOK | wxICON_WARNING);
    if (!RegisterHotKey(wxZRColaHKID_INVOKE_DECOMPOSE, MOD_WIN | MOD_SHIFT, 'Z'))
        wxMessageBox(_("ZRCola keyboard shortcut Win+Shift+Z could not be registered. Some functionality will not be available."), _("Warning"), wxOK | wxICON_WARNING);

    // Register frame specific hotkey(s).
    {
        wxAcceleratorEntry entries[2];
        entries[0].Set(wxACCEL_CTRL  , WXK_RETURN, wxID_SEND);
        entries[1].Set(wxACCEL_NORMAL, WXK_ESCAPE, wxID_SEND_ABORT);
        SetAcceleratorTable(wxAcceleratorTable(_countof(entries), entries));
    }
}


wxZRColaFrame::~wxZRColaFrame()
{
    // Unregister global hotkey(s).
    UnregisterHotKey(wxZRColaHKID_INVOKE_DECOMPOSE);
    UnregisterHotKey(wxZRColaHKID_INVOKE_COMPOSE);
}


void wxZRColaFrame::OnSendUpdate(wxUpdateUIEvent& event)
{
    event.Enable(m_hWndSource ? true : false);
}


void wxZRColaFrame::OnSend(wxCommandEvent& event)
{
    switch (m_hotkey) {
    case wxZRColaHKID_INVOKE_COMPOSE  : wxZRColaFrame::OnSendComposed  (event); break;
    case wxZRColaHKID_INVOKE_DECOMPOSE: wxZRColaFrame::OnSendDecomposed(event); break;
    default                           : event.Skip();
    }
}


void wxZRColaFrame::OnSendComposed(wxCommandEvent& event)
{
    if (m_hWndSource)
        DoSend(m_panel->m_composed->GetValue());

    event.Skip();
}


void wxZRColaFrame::OnSendDecomposed(wxCommandEvent& event)
{
    if (m_hWndSource)
        DoSend(m_panel->m_decomposed->GetValue());

    event.Skip();
}


void wxZRColaFrame::OnSendAbort(wxCommandEvent& event)
{
    if (m_hWndSource) {
        // Return focus to the source window.
        ::SetActiveWindow(m_hWndSource);
        ::SetForegroundWindow(m_hWndSource);
        m_hWndSource = NULL;
        m_hotkey = -1;

        // Select all input in composer to prepare for the overwrite next time.
        m_panel->m_decomposed->SelectAll();
        m_panel->m_composed->SelectAll();
    }

    event.Skip();
}


void wxZRColaFrame::OnExit(wxCommandEvent& event)
{
    Close();
}


void wxZRColaFrame::OnInsertGrave(wxCommandEvent& event)
{
    m_panel->m_decomposed->WriteText(L"\xE000");
}


void wxZRColaFrame::OnInsertAcute(wxCommandEvent& event)
{
    m_panel->m_decomposed->WriteText(L"\xE001");
}


void wxZRColaFrame::OnInsertCircumflex(wxCommandEvent& event)
{
    m_panel->m_decomposed->WriteText(L"\xE002");
}


void wxZRColaFrame::OnInsertInvertedBreveBelow(wxCommandEvent& event)
{
    m_panel->m_decomposed->WriteText(L"\xE02F");
}


void wxZRColaFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox(wxString::Format(_("ZRCola v%s\nCopyright 2015-%s Amebis"), wxT(ZRCOLA_VERSION_STR), wxT(ZRCOLA_BUILD_YEAR_STR)), _("About ZRCola"), wxOK | wxICON_INFORMATION);
}


void wxZRColaFrame::DoSend(const wxString& str)
{
    // Prepare the INPUT table.
    wxString::size_type n = str.length();
    wxString::const_iterator i_str = str.begin();
    std::vector<INPUT> input(n);
    for (std::vector<INPUT>::size_type i = 0; i < n; i++, i_str++) {
        wxString::char_type c = *i_str;
        INPUT &inp = input[i];
        inp.type           = INPUT_KEYBOARD;
        inp.ki.dwFlags     = KEYEVENTF_UNICODE;
        inp.ki.time        = 0;
        inp.ki.dwExtraInfo = 0;
        if (c == L'\n') {
            // Enter (Return) key is sent as CR virtual key code.
            inp.ki.wVk   = VK_RETURN;
            inp.ki.wScan = L'\r';
        } else if (L'a' <= c && c <= L'z') {
            // Small letters have the same virtual key code as their uppercase counterparts.
            inp.ki.wVk   = (WORD)c + L'A' - L'a';
            inp.ki.wScan = c;
        } else if (L'A' <= c && c <= L'Z' || L'0' <= c && c <= L'0' || c == L' ') {
            // Letters and symbols with matching virtual key codes.
            inp.ki.wVk   = c;
            inp.ki.wScan = c;
        } else {
            inp.ki.wVk   = 0;
            inp.ki.wScan = c;
        }
    }

    // Return focus to the source window and send the input.
    ::SetActiveWindow(m_hWndSource);
    ::SetForegroundWindow(m_hWndSource);
    ::Sleep(200);
    ::SendInput(input.size(), input.data(), sizeof(INPUT));
    m_hWndSource = NULL;
    m_hotkey = -1;

    // Select all input in composer and decomposed to prepare for the overwrite next time.
    m_panel->m_decomposed->SelectAll();
    m_panel->m_composed->SelectAll();
}


WXLRESULT wxZRColaFrame::MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{
    if (message == WM_HOTKEY) {
        // ZRCola hotkey was pressed. Remember the source window and focus ours.
        WXHWND hWndSource = ::GetForegroundWindow();

        switch (wParam) {
        case wxZRColaHKID_INVOKE_COMPOSE  : m_panel->m_decomposed->SetFocus(); break;
        case wxZRColaHKID_INVOKE_DECOMPOSE: m_panel->m_composed  ->SetFocus(); break;
        default:
            wxFAIL_MSG(wxT("not our registered shortcut"));
            return wxZRColaFrameBase::MSWWindowProc(message, wParam, lParam);
        }

        if (hWndSource == m_hWnd) {
            // This is our window user pressed the hotkey (again).
            return wxZRColaFrameBase::MSWWindowProc(message, wParam, lParam);
        }

        m_hWndSource = hWndSource;
        m_hotkey = wParam;

        //if (m_state == wxABS_FLOAT) {
            if (IsIconized()) {
                ::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
            } else {
                ::SetActiveWindow(m_hWnd);
                ::SetForegroundWindow(m_hWnd);
            }
        //} else if (wxAppBarIsDocked(m_state)) {
        //    // ZRCola window is currently docked.

        //    if (GetAutoHidden()) {
        //        // Unhide it.
        //        ShowAutoHideAppBar();
        //    }

        //    ::SetActiveWindow(m_hWnd);
        //    ::SetForegroundWindow(m_hWnd);
        //} else
        //    wxFAIL_MSG(wxT("unsupported application bar state"));

        return 0;
    } else
        return wxZRColaFrameBase::MSWWindowProc(message, wParam, lParam);
}
