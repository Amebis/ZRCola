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

wxDEFINE_EVENT(wxEVT_SEARCH_COMPLETE, wxThreadEvent);


wxZRColaCharSelect::wxZRColaCharSelect(wxWindow* parent) :
    m_searchChanged(false),
    m_unicodeChanged(false),
    m_char(0),
    m_searchThread(NULL),
    wxZRColaCharSelectBase(parent)
{
    Connect(wxID_ANY, wxEVT_SEARCH_COMPLETE, wxThreadEventHandler(wxZRColaCharSelect::OnSearchComplete), NULL, this);

    m_search_more->SetLabel(_(L"▸ Search Options"));

    m_unicode->SetValidator(wxHexValidator<wchar_t>(&m_char, wxNUM_VAL_DEFAULT, 4));

    // Fill categories.
    ZRColaApp *app = (ZRColaApp*)wxTheApp;
    for (size_t i = 0, n = app->m_cc_db.idxRnk.size(); i < n; i++) {
        const auto &cc = app->m_cc_db.idxRnk[i];
        int idx = m_categories->Insert(wxGetTranslation(wxString(cc.name, cc.name_len), wxT("ZRCola-zrcdb")), i);
        m_categories->Check(idx);
        m_ccOrder.insert(std::make_pair(cc.id, idx));
    }

    ResetResults();

    NavigationState state;
    state.m_char = m_char;
    state.m_related.m_selected.SetCol(m_gridRelated->GetGridCursorCol());
    state.m_related.m_selected.SetRow(m_gridRelated->GetGridCursorRow());
    m_historyCursor = m_history.insert(m_history.end(), state);
}


wxZRColaCharSelect::~wxZRColaCharSelect()
{
    if (m_searchThread)
        m_searchThread->Delete();

    Disconnect(wxID_ANY, wxEVT_SEARCH_COMPLETE, wxThreadEventHandler(wxZRColaCharSelect::OnSearchComplete), NULL, this);
}


void wxZRColaCharSelect::OnIdle(wxIdleEvent& event)
{
    event.Skip();

    if (m_unicodeChanged) {
        if (m_unicode->GetValidator()->TransferFromWindow()) {
            ZRColaApp *app = (ZRColaApp*)wxTheApp;

            m_gridPreview->SetCellValue(wxString(1, m_char), 0, 0);

            char chr[sizeof(ZRCola::character_db::character)] = {};
            ((ZRCola::character_db::character*)chr)->chr = m_char;
            size_t start;
            if (app->m_chr_db.idxChr.find(*(ZRCola::character_db::character*)chr, start)) {
                const auto &chr = app->m_chr_db.idxChr[start];
                // Update characted description.
                m_description->SetValue(wxString(chr.data, chr.desc_len));
                {
                    // See if this character has a key sequence registered.
                    char ks[sizeof(ZRCola::keyseq_db::keyseq)] = {};
                    ((ZRCola::keyseq_db::keyseq*)ks)->chr = m_char;
                    ZRCola::keyseq_db::indexKey::size_type start;
                    if (app->m_ks_db.idxChr.find(*(ZRCola::keyseq_db::keyseq*)ks, start)) {
                        ZRCola::keyseq_db::keyseq &seq = app->m_ks_db.idxChr[start];
                        wxString ks_str;
                        if (ZRCola::keyseq_db::GetSequenceAsText(seq.seq, seq.seq_len, ks_str))
                            m_shortcut->SetValue(ks_str);
                        else
                            m_shortcut->SetValue(wxEmptyString);
                    } else
                        m_shortcut->SetValue(wxEmptyString);
                }
                {
                    char cc[sizeof(ZRCola::chrcat_db::chrcat)] = {};
                    ((ZRCola::chrcat_db::chrcat*)cc)->id = chr.cat;
                    size_t start;
                    // Update character category.
                    if (app->m_cc_db.idxChrCat.find(*((ZRCola::chrcat_db::chrcat*)cc), start)) {
                        const auto &cat = app->m_cc_db.idxChrCat[start];
                        m_category->SetValue(wxGetTranslation(wxString(cat.name, cat.name_len), wxT("ZRCola-zrcdb")));
                    } else
                        m_category->SetValue(wxEmptyString);
                }
                // Update related characters.
                m_gridRelated->SetCharacters(wxString(chr.data + chr.desc_len, chr.rel_len));
            } else {
                m_description->SetValue(wxEmptyString);
                m_shortcut->SetValue(wxEmptyString);
                m_category->SetValue(wxEmptyString);
                m_gridRelated->ClearGrid();
            }

            m_gridRelated->GoToCell(m_historyCursor->m_related.m_selected);

            wxGridCellCoords coord(m_gridResults->GetCharacterCoords(m_char));
            if (coord.GetRow() != -1 && coord.GetCol() != -1) {
                m_gridResults->GoToCell(coord);
                m_gridResults->SetGridCursor(coord);
            } else
                m_gridResults->ClearSelection();
        }

        m_unicodeChanged = false;
    } else if (m_searchChanged) {
        if (m_searchThread)
            m_searchThread->Delete();

        wxString val(m_search->GetValue());
        if (!val.IsEmpty()) {
            ZRColaApp *app = (ZRColaApp*)wxTheApp;

            m_searchThread = new SearchThread(this);

            m_searchThread->m_search.assign(val.c_str(), val.Length());

            // Select categories.
            for (size_t i = 0, n = app->m_cc_db.idxRnk.size(); i < n; i++) {
                const auto &cc = app->m_cc_db.idxRnk[i];
                if (m_categories->IsChecked(i))
                    m_searchThread->m_cats.insert(cc.id);
            }

            if (m_searchThread->Run() != wxTHREAD_NO_ERROR) {
                wxFAIL_MSG("Can't create the thread!");
                delete m_searchThread;
                m_searchThread = NULL;
            }
        } else
            ResetResults();

        m_searchChanged = false;
    }
}


