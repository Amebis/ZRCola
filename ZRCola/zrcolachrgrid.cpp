/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2022 Amebis
*/

#include "pch.h"


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

    SetDefaultRowSize(FromDIP(wxZRColaCharacterGridRowHeight));

    // Create timer for saving the state.
    m_timerToolTip.SetOwner(this, wxID_TOOLTIP_TIMER);

    // wxEVT_MOTION event must be connected to the wxGridWindow, not wxGrid itself.
    wxWindow *gridWnd = GetGridWindow();
    gridWnd->Connect(gridWnd->GetId(), wxEVT_MOTION, wxMouseEventHandler(wxZRColaCharGrid::OnMotion), NULL, this);
}


wxZRColaCharGrid::~wxZRColaCharGrid()
{
    wxWindow *gridWnd = GetGridWindow();
    gridWnd->Disconnect(gridWnd->GetId(), wxEVT_MOTION, wxMouseEventHandler(wxZRColaCharGrid::OnMotion), NULL, this);
}


void wxZRColaCharGrid::Init()
{
    m_regenerate = false;
    m_isResizing = false;
    m_toolTipIdx = (size_t)-1;
}


void wxZRColaCharGrid::SetCharacters(const wxString &chars)
{
    m_chars.Clear();
    const wxCStrData chr = chars.GetData();
    for (size_t i = 0, i_end = chars.Length(), i_next; i < i_end; i = i_next + 1) {
        i_next = i + _tcsnlen(chr + i, i_end - i);
        m_chars.Add(wxString(chr + i, chr + i_next));
    };
    m_relevance.Clear();
    m_regenerate = true;

    // Invoke OnSize(), which will populate the grid.
    wxSizeEvent e(GetSize(), m_windowId);
    e.SetEventObject(this);
    HandleWindowEvent(e);
}


void wxZRColaCharGrid::SetCharacters(const wxArrayString &chars)
{
    m_chars = chars;
    m_relevance.Clear();
    m_regenerate = true;

    // Invoke OnSize(), which will populate the grid.
    wxSizeEvent e(GetSize(), m_windowId);
    e.SetEventObject(this);
    HandleWindowEvent(e);
}


void wxZRColaCharGrid::SetCharacters(const wxString &chars, const wxArrayShort &relevance)
{
    m_chars.Clear();
    const wxCStrData chr = chars.GetData();
    for (size_t i = 0, i_end = chars.Length(), i_next; i < i_end; i = i_next + 1) {
        i_next = i + _tcsnlen(chr + i, i_end - i);
        m_chars.Add(wxString(chr + i, chr + i_next));
    };
    m_relevance  = relevance;
    m_regenerate = true;

    // Invoke OnSize(), which will populate the grid.
    wxSizeEvent e(GetSize(), m_windowId);
    e.SetEventObject(this);
    HandleWindowEvent(e);
}


