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
    EVT_UPDATE_UI_RANGE(wxID_CUT, wxID_CLEAR, wxZRColaFrame::OnForwardEventUpdate)
    EVT_MENU_RANGE(wxID_CUT, wxID_CLEAR, wxZRColaFrame::OnForwardEvent)
    EVT_UPDATE_UI(wxID_SELECTALL, wxZRColaFrame::OnForwardEventUpdate)
    EVT_MENU(wxID_SELECTALL, wxZRColaFrame::OnForwardEvent)

    EVT_UPDATE_UI_RANGE(wxID_SEND_COMPOSED, wxID_SEND_ABORT, wxZRColaFrame::OnSendUpdate)
    EVT_MENU(wxID_SEND_COMPOSED  , wxZRColaFrame::OnSendComposed            )
    EVT_MENU(wxID_SEND_DECOMPOSED, wxZRColaFrame::OnSendDecomposed          )
    EVT_MENU(wxID_SEND_ABORT     , wxZRColaFrame::OnSendAbort               )

    EVT_UPDATE_UI_RANGE(wxID_DECOMP_LANGUAGE_START, wxID_DECOMP_LANGUAGE_END, wxZRColaFrame::OnDecomposedLanguageUpdate)
    EVT_MENU_RANGE(wxID_DECOMP_LANGUAGE_START, wxID_DECOMP_LANGUAGE_END, wxZRColaFrame::OnDecomposedLanguage)

    EVT_MENU(wxID_EXIT , wxZRColaFrame::OnExit )
    EVT_MENU(wxID_ABOUT, wxZRColaFrame::OnAbout)
wxEND_EVENT_TABLE()


wxZRColaFrame::wxZRColaFrame() :
    m_hWndSource(NULL),
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

    // Populate language lists.
    memcpy(m_lang, ZRCOLA_LANG_VOID, sizeof(m_lang));
    ZRColaApp *app = ((ZRColaApp*)wxTheApp);
    m_toolDecompLanguage->Clear();
    for (size_t i = 0, n = app->m_lang_db.idxLng.size(); i < n; i++) {
        const ZRCola::language_db::language &lang = app->m_lang_db.idxLng[i];
        wxString
            label(lang.name, lang.name_len),
            label_tran(wxGetTranslation(label));
        if (i < wxID_DECOMP_LANGUAGE_END - wxID_DECOMP_LANGUAGE_START + 1)
            m_menuDecompLanguage->Insert(i, wxID_DECOMP_LANGUAGE_START + i, label_tran, wxString::Format(_("Select %s language for decomposition"), (const wxStringCharType*)label_tran), wxITEM_RADIO);
        m_toolDecompLanguage->Insert(label_tran, i);
        if (memcmp(m_lang, lang.id, sizeof(m_lang)) == 0)
            m_toolDecompLanguage->Select(i);
    }

    // Set focus.
    m_panel->m_decomposed->SetFocus();

    // Register global hotkey(s).
    if (!RegisterHotKey(wxZRColaHKID_INVOKE_COMPOSE, wxMOD_WIN, VK_F5))
        wxMessageBox(_("ZRCola keyboard shortcut Win+F5 could not be registered. Some functionality will not be available."), _("Warning"), wxOK | wxICON_WARNING);
    if (!RegisterHotKey(wxZRColaHKID_INVOKE_DECOMPOSE, wxMOD_WIN, VK_F6))
        wxMessageBox(_("ZRCola keyboard shortcut Win+F6 could not be registered. Some functionality will not be available."), _("Warning"), wxOK | wxICON_WARNING);
}


wxZRColaFrame::~wxZRColaFrame()
{
    // Unregister global hotkey(s).
    UnregisterHotKey(wxZRColaHKID_INVOKE_DECOMPOSE);
    UnregisterHotKey(wxZRColaHKID_INVOKE_COMPOSE);
}


void wxZRColaFrame::OnForwardEventUpdate(wxUpdateUIEvent& event)
{
    wxControl *focusWnd = wxDynamicCast(FindFocus(), wxControl);
    if (focusWnd && !m_toolbar->IsDescendant(focusWnd))
        focusWnd->GetEventHandler()->ProcessEvent(event);
    else
        event.Enable(false);
}


void wxZRColaFrame::OnForwardEvent(wxCommandEvent& event)
{
    wxControl *focusWnd = wxDynamicCast(FindFocus(), wxControl);
    if (focusWnd)
        focusWnd->GetEventHandler()->ProcessEvent(event);
    else
        event.Skip();
}


void wxZRColaFrame::OnSendUpdate(wxUpdateUIEvent& event)
{
    event.Enable(m_hWndSource ? true : false);
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
    }

    // Select all input in composer to prepare for the overwrite next time.
    m_panel->m_decomposed->SelectAll();
    m_panel->m_composed->SelectAll();

    event.Skip();
}


