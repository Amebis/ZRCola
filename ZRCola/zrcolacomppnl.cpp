/*
    Copyright 2015-2018 Amebis

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
    m_sourceChanged(false),
    m_destinationChanged(false),
    m_selSource(0, 0),
    m_selDestination(0, 0),
    wxZRColaComposerPanelBase(parent)
{
    m_source->PushEventHandler(&m_keyhandler);

    // Restore the previously saved state (if exists).
    wxString fileName(GetStateFileName());
    if (wxFileExists(fileName)) {
        wxFFile file(fileName, wxT("rb"));
        if (file.IsOpened()) {
            // Load source text.
            unsigned __int64 n;
            file.Read(&n, sizeof(n));
            if (!file.Error()) {
                wxString source;
                file.Read(wxStringBuffer(source, n), sizeof(wchar_t)*n);
                if (!file.Error()) {
                    // Load destination text.
                    file.Read(&n, sizeof(n));
                    if (!file.Error()) {
                        wxString destination;
                        file.Read(wxStringBuffer(destination, n), sizeof(wchar_t)*n);
                        if (!file.Error()) {
                            // Restore state.
                            m_source->SetValue(source);
                            m_source->GetSelection(&m_selSource.first, &m_selSource.second);
                            SetHexValue(m_sourceHex, m_selSourceHex, m_mappingSourceHex, source.GetData(), source.Length(), m_selSource.first, m_selSource.second);
                            m_sourceChanged = false;

                            m_destination->SetValue(destination);
                            m_destination->GetSelection(&m_selDestination.first, &m_selDestination.second);
                            SetHexValue(m_destinationHex, m_selDestinationHex, m_mappingDestinationHex, destination.GetData(), destination.Length(), m_selDestination.first, m_selDestination.second);
                            m_destinationChanged = false;
                        }
                    }
                }
            }
        }
    }
}


wxZRColaComposerPanel::~wxZRColaComposerPanel()
{
    m_source->PopEventHandler();

    // This is a controlled exit. Purge saved state.
    wxString fileName(GetStateFileName());
    if (wxFileExists(fileName))
        wxRemoveFile(fileName);
}


void wxZRColaComposerPanel::SynchronizePanels()
{
    if (m_sourceChanged) {
        m_timerSave.Stop();

        auto app = dynamic_cast<ZRColaApp*>(wxTheApp);

        wxString src;
        size_t len = GetValue(m_source, src);
        std::wstring dst(src.data(), len), dst2;
        ZRCola::mapping_vector map;

        m_mapping.clear();

        if (app->m_mainWnd->m_composition) {
            // ZRCola decompose first, then re-compose.
            app->m_t_db.TranslateInv(ZRCOLA_TRANSEQID_DEFAULT, dst.data(), dst.size(), dst2, &map);
            m_mapping.push_back(std::move(map));

            app->m_t_db.Translate(ZRCOLA_TRANSEQID_DEFAULT, dst2.data(), dst2.size(), dst, &map);
            m_mapping.push_back(std::move(map));
        }

        // Other translations
        const ZRCola::transetid_t *sets_begin, *sets_end;
        GetTranslationSeq(sets_begin, sets_end);
        for (auto s = sets_begin; s != sets_end; ++s) {
            app->m_t_db.Translate(*s, dst.data(), dst.size(), dst2, &map);
            m_mapping.push_back(std::move(map));
            dst = std::move(dst2);
        }

        m_source->GetSelection(&m_selSource.first, &m_selSource.second);

        // Update source HEX dump.
        SetHexValue(m_sourceHex, m_selSourceHex, m_mappingSourceHex, src.data(), len, m_selSource.first, m_selSource.second);

        // Update destination text, and its HEX dump.
        m_destination->SetValue(dst);
        m_destination->SetSelection(
            m_selDestination.first  = MapToDestination(m_selSource.first ),
            m_selDestination.second = MapToDestination(m_selSource.second));
        SetHexValue(m_destinationHex, m_selDestinationHex, m_mappingDestinationHex, dst.data(), dst.length(), m_selDestination.first, m_selDestination.second);

        // Schedule state save after 3s.
        m_timerSave.Start(3000, true);
    } else if (m_destinationChanged) {
        m_timerSave.Stop();

        auto app = dynamic_cast<ZRColaApp*>(wxTheApp);

        wxString src;
        size_t len = GetValue(m_destination, src);
        std::wstring dst(src.data(), len), dst2;
        ZRCola::mapping_vector map;

        m_mapping.clear();

        // Other translations
        const ZRCola::transetid_t *sets_begin, *sets_end;
        GetTranslationSeq(sets_begin, sets_end);
        for (auto s = sets_end; (s--) != sets_begin;) {
            app->m_t_db.TranslateInv(*s, dst.data(), dst.size(), dst2, &map);
            dst = std::move(dst2);

            map.invert();
            m_mapping.push_back(std::move(map));
        }

        if (app->m_mainWnd->m_composition) {
            // ZRCola decompose.
            app->m_t_db.TranslateInv(ZRCOLA_TRANSEQID_DEFAULT, dst.data(), dst.size(), &app->m_lc_db, app->m_mainWnd->m_settings->m_lang, dst2, &map);
            dst = std::move(dst2);

            map.invert();
            m_mapping.push_back(std::move(map));
        }

        m_destination->GetSelection(&m_selDestination.first, &m_selDestination.second);

        // Update destination HEX dump.
        SetHexValue(m_destinationHex, m_selDestinationHex, m_mappingDestinationHex, src.data(), len, m_selDestination.first, m_selDestination.second);

        // Update source text, and its HEX dump.
        m_source->SetValue(dst);
        m_source->SetSelection(
            m_selSource.first  = MapToSource(m_selDestination.first ),
            m_selSource.second = MapToSource(m_selDestination.second));
        SetHexValue(m_sourceHex, m_selSourceHex, m_mappingSourceHex, dst.data(), dst.length(), m_selSource.first, m_selSource.second);

        // Schedule state save after 3s.
        m_timerSave.Start(3000, true);
    }

    m_sourceChanged      = false;
    m_destinationChanged = false;
}



void wxZRColaComposerPanel::OnSourcePaint(wxPaintEvent& event)
{
    event.Skip();

    long from, to;
    m_source->GetSelection(&from, &to);

    if (m_selSource.first != from || m_selSource.second != to) {
        // Save new selection first, to avoid loop.
        m_selSource.first  = from;
        m_selSource.second = to;

        m_sourceHex->SetSelection(
            m_selSourceHex.first  = m_mappingSourceHex.to_dst(from),
            m_selSourceHex.second = m_mappingSourceHex.to_dst(to  ));

        m_destination->SetSelection(
            m_selDestination.first  = MapToDestination(from),
            m_selDestination.second = MapToDestination(to  ));

        m_destinationHex->SetSelection(
            m_selDestinationHex.first  = m_mappingDestinationHex.to_dst(m_selDestination.first ),
            m_selDestinationHex.second = m_mappingDestinationHex.to_dst(m_selDestination.second));
    }
}


void wxZRColaComposerPanel::OnSourceHexPaint(wxPaintEvent& event)
{
    event.Skip();

    long from, to;
    m_sourceHex->GetSelection(&from, &to);

    if (m_selSourceHex.first != from || m_selSourceHex.second != to) {
        // Save new selection first, to avoid loop.
        m_selSourceHex.first  = from;
        m_selSourceHex.second = to;

        m_source->SetSelection(
            m_selSource.first  = m_mappingSourceHex.to_src(from),
            m_selSource.second = m_mappingSourceHex.to_src(to  ));

        m_destination->SetSelection(
            m_selDestination.first  = MapToDestination(m_selSource.first ),
            m_selDestination.second = MapToDestination(m_selSource.second));

        m_destinationHex->SetSelection(
            m_selDestinationHex.first  = m_mappingDestinationHex.to_dst(m_selDestination.first ),
            m_selDestinationHex.second = m_mappingDestinationHex.to_dst(m_selDestination.second));
    }
}


void wxZRColaComposerPanel::OnSourceText(wxCommandEvent& event)
{
    event.Skip();

    // Set the flag the source text changed to trigger idle-time translation.
    m_sourceChanged = true;
}


void wxZRColaComposerPanel::OnDestinationPaint(wxPaintEvent& event)
{
    event.Skip();

    long from, to;
    m_destination->GetSelection(&from, &to);

    if (m_selDestination.first != from || m_selDestination.second != to) {
        // Save new selection first, to avoid loop.
        m_selDestination.first  = from;
        m_selDestination.second = to;

        m_destinationHex->SetSelection(
            m_selDestinationHex.first  = m_mappingDestinationHex.to_dst(from),
            m_selDestinationHex.second = m_mappingDestinationHex.to_dst(to  ));

        m_source->SetSelection(
            m_selSource.first  = MapToSource(from),
            m_selSource.second = MapToSource(to  ));

        m_sourceHex->SetSelection(
            m_selSourceHex.first  = m_mappingSourceHex.to_dst(m_selSource.first ),
            m_selSourceHex.second = m_mappingSourceHex.to_dst(m_selSource.second));
    }
}


void wxZRColaComposerPanel::OnDestinationHexPaint(wxPaintEvent& event)
{
    event.Skip();

    long from, to;
    m_destinationHex->GetSelection(&from, &to);

    if (m_selDestinationHex.first != from || m_selDestinationHex.second != to) {
        // Save new selection first, to avoid loop.
        m_selDestinationHex.first  = from;
        m_selDestinationHex.second = to;

        m_destination->SetSelection(
            m_selDestination.first  = m_mappingDestinationHex.to_src(from),
            m_selDestination.second = m_mappingDestinationHex.to_src(to  ));

        m_source->SetSelection(
            m_selSource.first  = MapToSource(m_selDestination.first ),
            m_selSource.second = MapToSource(m_selDestination.second));

        m_sourceHex->SetSelection(
            m_selSourceHex.first  = m_mappingSourceHex.to_dst(m_selSource.first ),
            m_selSourceHex.second = m_mappingSourceHex.to_dst(m_selSource.second));
    }
}


void wxZRColaComposerPanel::OnDestinationText(wxCommandEvent& event)
{
    event.Skip();

    // Set the flag the destination text changed to trigger idle-time inverse translation.
    m_destinationChanged = true;
}


void wxZRColaComposerPanel::OnSaveTimer(wxTimerEvent& event)
{
    wxString fileName(GetStateFileName());
    wxFFile file(fileName, wxT("wb"));
    if (file.IsOpened()) {
        wxString text;
        size_t len;

        // Save source text.
        len = GetValue(m_source, text);
        file.Write(&len, sizeof(len));
        file.Write((const wchar_t*)text, sizeof(wchar_t)*len);

        // Save destination text.
        len = GetValue(m_destination, text);
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

const int wxPersistentZRColaComposerPanel::s_guiLevel = 1;


wxPersistentZRColaComposerPanel::wxPersistentZRColaComposerPanel(wxZRColaComposerPanel *wnd) : wxPersistentWindow<wxZRColaComposerPanel>(wnd)
{
}


wxString wxPersistentZRColaComposerPanel::GetKind() const
{
    return wxT(wxPERSIST_TLW_KIND);
}


void wxPersistentZRColaComposerPanel::Save() const
{
    auto wnd = static_cast<wxZRColaComposerPanel*>(GetWindow()); // dynamic_cast is not reliable as we are typically called late in the wxTopLevelWindowMSW destructor.

    SaveValue(wxT("guiLevel"       ), s_guiLevel);
    SaveValue(wxT("dpiX"           ), wxClientDC(wnd).GetPPI().x);
    SaveValue(wxT("splitDecomposed"), wnd->m_splitterSource     ->GetSashPosition());
    SaveValue(wxT("splitComposed"  ), wnd->m_splitterDestination->GetSashPosition());
}


bool wxPersistentZRColaComposerPanel::Restore()
{
    auto wnd = dynamic_cast<wxZRColaComposerPanel*>(GetWindow());

    int guiLevel;
    if (!RestoreValue(wxT("guiLevel"), &guiLevel) || guiLevel != s_guiLevel)
        return true;

    int dpiHorz = wxClientDC(wnd).GetPPI().x;
    int dpiHorzVal;
    int sashVal;

    if (!RestoreValue(wxT("dpiX"), &dpiHorzVal))
        dpiHorzVal = 96;

    if (RestoreValue(wxT("splitDecomposed"), &sashVal)) {
        // wxFormBuilder sets initial splitter stash in idle event handler after GUI settles. Overriding our loaded value. Disconnect it's idle event handler.
        wnd->m_splitterSource->Disconnect( wxEVT_IDLE, wxIdleEventHandler( wxZRColaComposerPanelBase::m_splitterSourceOnIdle ), NULL, wnd );
        wnd->m_splitterSource->SetSashPosition(wxMulDivInt32(sashVal, dpiHorz, dpiHorzVal));
    }

    if (RestoreValue(wxT("splitComposed"), &sashVal)) {
        // wxFormBuilder sets initial splitter stash in idle event handler after GUI settles. Overriding our loaded value. Disconnect it's idle event handler.
        wnd->m_splitterDestination->Disconnect( wxEVT_IDLE, wxIdleEventHandler( wxZRColaComposerPanelBase::m_splitterDestinationOnIdle ), NULL, wnd );
        wnd->m_splitterDestination->SetSashPosition(wxMulDivInt32(sashVal, dpiHorz, dpiHorzVal));
    }

    return true;
}
