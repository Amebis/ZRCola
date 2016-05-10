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
    EVT_UPDATE_UI      (wxID_AUTOSTART                                      , wxZRColaFrame::OnAutostartUpdate             )
    EVT_MENU           (wxID_AUTOSTART                                      , wxZRColaFrame::OnAutostart                   )
    EVT_MENU           (wxID_EXIT                                           , wxZRColaFrame::OnExit                        )

    EVT_UPDATE_UI_RANGE(wxID_CUT                  , wxID_CLEAR              , wxZRColaFrame::OnForwardEventUpdate          )
    EVT_MENU_RANGE     (wxID_CUT                  , wxID_CLEAR              , wxZRColaFrame::OnForwardEvent                )
    EVT_UPDATE_UI      (wxID_SELECTALL                                      , wxZRColaFrame::OnForwardEventUpdate          )
    EVT_MENU           (wxID_SELECTALL                                      , wxZRColaFrame::OnForwardEvent                )

    EVT_UPDATE_UI_RANGE(wxID_SEND_COMPOSED        , wxID_SEND_ABORT         , wxZRColaFrame::OnSendUpdate                  )
    EVT_MENU           (wxID_SEND_COMPOSED                                  , wxZRColaFrame::OnSendComposed                )
    EVT_MENU           (wxID_SEND_DECOMPOSED                                , wxZRColaFrame::OnSendDecomposed              )
    EVT_MENU           (wxID_SEND_ABORT                                     , wxZRColaFrame::OnSendAbort                   )

    EVT_UPDATE_UI      (wxID_DECOMP_LANG_AUTO                               , wxZRColaFrame::OnDecomposedLanguageAutoUpdate)
    EVT_MENU           (wxID_DECOMP_LANG_AUTO                               , wxZRColaFrame::OnDecomposedLanguageAuto      )
    EVT_UPDATE_UI_RANGE(wxID_DECOMP_LANGUAGE_START, wxID_DECOMP_LANGUAGE_END, wxZRColaFrame::OnDecomposedLanguageUpdate    )
    EVT_MENU_RANGE     (wxID_DECOMP_LANGUAGE_START, wxID_DECOMP_LANGUAGE_END, wxZRColaFrame::OnDecomposedLanguage          )

    EVT_UPDATE_UI      (wxID_TOOLBAR_EDIT                                   , wxZRColaFrame::OnToolbarEditUpdate           )
    EVT_MENU           (wxID_TOOLBAR_EDIT                                   , wxZRColaFrame::OnToolbarEdit                 )
    EVT_UPDATE_UI      (wxID_TOOLBAR_COMPOSE                                , wxZRColaFrame::OnToolbarComposeUpdate        )
    EVT_MENU           (wxID_TOOLBAR_COMPOSE                                , wxZRColaFrame::OnToolbarCompose              )
    EVT_UPDATE_UI      (wxID_PANEL_CHRGRPS                                  , wxZRColaFrame::OnPanelCharacterCatalogUpdate )
    EVT_MENU           (wxID_PANEL_CHRGRPS                                  , wxZRColaFrame::OnPanelCharacterCatalog       )
    EVT_MENU           (wxID_FOCUS_CHARACTER_CATALOG                        , wxZRColaFrame::OnPanelCharacterCatalogFocus  )

    EVT_MENU           (wxID_ABOUT                                          , wxZRColaFrame::OnAbout                       )
wxEND_EVENT_TABLE()