wxString wxZRColaCharGrid::GetToolTipText(int idx)
{
    wxASSERT_MSG(idx < (int)m_chars.GetCount(), wxT("index out of bounds"));

    auto app = dynamic_cast<ZRColaApp*>(wxTheApp);
    const auto &chr = m_chars[idx];

    // See if this character has a key sequence registered.
    std::unique_ptr<ZRCola::keyseq_db::keyseq> ks((ZRCola::keyseq_db::keyseq*)new char[sizeof(ZRCola::keyseq_db::keyseq) + sizeof(wchar_t)*chr.length()]);
    ks->ZRCola::keyseq_db::keyseq::keyseq(NULL, 0, chr.data(), chr.length());
    ZRCola::keyseq_db::indexKey::size_type start;
    if (app->m_ks_db.idxChr.find(*ks, start)) {
        ZRCola::keyseq_db::keyseq &seq = app->m_ks_db.idxChr[start];
        wxString ks_str;
        if (ZRCola::keyseq_db::GetSequenceAsText(seq.seq(), seq.seq_len(), ks_str))
            return wxString::Format(wxT("U+%s (%s)"), ZRCola::GetUnicodeDump(chr.data(), chr.length(), _T("+")).c_str(), ks_str.c_str());
    }

    return wxString::Format(wxT("U+%s"), ZRCola::GetUnicodeDump(chr.data(), chr.length(), _T("+")).c_str());
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
        char_count = m_chars.GetCount();
    int
        col_width  = FromDIP(wxZRColaCharacterGridColumnWidth),
        row_height = FromDIP(wxZRColaCharacterGridRowHeight),
        width      = size.GetWidth() - m_rowLabelWidth - m_extraWidth,
        cols       = std::max<int>(width / col_width, 1),
        rows       = std::max<int>((int)((char_count + cols - 1) / cols), 1);

    if (m_colLabelHeight + rows*row_height + m_extraHeight > size.GetHeight()) {
        // Vertical scrollbar will be shown. Adjust the width and recalculate layout to avoid horizontal scrollbar.
        width = std::max<int>(width - wxSystemSettings::GetMetric(wxSYS_VSCROLL_X, this), 0);
        cols  = std::max<int>(width / col_width, 1);
        rows  = std::max<int>((int)((char_count + cols - 1) / cols), 1);
    }

    BeginBatch();

    if (m_regenerate || cols != m_numCols) {
        // Build and set new grid data.
        wxGridStringTable *table = new wxGridStringTable(rows, cols);
        for (int r = 0, i = 0; r < rows; r++)
            for (int c = 0; c < cols; c++, i++)
                table->SetValue(r, c, i < (int)char_count ? m_chars[i] : wxEmptyString);
        SetTable(table, true);
        if (!m_relevance.IsEmpty()) {
            const wxColour colour_def;
            const wxColour colour_irr = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT);
            for (int r = 0, i = 0; r < rows; r++)
                for (int c = 0; c < cols; c++, i++)
                    SetCellBackgroundColour(r, c, i >= (int)char_count || ((unsigned short)(m_relevance[i/16]) & (1<<(i%16))) ? colour_def : colour_irr);
        } else {
            for (int r = 0, i = 0; r < rows; r++)
                for (int c = 0; c < cols; c++, i++)
                    SetAttr(r, c, NULL);
        }
        m_regenerate = false;
    }

    for (int c = 0; c < cols; c++)
        SetColSize(c, col_width);

    //// Set column widths to stretch to full width.
    //for (int c = 0, x_l = 0; c < cols; c++) {
    //    int x_r = (c + 1)*width/cols;
    //    SetColSize(c, x_r - x_l);
    //    x_l = x_r;
    //}

    EndBatch();
    m_isResizing = false;
}


void wxZRColaCharGrid::OnKeyDown(wxKeyEvent& event)
{
    wxWindow *parentWnd;

    int key_code = event.GetKeyCode();
    if (key_code == WXK_TAB && (parentWnd = GetParent()) != NULL) {
        wxNavigationKeyEvent eventNav;
        eventNav.SetDirection(!event.ShiftDown());
        eventNav.SetWindowChange(event.ControlDown());
        eventNav.SetEventObject(this);

        if (parentWnd->HandleWindowEvent(eventNav))
            return;
    } else if (key_code == WXK_LEFT && m_currentCellCoords.GetCol() == 0 && m_currentCellCoords.GetRow()) {
        GoToCell(m_currentCellCoords.GetRow() - 1, m_numCols - 1);
        event.StopPropagation();
        return;
    } else if (key_code == WXK_RIGHT && m_currentCellCoords.GetCol() == m_numCols - 1 && m_currentCellCoords.GetRow() < m_numRows - 1) {
        GoToCell(m_currentCellCoords.GetRow() + 1, 0);
        event.StopPropagation();
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

    size_t toolTipIdx = (size_t)row*m_numCols + col;
    if (toolTipIdx >= m_chars.GetCount()) {
        // Index out of range.
        m_toolTipIdx = (size_t)-1;
        m_timerToolTip.Stop();
        return;
    } else if (toolTipIdx != m_toolTipIdx) {
        // Cell changed.
        m_toolTipIdx = toolTipIdx;
        wxWindow *gridWnd = GetGridWindow();
        if (gridWnd->GetToolTip()) {
            // The tooltip is already shown. Update it immediately.
            gridWnd->SetToolTip(GetToolTipText((int)m_toolTipIdx));
        } else {
            // This must be our initial entry. Schedule tooltip display after 1s.
            m_timerToolTip.Start(1000, true);
        }
    }
}


void wxZRColaCharGrid::OnTooltipTimer(wxTimerEvent& event)
{
    event.Skip();

    if (m_toolTipIdx >= m_chars.GetCount())
        return;

    GetGridWindow()->SetToolTip(GetToolTipText((int)m_toolTipIdx));
}
