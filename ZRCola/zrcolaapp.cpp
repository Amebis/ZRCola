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
#if defined(__WXMSW__)
#pragma comment(lib, "msi.lib")
#endif


//////////////////////////////////////////////////////////////////////////
// ZRColaApp
//////////////////////////////////////////////////////////////////////////

wxIMPLEMENT_APP(ZRColaApp);


ZRColaApp::ZRColaApp() :
    m_mainWnd(NULL),
#ifdef __WXMSW__
    m_running(NULL),
#endif
    wxApp()
{
}


bool ZRColaApp::OnInit()
{
#if defined(__WXMSW__)
    // To compensate migration to non-advertised shortcut, do the Microsoft Installer's feature completeness check manually.
    // If execution got this far in the first place (EXE and dependent DLLs are present and loadable).
    // Furthermore, this increments program usage counter.
    if (::MsiQueryFeatureState(_T(ZRCOLA_VERSION_GUID), _T("featZRCola")) != INSTALLSTATE_UNKNOWN)
        ::MsiUseFeature(_T(ZRCOLA_VERSION_GUID), _T("featZRCola"));
#endif

    wxConfigBase *cfgPrev = wxConfigBase::Set(new wxConfig(wxT(ZRCOLA_CFG_APPLICATION), wxT(ZRCOLA_CFG_VENDOR)));
    if (cfgPrev) wxDELETE(cfgPrev);

    if (!wxApp::OnInit())
        return false;

    // Set desired locale.
    wxLanguage language = (wxLanguage)wxConfigBase::Get()->Read(wxT("Language"), wxLANGUAGE_DEFAULT);
    if (wxLocale::IsAvailable(language)) {
        wxString sPath;
        if (wxConfigBase::Get()->Read(wxT("LocalizationRepositoryPath"), &sPath))
            m_locale.AddCatalogLookupPathPrefix(sPath);
        wxVERIFY(m_locale.Init(language));
        wxVERIFY(m_locale.AddCatalog(wxT("wxExtend") wxT(wxExtendVersion)));
        wxVERIFY(m_locale.AddCatalog(wxT("Updater") wxT(wxUpdaterVersion)));
        wxVERIFY(m_locale.AddCatalog(wxT("libZRColaUI")));
        wxVERIFY(m_locale.AddCatalog(wxT("ZRCola")));
        wxVERIFY(m_locale.AddCatalog(wxT("ZRCola-zrcdb")));
    }

#ifdef __WXMSW__
    // Create global event.
    m_running = ::CreateEvent(NULL, FALSE, FALSE, _T(ZRCOLA_CFG_APPLICATION) _T("{BBDE7AAD-29B6-4B83-ADA1-92AFA81A0883}"));
    wxASSERT(m_running);
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
                            m_t_db.idxComp  .clear();
                            m_t_db.idxDecomp.clear();
                            m_t_db.data     .clear();
                        }
                    } else if (id == ZRCola::langchar_rec::id) {
                        dat >> ZRCola::langchar_rec(m_lc_db);
                        if (!dat.good()) {
                            wxFAIL_MSG(wxT("Error reading language character data from ZRCola.zrcdb."));
                            m_lc_db.idxChr.clear();
#ifdef ZRCOLA_LANGCHAR_LANG_IDX
                            m_lc_db.idxLng.clear();
#endif
                            m_lc_db.data  .clear();
                        }
                    } else if (id == ZRCola::language_rec::id) {
                        dat >> ZRCola::language_rec(m_lang_db);
                        if (!dat.good()) {
                            wxFAIL_MSG(wxT("Error reading language character data from ZRCola.zrcdb."));
                            m_lang_db.idxLng.clear();
                            m_lang_db.data  .clear();
                        }
                    } else if (id == ZRCola::keyseq_rec::id) {
                        dat >> ZRCola::keyseq_rec(m_ks_db);
                        if (!dat.good()) {
                            wxFAIL_MSG(wxT("Error reading key sequences data from ZRCola.zrcdb."));
                            m_ks_db.idxChr.clear();
                            m_ks_db.idxKey.clear();
                            m_ks_db.data  .clear();
                        }
                    } else if (id == ZRCola::character_rec::id) {
                        dat >> ZRCola::character_rec(m_chr_db);
                        if (!dat.good()) {
                            wxFAIL_MSG(wxT("Error reading character data from ZRCola.zrcdb."));
                            m_chr_db.idxChr.clear();
                            m_chr_db.data  .clear();
                        }
                    } else if (id == ZRCola::chrcat_rec::id) {
                        dat >> ZRCola::chrcat_rec(m_cc_db);
                        if (!dat.good()) {
                            wxFAIL_MSG(wxT("Error reading character category data from ZRCola.zrcdb."));
                            m_cc_db.idxChrCat.clear();
                            m_cc_db.idxRnk   .clear();
                            m_cc_db.data     .clear();
                        }
                    } else
                        stdex::idrec::ignore<ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>(dat);
                }

                if (!has_translation_data)
                    wxFAIL_MSG(wxT("ZRCola.zrcdb has no translation data."));
            }
        } else
            wxFAIL_MSG(wxT("ZRCola.zrcdb is not a valid ZRCola database."));
    }

    // Initialize sockets. Otherwise Updater thread will not work.
    wxSocketBase::Initialize();

    m_mainWnd = new wxZRColaFrame();
    wxPersistentRegisterAndRestore<wxZRColaFrame>(m_mainWnd);
    m_mainWnd->Show();

    return true;
}


int ZRColaApp::OnExit()
{
    wxSocketBase::Shutdown();

    int res = wxApp::OnExit();

#ifdef __WXMSW__
    if (m_running) {
        wxVERIFY(::CloseHandle(m_running));
        m_running = NULL;
    }
#endif

    return res;
}
