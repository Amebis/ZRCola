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

BEGIN_EVENT_TABLE(wxZRColaComposerPanel, wxZRColaComposerPanelBase)
    EVT_TIMER(wxZRColaComposerPanel::wxID_CHECKPOINT_TIMER, wxZRColaComposerPanel::OnTimerTimeout)
END_EVENT_TABLE()


wxZRColaComposerPanel::wxZRColaComposerPanel(wxWindow* parent) :
    m_progress(false),
    m_selDecomposed(0, 0),
    m_selComposed(0, 0),
    wxZRColaComposerPanelBase(parent)
{
    m_decomposed->PushEventHandler(&m_keyhandler);

    // Create timer for saving the state.
    m_timer = new wxTimer(this, wxID_CHECKPOINT_TIMER);

    // Restore the previously saved state (if exists).
    wxString fileName(GetStateFileName());
    if (wxFileExists(fileName)) {
        wxFFile file(fileName, wxT("rb"));
        if (file.IsOpened()) {
            // Load decomposed text.
            unsigned __int64 n;
            file.Read(&n, sizeof(n));
            if (!file.Error()) {
                wxString decomposed;
                file.Read(wxStringBuffer(decomposed, n), sizeof(wchar_t)*n);
                if (!file.Error()) {
                    // Load composed text.
                    file.Read(&n, sizeof(n));
                    if (!file.Error()) {
                        wxString composed;
                        file.Read(wxStringBuffer(composed, n), sizeof(wchar_t)*n);
                        if (!file.Error()) {
                            // Restore state.
                            m_progress = true;
                            m_decomposed->SetValue(decomposed);
                            m_composed->SetValue(composed);
                            m_progress = false;
                        }
                    }
                }
            }
        }
    }
}


wxZRColaComposerPanel::~wxZRColaComposerPanel()
{
    if (m_timer)
        delete m_timer;

    m_decomposed->PopEventHandler();

    // This is a controlled exit. Purge saved state.
    wxString fileName(GetStateFileName());
    if (wxFileExists(fileName))
        wxRemoveFile(fileName);
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
        m_decomposedHex->SetSelection(m_mappingDecomposedHex.to_dst(from), m_mappingDecomposedHex.to_dst(to));
        m_composed->SetSelection(from = m_mapping2.to_dst(m_mapping1.to_dst(from)), to = m_mapping2.to_dst(m_mapping1.to_dst(to)));
        m_composedHex->SetSelection(m_mappingComposedHex.to_dst(from), m_mappingComposedHex.to_dst(to));
    }
}


void wxZRColaComposerPanel::OnDecomposedHexPaint(wxPaintEvent& event)
{
    event.Skip();

    long from, to;
    m_decomposedHex->GetSelection(&from, &to);

    if (m_selDecomposedHex.first != from || m_selDecomposedHex.second != to) {
        // Save new selection first, to avoid loop.
        m_selDecomposedHex.first  = from;
        m_selDecomposedHex.second = to;
        m_decomposed->SetSelection(from = m_mappingDecomposedHex.to_src(from), to = m_mappingDecomposedHex.to_src(to));
        m_composed->SetSelection(from = m_mapping2.to_dst(m_mapping1.to_dst(from)), to = m_mapping2.to_dst(m_mapping1.to_dst(to)));
        m_composedHex->SetSelection(m_mappingComposedHex.to_dst(from), m_mappingComposedHex.to_dst(to));
    }
}


