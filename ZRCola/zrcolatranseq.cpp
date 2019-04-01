/*
    Copyright 2015-2019 Amebis

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
// wxZRColaTranslationSeq
//////////////////////////////////////////////////////////////////////////

wxZRColaTranslationSeq::wxZRColaTranslationSeq(wxWindow* parent) :
    wxZRColaTranslationSeqBase(parent)
{
    auto app = dynamic_cast<ZRColaApp*>(wxTheApp);

    m_available->Clear();
    for (size_t i = 0, n = app->m_ts_db.idxTranSet.size(); i < n; i++) {
        const ZRCola::transet_db::transet &ts = app->m_ts_db.idxTranSet[i];
        wxString
            src(ts.src(), ts.src_len()),
            dst(ts.dst(), ts.dst_len()),
            src_tran(wxGetTranslation(src, wxT("ZRCola-zrcdb"))),
            dst_tran(wxGetTranslation(dst, wxT("ZRCola-zrcdb"))),
            label_tran(wxString::Format(wxT("%s \x00bb %s"), src_tran.c_str(), dst_tran.c_str()));
        m_available->Append(label_tran, (void*)ts.set);
    }
}


void wxZRColaTranslationSeq::OnInitDialog(wxInitDialogEvent& event)
{
    auto app = dynamic_cast<ZRColaApp*>(wxTheApp);

    m_selected->Clear();
    for (auto s = m_transeq.cbegin(), s_end = m_transeq.cend(); s != s_end; ++s) {
        ZRCola::transet_db::indexTranSet::size_type start;
        if (app->m_ts_db.idxTranSet.find(ZRCola::transet_db::transet(*s), start)) {
            const ZRCola::transet_db::transet &ts = app->m_ts_db.idxTranSet[start];
            wxString
                src(ts.src(), ts.src_len()),
                dst(ts.dst(), ts.dst_len()),
                src_tran(wxGetTranslation(src, wxT("ZRCola-zrcdb"))),
                dst_tran(wxGetTranslation(dst, wxT("ZRCola-zrcdb"))),
                label_tran(wxString::Format(wxT("%s \x00bb %s"), src_tran.c_str(), dst_tran.c_str()));
            m_selected->Append(label_tran, (void*)ts.set);
        } else {
            wxString label_tran(wxString::Format(_("Unknown translation (%u)"), (unsigned int)*s));
            m_selected->Append(label_tran, (void*)*s);
        }
    }

    wxZRColaTranslationSeqBase::OnInitDialog(event);
}


void wxZRColaTranslationSeq::OnUpdate(wxUpdateUIEvent& event)
{
    m_add->Enable(m_available->GetSelection() != wxNOT_FOUND);

    int idx = m_selected->GetSelection();
    m_remove      ->Enable(idx != wxNOT_FOUND);
    m_selectedUp  ->Enable(idx != wxNOT_FOUND && (unsigned int)idx > 0);
    m_selectedDown->Enable(idx != wxNOT_FOUND && (unsigned int)idx < m_selected->GetCount() - 1);

    wxZRColaTranslationSeqBase::OnUpdate(event);
}


void wxZRColaTranslationSeq::OnAvailableChar(wxKeyEvent& event)
{
    if (event.GetRawKeyCode() == VK_INSERT) {
        int idx = m_available->GetSelection();
        if (idx != wxNOT_FOUND) {
            m_selected->Append(
                m_available->GetString(idx),
                m_available->GetClientData(idx));
        }
    }

    wxZRColaTranslationSeqBase::OnAvailableChar(event);
}


void wxZRColaTranslationSeq::OnAvailableDClick(wxCommandEvent& event)
{
    int idx = m_available->GetSelection();
    if (idx != wxNOT_FOUND) {
        m_selected->Append(
            m_available->GetString(idx),
            m_available->GetClientData(idx));
    }

    wxZRColaTranslationSeqBase::OnAvailableDClick(event);
}


void wxZRColaTranslationSeq::OnAdd(wxCommandEvent& event)
{
    int idx = m_available->GetSelection();
    wxASSERT_MSG(idx != wxNOT_FOUND, wxT("no available translation selected"));
    m_selected->Append(
        m_available->GetString(idx),
        m_available->GetClientData(idx));

    wxZRColaTranslationSeqBase::OnAdd(event);
}


void wxZRColaTranslationSeq::OnRemove(wxCommandEvent& event)
{
    int idx = m_selected->GetSelection();
    wxASSERT_MSG(idx != wxNOT_FOUND, wxT("no active translation selected"));
    m_selected->Delete(idx);

    wxZRColaTranslationSeqBase::OnRemove(event);
}


void wxZRColaTranslationSeq::OnSelectedChar(wxKeyEvent& event)
{
    if (event.GetRawKeyCode() == VK_DELETE) {
        int idx = m_selected->GetSelection();
        if (idx != wxNOT_FOUND)
            m_selected->Delete(idx);
    }

    wxZRColaTranslationSeqBase::OnSelectedChar(event);
}


void wxZRColaTranslationSeq::OnSelectedDClick(wxCommandEvent& event)
{
    int idx = m_selected->GetSelection();
    if (idx != wxNOT_FOUND)
        m_selected->Delete(idx);

    wxZRColaTranslationSeqBase::OnSelectedDClick(event);
}


void wxZRColaTranslationSeq::OnSelectedUp(wxCommandEvent& event)
{
    int idx = m_selected->GetSelection();
    wxASSERT_MSG(idx != wxNOT_FOUND && (unsigned int)idx > 0, wxT("no or first active translation selected"));
    wxString label = m_selected->GetString(idx);
    void *data = m_selected->GetClientData(idx);
    m_selected->Delete(idx);
    m_selected->Insert(label, idx - 1, data);
    m_selected->Select(idx - 1);

    wxZRColaTranslationSeqBase::OnSelectedUp(event);
}


void wxZRColaTranslationSeq::OnSelectedDown(wxCommandEvent& event)
{
    int idx = m_selected->GetSelection();
    wxASSERT_MSG(idx != wxNOT_FOUND && (unsigned int)idx < m_selected->GetCount() - 1, wxT("no or last active translation selected"));
    wxString label = m_selected->GetString(idx);
    void *data = m_selected->GetClientData(idx);
    m_selected->Delete(idx);
    m_selected->Insert(label, idx + 1, data);
    m_selected->Select(idx + 1);

    wxZRColaTranslationSeqBase::OnSelectedDown(event);
}


void wxZRColaTranslationSeq::OnApplyButtonClick(wxCommandEvent& event)
{
    event.Skip();

    auto app = dynamic_cast<ZRColaApp*>(wxTheApp);

    m_transeq.clear();
    for (unsigned int i = 0, n = m_selected->GetCount(); i < n; i++)
        m_transeq.push_back(static_cast<ZRCola::transetid_t>((size_t)m_selected->GetClientData(i)));

    // Notify source text something changed and should re-translate.
    wxCommandEvent event2(wxEVT_COMMAND_TEXT_UPDATED);
    app->m_mainWnd->m_panel->m_source->ProcessWindowEvent(event2);
}


void wxZRColaTranslationSeq::OnOKButtonClick(wxCommandEvent& event)
{
    wxZRColaTranslationSeq::OnApplyButtonClick(event);
}


//////////////////////////////////////////////////////////////////////////
// wxPersistentZRColaTranslationSeq
//////////////////////////////////////////////////////////////////////////

const int wxPersistentZRColaTranslationSeq::s_guiLevel = 1;


wxPersistentZRColaTranslationSeq::wxPersistentZRColaTranslationSeq(wxZRColaTranslationSeq *wnd) : wxPersistentTLWEx(wnd)
{
}


void wxPersistentZRColaTranslationSeq::Save() const
{
    SaveValue(wxT("guiLevel"), s_guiLevel);
    wxPersistentTLWEx::Save();

    auto wnd = static_cast<const wxZRColaTranslationSeq*>(GetWindow()); // dynamic_cast is not reliable as we are typically called late in the wxTopLevelWindowMSW destructor.

    wxString str;
    bool first = true;
    for (auto s = wnd->m_transeq.cbegin(), s_end = wnd->m_transeq.cend(); s != s_end; ++s, first = false)
        str += wxString::Format(first ? wxT("%u") : wxT("|%u"), *s);
    SaveValue(wxT("transeq"), str);
}


bool wxPersistentZRColaTranslationSeq::Restore()
{
    auto wnd = dynamic_cast<wxZRColaTranslationSeq*>(GetWindow());
    auto app = dynamic_cast<ZRColaApp*>(wxTheApp);

    wxString str;
    if (RestoreValue(wxT("transeq"), &str)) {
        std::vector<ZRCola::transetid_t> transet;
        for (wxStringTokenizer tok(str, wxT("|")); tok.HasMoreTokens(); ) {
            unsigned long val = _tcstoul(tok.GetNextToken().c_str(), NULL, 10);
            for (size_t i = 0, n = app->m_ts_db.idxTranSet.size(); i < n; i++) {
                const ZRCola::transet_db::transet &cg = app->m_ts_db.idxTranSet[i];
                if (cg.set == val) {
                    transet.push_back(val);
                    break;
                }
            }
        }
        if (!transet.empty())
            wnd->m_transeq = std::move(transet);
    }

    int guiLevel;
    return RestoreValue(wxT("guiLevel"), &guiLevel) && guiLevel == s_guiLevel ? wxPersistentTLWEx::Restore() : true;
}
