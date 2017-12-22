/*
    Copyright 2015-2017 Amebis

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
class wxZRColaCharacterCatalogPanel;


#pragma once

#include "zrcolagui.h"
#include <zrcolaui/chargroup.h>
#include <wx/persist/window.h>


///
/// ZRCola character groups panel
///
class wxZRColaCharacterCatalogPanel : public wxZRColaCharacterCatalogPanelBase
{
public:
    enum
    {
        wxID_FOCUS_DECOMPOSED = 6000,
    };

    wxZRColaCharacterCatalogPanel(wxWindow* parent);
    virtual ~wxZRColaCharacterCatalogPanel();

    friend class wxPersistentZRColaCharacterCatalogPanel;    // Allow saving/restoring window state.

protected:
    virtual void OnChoice(wxCommandEvent& event);
    virtual void OnGridClick(wxGridEvent& event);
    virtual void OnGridKeyDown(wxKeyEvent& event);
    virtual void OnShowAll(wxCommandEvent& event);
    void OnFocusDecomposed(wxCommandEvent& event);
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