void wxZRColaComposerPanel::OnDecomposedText(wxCommandEvent& event)
{
    if (m_progress) {
        // We are being updated by wxZRColaComposerPanel::OnComposedText()
        event.Skip();
    } else {
        m_timer->Stop();

#ifdef __WINDOWS__
        // Use Windows GetWindowText() function to avoid line ending conversion incompletely imposed by wxWidgets.
        WXHWND hWnd = m_decomposed->GetHWND();
        size_t len = ::GetWindowTextLengthW(hWnd);
        std::vector<wchar_t> src(len + 1);
        ::GetWindowTextW(hWnd, src.data(), src.size());
#else
        wxString src(m_decomposed->GetValue());
        size_t len = src.Length();
#endif

        std::wstring norm;
        ((ZRColaApp*)wxTheApp)->m_t_db.Decompose(src.data(), len, norm, &m_mapping1);

        std::wstring dst;
        ((ZRColaApp*)wxTheApp)->m_t_db.Compose(norm.data(), norm.size(), dst, &m_mapping2);

        long from, to;
        m_decomposed->GetSelection(&from, &to);

        // Update decomposed HEX dump.
        wxString hex;
        GetHex(hex, m_mappingDecomposedHex, src.data(), len);
        m_decomposedHex->SetValue(hex);
        m_decomposedHex->SetSelection(m_mappingDecomposedHex.to_dst(from), m_mappingDecomposedHex.to_dst(to));

        // Update composed text.
        m_progress = true;
        m_composed->SetValue(dst);
        m_composed->SetSelection(from = m_mapping2.to_dst(m_mapping1.to_dst(from)), to = m_mapping2.to_dst(m_mapping1.to_dst(to)));

        // Update composed HEX dump.
        GetHex(hex, m_mappingComposedHex, dst.data(), dst.length());
        m_composedHex->SetValue(hex);
        m_composedHex->SetSelection(m_mappingComposedHex.to_dst(from), m_mappingComposedHex.to_dst(to));

        event.Skip();
        m_progress = false;

        // Schedule state save after 3s.
        m_timer->Start(3000, true);
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
        m_composedHex->SetSelection(m_mappingComposedHex.to_dst(from), m_mappingComposedHex.to_dst(to));
        m_decomposed->SetSelection(from = m_mapping1.to_src(m_mapping2.to_src(from)), to = m_mapping1.to_src(m_mapping2.to_src(to)));
        m_decomposedHex->SetSelection(m_mappingDecomposedHex.to_dst(from), m_mappingDecomposedHex.to_dst(to));
    }
}


void wxZRColaComposerPanel::OnComposedHexPaint(wxPaintEvent& event)
{
    event.Skip();

    long from, to;
    m_composedHex->GetSelection(&from, &to);

    if (m_selComposedHex.first != from || m_selComposedHex.second != to) {
        // Save new selection first, to avoid loop.
        m_selComposedHex.first  = from;
        m_selComposedHex.second = to;
        m_composed->SetSelection(from = m_mappingComposedHex.to_src(from), to = m_mappingComposedHex.to_src(to));
        m_decomposed->SetSelection(from = m_mapping1.to_src(m_mapping2.to_src(from)), to = m_mapping1.to_src(m_mapping2.to_src(to)));
        m_decomposedHex->SetSelection(m_mappingDecomposedHex.to_dst(from), m_mappingDecomposedHex.to_dst(to));
    }
}


void wxZRColaComposerPanel::OnComposedText(wxCommandEvent& event)
{
    if (m_progress) {
        // We are being updated by wxZRColaComposerPanel::OnDecomposedText()
        event.Skip();
    } else {
        m_timer->Stop();

#ifdef __WINDOWS__
        // Use Windows GetWindowTextLength() function to avoid line ending conversion incompletely imposed by wxWidgets.
        WXHWND hWnd = m_composed->GetHWND();
        size_t len = ::GetWindowTextLengthW(hWnd);
        std::vector<wchar_t> src(len + 1);
        ::GetWindowTextW(hWnd, src.data(), src.size());
#else
        wxString src(m_composed->GetValue());
        size_t len = src.Length();
#endif

        ZRColaApp *app = (ZRColaApp*)wxTheApp;
        std::wstring dst;
        wxZRColaFrame *mainWnd = dynamic_cast<wxZRColaFrame*>(wxGetActiveWindow());
        if (mainWnd)
            app->m_t_db.Decompose(src.data(), len, &app->m_lc_db, mainWnd->m_lang, dst, &m_mapping2);
        else
            app->m_t_db.Decompose(src.data(), len, dst, &m_mapping2);

        m_mapping1.clear();
        m_mapping2.invert();

        long from, to;
        m_composed->GetSelection(&from, &to);

        // Update composed HEX dump.
        wxString hex;
        GetHex(hex, m_mappingComposedHex, src.data(), len);
        m_composedHex->SetValue(hex);
        m_composedHex->SetSelection(m_mappingComposedHex.to_dst(from), m_mappingComposedHex.to_dst(to));

        // Update decomposed text.
        m_progress = true;
        m_decomposed->SetValue(dst);
        m_decomposed->SetSelection(from = m_mapping1.to_src(m_mapping2.to_src(from)), to = m_mapping1.to_src(m_mapping2.to_src(to)));

        // Update decomposed HEX dump.
        GetHex(hex, m_mappingDecomposedHex, dst.data(), dst.length());
        m_decomposedHex->SetValue(hex);
        m_decomposedHex->SetSelection(m_mappingDecomposedHex.to_dst(from), m_mappingDecomposedHex.to_dst(to));

        event.Skip();
        m_progress = false;

        // Schedule state save after 3s.
        m_timer->Start(3000, true);
    }
}