void wxZRColaCharSelect::OnSearchText(wxCommandEvent& event)
{
    event.Skip();

    m_searchChanged = true;
}


void wxZRColaCharSelect::OnSearchMore(wxHyperlinkEvent& event)
{
    event.StopPropagation();

    if (m_search_panel->IsShown()) {
        m_search_panel->Show(false);
        m_search_more->SetLabel(_(L"▸ Search Options"));
    } else {
        m_search_panel->Show(true);
        m_search_more->SetLabel(_(L"▾ Search Options"));
    }

    this->Layout();
}


void wxZRColaCharSelect::OnCategoriesAll(wxHyperlinkEvent& event)
{
    event.StopPropagation();

    ZRColaApp *app = (ZRColaApp*)wxTheApp;
    for (size_t i = 0, n = app->m_cc_db.idxRnk.size(); i < n; i++)
        m_categories->Check(i, true);

    m_searchChanged = true;
}


void wxZRColaCharSelect::OnCategoriesNone(wxHyperlinkEvent& event)
{
    event.StopPropagation();

    ZRColaApp *app = (ZRColaApp*)wxTheApp;
    for (size_t i = 0, n = app->m_cc_db.idxRnk.size(); i < n; i++)
        m_categories->Check(i, false);

    m_searchChanged = true;
}


void wxZRColaCharSelect::OnCategoriesInvert(wxHyperlinkEvent& event)
{
    event.StopPropagation();

    ZRColaApp *app = (ZRColaApp*)wxTheApp;
    for (size_t i = 0, n = app->m_cc_db.idxRnk.size(); i < n; i++)
        m_categories->Check(i, !m_categories->IsChecked(i));

    m_searchChanged = true;
}


void wxZRColaCharSelect::OnCategoriesToggle(wxCommandEvent& event)
{
    event.Skip();

    m_searchChanged = true;
}


void wxZRColaCharSelect::OnSearchComplete(wxThreadEvent& event)
{
    event.Skip();

    if (m_searchThread) {
        // Display results.
        wxString chars;
        chars.reserve(m_searchThread->m_hits.size());
        for (auto i = m_searchThread->m_hits.cbegin(), i_end = m_searchThread->m_hits.cend(); i != i_end; ++i)
            chars += i->second;
        m_gridResults->SetCharacters(chars);

        m_searchThread->Delete();
        m_searchThread = NULL;

        m_gridResults->Scroll(0, 0);
    }
}


void wxZRColaCharSelect::OnResultSelectCell(wxGridEvent& event)
{
    if (m_unicodeChanged) return;

    wxString val(m_gridResults->GetCellValue(event.GetRow(), event.GetCol()));
    if (!val.IsEmpty())
        NavigateTo(val[0]);
}


