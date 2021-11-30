/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2021 Amebis
*/

#include "pch.h"
#if defined(__WXMSW__)
#pragma comment(lib, "msi.lib")
#endif


//////////////////////////////////////////////////////////////////////////
// ZRColaApp
//////////////////////////////////////////////////////////////////////////

#pragma warning(suppress: 28251) // wxWidgets implement WinMain without code analysis annotations.
wxIMPLEMENT_APP(ZRColaApp);


ZRColaApp::ZRColaApp() :
    m_mainWnd(NULL),
    #pragma warning(suppress: 26812) // wxLanguage is unscoped
    m_lang_ui(wxLANGUAGE_DEFAULT),
    wxApp()
{
}


bool ZRColaApp::OnInit()
{
#if defined(__WXMSW__)
    // To compensate migration to non-advertised shortcut, do the Microsoft Installer's feature completeness check manually.
    // If execution got this far in the first place (EXE and dependent DLLs are present and loadable).
    // Furthermore, this increments program usage counter.
    if (::MsiQueryFeatureState(_T(PRODUCT_VERSION_GUID), _T("featZRCola")) != INSTALLSTATE_UNKNOWN)
        ::MsiUseFeature(_T(PRODUCT_VERSION_GUID), _T("featZRCola"));
#endif

    // Initialize configuration.
    wxConfigBase *cfgPrev = wxConfigBase::Set(new wxConfig(wxT(PRODUCT_CFG_APPLICATION), wxT(PRODUCT_CFG_VENDOR)));
    if (cfgPrev) wxDELETE(cfgPrev);

    if (!wxApp::OnInit())
        return false;

    // Initialize locale.
    if (wxInitializeLocale(m_locale, &m_lang_ui)) {
        wxVERIFY(m_locale.AddCatalog(wxT("wxExtend") wxT(wxExtendVersion)));
        wxVERIFY(m_locale.AddCatalog(wxT("UpdaterZRCola")));
        wxVERIFY(m_locale.AddCatalog(wxT("libZRColaUI")));
        wxVERIFY(m_locale.AddCatalog(wxT("ZRCola")));
        wxVERIFY(m_locale.AddCatalog(wxT("ZRCola-zrcdb")));
    }

#ifdef __WXMSW__
    // Create global event.
    m_running.attach(::CreateEvent(NULL, FALSE, FALSE, _T(PRODUCT_CFG_APPLICATION) _T("{BBDE7AAD-29B6-4B83-ADA1-92AFA81A0883}")));
    if (::GetLastError() == ERROR_ALREADY_EXISTS) {
        // ZRCola is already running. Find its window.
        HWND okno = ::FindWindow(_T("wxWindowNR"), _("ZRCola"));
        if (okno) {
            if (::IsIconic(okno))
                ::SendMessage(okno, WM_SYSCOMMAND, SC_RESTORE, 0);
            ::SetActiveWindow(okno);
            ::SetForegroundWindow(okno);

            // Not an error condition actually; Just nothing else to do...
            return false;
        }
    }
#endif

    std::fstream dat((LPCTSTR)GetDatabaseFilePath(), std::ios_base::in | std::ios_base::binary);
    if (dat.good()) {
        if (stdex::idrec::find<ZRCola::recordid_t, ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>(dat, ZRCOLA_DB_ID, sizeof(ZRCola::recordid_t))) {
            ZRCola::recordsize_t size;
            dat.read((char*)&size, sizeof(ZRCola::recordsize_t));
            if (dat.good()) {
                bool has_translation_data = false;

                for (;;) {
                    ZRCola::recordid_t id;
                    if (!stdex::idrec::read_id(dat, id, size)) break;

                    if (id == ZRCola::translation_rec::id) {
                        dat >> ZRCola::translation_rec(m_t_db);
                        if (dat.good()) {
                            has_translation_data = true;
                        } else {
                            wxFAIL_MSG(wxT("Error reading translation data from ZRCola.zrcdb."));
                            m_t_db.clear();
                        }
                    } else if (id == ZRCola::transet_rec::id) {
                        dat >> ZRCola::transet_rec(m_ts_db);
                        if (!dat.good()) {
                            wxFAIL_MSG(wxT("Error reading translation set data from ZRCola.zrcdb."));
                            m_ts_db.clear();
                        }
                    } else if (id == ZRCola::transeq_rec::id) {
                        dat >> ZRCola::transeq_rec(m_tsq_db);
                        if (!dat.good()) {
                            wxFAIL_MSG(wxT("Error reading translation sequence data from ZRCola.zrcdb."));
                            m_tsq_db.clear();
                        }
                    } else if (id == ZRCola::langchar_rec::id) {
                        dat >> ZRCola::langchar_rec(m_lc_db);
                        if (!dat.good()) {
                            wxFAIL_MSG(wxT("Error reading language character data from ZRCola.zrcdb."));
                            m_lc_db.clear();
                        }
                    } else if (id == ZRCola::language_rec::id) {
                        dat >> ZRCola::language_rec(m_lang_db);
                        if (!dat.good()) {
                            wxFAIL_MSG(wxT("Error reading language character data from ZRCola.zrcdb."));
                            m_lang_db.clear();
                        }
                    } else if (id == ZRCola::keyseq_rec::id) {
                        dat >> ZRCola::keyseq_rec(m_ks_db);
                        if (!dat.good()) {
                            wxFAIL_MSG(wxT("Error reading key sequences data from ZRCola.zrcdb."));
                            m_ks_db.clear();
                        }
                    } else if (id == ZRCola::character_rec::id) {
                        dat >> ZRCola::character_rec(m_chr_db);
                        if (!dat.good()) {
                            wxFAIL_MSG(wxT("Error reading character data from ZRCola.zrcdb."));
                            m_chr_db.clear();
                        }
                    } else if (id == ZRCola::chrcat_rec::id) {
                        dat >> ZRCola::chrcat_rec(m_cc_db);
                        if (!dat.good()) {
                            wxFAIL_MSG(wxT("Error reading character category data from ZRCola.zrcdb."));
                            m_cc_db.clear();
                        }
                    } else if (id == ZRCola::chrtag_rec::id) {
                        dat >> ZRCola::chrtag_rec(m_ct_db);
                        if (!dat.good()) {
                            wxFAIL_MSG(wxT("Error reading character tag data from ZRCola.zrcdb."));
                            m_ct_db.clear();
                        }
                    } else if (id == ZRCola::tagname_rec::id) {
                        dat >> ZRCola::tagname_rec(m_tn_db);
                        if (!dat.good()) {
                            wxFAIL_MSG(wxT("Error reading tag name data from ZRCola.zrcdb."));
                            m_tn_db.clear();
                        }
                    } else
                        stdex::idrec::ignore<ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>(dat);
                }

                if (!has_translation_data)
                    wxFAIL_MSG(wxT("ZRCola.zrcdb has no translation data."));
            }
        } else
            wxFAIL_MSG(wxT("ZRCola.zrcdb is not a valid ZRCola database."));
    } else
        wxFAIL_MSG(wxT("ZRCola.zrcdb not found or cannot be opened."));

    // Initialize sockets. Otherwise Updater thread will not work.
    wxSocketBase::Initialize();

    m_mainWnd = new wxZRColaFrame();
    m_mainWnd->m_panel->RestoreFromStateFile();
    m_mainWnd->Show();

    return true;
}


int ZRColaApp::OnExit()
{
    wxSocketBase::Shutdown();

    int res = wxApp::OnExit();

#ifdef __WXMSW__
    m_running.free();
#endif

    return res;
}
