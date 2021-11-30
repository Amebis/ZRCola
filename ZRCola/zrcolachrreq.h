/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2021 Amebis
*/

///
/// Forward declarations
///
class wxZRColaCharRequest;
class wxPersistentZRColaCharRequest;


#pragma once

#include <wxex/common.h>
#pragma warning(push)
#pragma warning(disable: WXWIDGETS_CODE_ANALYSIS_WARNINGS)
#include "zrcolagui.h"
#pragma warning(pop)
#include "zrcolakeyhndlr.h"
#include <wxex/persist/toplevel.h>


///
/// ZRCola New Character Request dialog
///
class wxZRColaCharRequest : public wxZRColaCharRequestBase
{
public:
    wxZRColaCharRequest(wxWindow* parent);
    virtual ~wxZRColaCharRequest();

    friend class wxPersistentZRColaCharRequest; // Allow saving/restoring window state.

protected:
    virtual void OnOKButtonClick(wxCommandEvent& event);
};



///
/// Supports saving/restoring wxZRColaCharRequest state
///
class wxPersistentZRColaCharRequest : public wxPersistentTLWEx
{
public:
    wxPersistentZRColaCharRequest(wxZRColaCharRequest *wnd);

    virtual void Save() const;
    virtual bool Restore();

protected:
    static const int s_guiLevel;    ///< Persisted placements/sizing are considered incompatible between different GUI levels
};


inline wxPersistentObject *wxCreatePersistentObject(wxZRColaCharRequest *wnd)
{
    return new wxPersistentZRColaCharRequest(wnd);
}
