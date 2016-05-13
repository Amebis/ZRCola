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


static int __cdecl compare_hits(const void *a, const void *b)
{
    const std::pair<unsigned long, wchar_t> *_a = (const std::pair<unsigned long, wchar_t>*)a;
    const std::pair<unsigned long, wchar_t> *_b = (const std::pair<unsigned long, wchar_t>*)b;

         if (_a->first > _b->first) return -1;
    else if (_a->first < _b->first) return  1;

         if (_a->second < _b->second) return -1;
    else if (_a->second > _b->second) return  1;

    return 0;
}



//////////////////////////////////////////////////////////////////////////
// wxZRColaCharSelect
//////////////////////////////////////////////////////////////////////////

wxZRColaCharSelect::wxZRColaCharSelect(wxWindow* parent) :
    m_searchChanged(false),
    m_unicodeChanged(false),
    m_char(0),
    wxZRColaCharSelectBase(parent)
{
    m_unicode->SetValidator(wxHexValidator<wchar_t>(&m_char));

    // Fill categories.
    ZRColaApp *app = (ZRColaApp*)wxTheApp;
    for (size_t i = 0, n = app->m_cc_db.idxRnk.size(); i < n; i++) {
        const ZRCola::chrcat_db::chrcat &cc = app->m_cc_db.idxRnk[i];
        int idx = m_categories->Insert(wxGetTranslation(wxString(cc.name, cc.name_len), wxT("ZRCola-zrcdb")), i);
        m_categories->Check(idx);
        m_ccOrder.insert(std::make_pair(cc.id, idx));
    }

    ResetResults();
}


void wxZRColaCharSelect::OnIdle(wxIdleEvent& event)
{
    event.Skip();

    if (m_unicodeChanged) {
        if (m_unicode->GetValidator()->TransferFromWindow()) {
            ZRColaApp *app = (ZRColaApp*)wxTheApp;

            m_gridPreview->SetCellValue(wxString(1, m_char), 0, 0);

            {
                ZRCola::character_db::character *chr = (ZRCola::character_db::character*)new char[sizeof(ZRCola::character_db::character)];
                chr->chr = m_char;
                size_t start;
                if (app->m_chr_db.idxChr.find(*chr, start)) {
                    const ZRCola::character_db::character &chr = app->m_chr_db.idxChr[start];
                    m_description->SetValue(wxString(chr.data, chr.desc_len));
                    m_gridRelated->SetCharacters(wxString(chr.data + chr.desc_len, chr.rel_len));
                } else {
                    m_description->SetValue(wxEmptyString);
                    m_gridRelated->ClearGrid();
                }
                m_gridRelated->Scroll(0, 0);
                delete chr;
            }
        }

        m_unicodeChanged = false;
    } else if (m_searchChanged) {
        m_timerSearch.Start(1000, true);

        m_searchChanged = false;
    }
}


void wxZRColaCharSelect::OnSearchText(wxCommandEvent& event)
{
    event.Skip();

    m_timerSearch.Stop();
    m_searchChanged = true;
}


void wxZRColaCharSelect::OnSearchEnter(wxCommandEvent& event)
{
    event.Skip();

    m_timerSearch.Stop();
    wxTimerEvent e(m_timerSearch);
    GetEventHandler()->ProcessEvent(e);

    m_searchChanged = false;
}


void wxZRColaCharSelect::OnSearchTimer(wxTimerEvent& event)
{
    wxString val(m_search->GetValue());
    if (!val.IsEmpty()) {
        ZRColaApp *app = (ZRColaApp*)wxTheApp;
        std::map<wchar_t, unsigned long> hits;
        std::set<ZRCola::chrcatid_t> cats;

        // Select categories.
        for (size_t i = 0, n = app->m_cc_db.idxRnk.size(); i < n; i++) {
            const ZRCola::chrcat_db::chrcat &cc = app->m_cc_db.idxRnk[i];
            if (m_categories->IsChecked(i))
                cats.insert(cc.id);
        }

        {
            // Search by indexes and merge results.
            std::map<wchar_t, unsigned long> hits_sub;
            app->m_chr_db.Search(val.c_str(), cats, hits, hits_sub);
            for (std::map<wchar_t, unsigned long>::const_iterator i = hits_sub.cbegin(), i_end = hits_sub.cend(); i != i_end; ++i) {
                std::map<wchar_t, unsigned long>::iterator idx = hits.find(i->first);
                if (idx == hits.end())
                    hits.insert(std::make_pair(i->first, i->second / 4));
                else
                    idx->second += i->second / 4;
            }
        }

        // Now sort the characters by rank.
        std::vector< std::pair<unsigned long, wchar_t> > hits2;
        hits2.reserve(hits.size());
        for (std::map<wchar_t, unsigned long>::const_iterator i = hits.cbegin(), i_end = hits.cend(); i != i_end; ++i)
            hits2.push_back(std::make_pair(i->second, i->first));
        std::qsort(hits2.data(), hits2.size(), sizeof(std::pair<unsigned long, wchar_t>), compare_hits);

        // Display results.
        wxString chars;
        chars.reserve(hits2.size());
        for (std::vector< std::pair<unsigned long, wchar_t> >::const_iterator i = hits2.cbegin(), i_end = hits2.cend(); i != i_end; ++i)
            chars += i->second;
        m_gridResults->SetCharacters(chars);
    } else
        ResetResults();

    m_gridResults->Scroll(0, 0);
}