wxZRColaFrame::wxZRColaFrame() :
    m_lang_auto(true),
    m_hWndSource(NULL),
    wxZRColaFrameBase(NULL)
{
    {
        // wxFrameBuilder 3.5 does not support wxAUI_TB_HORIZONTAL flag. Add it manually.
        wxAuiPaneInfo &paneInfo = m_mgr.GetPane(m_toolbarCompose);
        paneInfo.LeftDockable(false);
        paneInfo.RightDockable(false);
        m_toolbarCompose->SetWindowStyleFlag(m_toolbarCompose->GetWindowStyleFlag() | wxAUI_TB_HORIZONTAL);
    }

    // Restore the wxAuiManager's state here to keep symmetric with save in the destructor below.
    // See the comment in destructor why.
    wxPersistentAuiManager(&m_mgr).Restore();

    // Load main window icons.
#ifdef __WINDOWS__
    wxIcon icon_small(wxT("00_zrcola.ico"), wxBITMAP_TYPE_ICO_RESOURCE, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
    wxIconBundle icons;
    icons.AddIcon(icon_small);
    icons.AddIcon(wxIcon(wxT("00_zrcola.ico"), wxBITMAP_TYPE_ICO_RESOURCE, ::GetSystemMetrics(SM_CXICON  ), ::GetSystemMetrics(SM_CYICON  )));
    SetIcons(icons);
#else
    wxIcon icon_small(wxICON(00_zrcola.ico));
    SetIcon(icon_small);
#endif

    m_taskBarIcon = new wxTaskBarIcon();
    if (m_taskBarIcon->IsOk()) {
        m_taskBarIcon->SetIcon(icon_small, _("ZRCola"));
        m_taskBarIcon->Connect(wxEVT_TASKBAR_LEFT_DOWN, wxTaskBarIconEventHandler(wxZRColaFrame::OnTaskbarIconClick), NULL, this);
    } else {
        // Taskbar icon creation failed. Not the end of the world. No taskbar icon then.
        delete m_taskBarIcon;
    }

    {
        // Populate language lists.
        memcpy(m_lang, ZRCOLA_LANG_VOID, sizeof(m_lang));
        ZRColaApp *app = ((ZRColaApp*)wxTheApp);
        m_toolDecompLanguage->Clear();
        wxString label1_tran(_("Select %s language for decomposition"));
        for (size_t i = 0, n = app->m_lang_db.idxLng.size(); i < n; i++) {
            const ZRCola::language_db::language &lang = app->m_lang_db.idxLng[i];
            wxString
                label(lang.name, lang.name_len),
                label_tran2(wxGetTranslation(label, wxT("ZRCola-zrcdb")));
            if (i < wxID_DECOMP_LANGUAGE_END - wxID_DECOMP_LANGUAGE_START + 1)
                m_menuDecompLanguage->AppendRadioItem(wxID_DECOMP_LANGUAGE_START + i, label_tran2, wxString::Format(label1_tran, (const wxStringCharType*)label_tran2));
            m_toolDecompLanguage->Insert(label_tran2, i);
            if (memcmp(m_lang, lang.id, sizeof(m_lang)) == 0)
                m_toolDecompLanguage->Select(i);
        }
    }

    // Set focus.
    m_panel->m_decomposed->SetFocus();

    // Register global hotkey(s).
    if (!RegisterHotKey(wxZRColaHKID_INVOKE_COMPOSE, wxMOD_WIN, VK_F5))
        wxMessageBox(_("ZRCola keyboard shortcut Win+F5 could not be registered. Some functionality will not be available."), _("Warning"), wxOK | wxICON_WARNING);
    if (!RegisterHotKey(wxZRColaHKID_INVOKE_DECOMPOSE, wxMOD_WIN, VK_F6))
        wxMessageBox(_("ZRCola keyboard shortcut Win+F6 could not be registered. Some functionality will not be available."), _("Warning"), wxOK | wxICON_WARNING);

#if defined(__WXMSW__)
    // Register notification sink for language detection.
    m_ulRefCount = 1;
    m_tfSource = NULL;
    ITfInputProcessorProfiles *pProfiles;
    HRESULT hr = CoCreateInstance(CLSID_TF_InputProcessorProfiles, NULL, CLSCTX_INPROC_SERVER, IID_ITfInputProcessorProfiles, (LPVOID*)&pProfiles);
    if(SUCCEEDED(hr)) {
        hr = pProfiles->QueryInterface(IID_ITfSource, (LPVOID*)&m_tfSource);
        if(SUCCEEDED(hr)) {
            hr = m_tfSource->AdviseSink(IID_ITfLanguageProfileNotifySink, (ITfLanguageProfileNotifySink*)this, &m_dwCookie);
            if (FAILED(hr) || m_dwCookie == -1) {
                m_tfSource->Release();
                m_tfSource = NULL;
            }
        }

        pProfiles->Release();
    }
#endif

    // Register frame specific hotkey(s).
    {
        wxAcceleratorEntry entries[1];
        entries[0].Set(wxACCEL_NORMAL, WXK_F4, wxID_FOCUS_CHARACTER_CATALOG);
        SetAcceleratorTable(wxAcceleratorTable(_countof(entries), entries));
    }
}


wxZRColaFrame::~wxZRColaFrame()
{
#if defined(__WXMSW__)
    if (m_tfSource) {
        m_tfSource->UnadviseSink(m_dwCookie);
        m_tfSource->Release();
    }
#endif

    // Unregister global hotkey(s).
    UnregisterHotKey(wxZRColaHKID_INVOKE_DECOMPOSE);
    UnregisterHotKey(wxZRColaHKID_INVOKE_COMPOSE);

    if (m_taskBarIcon) {
        m_taskBarIcon->Disconnect(wxEVT_TASKBAR_LEFT_DOWN, wxTaskBarIconEventHandler(wxZRColaFrame::OnTaskbarIconClick), NULL, this);
        delete m_taskBarIcon;
    }

    // Save wxAuiManager's state before return to parent's destructor.
    // Since the later calls m_mgr.UnInit() the regular persistence mechanism is useless to save wxAuiManager's state.
    wxPersistentAuiManager((wxAuiManager*)&m_mgr).Save();
}


void wxZRColaFrame::OnAutostartUpdate(wxUpdateUIEvent& event)
{
#if defined(__WXMSW__)
    wxString linkName(wxExpandEnvVars("%APPDATA%\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\ZRCola.lnk"));
    event.Check(wxFileExists(linkName));
#else
    event.Enable(false);
#endif
}


void wxZRColaFrame::OnAutostart(wxCommandEvent& event)
{
#if defined(__WXMSW__)
    wxString linkName(wxExpandEnvVars("%APPDATA%\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\ZRCola.lnk"));
    if (wxFileExists(linkName)) {
        // The shortcut already exists. Remove it.
        wxRemoveFile(linkName);
    } else {
        // Create the shortcut.
        IShellLink *sl;
        HRESULT hr = ::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&sl);
        if (SUCCEEDED(hr)) {
            // Setup ZRCola shortcut.
            sl->SetPath(wxTheApp->argv[0]);
            sl->SetDescription(_("Start ZRCola automatically on logon"));
            sl->SetShowCmd(SW_SHOWMINNOACTIVE);

            // Query IShellLink for the IPersistFile interface, used for saving the
            // shortcut in persistent storage.
            IPersistFile *pf;
            hr = sl->QueryInterface(IID_IPersistFile, (LPVOID*)&pf);
            if (SUCCEEDED(hr)) {
                // Save the link by calling IPersistFile::Save.
                hr = pf->Save(linkName, TRUE);
                pf->Release();
            }

            sl->Release();
        }
    }
#endif
}