void wxZRColaCharSelect::OnResultCellDClick(wxGridEvent& event)
{
    event.Skip();

    wxString val(m_gridResults->GetCellValue(event.GetRow(), event.GetCol()));
    if (!val.IsEmpty()) {
        NavigateTo(val[0]);
        wxCommandEvent e(wxEVT_COMMAND_BUTTON_CLICKED, wxID_OK);
        m_sdbSizerButtonsOK->GetEventHandler()->ProcessEvent(e);
    }
}


void wxZRColaCharSelect::OnResultsKeyDown(wxKeyEvent& event)
{
    switch (event.GetKeyCode()) {
    case WXK_RETURN:
    case WXK_NUMPAD_ENTER:
        wxString val(m_gridResults->GetCellValue(m_gridResults->GetCursorRow(), m_gridResults->GetCursorColumn()));
        if (!val.IsEmpty()) {
            NavigateTo(val[0]);
            wxCommandEvent e(wxEVT_COMMAND_BUTTON_CLICKED, wxID_OK);
            m_sdbSizerButtonsOK->GetEventHandler()->ProcessEvent(e);

            event.StopPropagation();
            return;
        }
    }

    event.Skip();
}


void wxZRColaCharSelect::OnRecentSelectCell(wxGridEvent& event)
{
    if (m_unicodeChanged) return;

    wxString val(m_gridRecent->GetCellValue(event.GetRow(), event.GetCol()));
    if (!val.IsEmpty())
        NavigateTo(val[0]);
}


void wxZRColaCharSelect::OnRecentCellDClick(wxGridEvent& event)
{
    event.Skip();

    wxString val(m_gridRecent->GetCellValue(event.GetRow(), event.GetCol()));
    if (!val.IsEmpty()) {
        NavigateTo(val[0]);
        wxCommandEvent e(wxEVT_COMMAND_BUTTON_CLICKED, wxID_OK);
        m_sdbSizerButtonsOK->GetEventHandler()->ProcessEvent(e);
    }
}


void wxZRColaCharSelect::OnRecentKeyDown(wxKeyEvent& event)
{
    switch (event.GetKeyCode()) {
    case WXK_RETURN:
    case WXK_NUMPAD_ENTER:
        wxString val(m_gridRecent->GetCellValue(m_gridRecent->GetCursorRow(), m_gridRecent->GetCursorColumn()));
        if (!val.IsEmpty()) {
            NavigateTo(val[0]);
            wxCommandEvent e(wxEVT_COMMAND_BUTTON_CLICKED, wxID_OK);
            m_sdbSizerButtonsOK->GetEventHandler()->ProcessEvent(e);

            event.StopPropagation();
            return;
        }
    }

    event.Skip();
}


void wxZRColaCharSelect::OnUnicodeText(wxCommandEvent& event)
{
    event.Skip();

    m_unicodeChanged = true;
}


void wxZRColaCharSelect::OnPreviewKeyDown(wxKeyEvent& event)
{
    int key_code = event.GetKeyCode();
    if (key_code == WXK_TAB != NULL) {
        wxNavigationKeyEvent eventNav;
        eventNav.SetDirection(!event.ShiftDown());
        eventNav.SetWindowChange(event.ControlDown());
        eventNav.SetEventObject(this);

        if (HandleWindowEvent(eventNav))
            return;
    }

    event.Skip();
}


void wxZRColaCharSelect::OnNavigateBack(wxHyperlinkEvent& event)
{
    event.StopPropagation();

    NavigateBy(-1);
}


void wxZRColaCharSelect::OnNavigateForward(wxHyperlinkEvent& event)
{
    event.StopPropagation();

    NavigateBy(+1);
}


void wxZRColaCharSelect::OnRelatedSelectCell(wxGridEvent& event)
{
    if (m_unicodeChanged) return;

    wxString val(m_gridRelated->GetCellValue(event.GetRow(), event.GetCol()));
    if (!val.IsEmpty())
        NavigateTo(val[0]);
}


void wxZRColaCharSelect::OnOKButtonClick(wxCommandEvent& event)
{
    event.Skip();

    wxString
        recent(m_gridRecent->GetCharacters()),
        val(1, m_char);
    for (size_t i = 0, n = recent.Length(); i < n; i++) {
        const wxStringCharType c = recent[i];
        if (c != m_char)
            val += c;
    }

    m_gridRecent->SetCharacters(val);
}