void wxZRColaFrame::OnDecomposedLanguageUpdate(wxUpdateUIEvent& event)
{
    ZRColaApp *app = ((ZRColaApp*)wxTheApp);
    const ZRCola::language_db::language &lang = app->m_lang_db.idxLng[event.GetId() - wxID_DECOMP_LANGUAGE_START];
    event.Check(memcmp(m_lang, lang.id, sizeof(m_lang)) == 0);
}


void wxZRColaFrame::OnDecomposedLanguage(wxCommandEvent& event)
{
    ZRColaApp *app = ((ZRColaApp*)wxTheApp);
    size_t i = event.GetId() - wxID_DECOMP_LANGUAGE_START;
    const ZRCola::language_db::language &lang = app->m_lang_db.idxLng[i];

    if (memcmp(m_lang, lang.id, sizeof(m_lang)) != 0) {
        memcpy(m_lang, lang.id, sizeof(m_lang));
        m_toolDecompLanguage->Select(i);

        // Notify composed text something changed and should re-decompose.
        wxCommandEvent event2(wxEVT_COMMAND_TEXT_UPDATED);
        m_panel->m_composed->ProcessWindowEvent(event2);
    }
}


void wxZRColaFrame::OnDecompLanguageChoice(wxCommandEvent& event)
{
    ZRColaApp *app = ((ZRColaApp*)wxTheApp);
    size_t i = event.GetSelection();
    const ZRCola::language_db::language &lang = app->m_lang_db.idxLng[i];

    if (memcmp(m_lang, lang.id, sizeof(m_lang)) != 0) {
        memcpy(m_lang, lang.id, sizeof(m_lang));

        // Notify composed text something changed and should re-decompose.
        wxCommandEvent event2(wxEVT_COMMAND_TEXT_UPDATED);
        m_panel->m_composed->ProcessWindowEvent(event2);
    }
}


void wxZRColaFrame::OnExit(wxCommandEvent& event)
{
    Close();
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
    std::vector<INPUT> input;
    input.reserve(n*2);
    for (std::vector<INPUT>::size_type i = 0; i < n; i++, i_str++) {
        wxString::char_type c = *i_str;

        // Add key down event.
        INPUT inp = { INPUT_KEYBOARD };
        inp.ki.dwFlags = KEYEVENTF_UNICODE;
        inp.ki.wScan = c != L'\n' ? c : L'\r'; // Enter (Return) key is sent as CR.
        input.push_back(inp);

        // Add key up event.
        inp.ki.dwFlags |= KEYEVENTF_KEYUP;
        input.push_back(inp);
    }

    // Return focus to the source window and send the input.
    ::SetActiveWindow(m_hWndSource);
    ::SetForegroundWindow(m_hWndSource);
    ::Sleep(200);
    ::SendInput(input.size(), input.data(), sizeof(INPUT));
    m_hWndSource = NULL;

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


//////////////////////////////////////////////////////////////////////////
// wxPersistentZRColaFrame
//////////////////////////////////////////////////////////////////////////

wxPersistentZRColaFrame::wxPersistentZRColaFrame(wxZRColaFrame *wnd) : wxPersistentTLW(wnd)
{
}


void wxPersistentZRColaFrame::Save() const
{
    //
    const wxZRColaFrame * const wnd = static_cast<const wxZRColaFrame*>(GetWindow());

    SaveValue(wxT("lang"), wxString::FromAscii(wnd->m_lang, sizeof(wnd->m_lang)));
    wxPersistentTLW::Save();
}


bool wxPersistentZRColaFrame::Restore()
{
    const bool r = wxPersistentTLW::Restore();

    wxZRColaFrame * const wnd = static_cast<wxZRColaFrame*>(GetWindow());

    ZRColaApp *app = ((ZRColaApp*)wxTheApp);
    wxString lang;
    if (RestoreValue(wxT("lang"), &lang) && lang.Length() == 3) {
        memcpy(wnd->m_lang, (const char*)lang.c_str(), sizeof(wnd->m_lang));

        ZRCola::language_db::language *lang = new ZRCola::language_db::language;
        memcpy(lang->id, wnd->m_lang, sizeof(lang->id));
        lang->name_len = 0;
        ZRCola::language_db::indexLang::size_type start, end;
        wnd->m_toolDecompLanguage->SetSelection(app->m_lang_db.idxLng.find(*lang, start, end) ? start : -1);
        delete lang;
    } else {
        if (!app->m_lang_db.idxLng.empty()) {
            const ZRCola::language_db::language &lang = app->m_lang_db.idxLng[0];
            memcpy(wnd->m_lang, lang.id, sizeof(wnd->m_lang));
            wnd->m_toolDecompLanguage->Select(0);
        } else {
            memcpy(wnd->m_lang, ZRCOLA_LANG_VOID, sizeof(wnd->m_lang));
        }
    }

    return r;
}