void wxZRColaComposerPanel::OnTimerTimeout(wxTimerEvent& event)
{
    wxString fileName(GetStateFileName());
    wxFFile file(fileName, wxT("wb"));
    if (file.IsOpened()) {
        // Save decomposed text.
        {
#ifdef __WINDOWS__
            // Use Windows GetWindowText() function to avoid line ending conversion incompletely imposed by wxWidgets.
            WXHWND hWnd = m_decomposed->GetHWND();
            unsigned __int64 len = ::GetWindowTextLengthW(hWnd);
            std::vector<wchar_t> text(len + 1);
            ::GetWindowTextW(hWnd, text.data(), text.size());
#else
            wxString text(m_decomposed->GetValue());
            unsigned __int64 len = text.Length();
#endif
            file.Write(&len, sizeof(len));
            file.Write(text.data(), sizeof(wchar_t)*len);
        }

        // Save composed text.
        {
#ifdef __WINDOWS__
            // Use Windows GetWindowText() function to avoid line ending conversion incompletely imposed by wxWidgets.
            WXHWND hWnd = m_composed->GetHWND();
            unsigned __int64 len = ::GetWindowTextLengthW(hWnd);
            std::vector<wchar_t> text(len + 1);
            ::GetWindowTextW(hWnd, text.data(), text.size());
#else
            wxString text(m_composed->GetValue());
            unsigned __int64 len = text.Length();
#endif
            file.Write(&len, sizeof(len));
            file.Write(text.data(), sizeof(wchar_t)*len);
        }
    }

    event.Skip();
}


wxString wxZRColaComposerPanel::GetStateFileName()
{
    wxString path;

    path = wxFileName::GetTempDir();
    if (!wxEndsWithPathSeparator(path))
        path += wxFILE_SEP_PATH;

    if (!wxDirExists(path))
        wxMkdir(path);

    wxString fileName(path);
    fileName += wxT("ZRColaComposerPanel-state.tmp");

    return fileName;
}


void wxZRColaComposerPanel::GetHex(wxString &hex, ZRCola::mapping_vector &mapping, const wchar_t *src, size_t len)
{
    bool first = true;
    hex.clear();
    mapping.clear();
    for (size_t i = 0; i < len && src[i]; i++) {
        wchar_t c = src[i];
        if (c == L'\n' || c == '\r') {
            hex += c;
            first = true;
        } else {
            hex += wxString::Format(first ? wxT("%04X") : wxT(" %04X"), src[i]);
            mapping.push_back(ZRCola::mapping(i + 1, hex.Length()));
            first = false;
        }
    }
}


//////////////////////////////////////////////////////////////////////////
// wxPersistentZRColaComposerPanel
//////////////////////////////////////////////////////////////////////////

wxPersistentZRColaComposerPanel::wxPersistentZRColaComposerPanel(wxZRColaComposerPanel *wnd) : wxPersistentWindow<wxZRColaComposerPanel>(wnd)
{
}


wxString wxPersistentZRColaComposerPanel::GetKind() const
{
    return wxT(wxPERSIST_TLW_KIND);
}


void wxPersistentZRColaComposerPanel::Save() const
{
    const wxZRColaComposerPanel * const wnd = static_cast<const wxZRColaComposerPanel*>(GetWindow());

    SaveValue(wxT("splitDecomposed"), wnd->m_splitterDecomposed->GetSashPosition());
    SaveValue(wxT("splitComposed"  ), wnd->m_splitterComposed  ->GetSashPosition());
}


bool wxPersistentZRColaComposerPanel::Restore()
{
    wxZRColaComposerPanel * const wnd = static_cast<wxZRColaComposerPanel*>(GetWindow());

    int sashVal;

    if (RestoreValue(wxT("splitDecomposed"), &sashVal)) {
        // wxFormBuilder sets initial splitter stash in idle event handler after GUI settles. Overriding our loaded value. Disconnect it's idle event handler.
        wnd->m_splitterDecomposed->Disconnect( wxEVT_IDLE, wxIdleEventHandler( wxZRColaComposerPanelBase::m_splitterDecomposedOnIdle ), NULL, wnd );
        wnd->m_splitterDecomposed->SetSashPosition(sashVal);
    }

    if (RestoreValue(wxT("splitComposed"), &sashVal)) {
        // wxFormBuilder sets initial splitter stash in idle event handler after GUI settles. Overriding our loaded value. Disconnect it's idle event handler.
        wnd->m_splitterComposed->Disconnect( wxEVT_IDLE, wxIdleEventHandler( wxZRColaComposerPanelBase::m_splitterComposedOnIdle ), NULL, wnd );
        wnd->m_splitterComposed->SetSashPosition(sashVal);
    }

    return true;
}