void wxZRColaCharSelect::ResetResults()
{
    // Fill the results.
    ZRColaApp *app = (ZRColaApp*)wxTheApp;
    size_t i, n = app->m_chr_db.idxChr.size();
    wxString val;
    val.reserve(n);
    for (i = 0; i < n; i++) {
        const auto &chr = app->m_chr_db.idxChr[i];
        auto idx = m_ccOrder.find(chr.cat);
        if (idx == m_ccOrder.end() || m_categories->IsChecked(idx->second))
            val += chr.chr;
    }
    m_gridResults->SetCharacters(val);
}


void wxZRColaCharSelect::NavigateBy(int offset)
{
    if (offset != 0) {
        // Update history state
        m_historyCursor->m_related.m_selected.SetCol(m_gridRelated->GetGridCursorCol());
        m_historyCursor->m_related.m_selected.SetRow(m_gridRelated->GetGridCursorRow());

        if (offset < 0) {
            while (m_historyCursor != m_history.begin() && offset) {
                --m_historyCursor; offset++;
                m_char = m_historyCursor->m_char;
                m_unicodeChanged = true;
            }
        } else {
            while (offset) {
                ++m_historyCursor;
                if (m_historyCursor == m_history.end()) {
                    // We're past the last history entry.
                    --m_historyCursor;
                    break;
                }
                offset--;
                m_char = m_historyCursor->m_char;
                m_unicodeChanged = true;
            }
        }

        m_navigateBack->Enable(m_historyCursor != m_history.begin());
        auto cursor_next = m_historyCursor;
        ++cursor_next;
        m_navigateForward->Enable(cursor_next != m_history.end());

        if (m_unicodeChanged)
            m_unicode->GetValidator()->TransferToWindow();
    }
}


void wxZRColaCharSelect::NavigateTo(wchar_t c)
{
    if (m_char != c) {
        // Update history state
        m_historyCursor->m_related.m_selected.SetCol(m_gridRelated->GetGridCursorCol());
        m_historyCursor->m_related.m_selected.SetRow(m_gridRelated->GetGridCursorRow());

        ++m_historyCursor;

        // Create new state.
        NavigationState state;
        state.m_char = m_char = c;
        state.m_related.m_selected.SetCol(0);
        state.m_related.m_selected.SetRow(0);
        m_historyCursor = m_history.insert(m_historyCursor, state);

        // Purge the history's tail.
        auto cursor_next = m_historyCursor;
        ++cursor_next;
        m_history.erase(cursor_next, m_history.end());

        m_unicode->GetValidator()->TransferToWindow();

        m_navigateBack->Enable(true);
        m_navigateForward->Enable(false);
    }
}


wxZRColaCharSelect::SearchThread::SearchThread(wxZRColaCharSelect *parent) :
    m_parent(parent),
    wxThread(wxTHREAD_JOINABLE)
{
    //// This is a worker thread. Set priority between minimal and normal.
    //SetPriority((wxPRIORITY_MIN + wxPRIORITY_DEFAULT) / 2);
}


