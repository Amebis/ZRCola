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
    EVT_KEY_DOWN(wxZRColaCharGrid::OnKeyDown)
    EVT_TIMER(wxZRColaCharGrid::wxID_TOOLTIP_TIMER, wxZRColaCharGrid::OnTooltipTimer)
END_EVENT_TABLE()


wxZRColaCharGrid::wxZRColaCharGrid() : wxGrid()
{
    Init();
}


wxZRColaCharGrid::wxZRColaCharGrid(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name) : wxGrid(parent, id, pos, size, wxWANTS_CHARS, name)
{
    Init();

    SetDefaultRowSize(wxZRColaCharacterGridRowHeight);

    // Create timer for saving the state.
    m_toolTipTimer = new wxTimer(this, wxID_TOOLTIP_TIMER);

    // wxEVT_MOTION event must be connected to the wxGridWindow, not wxGrid itself.
    GetGridWindow()->Connect(GetGridWindow()->GetId(), wxEVT_MOTION, wxMouseEventHandler(wxZRColaCharGrid::OnMotion), NULL, this);
}


wxZRColaCharGrid::~wxZRColaCharGrid()
{
    GetGridWindow()->Disconnect(GetGridWindow()->GetId(), wxEVT_MOTION, wxMouseEventHandler(wxZRColaCharGrid::OnMotion), NULL, this);

    if (m_toolTipTimer)
        delete m_toolTipTimer;

    if (m_toolTip) {
        m_toolTip->SetTipWindowPtr(NULL);
        m_toolTip->Close();
    }
}


void wxZRColaCharGrid::Init()
{
    m_isResizing   = false;
    m_toolTip      = NULL;
    m_toolTipTimer = NULL;
    m_toolTipIdx   = (size_t)-1;
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


void wxZRColaCharGrid::OnKeyDown(wxKeyEvent& event)
{
    wxWindow *parentWnd;

    if (event.GetKeyCode() == WXK_TAB && (parentWnd = GetParent()) != NULL) {
        wxNavigationKeyEvent eventNav;
        eventNav.SetDirection(!event.ShiftDown());
        eventNav.SetWindowChange(event.ControlDown());
        eventNav.SetEventObject(this);

        if (parentWnd->HandleWindowEvent(eventNav))
            return;
    }

    event.Skip();
}


void wxZRColaCharGrid::OnMotion(wxMouseEvent& event)
{
    event.Skip();

    wxPoint ptMouse(CalcUnscrolledPosition(event.GetPosition()));
    int
        col = XToCol(ptMouse.x - m_rowLabelWidth ),
        row = YToRow(ptMouse.y - m_colLabelHeight);

    if (col == wxNOT_FOUND || row == wxNOT_FOUND )
        return;

    size_t toolTipIdx = row*m_numCols + col;
    if (toolTipIdx >= m_chars.Length()) {
        // Index out of range.
        m_toolTipIdx = (size_t)-1;
        m_toolTipTimer->Stop();
        return;
    } else if (toolTipIdx != m_toolTipIdx) {
        // Cell changed. Schedule tooltip display after 1s.
        m_toolTipIdx = toolTipIdx;
        m_toolTipTimer->Start(1000, true);
    }
}


void wxZRColaCharGrid::OnTooltipTimer(wxTimerEvent& event)
{
    event.Skip();

    if (m_toolTipIdx >= m_chars.Length())
        return;

    if (m_toolTip) {
        m_toolTip->SetTipWindowPtr(NULL);
        m_toolTip->Close();
        m_toolTip = NULL;
    }

    // Set tooltip.
    wxRect rcCell(CellToRect(m_toolTipIdx / m_numCols, m_toolTipIdx % m_numCols));
    rcCell.SetLeftTop(GetGridWindow()->ClientToScreen(CalcScrolledPosition(rcCell.GetLeftTop())));
    m_toolTip = new wxTipWindow(this, wxString::Format(wxT("U+%04X"), (int)m_chars[m_toolTipIdx]), 100, &m_toolTip, &rcCell);
}
