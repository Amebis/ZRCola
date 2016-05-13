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
    m_decomposedChanged(false),
    m_composedChanged(false),
    m_selDecomposed(0, 0),
    m_selComposed(0, 0),
    wxZRColaComposerPanelBase(parent)
{
    m_decomposed->PushEventHandler(&m_keyhandler);

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
                            m_decomposed->SetValue(decomposed);
                            m_decomposed->GetSelection(&m_selDecomposed.first, &m_selDecomposed.second);
                            SetHexValue(m_decomposedHex, m_selDecomposedHex, m_mappingDecomposedHex, decomposed.GetData(), decomposed.Length(), m_selDecomposed.first, m_selDecomposed.second);
                            m_decomposedChanged = false;

                            m_composed->SetValue(composed);
                            m_composed->GetSelection(&m_selComposed.first, &m_selComposed.second);
                            SetHexValue(m_composedHex, m_selComposedHex, m_mappingComposedHex, composed.GetData(), composed.Length(), m_selComposed.first, m_selComposed.second);
                            m_composedChanged = false;
                        }
                    }
                }
            }
        }
    }
}


wxZRColaComposerPanel::~wxZRColaComposerPanel()
{
    m_decomposed->PopEventHandler();

    // This is a controlled exit. Purge saved state.
    wxString fileName(GetStateFileName());
    if (wxFileExists(fileName))
        wxRemoveFile(fileName);
}


void wxZRColaComposerPanel::SynchronizePanels()
{
    if (m_decomposedChanged) {
        m_timerSave.Stop();

        wxString src;
        size_t len = GetValue(m_decomposed, src);

        std::wstring norm;
        ((ZRColaApp*)wxTheApp)->m_t_db.Decompose(src.data(), len, norm, &m_mapping1);

        std::wstring dst;
        ((ZRColaApp*)wxTheApp)->m_t_db.Compose(norm.data(), norm.size(), dst, &m_mapping2);

        m_decomposed->GetSelection(&m_selDecomposed.first, &m_selDecomposed.second);

        // Update decomposed HEX dump.
        SetHexValue(m_decomposedHex, m_selDecomposedHex, m_mappingDecomposedHex, src.data(), len, m_selDecomposed.first, m_selDecomposed.second);

        // Update composed text, and its HEX dump.
        m_composed->SetValue(dst);
        m_composed->SetSelection(
            m_selComposed.first  = m_mapping2.to_dst(m_mapping1.to_dst(m_selDecomposed.first )),
            m_selComposed.second = m_mapping2.to_dst(m_mapping1.to_dst(m_selDecomposed.second)));
        SetHexValue(m_composedHex, m_selComposedHex, m_mappingComposedHex, dst.data(), dst.length(), m_selComposed.first, m_selComposed.second);

        // Schedule state save after 3s.
        m_timerSave.Start(3000, true);
    } else if (m_composedChanged) {
        m_timerSave.Stop();

        wxString src;
        size_t len = GetValue(m_composed, src);

        ZRColaApp *app = (ZRColaApp*)wxTheApp;
        std::wstring dst;
        wxZRColaFrame *mainWnd = dynamic_cast<wxZRColaFrame*>(wxGetActiveWindow());
        if (mainWnd)
            app->m_t_db.Decompose(src.data(), len, &app->m_lc_db, mainWnd->m_settings->m_lang, dst, &m_mapping2);
        else
            app->m_t_db.Decompose(src.data(), len, dst, &m_mapping2);

        m_mapping1.clear();
        m_mapping2.invert();

        m_composed->GetSelection(&m_selComposed.first, &m_selComposed.second);

        // Update composed HEX dump.
        SetHexValue(m_composedHex, m_selComposedHex, m_mappingComposedHex, src.data(), len, m_selComposed.first, m_selComposed.second);

        // Update decomposed text, and its HEX dump.
        m_decomposed->SetValue(dst);
        m_decomposed->SetSelection(
            m_selDecomposed.first  = m_mapping1.to_src(m_mapping2.to_src(m_selComposed.first )),
            m_selDecomposed.second = m_mapping1.to_src(m_mapping2.to_src(m_selComposed.second)));
        SetHexValue(m_decomposedHex, m_selDecomposedHex, m_mappingDecomposedHex, dst.data(), dst.length(), m_selDecomposed.first, m_selDecomposed.second);

        // Schedule state save after 3s.
        m_timerSave.Start(3000, true);
    }

    m_decomposedChanged = false;
    m_composedChanged   = false;
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

        m_decomposedHex->SetSelection(
            m_selDecomposedHex.first  = m_mappingDecomposedHex.to_dst(from),
            m_selDecomposedHex.second = m_mappingDecomposedHex.to_dst(to  ));

        m_composed->SetSelection(
            m_selComposed.first  = m_mapping2.to_dst(m_mapping1.to_dst(from)),
            m_selComposed.second = m_mapping2.to_dst(m_mapping1.to_dst(to  )));

        m_composedHex->SetSelection(
            m_selComposedHex.first  = m_mappingComposedHex.to_dst(m_selComposed.first ),
            m_selComposedHex.second = m_mappingComposedHex.to_dst(m_selComposed.second));
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

        m_decomposed->SetSelection(
            m_selDecomposed.first  = m_mappingDecomposedHex.to_src(from),
            m_selDecomposed.second = m_mappingDecomposedHex.to_src(to  ));

        m_composed->SetSelection(
            m_selComposed.first  = m_mapping2.to_dst(m_mapping1.to_dst(m_selDecomposed.first )),
            m_selComposed.second = m_mapping2.to_dst(m_mapping1.to_dst(m_selDecomposed.second)));

        m_composedHex->SetSelection(
            m_selComposedHex.first  = m_mappingComposedHex.to_dst(m_selComposed.first ),
            m_selComposedHex.second = m_mappingComposedHex.to_dst(m_selComposed.second));
    }
}


