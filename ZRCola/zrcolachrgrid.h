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
class wxZRColaCharGrid;


#pragma once

#include <wx/grid.h>


///
/// Character grid layout
///
#define wxZRColaCharacterGridColumnWidth   35
#define wxZRColaCharacterGridRowHeight     35


///
/// ZRCola character grid
///

class wxZRColaCharGrid : public wxGrid
{
public:
    wxZRColaCharGrid();
    wxZRColaCharGrid(wxWindow *parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxWANTS_CHARS, const wxString& name = wxGridNameStr);

    ///
    /// Sets new array of characters to display
    ///
    /// \param[in] chars  The string containing characters to display
    ///
    void SetCharacters(const wxString &chars);

protected:
    void OnSize(wxSizeEvent& event);
    DECLARE_EVENT_TABLE()

protected:
    wxString m_chars;       ///< Array of Unicode characters to display in the grid

private:
    bool m_isResizing;      ///< Prevents nesting of OnSize() method.
};
