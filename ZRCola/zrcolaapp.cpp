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


bool ZRColaApp::GetKeySequenceAsText(_In_count_(seq_len) const ZRCola::keyseq_db::keyseq::key_t *seq, _In_ size_t seq_len, _Out_ wxString& str)
{
    assert(seq || !seq_len);

    str.Clear();
    for (size_t i = 0; i < seq_len; i++) {
        if (i) str += L", ";
        if (seq[i].modifiers & ZRCola::keyseq_db::keyseq::CTRL ) str += L"Ctrl+";
        if (seq[i].modifiers & ZRCola::keyseq_db::keyseq::ALT  ) str += L"Alt+";
        if (seq[i].modifiers & ZRCola::keyseq_db::keyseq::SHIFT) str += L"Shift+";
        wchar_t k = seq[i].key;
#if defined(__WXMSW__)
        // Translate from U.S. Keyboard to scan code.
        static const HKL s_hkl = ::LoadKeyboardLayout(_T("00000409"), 0);
        k = ::MapVirtualKeyEx(k, MAPVK_VK_TO_VSC, s_hkl);

        // Translate from scan code to local keyboard.
        k = ::MapVirtualKey(k, MAPVK_VSC_TO_VK);
#endif
        switch (k) {
            case 0            : return false;

            case WXK_ESCAPE   : str += _("Esc"         ); break;

            case WXK_F1       : str += _("F1"          ); break;
            case WXK_F2       : str += _("F2"          ); break;
            case WXK_F3       : str += _("F3"          ); break;
            case WXK_F4       : str += _("F4"          ); break;
            case WXK_F5       : str += _("F5"          ); break;
            case WXK_F6       : str += _("F6"          ); break;
            case WXK_F7       : str += _("F7"          ); break;
            case WXK_F8       : str += _("F8"          ); break;
            case WXK_F9       : str += _("F9"          ); break;
            case WXK_F10      : str += _("F10"         ); break;
            case WXK_F11      : str += _("F11"         ); break;
            case WXK_F12      : str += _("F12"         ); break;

            case WXK_PRINT    : str += _("Print Screen"); break;
            case WXK_SCROLL   : str += _("Scroll Lock" ); break;
            case WXK_PAUSE    : str += _("Pause"       ); break;

            case WXK_BACK     : str += _("Backspace"   ); break;
            case WXK_TAB      : str += _("Tab"         ); break;
            case WXK_CAPITAL  : str += _("Caps Lock"   ); break;
            case WXK_RETURN   : str += _("Return"      ); break;
            case WXK_SPACE    : str += _("Space"       ); break;

            case WXK_SHIFT    : str += _("Shift"       ); break;
            case WXK_ALT      : str += _("Alt"         ); break;
            case WXK_CONTROL  : str += _("Ctrl"        ); break;
            case WXK_MENU     : str += _("Menu"        ); break;

            case WXK_INSERT   : str += _("Insert"      ); break;
            case WXK_DELETE   : str += _("Delete"      ); break;
            case WXK_PAGEUP   : str += _("Page Up"     ); break;
            case WXK_PAGEDOWN : str += _("Page Down"   ); break;
            case WXK_HOME     : str += _("Home"        ); break;
            case WXK_END      : str += _("End"         ); break;

            case WXK_LEFT     : str += _("Left"        ); break;
            case WXK_UP       : str += _("Up"          ); break;
            case WXK_RIGHT    : str += _("Right"       ); break;
            case WXK_DOWN     : str += _("Down"        ); break;

            case WXK_NUMLOCK  : str += _("Num Lock"    ); break;

            default:
#if defined(__WXMSW__)
                k = ::MapVirtualKey(k, MAPVK_VK_TO_CHAR);
#endif
                str += k;
        }
    }

    return true;
}
