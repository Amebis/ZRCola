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
class wxZRColaCharSelect;


#pragma once

#include "zrcolagui.h"


///
/// ZRCola character select dialog
///
class wxZRColaCharSelect : public wxZRColaCharSelectBase
{
public:
    wxZRColaCharSelect(wxWindow* parent);
};


///
/// Supports saving/restoring wxZRColaCharSelect state
///
class wxPersistentZRColaCharSelect : public wxPersistentWindow<wxZRColaCharSelect>
{
public:
    wxPersistentZRColaCharSelect(wxZRColaCharSelect *wnd);

    virtual wxString GetKind() const;
    virtual void Save() const;
    virtual bool Restore();
};


inline wxPersistentObject *wxCreatePersistentObject(wxZRColaCharSelect *wnd)
{
    return new wxPersistentZRColaCharSelect(wnd);
}
