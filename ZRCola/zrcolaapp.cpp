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
        wxVERIFY(m_locale.AddCatalog(wxT("wxExtend")));
        wxVERIFY(m_locale.AddCatalog(wxT("libZRColaUI")));
        wxVERIFY(m_locale.AddCatalog(wxT("ZRCola")));
        wxVERIFY(m_locale.AddCatalog(wxT("ZRCola-zrcdb")));
    }

    std::fstream dat((LPCTSTR)GetDatabasePath(), std::ios_base::in | std::ios_base::binary);
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
                    }
                }

                if (!has_translation_data)
                    wxFAIL_MSG(wxT("ZRCola.zrcdb has no translation data."));
            }
        } else
            wxFAIL_MSG(wxT("ZRCola.zrcdb is not a valid ZRCola database."));
    }

    m_mainWnd = new wxZRColaFrame();
    wxPersistentRegisterAndRestore<wxZRColaFrame>(m_mainWnd);
    m_mainWnd->Show();

    return true;
}
