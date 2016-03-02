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
// wxZRColaComposerPanelEvtHandler
//////////////////////////////////////////////////////////////////////////

wxZRColaComposerPanelEvtHandler::wxZRColaComposerPanelEvtHandler(wxZRColaComposerPanel *target) :
    m_target(target),
    wxEvtHandler()
{
}


bool wxZRColaComposerPanelEvtHandler::ProcessEvent(wxEvent& event)
{
    if (m_target && event.IsCommandEvent()) {
        int id = event.GetId();
        if (wxZRColaComposerPanel::wxID_ACCEL <= id && id < wxZRColaComposerPanel::wxID_ACCEL + m_target->m_ks_db.idxKey.size()) {
            const ZRCola::keyseq_db::keyseq &ks = (const ZRCola::keyseq_db::keyseq&)m_target->m_ks_db.data[m_target->m_ks_db.idxKey[id - wxZRColaComposerPanel::wxID_ACCEL]];
            m_target->m_decomposed->WriteText(ks.chr);
            return true;
        }
    }

    return wxEvtHandler::ProcessEvent(event);
}


//////////////////////////////////////////////////////////////////////////
// wxZRColaComposerPanel
//////////////////////////////////////////////////////////////////////////

wxZRColaComposerPanel::wxZRColaComposerPanel(wxWindow* parent) :
    m_progress(false),
    m_selDecomposed(0, 0),
    m_selComposed(0, 0),
    eh(this),
    wxZRColaComposerPanelBase(parent)
{
    wxString sPath(wxPathOnly(wxTheApp->argv[0]));
    sPath << wxT("\\..\\data\\ZRCola.zrcdb"); // TODO: Make database file configurable

    std::fstream dat((LPCTSTR)sPath, std::ios_base::in | std::ios_base::binary);
    if (dat.good()) {
        if (stdex::idrec::find<ZRCola::recordid_t, ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>(dat, ZRCOLA_DB_ID, sizeof(ZRCola::recordid_t))) {
            ZRCola::recordsize_t size;
            dat.read((char*)&size, sizeof(ZRCola::recordsize_t));
            if (dat.good()) {
                bool has_translation_data = false;

                for (;;) {
                    ZRCola::recordid_t id;
                    if (!stdex::idrec::read_id(dat, id, size)) break;

                    if (id == ZRCola::translation_rec::id) {
                        dat >> ZRCola::translation_rec(m_t_db);
                        if (dat.good()) {
                            has_translation_data = true;
                        } else {
                            wxFAIL_MSG(wxT("Error reading translation data from ZRCola.zrcdb."));
                            m_t_db.idxComp  .clear();
                            m_t_db.idxDecomp.clear();
                            m_t_db.data     .clear();
                        }
                    } else if (id == ZRCola::keyseq_rec::id) {
                        dat >> ZRCola::keyseq_rec(m_ks_db);
                        if (!dat.good()) {
                            wxFAIL_MSG(wxT("Error reading key sequence data from ZRCola.zrcdb."));
                            m_ks_db.idxChr.clear();
                            m_ks_db.idxKey.clear();
                            m_ks_db.data  .clear();
                        }
                    }
                }

                if (!has_translation_data)
                    wxFAIL_MSG(wxT("ZRCola.zrcdb has no translation data."));
            }
        } else
            wxFAIL_MSG(wxT("ZRCola.zrcdb is not a valid ZRCola database."));
    }

    std::vector<unsigned __int32>::size_type n = m_ks_db.idxKey.size();
    std::vector<wxAcceleratorEntry> entries;
    entries.reserve(n);
    for (std::vector<unsigned __int32>::size_type i = 0; i < n; i++) {
        const ZRCola::keyseq_db::keyseq &ks = (const ZRCola::keyseq_db::keyseq&)m_ks_db.data[m_ks_db.idxKey[i]];
        if (ks.seq_len == 1) {
            // The key sequence is trivial.
            entries.push_back(wxAcceleratorEntry(
                ((ks.seq[0].modifiers & ZRCola::keyseq_db::keyseq::SHIFT) ? wxACCEL_SHIFT : 0) |
                ((ks.seq[0].modifiers & ZRCola::keyseq_db::keyseq::CTRL ) ? wxACCEL_CTRL  : 0) |
                ((ks.seq[0].modifiers & ZRCola::keyseq_db::keyseq::ALT  ) ? wxACCEL_ALT   : 0), ks.seq[0].key, wxID_ACCEL + i));
        }
    }
    wxAcceleratorTable accel(entries.size(), entries.data());
    SetAcceleratorTable(accel);

    PushEventHandler(&eh);
}


