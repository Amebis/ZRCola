/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2022 Amebis
*/

///
/// Forward declarations
///
class wxZRColaSettings;
class wxPersistentZRColaSettings;


#pragma once

#include <wxex/common.h>
#pragma warning(push)
#pragma warning(disable: WXWIDGETS_CODE_ANALYSIS_WARNINGS)
#include "zrcolagui.h"
#pragma warning(pop)
#include <wxex/persist/toplevel.h>
#include <vector>


///
/// Configuration dialog
///
class wxZRColaSettings : public wxZRColaSettingsBase
{
public:
    wxZRColaSettings(wxWindow* parent);

    friend class wxPersistentZRColaSettings;    // Allow saving/restoring window state.

protected:
    virtual void OnInitDialog(wxInitDialogEvent& event);
    virtual void OnLanguageUpdate(wxUpdateUIEvent& event);
    virtual void OnApplyButtonClick(wxCommandEvent& event);
    virtual void OnOKButtonClick(wxCommandEvent& event);

public:
    bool m_lang_auto;           ///< Is language for inverse translation resolved using currently selected keyboard
    ZRCola::langid_t m_lang;    ///< Language for inverse translation
};


///
/// Supports saving/restoring wxZRColaSettings state
///
class wxPersistentZRColaSettings : public wxPersistentTLWEx
{
public:
    wxPersistentZRColaSettings(wxZRColaSettings *wnd);

    virtual void Save() const;
    virtual bool Restore();

protected:
    static const int s_guiLevel;    ///< Persisted placements/sizing are considered incompatible between different GUI levels
};


inline wxPersistentObject *wxCreatePersistentObject(wxZRColaSettings *wnd)
{
    return new wxPersistentZRColaSettings(wnd);
}
