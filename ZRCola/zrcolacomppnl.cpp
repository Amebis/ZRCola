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
// wxZRColaComposerPanel
//////////////////////////////////////////////////////////////////////////

wxZRColaComposerPanel::wxZRColaComposerPanel(wxWindow* parent) :
    m_progress(false),
    m_selDecomposed(0, 0),
    m_selComposed(0, 0),
    wxZRColaComposerPanelBase(parent)
{
    std::fstream dat((LPCTSTR)((ZRColaApp*)wxTheApp)->GetDatabasePath(), std::ios_base::in | std::ios_base::binary);
    if (dat.good()) {
        if (stdex::idrec::find<ZRCola::recordid_t, ZRCola::recordsize_t, ZRCOLA_RECORD_ALIGN>(dat, ZRCOLA_DB_ID, sizeof(ZRCola::recordid_t))) {
            ZRCola::recordsize_t size;
            dat.read((char*)&size, sizeof(ZRCola::recordsize_t));
            if (dat.good()) {
                ZRCola::translation_rec rec(m_t_db);
                if (rec.find(dat, size)) {
                    dat >> rec;
                    if (!dat.good()) {
                        wxFAIL_MSG(wxT("Error reading translation data from ZRCola.zrcdb."));
                        m_t_db.idxComp  .clear();
                        m_t_db.idxDecomp.clear();
                        m_t_db.data     .clear();
                    }
                } else
                    wxFAIL_MSG(wxT("ZRCola.zrcdb has no translation data."));
            }
        } else
            wxFAIL_MSG(wxT("ZRCola.zrcdb is not a valid ZRCola database."));
    }

    m_decomposed->PushEventHandler(&m_keyhandler);
}


wxZRColaComposerPanel::~wxZRColaComposerPanel()
{
    m_decomposed->PopEventHandler();
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