void wxZRColaFrame::OnExit(wxCommandEvent& event)
{
    Close();
}


void wxZRColaFrame::OnForwardEventUpdate(wxUpdateUIEvent& event)
{
    wxControl *focusWnd = wxDynamicCast(FindFocus(), wxControl);
    if (focusWnd && focusWnd->IsKindOf(wxCLASSINFO(wxTextCtrl)))
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


void wxZRColaFrame::OnDecomposedLanguageAutoUpdate(wxUpdateUIEvent& event)
{
#if defined(__WXMSW__)
    event.Check(m_lang_auto);
#else
    event.Enable(false);
#endif
}


void wxZRColaFrame::OnDecomposedLanguageAuto(wxCommandEvent& event)
{
    // Toggle auto language flag.
    m_lang_auto = !m_lang_auto;

    if (m_lang_auto) {
#if defined(__WXMSW__)
        // Set keyboard language.
        HKL hkl = ::GetKeyboardLayout(0);
        ZRCola::LangConvert(LOWORD(hkl), m_lang);
        UpdateDecomposedLanguage();
#endif
    }
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
        m_lang_auto = false;
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
        m_lang_auto = false;
    }

    event.Skip();
}


void wxZRColaFrame::OnIdle(wxIdleEvent& event)
{
    m_panel->SynchronizePanels();

    event.Skip();
}