wxThread::ExitCode wxZRColaCharSelect::SearchThread::Entry()
{
    ZRColaApp *app = (ZRColaApp*)wxTheApp;
    std::map<wchar_t, ZRCola::charrank_t> hits;

    if (TestDestroy()) return (wxThread::ExitCode)1;

    {
        // Search by indexes and merge results.
        std::map<wchar_t, ZRCola::charrank_t> hits_sub;
        if (!app->m_chr_db.Search(m_search.c_str(), m_cats, hits, hits_sub, TestDestroyS, this)) return (wxThread::ExitCode)1;
        for (auto i = hits_sub.cbegin(), i_end = hits_sub.cend(); i != i_end; ++i) {
            if (TestDestroy()) return (wxThread::ExitCode)1;
            auto idx = hits.find(i->first);
            if (idx == hits.end())
                hits.insert(std::make_pair(i->first, i->second / 4));
            else
                idx->second += i->second / 4;
        }
    }

    // Get best rank.
    ZRCola::charrank_t rank_ref = 0;
    for (auto i = hits.cbegin(), i_end = hits.cend(); i != i_end; ++i) {
        if (TestDestroy()) return (wxThread::ExitCode)1;
        if (i->second > rank_ref)
            rank_ref = i->second;
    }

    // Now sort the characters by rank (taking only top 3/4 by rank).
    ZRCola::charrank_t rank_threshold = rank_ref*3/4;
    m_hits.reserve(hits.size());
    for (auto i = hits.cbegin(), i_end = hits.cend(); i != i_end; ++i) {
        if (TestDestroy()) return (wxThread::ExitCode)1;
        if (i->second > rank_threshold)
            m_hits.push_back(std::make_pair(i->second, i->first));
    }
    std::qsort(m_hits.data(), m_hits.size(), sizeof(std::pair<ZRCola::charrank_t, wchar_t>), CompareHits);

    // Signal the event handler that this thread is going to be destroyed.
    // NOTE: here we assume that using the m_parent pointer is safe,
    //       (in this case this is assured by the wxZRColaCharSelect destructor)
    wxQueueEvent(m_parent, new wxThreadEvent(wxEVT_SEARCH_COMPLETE));

    return 0;
}


int __cdecl wxZRColaCharSelect::SearchThread::CompareHits(const void *a, const void *b)
{
    const std::pair<ZRCola::charrank_t, wchar_t> *_a = (const std::pair<ZRCola::charrank_t, wchar_t>*)a;
    const std::pair<ZRCola::charrank_t, wchar_t> *_b = (const std::pair<ZRCola::charrank_t, wchar_t>*)b;

         if (_a->first > _b->first) return -1;
    else if (_a->first < _b->first) return  1;

         if (_a->second < _b->second) return -1;
    else if (_a->second > _b->second) return  1;

    return 0;
}


bool __cdecl wxZRColaCharSelect::SearchThread::TestDestroyS(void *cookie)
{
    return static_cast<wxZRColaCharSelect::SearchThread*>(cookie)->TestDestroy();
}


//////////////////////////////////////////////////////////////////////////
// wxPersistentZRColaCharSelect
//////////////////////////////////////////////////////////////////////////

wxPersistentZRColaCharSelect::wxPersistentZRColaCharSelect(wxZRColaCharSelect *wnd) : wxPersistentDialog(wnd)
{
}


void wxPersistentZRColaCharSelect::Save() const
{
    wxPersistentDialog::Save();

    const wxZRColaCharSelect * const wnd = static_cast<const wxZRColaCharSelect*>(GetWindow());

    SaveValue(wxT("recentChars"), wnd->m_gridRecent->GetCharacters());

    ZRColaApp *app = (ZRColaApp*)wxTheApp;
    for (size_t i = 0, n = app->m_cc_db.idxRnk.size(); i < n; i++) {
        const auto &cc = app->m_cc_db.idxRnk[i];
        wxString name(wxT("category"));
        name.Append(cc.id.data, _countof(cc.id.data));
        SaveValue(name, wnd->m_categories->IsChecked(i));
    }

    SaveValue(wxT("searchPanel"), wnd->m_search_panel->IsShown());
}


bool wxPersistentZRColaCharSelect::Restore()
{
    wxZRColaCharSelect * const wnd = static_cast<wxZRColaCharSelect*>(GetWindow());

    wxString recent;
    if (RestoreValue(wxT("recentChars"), &recent))
        wnd->m_gridRecent->SetCharacters(recent);

    ZRColaApp *app = (ZRColaApp*)wxTheApp;
    for (size_t i = 0, n = app->m_cc_db.idxRnk.size(); i < n; i++) {
        const auto &cc = app->m_cc_db.idxRnk[i];
        wxString name(wxT("category"));
        name.Append(cc.id.data, _countof(cc.id.data));
        bool val;
        if (RestoreValue(name, &val))
            wnd->m_categories->Check(i, val);
    }

    bool search_panel;
    if (RestoreValue(wxT("searchPanel"), &search_panel)) {
        if (search_panel) {
            wnd->m_search_panel->Show(true);
            wnd->m_search_more->SetLabel(_(L"▾ Search Options"));
        } else {
            wnd->m_search_panel->Show(false);
            wnd->m_search_more->SetLabel(_(L"▸ Search Options"));
        }
    }

    wnd->ResetResults();

    return wxPersistentDialog::Restore();
}
