/*
    Copyright 2016 Amebis

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

#pragma once

#include <wx/wx.h>


//////////////////////////////////////////////////////////////////////////
// ZRColaFrame
//////////////////////////////////////////////////////////////////////////

class ZRColaFrame : public wxFrame
{
public:
    ZRColaFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    enum {
        ID_Hello = 1
    };

protected:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();
};


//////////////////////////////////////////////////////////////////////////
// ZRColaApp
//////////////////////////////////////////////////////////////////////////

class ZRColaApp: public wxApp
{
public:
    virtual bool OnInit();

protected:
    wxLocale m_locale;
};
