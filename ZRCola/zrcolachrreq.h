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
class wxZRColaCharRequest;


#pragma once

#include "zrcolagui.h"
#include "zrcolakeyhndlr.h"


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
class wxPersistentZRColaCharRequest : public wxPersistentDialog
{
public:
    wxPersistentZRColaCharRequest(wxZRColaCharRequest *wnd);

    virtual void Save() const;
    virtual bool Restore();
};


inline wxPersistentObject *wxCreatePersistentObject(wxZRColaCharRequest *wnd)
{
    return new wxPersistentZRColaCharRequest(wnd);
}