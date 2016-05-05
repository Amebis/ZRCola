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
// wxZRColaCharGrid
//////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(wxZRColaCharGrid, wxGrid)
    EVT_SIZE(wxZRColaCharGrid::OnSize)
END_EVENT_TABLE()


wxZRColaCharGrid::wxZRColaCharGrid() :
    m_isResizing(false),
    wxGrid()
{
}


wxZRColaCharGrid::wxZRColaCharGrid(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name) :
    m_isResizing(false),
    wxGrid(parent, id, pos, size, wxWANTS_CHARS, name)
{
    // Cell Defaults
    SetDefaultRowSize(wxZRColaCharacterGridRowHeight);
}


void wxZRColaCharGrid::SetCharacters(const wxString &chars)
{
    m_chars = chars;

    // Build and set new grid data.
    size_t char_len = m_chars.Length();
    int rows = std::max<int>((char_len + m_numCols - 1) / m_numCols, 1);
    wxGridStringTable *table = new wxGridStringTable(rows, m_numCols);
    for (int r = 0, i = 0; r < rows; r++)
        for (int c = 0; c < m_numCols; c++, i++)
            table->SetValue(r, c, i < char_len ? wxString(1, m_chars[i]) : wxEmptyString);
    SetTable(table, true);
}


void wxZRColaCharGrid::OnSize(wxSizeEvent& event)
{
    event.Skip();

    if (m_isResizing)
        return;

    m_isResizing = true;

    // Calculate initial estimate of columns and rows.
    wxSize size(event.GetSize());
    size_t
        char_len = m_chars.Length();
    int
        width    = size.GetWidth() - m_rowLabelWidth - m_extraWidth,
        cols     = std::max<int>(width / wxZRColaCharacterGridColumnWidth, 1),
        rows     = std::max<int>((char_len + cols - 1) / cols, 1);

    if (m_colLabelHeight + rows*wxZRColaCharacterGridRowHeight + m_extraHeight > size.GetHeight()) {
        // Vertical scrollbar will be shown. Adjust the width and recalculate layout to avoid horizontal scrollbar.
        width = std::max<int>(width - wxSystemSettings::GetMetric(wxSYS_VSCROLL_X, this), 0);
        cols  = std::max<int>(width / wxZRColaCharacterGridColumnWidth, 1);
        rows  = std::max<int>((char_len + cols - 1) / cols, 1);
    }

    BeginBatch();

    if (cols != m_numCols) {
        // Build and set new grid data.
        wxGridStringTable *table = new wxGridStringTable(rows, cols);
        for (int r = 0, i = 0; r < rows; r++)
            for (int c = 0; c < cols; c++, i++)
                table->SetValue(r, c, i < char_len ? wxString(1, m_chars[i]) : wxEmptyString);
        SetTable(table, true);
    }

    // Set column widths to stretch to full width.
    for (int c = 0, x_l = 0; c < cols; c++) {
        int x_r = (c + 1)*width/cols;
        SetColSize(c, x_r - x_l);
        x_l = x_r;
    }

    EndBatch();
    m_isResizing = false;
}
