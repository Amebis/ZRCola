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
    EVT_MENU           (wxID_EXIT                                  , wxZRColaFrame::OnExit                       )

    EVT_UPDATE_UI_RANGE(wxID_CUT                  , wxID_CLEAR     , wxZRColaFrame::OnForwardEventUpdate         )
    EVT_MENU_RANGE     (wxID_CUT                  , wxID_CLEAR     , wxZRColaFrame::OnForwardEvent               )
    EVT_UPDATE_UI      (wxID_SELECTALL                             , wxZRColaFrame::OnForwardEventUpdate         )
    EVT_MENU           (wxID_SELECTALL                             , wxZRColaFrame::OnForwardEvent               )

    EVT_MENU           (wxID_CHARACTER_SELECTOR                    , wxZRColaFrame::OnInsertCharacter            )

    EVT_UPDATE_UI_RANGE(wxID_SEND_DESTINATION     , wxID_SEND_ABORT, wxZRColaFrame::OnSendUpdate                 )
    EVT_MENU           (wxID_COPY_DESTINATION_AND_RETURN           , wxZRColaFrame::OnCopyDestinationAndReturn   )
    EVT_MENU           (wxID_SEND_DESTINATION                      , wxZRColaFrame::OnSendDestination            )
    EVT_MENU           (wxID_COPY_SOURCE_AND_RETURN                , wxZRColaFrame::OnCopySourceAndReturn        )
    EVT_MENU           (wxID_SEND_SOURCE                           , wxZRColaFrame::OnSendSource                 )
    EVT_MENU           (wxID_SEND_ABORT                            , wxZRColaFrame::OnSendAbort                  )

    EVT_MENU           (wxID_SETTINGS                              , wxZRColaFrame::OnSettings                   )

    EVT_UPDATE_UI      (wxID_TOOLBAR_EDIT                          , wxZRColaFrame::OnToolbarEditUpdate          )
    EVT_MENU           (wxID_TOOLBAR_EDIT                          , wxZRColaFrame::OnToolbarEdit                )
    EVT_UPDATE_UI      (wxID_TOOLBAR_TRANSFORM                     , wxZRColaFrame::OnToolbarTransformUpdate     )
    EVT_MENU           (wxID_TOOLBAR_TRANSFORM                     , wxZRColaFrame::OnToolbarTransform           )
    EVT_UPDATE_UI      (wxID_PANEL_CHRGRPS                         , wxZRColaFrame::OnPanelCharacterCatalogUpdate)
    EVT_MENU           (wxID_PANEL_CHRGRPS                         , wxZRColaFrame::OnPanelCharacterCatalog      )
    EVT_MENU           (wxID_FOCUS_CHARACTER_CATALOG               , wxZRColaFrame::OnPanelCharacterCatalogFocus )

    EVT_MENU           (wxID_HELP_INSTRUCTIONS                     , wxZRColaFrame::OnHelpInstructions           )
    EVT_MENU           (wxID_HELP_SHORTCUTS                        , wxZRColaFrame::OnHelpShortcuts              )

    EVT_MENU           (wxID_HELP_REQCHAR                          , wxZRColaFrame::OnHelpReqChar                )
    EVT_MENU           (wxID_HELP_UPDATE                           , wxZRColaFrame::OnHelpUpdate                 )
    EVT_MENU           (wxID_ABOUT                                 , wxZRColaFrame::OnHelpAbout                  )
wxEND_EVENT_TABLE()


wxZRColaFrame::wxZRColaFrame() :
    m_hWndSource(NULL),
    m_chrSelect(NULL),
    m_settings(NULL),
    m_chrReq(NULL),
    wxZRColaFrameBase(NULL)
{
    {
        // wxFrameBuilder 3.5 does not support wxAUI_TB_HORIZONTAL flag. Add it manually.
        wxAuiPaneInfo &paneInfo = m_mgr.GetPane(m_toolbarTransform);
        paneInfo.LeftDockable(false);
        paneInfo.RightDockable(false);
        m_toolbarTransform->SetWindowStyleFlag(m_toolbarTransform->GetWindowStyleFlag() | wxAUI_TB_HORIZONTAL);
    }

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

    m_settings = new wxZRColaSettings(this);
    wxPersistentRegisterAndRestore<wxZRColaSettings>(m_settings);

    m_chrSelect = new wxZRColaCharSelect(this);
    wxPersistentRegisterAndRestore<wxZRColaCharSelect>(m_chrSelect);

    m_chrReq = new wxZRColaCharRequest(this);
    wxPersistentRegisterAndRestore<wxZRColaCharRequest>(m_chrReq);

    // Set focus.
    m_panel->m_source->SetFocus();

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

    // Restore persistent state of wxAuiManager manually, since m_mgr is not on the heap.
    wxPersistentAuiManager(&m_mgr).Restore();

    // Register global hotkey(s).
    if (!RegisterHotKey(wxZRColaHKID_INVOKE_TRANSFORM, wxMOD_WIN, VK_F5))
        wxMessageBox(_("ZRCola keyboard shortcut Win+F5 could not be registered. Some functionality will not be available."), _("Warning"), wxOK | wxICON_WARNING);
    if (!RegisterHotKey(wxZRColaHKID_INVOKE_TRANSFORM_INV, wxMOD_WIN, VK_F6))
        wxMessageBox(_("ZRCola keyboard shortcut Win+F6 could not be registered. Some functionality will not be available."), _("Warning"), wxOK | wxICON_WARNING);
}


