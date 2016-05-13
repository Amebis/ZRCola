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
    enum
    {
        wxID_TOOLTIP_TIMER = 2000,
    };

    wxZRColaCharGrid();
    wxZRColaCharGrid(wxWindow *parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxWANTS_CHARS, const wxString& name = wxGridNameStr);
    virtual ~wxZRColaCharGrid();

    ///
    /// Sets new array of characters to display
    ///
    /// \param[in] chars  The string containing characters to display
    ///
    void SetCharacters(const wxString &chars);

    ///
    /// Returns displayed characters
    ///
    /// \returns  The string containing displayed characters
    ///
    inline wxString GetCharacters() const
    {
        return m_chars;
    }

protected:
    virtual wxString GetToolTipText(int idx);

    void OnSize(wxSizeEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnMotion(wxMouseEvent& event);
    void OnTooltipTimer(wxTimerEvent& event);
    DECLARE_EVENT_TABLE()

private:
    void Init();            // common part of all ctors

protected:
    wxString m_chars;       ///< Array of Unicode characters to display in the grid

private:
    bool m_isResizing;      ///< Prevents nesting of OnSize() method.
    wxTimer m_timerToolTip; ///< Timer for displaying tooltip
    size_t m_toolTipIdx;    ///< Index of cell for tooltip display
};