void wxZRColaFrame::OnTaskbarIconClick(wxTaskBarIconEvent& event)
{
    Iconize(false);
    Show(true);
    Raise();

    event.Skip();
}


void wxZRColaFrame::OnIconize(wxIconizeEvent& event)
{
    if (m_taskBarIcon)
        Show(!event.IsIconized());

    event.Skip();
}


void wxZRColaFrame::OnToolbarEditUpdate(wxUpdateUIEvent& event)
{
    event.Check(m_mgr.GetPane(m_toolbarEdit).IsShown());
}


void wxZRColaFrame::OnToolbarEdit(wxCommandEvent& event)
{
    wxAuiPaneInfo &paneInfo = m_mgr.GetPane(m_toolbarEdit);
    paneInfo.Show(!paneInfo.IsShown());
    m_mgr.Update();
}


void wxZRColaFrame::OnToolbarComposeUpdate(wxUpdateUIEvent& event)
{
    event.Check(m_mgr.GetPane(m_toolbarCompose).IsShown());
}


void wxZRColaFrame::OnToolbarCompose(wxCommandEvent& event)
{
    wxAuiPaneInfo &paneInfo = m_mgr.GetPane(m_toolbarCompose);
    paneInfo.Show(!paneInfo.IsShown());
    m_mgr.Update();
}


void wxZRColaFrame::OnPanelCharacterCatalogUpdate(wxUpdateUIEvent& event)
{
    event.Check(m_mgr.GetPane(m_panelChrCat).IsShown());
}


void wxZRColaFrame::OnPanelCharacterCatalog(wxCommandEvent& event)
{
    wxAuiPaneInfo &paneInfo = m_mgr.GetPane(m_panelChrCat);
    paneInfo.Show(!paneInfo.IsShown());
    m_mgr.Update();
}


void wxZRColaFrame::OnPanelCharacterCatalogFocus(wxCommandEvent& event)
{
    wxAuiPaneInfo &paneInfo = m_mgr.GetPane(m_panelChrCat);
    if (!paneInfo.IsShown()) {
        paneInfo.Show(true);
        m_mgr.Update();
    }

    m_panelChrCat->SetFocus();
}


void wxZRColaFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox(wxString::Format(_("ZRCola v%s\nCopyright 2015-%s Amebis"), wxT(ZRCOLA_VERSION_STR), wxT(ZRCOLA_BUILD_YEAR_STR)), _("About ZRCola"), wxOK | wxICON_INFORMATION);
}


#if defined(__WXMSW__)

HRESULT STDMETHODCALLTYPE wxZRColaFrame::OnLanguageChange(LANGID langid, __RPC__out BOOL *pfAccept)
{
    if (pfAccept) *pfAccept = TRUE;
    return S_OK;
}


HRESULT STDMETHODCALLTYPE wxZRColaFrame::OnLanguageChanged()
{
    if (m_lang_auto) {
        // Set keyboard language.
        HKL hkl = ::GetKeyboardLayout(0);
        ZRCola::LangConvert(LOWORD(hkl), m_lang);
        UpdateDecomposedLanguage();
    }

    return S_OK;
}


