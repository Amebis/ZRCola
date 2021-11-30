/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2021 Amebis
*/

///
/// Forward declarations
///
class wxZRColaCharacterCatalogPanel;


#pragma once

#include <wxex/common.h>
#pragma warning(push)
#pragma warning(disable: WXWIDGETS_CODE_ANALYSIS_WARNINGS)
#include "zrcolagui.h"
#pragma warning(pop)
#include <zrcolaui/chargroup.h>
#pragma warning(push)
#pragma warning(disable: WXWIDGETS_CODE_ANALYSIS_WARNINGS)
#include <wx/persist/window.h>
#pragma warning(pop)


///
/// ZRCola character groups panel
///
class wxZRColaCharacterCatalogPanel : public wxZRColaCharacterCatalogPanelBase
{
public:
    enum
    {
        wxID_FOCUS_SOURCE = 6000,
    };

    wxZRColaCharacterCatalogPanel(wxWindow* parent);
    virtual ~wxZRColaCharacterCatalogPanel();

    friend class wxPersistentZRColaCharacterCatalogPanel;    // Allow saving/restoring window state.

protected:
    virtual void OnChoice(wxCommandEvent& event);
    virtual void OnGridClick(wxGridEvent& event);
    virtual void OnGridKeyDown(wxKeyEvent& event);
    virtual void OnShowAll(wxCommandEvent& event);
    void OnFocusSource(wxCommandEvent& event);
    DECLARE_EVENT_TABLE()

    void Update();

protected:
    ZRCola::chrgrp_db m_cg_db;      ///< Character group database
    int m_cg_id;                    ///< Selected character group ID
};


///
/// Supports saving/restoring wxZRColaCharacterCatalogPanel state
///
class wxPersistentZRColaCharacterCatalogPanel : public wxPersistentWindow<wxZRColaCharacterCatalogPanel>
{
public:
    wxPersistentZRColaCharacterCatalogPanel(wxZRColaCharacterCatalogPanel *wnd);

    virtual wxString GetKind() const;
    virtual void Save() const;
    virtual bool Restore();
};


inline wxPersistentObject *wxCreatePersistentObject(wxZRColaCharacterCatalogPanel *wnd)
{
    return new wxPersistentZRColaCharacterCatalogPanel(wnd);
}
