/*
    Copyright 2015-2018 Amebis

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
class wxZRColaTranslationSeq;
class wxPersistentZRColaTranslationSeq;


#pragma once

#include "zrcolagui.h"
#include <zrcola/translate.h>
#include <wxex/persist/dialog.h>
#include <vector>


///
/// Configuration dialog
///
class wxZRColaTranslationSeq : public wxZRColaTranslationSeqBase
{
public:
    wxZRColaTranslationSeq(wxWindow* parent);

    friend class wxPersistentZRColaTranslationSeq;    // Allow saving/restoring window state.

protected:
    virtual void OnInitDialog(wxInitDialogEvent& event);
    virtual void OnUpdate(wxUpdateUIEvent& event);
    virtual void OnAvailableChar(wxKeyEvent& event);
    virtual void OnAvailableDClick(wxCommandEvent& event);
    virtual void OnAdd(wxCommandEvent& event);
    virtual void OnRemove(wxCommandEvent& event);
    virtual void OnSelectedChar(wxKeyEvent& event);
    virtual void OnSelectedDClick(wxCommandEvent& event);
    virtual void OnSelectedUp(wxCommandEvent& event);
    virtual void OnSelectedDown(wxCommandEvent& event);
    virtual void OnApplyButtonClick(wxCommandEvent& event);
    virtual void OnOKButtonClick(wxCommandEvent& event);

public:
    std::vector<ZRCola::transetid_t> m_transeq; ///< Custom translation set sequence
};


///
/// Supports saving/restoring wxZRColaTranslationSeq state
///
class wxPersistentZRColaTranslationSeq : public wxPersistentDialog
{
public:
    wxPersistentZRColaTranslationSeq(wxZRColaTranslationSeq *wnd);

    virtual void Save() const;
    virtual bool Restore();
};


inline wxPersistentObject *wxCreatePersistentObject(wxZRColaTranslationSeq *wnd)
{
    return new wxPersistentZRColaTranslationSeq(wnd);
}