HRESULT STDMETHODCALLTYPE wxZRColaFrame::QueryInterface(REFIID riid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject)
{
    if (!ppvObject)
        return E_INVALIDARG;

    if (riid == IID_IUnknown)
        *ppvObject = static_cast<IUnknown*>(this);
    else if (riid == IID_ITfLanguageProfileNotifySink)
        *ppvObject = static_cast<ITfLanguageProfileNotifySink*>(this);
    else {
        *ppvObject = NULL;
        return E_NOINTERFACE;
    }

    AddRef();
    return NOERROR;
}


ULONG STDMETHODCALLTYPE wxZRColaFrame::AddRef()
{
    InterlockedIncrement(&m_ulRefCount);
    return m_ulRefCount;
}


ULONG STDMETHODCALLTYPE wxZRColaFrame::Release()
{
    // Decrement the object's internal counter.
    ULONG ulRefCount = InterlockedDecrement(&m_ulRefCount);
    if (m_ulRefCount == 0)
        delete this;
    return ulRefCount;
}

#endif


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


void wxZRColaFrame::UpdateDecomposedLanguage()
{
    ZRColaApp *app = ((ZRColaApp*)wxTheApp);

    // Find language on the language list.
    ZRCola::language_db::language *l = new ZRCola::language_db::language;
    memcpy(l->id, m_lang, sizeof(l->id));
    l->name_len = 0;
    ZRCola::language_db::indexLang::size_type start, end;
    m_toolDecompLanguage->SetSelection(app->m_lang_db.idxLng.find(*l, start, end) ? start : -1);
    delete l;
}


#ifdef __WXMSW__

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

#endif


//////////////////////////////////////////////////////////////////////////
// wxPersistentZRColaFrame
//////////////////////////////////////////////////////////////////////////

wxPersistentZRColaFrame::wxPersistentZRColaFrame(wxZRColaFrame *wnd) : wxPersistentTLW(wnd)
{
}


void wxPersistentZRColaFrame::Save() const
{
    const wxZRColaFrame * const wnd = static_cast<const wxZRColaFrame*>(GetWindow());

    wxPersistentZRColaComposerPanel(wnd->m_panel).Save();
    wxPersistentZRColaCharacterCatalogPanel(wnd->m_panelChrCat).Save();

    SaveValue(wxT("langAuto" ), wnd->m_lang_auto);
    SaveValue(wxT("lang"     ), wxString::FromAscii(wnd->m_lang, sizeof(wnd->m_lang)));
    wxPersistentTLW::Save();
}


bool wxPersistentZRColaFrame::Restore()
{
    const bool r = wxPersistentTLW::Restore();

    wxZRColaFrame * const wnd = static_cast<wxZRColaFrame*>(GetWindow());

    ZRColaApp *app = ((ZRColaApp*)wxTheApp);
    wxString lang;

    // Restore automatic language detection setting first.
    RestoreValue(wxT("langAuto"), &(wnd->m_lang_auto));
    if (wnd->m_lang_auto) {
#if defined(__WXMSW__)
        // Set keyboard language.
        HKL hkl = ::GetKeyboardLayout(0);
        ZRCola::LangConvert(LOWORD(hkl), wnd->m_lang);
#endif
    } else if (RestoreValue(wxT("lang"), &lang) && lang.Length() == 3) {
        // The language was read from configuration.
        memcpy(wnd->m_lang, (const char*)lang.c_str(), sizeof(wnd->m_lang));
    } else if (!app->m_lang_db.idxLng.empty()) {
        const ZRCola::language_db::language &lang = app->m_lang_db.idxLng[0];
        memcpy(wnd->m_lang, lang.id, sizeof(wnd->m_lang));
    } else
        memcpy(wnd->m_lang, ZRCOLA_LANG_VOID, sizeof(wnd->m_lang));
    wnd->UpdateDecomposedLanguage();

    wxPersistentZRColaCharacterCatalogPanel(wnd->m_panelChrCat).Restore();
    wxPersistentZRColaComposerPanel(wnd->m_panel).Restore();

    return r;
}
