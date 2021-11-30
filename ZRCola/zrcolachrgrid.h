/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2021 Amebis
*/

///
/// Forward declarations
///
class wxZRColaCharGrid;


#pragma once

#include <wxex/common.h>
#pragma warning(push)
#pragma warning(disable: WXWIDGETS_CODE_ANALYSIS_WARNINGS)
#include <wx/grid.h>
#pragma warning(pop)


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
                return wxGridCellCoords((int)(i / m_numCols), i % m_numCols);
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