wxZRColaFrame::~wxZRColaFrame()
{
    // Unregister global hotkey(s).
    UnregisterHotKey(wxZRColaHKID_INVOKE_TRANSFORM_INV);
    UnregisterHotKey(wxZRColaHKID_INVOKE_TRANSFORM    );

#if defined(__WXMSW__)
    if (m_tfSource) {
        m_tfSource->UnadviseSink(m_dwCookie);
        m_tfSource->Release();
    }
#endif

    if (m_taskBarIcon) {
        m_taskBarIcon->Disconnect(wxEVT_TASKBAR_LEFT_DOWN, wxTaskBarIconEventHandler(wxZRColaFrame::OnTaskbarIconClick), NULL, this);
        delete m_taskBarIcon;
    }
}


void wxZRColaFrame::OnClose(wxCloseEvent& event)
{
    event.Skip();

    // Save wxAuiManager's state before destructor.
    // Since the destructor calls m_mgr.UnInit() the regular persistence mechanism is useless to save wxAuiManager's state.
    wxPersistentAuiManager(&m_mgr).Save();
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


void wxZRColaFrame::OnInsertCharacter(wxCommandEvent& event)
{
    if (m_chrSelect->ShowModal() == wxID_OK && !m_chrSelect->m_char.empty()) {
        m_panel->m_source->WriteText(m_chrSelect->m_char);
        m_panel->m_source->SetFocus();
    }
}


void wxZRColaFrame::OnSendUpdate(wxUpdateUIEvent& event)
{
    event.Enable(m_hWndSource ? true : false);
}


void wxZRColaFrame::OnSendDestination(wxCommandEvent& event)
{
    if (m_hWndSource)
        DoSend(m_panel->m_destination->GetValue());

    event.Skip();
}


void wxZRColaFrame::OnCopyDestinationAndReturn(wxCommandEvent& event)
{
    if (m_hWndSource)
        DoCopyAndReturn(m_panel->m_destination->GetValue());

    event.Skip();
}


void wxZRColaFrame::OnSendSource(wxCommandEvent& event)
{
    if (m_hWndSource)
        DoSend(m_panel->m_source->GetValue());

    event.Skip();
}


void wxZRColaFrame::OnCopySourceAndReturn(wxCommandEvent& event)
{
    if (m_hWndSource)
        DoCopyAndReturn(m_panel->m_source->GetValue());

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
    m_panel->m_source     ->SelectAll();
    m_panel->m_destination->SelectAll();

    event.Skip();
}


void wxZRColaFrame::OnSettings(wxCommandEvent& event)
{
    m_settings->ShowModal();
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


void wxZRColaFrame::OnToolbarTransformUpdate(wxUpdateUIEvent& event)
{
    event.Check(m_mgr.GetPane(m_toolbarTransform).IsShown());
}


void wxZRColaFrame::OnToolbarTransform(wxCommandEvent& event)
{
    wxAuiPaneInfo &paneInfo = m_mgr.GetPane(m_toolbarTransform);
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


void wxZRColaFrame::OnHelpInstructions(wxCommandEvent& event)
{
    wxLaunchDefaultBrowser(_("http://zrcola.zrc-sazu.si/en/info/instructions/"));
}


void wxZRColaFrame::OnHelpShortcuts(wxCommandEvent& event)
{
    winstd::tstring pdf_path;

#ifdef __WXMSW__
    // Search and try to launch installed PDF.
    INSTALLSTATE pdf_is = ::MsiGetComponentPath(_T(PRODUCT_VERSION_GUID), _T("{68AC2C38-10E2-41A3-B92C-844C03FFDF6A}"), pdf_path);
    if ((pdf_is == INSTALLSTATE_LOCAL || pdf_is == INSTALLSTATE_SOURCE) &&
        wxFileExists(pdf_path) &&
        (int)::ShellExecute(GetHWND(), NULL, pdf_path.c_str(), NULL, NULL, SW_SHOWNORMAL) > 32) return;
#endif

    // Search and try to launch local PDF copy.
    auto app = dynamic_cast<ZRColaApp*>(wxTheApp);
    pdf_path  = app->GetDatabasePath();
    pdf_path += _T("ZRCola_keyboard.pdf");
    if (wxFileExists(pdf_path) &&
        (int)::ShellExecute(GetHWND(), NULL, pdf_path.c_str(), NULL, NULL, SW_SHOWNORMAL) > 32) return;

    // When everything else fail, try the online version.
    wxLaunchDefaultBrowser(_("http://zrcola.zrc-sazu.si/wp-content/uploads/2016/06/ZRCola_tipkovnica_Jun2016.pdf"));
}


void wxZRColaFrame::OnHelpReqChar(wxCommandEvent& event)
{
    m_chrReq->ShowModal();
}


void wxZRColaFrame::OnHelpUpdate(wxCommandEvent& event)
{
    wxZRColaUpdater dlg(this);
    dlg.ShowModal();
}


void wxZRColaFrame::OnHelpAbout(wxCommandEvent& event)
{
    wxZRColaAbout dlg(this);
    dlg.ShowModal();
}


#if defined(__WXMSW__)

HRESULT STDMETHODCALLTYPE wxZRColaFrame::OnLanguageChange(LANGID langid, __RPC__out BOOL *pfAccept)
{
    if (pfAccept) *pfAccept = TRUE;
    return S_OK;
}


HRESULT STDMETHODCALLTYPE wxZRColaFrame::OnLanguageChanged()
{
    if (m_settings->m_lang_auto) {
        // Set keyboard language.
        HKL hkl = ::GetKeyboardLayout(0);
        ZRCola::LangConvert(LOWORD(hkl), m_settings->m_lang);
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
    auto i_str = str.begin();
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

    // Select all input in source and destination to prepare for the overwrite next time.
    m_panel->m_source     ->SelectAll();
    m_panel->m_destination->SelectAll();
}


void wxZRColaFrame::DoCopyAndReturn(const wxString& str)
{
    if (wxTheClipboard->Open()) {
        wxTheClipboard->SetData(new wxTextDataObject(str));
        wxTheClipboard->Close();
    }

    // Return focus to the source window.
    ::SetActiveWindow(m_hWndSource);
    ::SetForegroundWindow(m_hWndSource);
    m_hWndSource = NULL;

    // Select all input in composer and source to prepare for the overwrite next time.
    m_panel->m_source     ->SelectAll();
    m_panel->m_destination->SelectAll();
}


#ifdef __WXMSW__

WXLRESULT wxZRColaFrame::MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{
    if (message == WM_HOTKEY) {
        // ZRCola hotkey was pressed. Remember the source window and focus ours.
        WXHWND hWndSource = ::GetForegroundWindow();

        switch (wParam) {
        case wxZRColaHKID_INVOKE_TRANSFORM    : m_panel->m_source     ->SetFocus(); break;
        case wxZRColaHKID_INVOKE_TRANSFORM_INV: m_panel->m_destination->SetFocus(); break;
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
            if (IsIconized())
                ::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
            ::SetActiveWindow(m_hWnd);
            ::SetForegroundWindow(m_hWnd);
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
    wxPersistentTLW::Save();

    auto wnd = static_cast<const wxZRColaFrame*>(GetWindow()); // dynamic_cast is not reliable as we are typically called late in the wxTopLevelWindowMSW destructor.

    wxPersistentZRColaComposerPanel(wnd->m_panel).Save();
    wxPersistentZRColaCharacterCatalogPanel(wnd->m_panelChrCat).Save();
}


bool wxPersistentZRColaFrame::Restore()
{
    auto wnd = dynamic_cast<wxZRColaFrame*>(GetWindow());

    wxPersistentZRColaCharacterCatalogPanel(wnd->m_panelChrCat).Restore();
    wxPersistentZRColaComposerPanel(wnd->m_panel).Restore();

    return wxPersistentTLW::Restore();
}
