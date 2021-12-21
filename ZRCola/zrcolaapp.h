/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2021 Amebis
*/

///
/// Forward declarations
///
class ZRColaApp;


#pragma once

#include "zrcolafrm.h"
#include <WinStd/Win.h>
#include <wxex/common.h>
#pragma warning(push)
#pragma warning(disable: WXWIDGETS_CODE_ANALYSIS_WARNINGS)
#include <wx/app.h>
#include <wx/config.h>
#include <wx/intl.h>
#pragma warning(pop)
#include <zrcola/character.h>
#include <zrcola/highlight.h>
#include <zrcola/language.h>
#include <zrcola/translate.h>
#include <zrcola/tag.h>
#include <zrcolaui/keyboard.h>


///
/// ZRCola application
///
class ZRColaApp : public wxApp
{
public:
    ZRColaApp();

    ///
    /// Called when application initializes.
    ///
    /// \returns
    /// - \c true if initialization succeeded
    /// - \c false otherwise
    ///
    virtual bool OnInit();

    ///
    /// Called when application uninitializes.
    ///
    /// \returns Result code to return to OS
    ///
    virtual int OnExit();

    ///
    /// \returns Path to database files
    ///
    inline wxString GetDatabasePath() const;

    ///
    /// \returns Path to ZRCola.zrcdb file
    ///
    inline wxString GetDatabaseFilePath() const;

public:
    ZRCola::translation_db m_t_db;  ///< Translation database
    ZRCola::transet_db m_ts_db;     ///< Translation set database
    ZRCola::transeq_db m_tsq_db;    ///< Translation sequence database
    ZRCola::langchar_db m_lc_db;    ///< Language character database
    ZRCola::language_db m_lang_db;  ///< Language database
    ZRCola::keyseq_db m_ks_db;      ///< Key sequence database
    ZRCola::character_db m_chr_db;  ///< Character database
    ZRCola::chrcat_db m_cc_db;      ///< Characted category database
    ZRCola::chrtag_db m_ct_db;      ///< Character tag database
    ZRCola::tagname_db m_tn_db;     ///< Tag name database
    ZRCola::highlight_db m_h_db;    ///< Highlight database

    wxZRColaFrame *m_mainWnd;       ///< Main window

    wxLocale m_locale;              ///< Current locale
    wxLanguage m_lang_ui;           ///< Translation language (language of User Interface, wxLANGUAGE_DEFAULT = English)

protected:
#ifdef __WXMSW__
    winstd::win_handle<NULL> m_running; ///< Global Win32 event to determine if another instance of ZRCola is already running
#endif
};


wxDECLARE_APP(ZRColaApp);


inline wxString ZRColaApp::GetDatabasePath() const
{
    wxString sPath;
    if (wxConfigBase::Get()->Read(wxT("DatabasePath"), &sPath)) {
        if (!wxEndsWithPathSeparator(sPath))
            sPath += wxFILE_SEP_PATH;
    } else {
        sPath = wxPathOnly(argv[0]);
        sPath += wxT("\\..\\data\\");
    }

    return sPath;
}


inline wxString ZRColaApp::GetDatabaseFilePath() const
{
    return GetDatabasePath() + wxT("ZRCola.zrcdb");
}
