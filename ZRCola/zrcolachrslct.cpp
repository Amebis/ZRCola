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

#include "stdafx.h"


//////////////////////////////////////////////////////////////////////////
// wxZRColaCharSelect
//////////////////////////////////////////////////////////////////////////

wxZRColaCharSelect::wxZRColaCharSelect(wxWindow* parent) : wxZRColaCharSelectBase(parent)
{
    wxTextValidator *validator = dynamic_cast<wxTextValidator*>(m_unicode->GetValidator());
    if (validator)
        validator->SetCharIncludes(wxT("0123456789ABCDEFabcdef"));

    ZRColaApp *app = (ZRColaApp*)wxTheApp;
    for (size_t i = 0, n = app->m_cc_db.idxRnk.size(); i < n; i++) {
        const ZRCola::chrcat_db::chrcat &cc = app->m_cc_db.idxRnk[i];
        m_categories->Check(m_categories->Insert(wxGetTranslation(wxString(cc.name, cc.name_len), wxT("ZRCola-zrcdb")), i));
    }
}


//////////////////////////////////////////////////////////////////////////
// wxPersistentZRColaCharSelect
//////////////////////////////////////////////////////////////////////////

wxPersistentZRColaCharSelect::wxPersistentZRColaCharSelect(wxZRColaCharSelect *wnd) : wxPersistentWindow<wxZRColaCharSelect>(wnd)
{
}


wxString wxPersistentZRColaCharSelect::GetKind() const
{
    return wxT(wxPERSIST_TLW_KIND);
}


void wxPersistentZRColaCharSelect::Save() const
{
    const wxZRColaCharSelect * const wnd = static_cast<const wxZRColaCharSelect*>(GetWindow());

    // Code copied from wxPersistentTLW::Save()
    const wxPoint pos = wnd->GetScreenPosition();
    SaveValue(wxPERSIST_TLW_X, pos.x);
    SaveValue(wxPERSIST_TLW_Y, pos.y);
}


bool wxPersistentZRColaCharSelect::Restore()
{
    wxZRColaCharSelect * const wnd = static_cast<wxZRColaCharSelect*>(GetWindow());

    // Code copied from wxPersistentTLW::Restore()
    long
        x wxDUMMY_INITIALIZE(-1),
        y wxDUMMY_INITIALIZE(-1);
    const wxSize size = wnd->GetSize();
    const bool hasPos = RestoreValue(wxPERSIST_TLW_X, &x) &&
                        RestoreValue(wxPERSIST_TLW_Y, &y);

    if (hasPos) {
        // to avoid making the window completely invisible if it had been
        // shown on a monitor which was disconnected since the last run
        // (this is pretty common for notebook with external displays)
        //
        // NB: we should allow window position to be (slightly) off screen,
        //     it's not uncommon to position the window so that its upper
        //     left corner has slightly negative coordinate
        if (wxDisplay::GetFromPoint(wxPoint(x         , y         )) != wxNOT_FOUND ||
            wxDisplay::GetFromPoint(wxPoint(x + size.x, y + size.y)) != wxNOT_FOUND)
        {
            wnd->Move(x, y, wxSIZE_ALLOW_MINUS_ONE);
        }
    }

    return true;
}
