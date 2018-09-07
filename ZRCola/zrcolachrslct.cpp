/*
    Copyright 2015-2017 Amebis

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


//////////////////////////////////////////////////////////////////////
// wxZRColaUTF16CharValidator
//////////////////////////////////////////////////////////////////////

wxIMPLEMENT_DYNAMIC_CLASS(wxZRColaUTF16CharValidator, wxValidator);


wxZRColaUTF16CharValidator::wxZRColaUTF16CharValidator(wchar_t *val) :
    m_val(val),
    wxValidator()
{
}


wxObject* wxZRColaUTF16CharValidator::Clone() const
{
    return new wxZRColaUTF16CharValidator(*this);
}


bool wxZRColaUTF16CharValidator::Validate(wxWindow *parent)
{
    wxASSERT(GetWindow()->IsKindOf(CLASSINFO(wxTextCtrl)));
    wxTextCtrl *ctrl = (wxTextCtrl*)GetWindow();
    if (!ctrl->IsEnabled()) return true;

    wxString val(ctrl->GetValue());
    return Parse(val, 0, val.Length(), ctrl, parent);
}


bool wxZRColaUTF16CharValidator::TransferToWindow()
{
    wxASSERT(GetWindow()->IsKindOf(CLASSINFO(wxTextCtrl)));

    if (m_val)
        ((wxTextCtrl*)GetWindow())->SetValue(wxString::Format(wxT("%04X"), *m_val));

    return true;
}


bool wxZRColaUTF16CharValidator::TransferFromWindow()
{
    wxASSERT(GetWindow()->IsKindOf(CLASSINFO(wxTextCtrl)));
    wxTextCtrl *ctrl = (wxTextCtrl*)GetWindow();

    wxString val(ctrl->GetValue());
    return Parse(val, 0, val.Length(), ctrl, NULL, m_val);
}


bool wxZRColaUTF16CharValidator::Parse(const wxString &val_in, size_t i_start, size_t i_end, wxTextCtrl *ctrl, wxWindow *parent, wchar_t *val_out)
{
    const wxStringCharType *buf = val_in;

    wchar_t chr = 0;
    for (size_t i = i_start;;) {
        if (i >= i_end) {
            // End of Unicode found.
            if (val_out) *val_out = chr;
            return true;
        } else if (i >= i_start + 4) {
            // Maximum characters exceeded.
            ctrl->SetFocus();
            ctrl->SetSelection(i, i_end);
            wxMessageBox(_("Too many digits in Unicode."), _("Validation conflict"), wxOK | wxICON_EXCLAMATION, parent);
            return false;
        } else if (_T('0') <= buf[i] && buf[i] <= _T('9')) {
            // Digit found.
            chr = (chr << 4) | (buf[i] - _T('0'));
            i++;
        } else if (_T('A') <= buf[i] && buf[i] <= _T('F')) {
            // Capital letter found.
            chr = (chr << 4) | (buf[i] - _T('A') + 10);
            i++;
        } else if (_T('a') <= buf[i] && buf[i] <= _T('f')) {
            // Lower letter found.
            chr = (chr << 4) | (buf[i] - _T('a') + 10);
            i++;
        } else {
            // Invalid character found.
            ctrl->SetFocus();
            ctrl->SetSelection(i, i + 1);
            wxMessageBox(wxString::Format(_("Invalid character in Unicode found: %c"), buf[i]), _("Validation conflict"), wxOK | wxICON_EXCLAMATION, parent);
            return false;
        }
    }
}


//////////////////////////////////////////////////////////////////////
// wxZRColaUnicodeDumpValidator
//////////////////////////////////////////////////////////////////////

wxIMPLEMENT_DYNAMIC_CLASS(wxZRColaUnicodeDumpValidator, wxValidator);


wxZRColaUnicodeDumpValidator::wxZRColaUnicodeDumpValidator(wxString *val) :
    m_val(val),
    wxValidator()
{
}


wxObject* wxZRColaUnicodeDumpValidator::Clone() const
{
    return new wxZRColaUnicodeDumpValidator(*this);
}


bool wxZRColaUnicodeDumpValidator::Validate(wxWindow *parent)
{
    wxASSERT(GetWindow()->IsKindOf(CLASSINFO(wxTextCtrl)));
    wxTextCtrl *ctrl = (wxTextCtrl*)GetWindow();
    if (!ctrl->IsEnabled()) return true;

    wxString val(ctrl->GetValue());
    return Parse(val, 0, val.Length(), ctrl, parent);
}


bool wxZRColaUnicodeDumpValidator::TransferToWindow()
{
    wxASSERT(GetWindow()->IsKindOf(CLASSINFO(wxTextCtrl)));

    if (m_val)
        ((wxTextCtrl*)GetWindow())->SetValue(ZRCola::GetUnicodeDumpW(m_val->c_str(), m_val->length(), L"+"));

    return true;
}


bool wxZRColaUnicodeDumpValidator::TransferFromWindow()
{
    wxASSERT(GetWindow()->IsKindOf(CLASSINFO(wxTextCtrl)));
    wxTextCtrl *ctrl = (wxTextCtrl*)GetWindow();

    wxString val(ctrl->GetValue());
    return Parse(val, 0, val.Length(), ctrl, NULL, m_val);
}


bool wxZRColaUnicodeDumpValidator::Parse(const wxString &val_in, size_t i_start, size_t i_end, wxTextCtrl *ctrl, wxWindow *parent, wxString *val_out)
{
    const wxStringCharType *buf = val_in;

    wxString str;
    for (size_t i = i_start;;) {
        const wxStringCharType *buf_next;
        wchar_t chr;
        if ((buf_next = wmemchr(buf + i, L'+', i_end - i)) != NULL) {
            // Unicode dump separator found.
            if (!wxZRColaUTF16CharValidator::Parse(val_in, i, buf_next - buf, ctrl, parent, &chr))
                return false;
            str += chr;
            i = buf_next - buf + 1;
        } else if (wxZRColaUTF16CharValidator::Parse(val_in, i, i_end, ctrl, parent, &chr)) {
            // The rest of the FQDN parsed succesfully.
            if (chr) str += chr;
            if (val_out) *val_out = str;
            return true;
        } else
            return false;
    }
}


//////////////////////////////////////////////////////////////////////////
// wxZRColaCharSelect
//////////////////////////////////////////////////////////////////////////

wxDEFINE_EVENT(wxEVT_SEARCH_COMPLETE, wxThreadEvent);


wxZRColaCharSelect::wxZRColaCharSelect(wxWindow* parent) :
    m_searchChanged(false),
    m_unicodeChanged(false),
    m_searchThread(NULL),
    wxZRColaCharSelectBase(parent)
{
    // Set tag lookup locale.
    wxLanguage language = dynamic_cast<ZRColaApp*>(wxTheApp)->m_lang_ui;
         if (wxLANGUAGE_DEFAULT   == language ||
             wxLANGUAGE_ENGLISH   <= language && language <= wxLANGUAGE_ENGLISH_ZIMBABWE) m_locale = MAKELCID(MAKELANGID(LANG_ENGLISH  , SUBLANG_DEFAULT), SORT_DEFAULT);
    else if (wxLANGUAGE_RUSSIAN   <= language && language <= wxLANGUAGE_RUSSIAN_UKRAINE ) m_locale = MAKELCID(MAKELANGID(LANG_RUSSIAN  , SUBLANG_DEFAULT), SORT_DEFAULT);
    else if (wxLANGUAGE_SLOVENIAN == language                                           ) m_locale = MAKELCID(MAKELANGID(LANG_SLOVENIAN, SUBLANG_DEFAULT), SORT_DEFAULT);
    else                                                                                  m_locale = MAKELCID(MAKELANGID(LANG_ENGLISH  , SUBLANG_DEFAULT), SORT_DEFAULT);

    Connect(wxID_ANY, wxEVT_SEARCH_COMPLETE, wxThreadEventHandler(wxZRColaCharSelect::OnSearchComplete), NULL, this);

    m_search_more->SetLabel(_(L"▸ Search Options"));

    m_unicode->SetValidator(wxZRColaUnicodeDumpValidator(&m_char));

    // Fill categories.
    auto app = dynamic_cast<ZRColaApp*>(wxTheApp);
    for (size_t i = 0, n = app->m_cc_db.idxRank.size(); i < n; i++) {
        const auto &cc = app->m_cc_db.idxRank[i];
        int idx = m_categories->Insert(wxGetTranslation(wxString(cc.name(), cc.name_len()), wxT("ZRCola-zrcdb")), i);
        m_categories->Check(idx);
        m_ccOrder.insert(std::make_pair(cc.cat, idx));
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
            auto app = dynamic_cast<ZRColaApp*>(wxTheApp);

            m_gridPreview->SetCellValue(0, 0, m_char);

            std::unique_ptr<ZRCola::character_db::character> ch((ZRCola::character_db::character*)new char[sizeof(ZRCola::character_db::character) + sizeof(wchar_t)*m_char.length()]);
            ch->ZRCola::character_db::character::character(m_char.data(), m_char.length());
            ZRCola::character_db::indexChr::size_type ch_start;
            if (app->m_chr_db.idxChr.find(*ch, ch_start)) {
                const auto &chr = app->m_chr_db.idxChr[ch_start];
                // Update character description.
                m_description->SetValue(wxString(chr.desc(), chr.desc_len()));
                {
                    // See if this character has a key sequence registered.
                    std::unique_ptr<ZRCola::keyseq_db::keyseq> ks((ZRCola::keyseq_db::keyseq*)new char[sizeof(ZRCola::keyseq_db::keyseq) + sizeof(wchar_t)*m_char.length()]);
                    ks->ZRCola::keyseq_db::keyseq::keyseq(NULL, 0, m_char.data(), m_char.length());
                    ZRCola::keyseq_db::indexKey::size_type ks_start;
                    if (app->m_ks_db.idxChr.find(*ks, ks_start)) {
                        const auto &seq = app->m_ks_db.idxChr[ks_start];
                        wxString ks_str;
                        if (ZRCola::keyseq_db::GetSequenceAsText(seq.seq(), seq.seq_len(), ks_str))
                            m_shortcut->SetValue(ks_str);
                        else
                            m_shortcut->SetValue(wxEmptyString);
                    } else
                        m_shortcut->SetValue(wxEmptyString);
                }
                {
                    // Update character category.
                    ZRCola::chrcat_db::indexChrCat::size_type cc_start;
                    if (app->m_cc_db.idxChrCat.find(ZRCola::chrcat_db::chrcat(chr.cat), cc_start)) {
                        const auto &cat = app->m_cc_db.idxChrCat[cc_start];
                        m_category->SetValue(wxGetTranslation(wxString(cat.name(), cat.name_len()), wxT("ZRCola-zrcdb")));
                    } else
                        m_category->SetValue(wxEmptyString);
                }
                // Update related characters.
                m_gridRelated->SetCharacters(wxString(chr.rel(), chr.rel_end()));
            } else {
                m_description->SetValue(wxEmptyString);
                m_shortcut->SetValue(wxEmptyString);
                m_category->SetValue(wxEmptyString);
                m_gridRelated->ClearGrid();
            }

            // Find character tags.
            std::list<std::wstring> tag_names;
            std::unique_ptr<ZRCola::chrtag_db::chrtag> ct((ZRCola::chrtag_db::chrtag*)new char[sizeof(ZRCola::chrtag_db::chrtag) + sizeof(wchar_t)*m_char.length()]);
            ct->ZRCola::chrtag_db::chrtag::chrtag(m_char.data(), m_char.length());
            ZRCola::chrtag_db::indexChr::size_type ct_start, ct_end;
            if (app->m_ct_db.idxChr.find(*ct, ct_start, ct_end)) {
                for (auto i = ct_start; i < ct_end; i++) {
                    // Find tag names.
                    ZRCola::tagname_db::indexTag::size_type tn_start, tn_end;
                    if (app->m_tn_db.idxTag.find(ZRCola::tagname_db::tagname(app->m_ct_db.idxChr[i].tag, m_locale), tn_start, tn_end)) {
                        for (auto j = tn_start; j < tn_end; j++) {
                            const auto &tn = app->m_tn_db.idxTag[j];

                            // Add tag name to the list (prevent duplicates).
                            for (auto name = tag_names.cbegin(), name_end = tag_names.cend();; ++name) {
                                if (name == name_end) {
                                    // Add name to the list.
                                    tag_names.push_back(std::wstring(tn.name(), tn.name_end()));
                                    break;
                                } else if (ZRCola::tagname_db::tagname::CompareName(m_locale, name->data(), (unsigned __int16)name->length(), tn.name(), tn.name_len()) == 0)
                                    // Name is already on the list.
                                    break;
                            }
                        }
                    }
                }
            }
            wxString tags;
            for (auto name = tag_names.cbegin(), name_end = tag_names.cend(); name != name_end; ++name) {
                if (!tags.empty())
                    tags += _(", ");
                tags += *name;
            }
            m_tags->SetValue(tags);

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
            auto app = dynamic_cast<ZRColaApp*>(wxTheApp);

            m_searchThread = new SearchThread(this);

            m_searchThread->m_search.assign(val.c_str(), val.Length());

            // Select categories.
            for (size_t i = 0, n = app->m_cc_db.idxRank.size(); i < n; i++) {
                const auto &cc = app->m_cc_db.idxRank[i];
                if (m_categories->IsChecked(i))
                    m_searchThread->m_cats.insert(cc.cat);
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

    auto app = dynamic_cast<ZRColaApp*>(wxTheApp);
    for (size_t i = 0, n = app->m_cc_db.idxRank.size(); i < n; i++)
        m_categories->Check(i, true);

    m_searchChanged = true;
}


void wxZRColaCharSelect::OnCategoriesNone(wxHyperlinkEvent& event)
{
    event.StopPropagation();

    auto app = dynamic_cast<ZRColaApp*>(wxTheApp);
    for (size_t i = 0, n = app->m_cc_db.idxRank.size(); i < n; i++)
        m_categories->Check(i, false);

    m_searchChanged = true;
}


void wxZRColaCharSelect::OnCategoriesInvert(wxHyperlinkEvent& event)
{
    event.StopPropagation();

    auto app = dynamic_cast<ZRColaApp*>(wxTheApp);
    for (size_t i = 0, n = app->m_cc_db.idxRank.size(); i < n; i++)
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
        wxArrayString chars;
        chars.reserve(m_searchThread->m_hits.size());
        for (auto i = m_searchThread->m_hits.cbegin(), i_end = m_searchThread->m_hits.cend(); i != i_end; ++i)
            chars.Add(i->second);
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
        NavigateTo(val);
}


void wxZRColaCharSelect::OnResultCellDClick(wxGridEvent& event)
{
    event.Skip();

    wxString val(m_gridResults->GetCellValue(event.GetRow(), event.GetCol()));
    if (!val.IsEmpty()) {
        NavigateTo(val);
        wxCommandEvent e(wxEVT_COMMAND_BUTTON_CLICKED, wxID_OK);
        m_sdbSizerButtonsOK->GetEventHandler()->ProcessEvent(e);
    }
}


void wxZRColaCharSelect::OnResultsKeyDown(wxKeyEvent& event)
{
    switch (event.GetKeyCode()) {
    case WXK_RETURN:
    case WXK_NUMPAD_ENTER:
        wxString val(m_gridResults->GetCellValue(m_gridResults->GetGridCursorRow(), m_gridResults->GetGridCursorCol()));
        if (!val.IsEmpty()) {
            NavigateTo(val);
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
        NavigateTo(val);
}


void wxZRColaCharSelect::OnRecentCellDClick(wxGridEvent& event)
{
    event.Skip();

    wxString val(m_gridRecent->GetCellValue(event.GetRow(), event.GetCol()));
    if (!val.IsEmpty()) {
        NavigateTo(val);
        wxCommandEvent e(wxEVT_COMMAND_BUTTON_CLICKED, wxID_OK);
        m_sdbSizerButtonsOK->GetEventHandler()->ProcessEvent(e);
    }
}


void wxZRColaCharSelect::OnRecentKeyDown(wxKeyEvent& event)
{
    switch (event.GetKeyCode()) {
    case WXK_RETURN:
    case WXK_NUMPAD_ENTER:
        wxString val(m_gridRecent->GetCellValue(m_gridRecent->GetGridCursorRow(), m_gridRecent->GetGridCursorCol()));
        if (!val.IsEmpty()) {
            NavigateTo(val);
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
        NavigateTo(val);
}


void wxZRColaCharSelect::OnOKButtonClick(wxCommandEvent& event)
{
    event.Skip();

    const wxArrayString &recent = m_gridRecent->GetCharacters();
    wxArrayString val;
    val.reserve(recent.GetCount() + 1);
    val.Add(m_char);
    for (size_t i = 0, n = recent.GetCount(); i < n; i++) {
        const wxString &c = recent[i];
        if (c != m_char)
            val.Add(c);
    }
    m_gridRecent->SetCharacters(val);
}


void wxZRColaCharSelect::ResetResults()
{
    // Fill the results.
    auto app = dynamic_cast<ZRColaApp*>(wxTheApp);
    size_t i, n = app->m_chr_db.idxChr.size();
    wxArrayString val;
    val.reserve(n);
    for (i = 0; i < n; i++) {
        const auto &chr = app->m_chr_db.idxChr[i];
        auto idx = m_ccOrder.find(chr.cat);
        if (idx == m_ccOrder.end() || m_categories->IsChecked(idx->second))
            val.Add(wxString(chr.chr(), chr.chr_len()));
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


void wxZRColaCharSelect::NavigateTo(const wxString &c)
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
    auto app = dynamic_cast<ZRColaApp*>(wxTheApp);
    std::map<std::wstring, ZRCola::charrank_t> hits;

    if (TestDestroy()) return (wxThread::ExitCode)1;

    {
        // Search by tags: Get tags with given names. Then, get characters of found tags.
        std::map<ZRCola::tagid_t, unsigned __int16> hits_tag;
        if (!app->m_tn_db.Search(m_search.c_str(), m_parent->m_locale, hits_tag, TestDestroyS, this)) return (wxThread::ExitCode)1;
        if (!app->m_ct_db.Search(hits_tag, app->m_chr_db, m_cats, hits, TestDestroyS, this)) return (wxThread::ExitCode)1;
    }

    {
        // Search by description and merge results.
        std::map<std::wstring, ZRCola::charrank_t> hits_sub;
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
    std::qsort(m_hits.data(), m_hits.size(), sizeof(std::pair<ZRCola::charrank_t, std::wstring>), CompareHits);

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

    auto wnd = static_cast<const wxZRColaCharSelect*>(GetWindow()); // dynamic_cast is not reliable as we are typically called late in the wxTopLevelWindowMSW destructor.
    auto app = dynamic_cast<ZRColaApp*>(wxTheApp);

    wxString str, str2;
    auto &recent = wnd->m_gridRecent->GetCharacters();
    for (size_t i = 0, n = recent.GetCount(); i < n; i++) {
        if (i) str2 += wxT('|');
        auto &chr = recent[i];
        for (size_t j = 0, m = chr.Length(); j < m; j++) {
            if (j) str2 += wxT('+');
            str2 += wxString::Format(wxT("%04X"), chr[j]);
        }
        if (chr.Length() == 1)
            str += chr[0];
    }
    SaveValue(wxT("recentChars" ), str ); // Save in legacy format for backward compatibility.
    SaveValue(wxT("recentChars2"), str2); // Save in native format

    for (size_t i = 0, n = app->m_cc_db.idxRank.size(); i < n; i++) {
        const auto &cc = app->m_cc_db.idxRank[i];
        wxString name(wxT("category"));
        name.Append(cc.cat.data, _countof(cc.cat.data));
        SaveValue(name, wnd->m_categories->IsChecked(i));
    }

    SaveValue(wxT("searchPanel"), wnd->m_search_panel->IsShown());
}


bool wxPersistentZRColaCharSelect::Restore()
{
    auto wnd = dynamic_cast<wxZRColaCharSelect*>(GetWindow());
    auto app = dynamic_cast<ZRColaApp*>(wxTheApp);

    wxString str;
    if (RestoreValue(wxT("recentChars2"), &str)) {
        // Native format found.
        wxArrayString val;
        for (wxStringTokenizer tok(str, wxT("|")); tok.HasMoreTokens(); ) {
            wxString chr;
            for (wxStringTokenizer tok_chr(tok.GetNextToken(), wxT("+")); tok_chr.HasMoreTokens(); )
                chr += (wchar_t)_tcstoul(tok_chr.GetNextToken().c_str(), NULL, 16);
            val.Add(chr);
        }
        wnd->m_gridRecent->SetCharacters(val);
    } else if (RestoreValue(wxT("recentChars"), &str)) {
        // Legacy value found.
        wxArrayString val;
        for (size_t i = 0, n = str.Length(); i < n; i++)
            val.Add(wxString(1, str[i]));
        wnd->m_gridRecent->SetCharacters(val);
    }

    for (size_t i = 0, n = app->m_cc_db.idxRank.size(); i < n; i++) {
        const auto &cc = app->m_cc_db.idxRank[i];
        wxString name(wxT("category"));
        name.Append(cc.cat.data, _countof(cc.cat.data));
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
