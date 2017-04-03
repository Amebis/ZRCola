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

///
/// Forward declarations
///
class wxZRColaSettings;
class wxPersistentZRColaSettings;


#pragma once

#include "zrcolagui.h"
#include <zrcola/translate.h>
#include <wxex/persist/dialog.h>
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
    virtual void OnTransformationUpdate(wxUpdateUIEvent& event);
    virtual void OnTransActivate(wxCommandEvent& event);
    virtual void OnTransDeactivate(wxCommandEvent& event);
    virtual void OnTransActiveUp(wxCommandEvent& event);
    virtual void OnTransActiveDown(wxCommandEvent& event);
    virtual void OnApplyButtonClick(wxCommandEvent& event);
    virtual void OnOKButtonClick(wxCommandEvent& event);

public:
    bool m_lang_auto;                           ///< Is language for inverse translation resolved using currently selected keyboard
    ZRCola::langid_t m_lang;                    ///< Language for inverse translation
    std::vector<ZRCola::transetid_t> m_transeq; ///< Transformation set sequence
};


///
/// Supports saving/restoring wxZRColaSettings state
///
class wxPersistentZRColaSettings : public wxPersistentDialog
{
public:
    wxPersistentZRColaSettings(wxZRColaSettings *wnd);

    virtual void Save() const;
    virtual bool Restore();
};


inline wxPersistentObject *wxCreatePersistentObject(wxZRColaSettings *wnd)
{
    return new wxPersistentZRColaSettings(wnd);
}
