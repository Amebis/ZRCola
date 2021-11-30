/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2021 Amebis
*/

///
/// Forward declarations
///
class wxZRColaTranslationSeq;
class wxPersistentZRColaTranslationSeq;


#pragma once

#include <wxex/common.h>
#pragma warning(push)
#pragma warning(disable: WXWIDGETS_CODE_ANALYSIS_WARNINGS)
#include "zrcolagui.h"
#pragma warning(pop)
#include <zrcola/translate.h>
#include <wxex/persist/toplevel.h>
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
class wxPersistentZRColaTranslationSeq : public wxPersistentTLWEx
{
public:
    wxPersistentZRColaTranslationSeq(wxZRColaTranslationSeq *wnd);

    virtual void Save() const;
    virtual bool Restore();

protected:
    static const int s_guiLevel;    ///< Persisted placements/sizing are considered incompatible between different GUI levels
};


inline wxPersistentObject *wxCreatePersistentObject(wxZRColaTranslationSeq *wnd)
{
    return new wxPersistentZRColaTranslationSeq(wnd);
}