void wxZRColaCharSelect::OnCategoriesToggle(wxCommandEvent& event)
{
    m_timerSearch.Stop();
    m_timerSearch.Start(500, true);
}


void wxZRColaCharSelect::OnResultSelectCell(wxGridEvent& event)
{
    wxString val(m_gridResults->GetCellValue(event.GetRow(), event.GetCol()));
    m_char = val.IsEmpty() ? 0 : val[0];
    m_unicode->GetValidator()->TransferToWindow();
}


void wxZRColaCharSelect::OnResultCellDClick(wxGridEvent& event)
{
    event.Skip();

    wxString val(m_gridResults->GetCellValue(event.GetRow(), event.GetCol()));
    if (!val.IsEmpty()) {
        m_char = val[0];
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
            m_char = val[0];
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
    wxString val(m_gridRecent->GetCellValue(event.GetRow(), event.GetCol()));
    m_char = val.IsEmpty() ? 0 : val[0];
    m_unicode->GetValidator()->TransferToWindow();
}


void wxZRColaCharSelect::OnRecentCellDClick(wxGridEvent& event)
{
    event.Skip();

    wxString val(m_gridRecent->GetCellValue(event.GetRow(), event.GetCol()));
    if (!val.IsEmpty()) {
        m_char = val[0];
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
            m_char = val[0];
            wxCommandEvent e(wxEVT_COMMAND_BUTTON_CLICKED, wxID_OK);
            m_sdbSizerButtonsOK->GetEventHandler()->ProcessEvent(e);

            event.StopPropagation();
            return;
        }
    }

    event.Skip();
}


void wxZRColaCharSelect::OnRelatedSelectCell(wxGridEvent& event)
{
    wxString val(m_gridRelated->GetCellValue(event.GetRow(), event.GetCol()));
    m_char = val.IsEmpty() ? 0 : val[0];
    m_unicode->GetValidator()->TransferToWindow();
}


void wxZRColaCharSelect::OnUnicodeText(wxCommandEvent& event)
{
    event.Skip();

    m_unicodeChanged = true;
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
        const ZRCola::character_db::character &chr = app->m_chr_db.idxChr[i];
        std::map<ZRCola::chrcatid_t, int>::const_iterator idx = m_ccOrder.find(chr.cat);
        if (idx == m_ccOrder.end() || m_categories->IsChecked(idx->second))
            val += chr.chr;
    }
    m_gridResults->SetCharacters(val);
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
        const ZRCola::chrcat_db::chrcat &cc = app->m_cc_db.idxRnk[i];
        wxString name(wxT("category"));
        name.Append(cc.id.data, _countof(cc.id.data));
        SaveValue(name, wnd->m_categories->IsChecked(i));
    }
}


bool wxPersistentZRColaCharSelect::Restore()
{
    wxZRColaCharSelect * const wnd = static_cast<wxZRColaCharSelect*>(GetWindow());

    wxString recent;
    if (RestoreValue(wxT("recentChars"), &recent))
        wnd->m_gridRecent->SetCharacters(recent);

    ZRColaApp *app = (ZRColaApp*)wxTheApp;
    for (size_t i = 0, n = app->m_cc_db.idxRnk.size(); i < n; i++) {
        const ZRCola::chrcat_db::chrcat &cc = app->m_cc_db.idxRnk[i];
        wxString name(wxT("category"));
        name.Append(cc.id.data, _countof(cc.id.data));
        bool val;
        if (RestoreValue(name, &val))
            wnd->m_categories->Check(i, val);
    }
    wnd->ResetResults();

    return wxPersistentDialog::Restore();
}
