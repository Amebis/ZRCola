/*
    Copyright 2015-2020 Amebis

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
