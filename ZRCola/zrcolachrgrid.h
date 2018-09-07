/*
    Copyright 2015-2018 Amebis

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
    /// \param[in] chars  The string containing characters to display (zero delimited)
    ///
    void SetCharacters(const wxString &chars);

    ///
    /// Sets new array of characters to display
    ///
    /// \param[in] chars  The array of characters to display
    ///
    void SetCharacters(const wxArrayString &chars);

    ///
    /// Sets new array of characters to display
    ///
    /// \param[in] chars      The string containing characters to display (zero delimited)
    /// \param[in] relevance  Bit-array of \p chars relevance (1=more relevant, 0=less relevant)
    ///
    void SetCharacters(const wxString &chars, const wxArrayShort &relevance);

    ///
    /// Returns displayed characters
    ///
    /// \returns  The string containing displayed characters
    ///
    inline const wxArrayString& GetCharacters() const
    {
        return m_chars;
    }

    ///
    /// Returns given character coordinates
    ///
    /// \param[in] c  Character
    ///
    /// \returns Grid coordinates of selected character or (-1, -1) if character not found.
    ///
    inline wxGridCellCoords GetCharacterCoords(const wxString &c) const
    {
        for (size_t i = 0, n = m_chars.GetCount(); ; i++) {
            if (i >= n)
                return wxGridCellCoords(-1, -1);
            else if (m_chars[i] == c)
                return wxGridCellCoords(i / m_numCols, i % m_numCols);
        }
    }

protected:
    virtual wxString GetToolTipText(int idx);

    void OnSize(wxSizeEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnMotion(wxMouseEvent& event);
    void OnTooltipTimer(wxTimerEvent& event);
    DECLARE_EVENT_TABLE()

private:
    void Init();                // common part of all ctors

protected:
    wxArrayString m_chars;      ///< Array of Unicode characters to display in the grid
    wxArrayShort m_relevance;   ///< Bit-array of `m_chars` relevance

private:
    bool m_regenerate;          ///< Force regenerate grid table
    bool m_isResizing;          ///< Prevents nesting of OnSize() method.
    wxTimer m_timerToolTip;     ///< Timer for displaying tooltip
    size_t m_toolTipIdx;        ///< Index of cell for tooltip display
};