wxZRColaComposerPanel::~wxZRColaComposerPanel()
{
    PopEventHandler();
}


void wxZRColaComposerPanel::OnDecomposedPaint(wxPaintEvent& event)
{
    event.Skip();

    long from, to;
    m_decomposed->GetSelection(&from, &to);

    if (m_selDecomposed.first != from || m_selDecomposed.second != to) {
        // Save new selection first, to avoid loop.
        m_selDecomposed.first  = from;
        m_selDecomposed.second = to;
        m_composed->SetSelection(m_mapping.to_composed(from), m_mapping.to_composed(to));
    }
}


void wxZRColaComposerPanel::OnDecomposedText(wxCommandEvent& event)
{
    if (m_progress) {
        // We are being updated by wxZRColaComposerPanel::OnComposedText()
        event.Skip();
    } else {
#ifdef __WINDOWS__
        // Use Windows GetWindowText() function to avoid line ending conversion incompletely imposed by wxWidgets.
        WXHWND hWnd = m_decomposed->GetHWND();
        std::vector<wchar_t> src((std::vector<wchar_t>::size_type)::GetWindowTextLengthW(hWnd) + 1);
        ::GetWindowTextW(hWnd, src.data(), src.size());
#else
        wxString src(m_decomposed->GetValue());
#endif

        std::wstring dst;
        m_t_db.Compose(src.data(), src.size(), dst, &m_mapping);

        long from, to;
        m_decomposed->GetSelection(&from, &to);

        // Update composed text.
        m_progress = true;
        m_composed->SetValue(dst);
        m_composed->SetSelection(m_mapping.to_composed(from), m_mapping.to_composed(to));
        event.Skip();
        m_progress = false;
    }
}


void wxZRColaComposerPanel::OnComposedPaint(wxPaintEvent& event)
{
    event.Skip();

    long from, to;
    m_composed->GetSelection(&from, &to);

    if (m_selComposed.first != from || m_selComposed.second != to) {
        // Save new selection first, to avoid loop.
        m_selComposed.first  = from;
        m_selComposed.second = to;
        m_decomposed->SetSelection(m_mapping.to_decomposed(from), m_mapping.to_decomposed(to));
    }
}


void wxZRColaComposerPanel::OnComposedText(wxCommandEvent& event)
{
    if (m_progress) {
        // We are being updated by wxZRColaComposerPanel::OnDecomposedText()
        event.Skip();
    } else {
#ifdef __WINDOWS__
        // Use Windows GetWindowTextLength() function to avoid line ending conversion incompletely imposed by wxWidgets.
        WXHWND hWnd = m_composed->GetHWND();
        std::vector<wchar_t> src((std::vector<wchar_t>::size_type)::GetWindowTextLengthW(hWnd) + 1);
        ::GetWindowTextW(hWnd, src.data(), src.size());
#else
        wxString src(m_composed->GetValue());
#endif

        std::wstring dst;
        m_t_db.Decompose(src.data(), src.size(), dst, &m_mapping);

        long from, to;
        m_composed->GetSelection(&from, &to);

        // Update decomposed text.
        m_progress = true;
        m_decomposed->SetValue(dst);
        m_decomposed->SetSelection(m_mapping.to_decomposed(from), m_mapping.to_decomposed(to));
        event.Skip();
        m_progress = false;
    }
}