void wxZRColaComposerPanel::OnDecomposedText(wxCommandEvent& event)
{
    event.Skip();

    // Set the flag the decomposed text changed to trigger idle-time composition.
    m_decomposedChanged = true;
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

        m_composedHex->SetSelection(
            m_selComposedHex.first  = m_mappingComposedHex.to_dst(from),
            m_selComposedHex.second = m_mappingComposedHex.to_dst(to  ));

        m_decomposed->SetSelection(
            m_selDecomposed.first  = m_mapping1.to_src(m_mapping2.to_src(from)),
            m_selDecomposed.second = m_mapping1.to_src(m_mapping2.to_src(to  )));

        m_decomposedHex->SetSelection(
            m_selDecomposedHex.first  = m_mappingDecomposedHex.to_dst(m_selDecomposed.first ),
            m_selDecomposedHex.second = m_mappingDecomposedHex.to_dst(m_selDecomposed.second));
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

        m_composed->SetSelection(
            m_selComposed.first  = m_mappingComposedHex.to_src(from),
            m_selComposed.second = m_mappingComposedHex.to_src(to  ));

        m_decomposed->SetSelection(
            m_selDecomposed.first  = m_mapping1.to_src(m_mapping2.to_src(m_selComposed.first )),
            m_selDecomposed.second = m_mapping1.to_src(m_mapping2.to_src(m_selComposed.second)));

        m_decomposedHex->SetSelection(
            m_selDecomposedHex.first  = m_mappingDecomposedHex.to_dst(m_selDecomposed.first ),
            m_selDecomposedHex.second = m_mappingDecomposedHex.to_dst(m_selDecomposed.second));
    }
}


void wxZRColaComposerPanel::OnComposedText(wxCommandEvent& event)
{
    event.Skip();

    // Set the flag the composed text changed to trigger idle-time decomposition.
    m_composedChanged = true;
}


void wxZRColaComposerPanel::OnSaveTimer(wxTimerEvent& event)
{
    wxString fileName(GetStateFileName());
    wxFFile file(fileName, wxT("wb"));
    if (file.IsOpened()) {
        wxString text;
        size_t len;

        // Save decomposed text.
        len = GetValue(m_decomposed, text);
        file.Write(&len, sizeof(len));
        file.Write((const wchar_t*)text, sizeof(wchar_t)*len);

        // Save composed text.
        len = GetValue(m_composed, text);
        file.Write(&len, sizeof(len));
        file.Write((const wchar_t*)text, sizeof(wchar_t)*len);
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


size_t wxZRColaComposerPanel::GetValue(wxTextCtrl *wnd, wxString &text)
{
#ifdef __WINDOWS__
    // Use Windows GetWindowText() function to avoid line ending conversion incompletely imposed by wxWidgets.
    WXHWND hWnd = wnd->GetHWND();
    size_t len = ::GetWindowTextLengthW(hWnd);
    if (len < 0x100) {
        WCHAR buf[0x100];
        ::GetWindowTextW(hWnd, buf, len + 1);
        text.assign(buf, len);
    } else {
        LPWSTR buf = new WCHAR[len + 1];
        ::GetWindowTextW(hWnd, buf, len + 1);
        text.assign(buf, len);
        delete [] buf;
    }

    return len;
#else
    text = wnd->GetValue();
    return text.Length();
#endif
}


void wxZRColaComposerPanel::SetHexValue(wxTextCtrl *wnd, std::pair<long, long> &range, ZRCola::mapping_vector &mapping, const wchar_t *src, size_t len, long from, long to)
{
    wxString hex;
    bool first = true;

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

    wnd->SetValue(hex);
    wnd->SetSelection(
        range.first  = mapping.to_dst(from),
        range.second = mapping.to_dst(to  ));
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
